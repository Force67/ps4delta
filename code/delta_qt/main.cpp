
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>
#include <logger/logger.h>

#include <QApplication>
#include <QCommandLineParser>
#include <QFileInfo>
#include <utl/path.h>

#include "ui/mainwindow.h"

#include <core.h>

inline std::string sstr(const QString& _in) {
    return _in.toStdString();
}

class deltaApp final : public QApplication {
public:
    deltaApp(int& argc, char** argv) : QApplication(argc, argv) {}

    bool init();

private:
    std::unique_ptr<mainWindow> window;
    bool headless = false;
};

bool deltaApp::init() {
    LOG_INFO("Initializing delta qt app " rsc_copyright);

    auto& sys = core::System::get();
    if (!sys.init())
        return false;

    if (!headless)
        window = std::make_unique<mainWindow>(sys);
    else
        LOG_INFO("Starting in headless mode");

    if (window)
        window->init();

    return true;
}

#ifdef _WIN32

#include <Windows.h>
#include <memory.h>
#include <utl/mcrt_win.h>

static void applyQtFixups() {
    using NtQueryTimerResolution_t = LONG(WINAPI*)(PULONG, PULONG, PULONG);
    using NtSetTimerResolution_t = LONG(WINAPI*)(ULONG, BOOLEAN, PULONG);

    auto hNtLib = GetModuleHandleW(L"ntdll.dll");
    auto NtQueryTimerResolution_f = reinterpret_cast<NtQueryTimerResolution_t>(
        GetProcAddress(hNtLib, "NtQueryTimerResolution"));
    auto NtSetTimerResolution_f =
        reinterpret_cast<NtSetTimerResolution_t>(GetProcAddress(hNtLib, "NtSetTimerResolution"));

    // qt resets our timer, so wet set it back manually
    ULONG min_res, max_res, orig_res, new_res;
    if (NtQueryTimerResolution_f(&min_res, &max_res, &orig_res) == 0)
        NtSetTimerResolution_f(max_res, TRUE, &new_res);

    setlocale(LC_ALL, "C");
}
#endif

inline int deltaMain(int argc, char** argv) {
    utl::createLogger(true);

    // search plugins in /qt/ dir so we don't bloat main dir
    // *too* much
    {
        auto plugin_dir = utl::make_abs_path("qt");
        QCoreApplication::addLibraryPath(QString::fromUtf8(plugin_dir.c_str()));
    }

    QCoreApplication::setOrganizationName(rsc_company);
    QCoreApplication::setApplicationName(rsc_productname);
    QCoreApplication::setApplicationVersion(rsc_productversion);
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QScopedPointer<deltaApp> app(new deltaApp(argc, argv));

#ifdef _WIN32
    applyQtFixups();
#endif

    QCommandLineParser parser;
    parser.setApplicationDescription("Welcome to the " rsc_productname " command line.");
    parser.addPositionalArgument("File", "Path for directly loading a file");
    parser.addPositionalArgument("[Args...]", "Optional args for the executable");
    parser.addOption(QCommandLineOption("headless", "Run " rsc_productname " in headless mode."));
    parser.process(*app);

    if (app->init()) {
        // propagate command line arguments
        auto args = parser.positionalArguments();
        if (args.length() > 0) {
            std::vector<std::string> argv;

            if (args.length() > 1) {
                argv.emplace_back();

                for (int i = 1; i < args.length(); i++) {
                    argv.emplace_back(args[i].toStdString());
                }
                core::System::get().argv = std::move(argv);
            }

            auto path = sstr(QFileInfo(args.at(0)).canonicalFilePath());
            core::System::get().load(path);
        }

        return app->exec();
    }

    return 0;
}

#ifdef _WIN32
EXPORT int delta_main(memory::init_info& info) {
    // pre init hook
    memory::preinit(info);

    int32_t nArgs = 0;
    auto** args = utl::cmdl_to_argv(GetCommandLineA(), nArgs);

    return deltaMain(nArgs, args);
}
#else
EXPORT int delta_main(int argc, char** argv) {
    return deltaMain(argc, argv);
}
#endif