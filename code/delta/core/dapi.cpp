
// Copyright (C) 2019 Force67

#include <base.h>
#include <logger/logger.h>
#include <utl/mem.h>
#include <xbyak_util.h>

#include <QCommandLineParser>
#include <QFileInfo>

#ifdef _WIN32
#include <VersionHelpers.h>
#endif

#include "dcore.h"

inline std::string sstr(const QString &_in) { return _in.toStdString(); }

static deltaCore *createCore(int &argc, char **argv) {

  auto *app = new deltaCore(argc, argv);

  return app;
}

static bool verifyViablity() {

#ifdef _WIN32
  if (!IsWindows8OrGreater()) {
    LOG_ERROR("Your operating system is outdated. Please update to windows 8 "
              "or newer.");
    return false;
  }
#endif

  constexpr size_t one_mb = 1024ull * 1024ull;
  constexpr size_t eight_gb = 8ull * 1024ull * one_mb;

  if (utl::getAvailableMem() < eight_gb) {
    LOG_ERROR("Your system doesn't have enough physical memory to run " FXNAME);
    return false;
  }

  std::string missingFeatures;
  Xbyak::util::Cpu cpu;

#define CHECK_FEATURE(x, y)                                                    \
  if (!cpu.has(Xbyak::util::Cpu::t##x)) {                                      \
    missingFeatures += std::string(y) + ";";                                   \
  }

  CHECK_FEATURE(SSE, "SSE");
  CHECK_FEATURE(SSE2, "SSE2");
  CHECK_FEATURE(SSE3, "SSE3");
  CHECK_FEATURE(SSSE3, "SSSE3");
  CHECK_FEATURE(SSE41, "SSE4.1");
  CHECK_FEATURE(SSE42, "SSE4.2");
  CHECK_FEATURE(AESNI, "AES");
  CHECK_FEATURE(AVX, "AVX");
  CHECK_FEATURE(PCLMULQDQ, "CLMUL");
  CHECK_FEATURE(F16C, "F16C");
  CHECK_FEATURE(BMI1, "BM1");

  if (!missingFeatures.empty()) {
    LOG_ERROR("Your cpu is missing the following instructions: {}");
    return false;
  }

  return true;
}

#ifdef _WIN32
static void win32PostInit() {
  using NtQueryTimerResolution_t = LONG(WINAPI *)(PULONG, PULONG, PULONG);
  using NtSetTimerResolution_t = LONG(WINAPI *)(ULONG, BOOLEAN, PULONG);

  auto hNtLib = GetModuleHandleW(L"ntdll.dll");
  auto NtQueryTimerResolution_f = reinterpret_cast<NtQueryTimerResolution_t>(
      GetProcAddress(hNtLib, "NtQueryTimerResolution"));
  auto NtSetTimerResolution_f = reinterpret_cast<NtSetTimerResolution_t>(
      GetProcAddress(hNtLib, "NtSetTimerResolution"));

  // qt resets our timer, so wet set it back manually
  ULONG min_res, max_res, orig_res, new_res;
  if (NtQueryTimerResolution_f(&min_res, &max_res, &orig_res) == 0)
    NtSetTimerResolution_f(max_res, TRUE, &new_res);
}
#endif

EXPORT int dcoreMain(int argc, char **argv) {
  utl::createLogger(true);

  if (!verifyViablity())
    return -1;

  QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QScopedPointer<deltaCore> appInstance(createCore(argc, argv));

  QCommandLineParser parser;
  parser.setApplicationDescription("Welcome to the " rsc_productname
                                   " command line.");
  parser.addPositionalArgument("File", "Path for directly loading a file");
  parser.addPositionalArgument("[Args...]", "Optional args for the executable");
  parser.addOption(QCommandLineOption("headless", "Run " rsc_productname
                                                  " in headless mode."));
  parser.process(*appInstance);

  if (appInstance) {
    if (appInstance->init()) {

#ifdef _WIN32
      win32PostInit();
#endif

      auto args = parser.positionalArguments();
      if (args.length() > 0) {
        // Propagate command line arguments
        std::vector<std::string> argv;

        if (args.length() > 1) {
          argv.emplace_back();

          for (int i = 1; i < args.length(); i++) {
            argv.emplace_back(args[i].toStdString());
          }

          appInstance->argv = std::move(argv);
        }

        auto path = sstr(QFileInfo(args.at(0)).canonicalFilePath());
        appInstance->boot(path);
      }

      // enter event loop
      return appInstance->exec();
    }
  }

  return 0;
}