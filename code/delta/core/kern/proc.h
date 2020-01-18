#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <memory>
#include <string>
#include <vector>

#include "dev/device.h"
#include "module.h"
#include "object.h"
#include "util/object_table.h"
#include "vm_manager.h"

namespace krnl {
struct procInfo {
  uint32_t ripZoneSize = 5 * 1024;
  uint8_t *userStack = nullptr;
  size_t userStackSize = 20 * 1024 * 1024;
  void *fsBase = nullptr;
};

class smodule;
class kObject;

/*TODO: FIX MISUSE OF modulePtr*/
using modulePtr = utl::object_ref<smodule>;

class proc {
  friend class smodule;

public:
  using moduleList = std::vector<modulePtr>;

  proc();
  bool create(const std::string &);
  void start();

  static proc *getActive();

  // inline modulePtr getMainModule() { return modules[0]; }
  inline moduleList &getModuleList() { return modules; }
  inline objectTable &getObjTable() { return objects; }

  modulePtr loadModule(std::string_view);
  modulePtr getModule(std::string_view);
  modulePtr getModule(uint32_t);

  inline vmManager &getVma() { return vmem; }
  inline procInfo &getEnv() { return env; }

private:
  vmManager vmem;
  procInfo env;
  moduleList modules;
  objectTable objects;
  uint32_t handleCounter = 1;

  uint32_t nextFreeTLS() { return -1; }
};
}