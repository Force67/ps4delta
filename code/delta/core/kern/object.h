#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <atomic>
#include <mutex>
#include <string>
#include <utl/object_ref.h>
#include <vector>

namespace krnl {
class proc;

class kObject {
public:
  using handleList = std::vector<uint32_t>;

  enum class oType {
    file,
    device,
  };

  explicit kObject(proc *, oType);

  void retain();
  void release();
  void retainHandle();
  void releaseHandle();

  oType type() const { return otype; }

  handleList &handles() { return handleCollection; }

  uint32_t handle() const { return handleCollection[0]; }

protected:
  oType otype;
  proc *process;
  std::string name;

private:
  handleList handleCollection;
  std::atomic<int32_t> refCount;
};

template <typename T> utl::object_ref<T> retain_object(T *ptr) {
  if (ptr)
    ptr->retain();
  return utl::object_ref<T>(ptr);
}
}