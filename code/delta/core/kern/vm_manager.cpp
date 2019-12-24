
// Copyright (C) Force67 2019

#include <utl/mem.h>

#include "proc.h"
#include "vm_manager.h"

namespace krnl
{
	vmManager::vmManager(procInfo& info) :
		pinfo(info)
	{}

	vmManager::~vmManager()
	{
		if (pinfo.userStack)
			utl::freeMem(pinfo.userStack);

		pinfo.userStack = nullptr;
	}

	bool vmManager::init()
	{
		/*reserve address space for the user stack*/
		pinfo.userStack = static_cast<uint8_t*>(
			utl::allocMem(nullptr, pinfo.userStackSize,
				utl::pageProtection::priv,
				utl::allocationType::reserve));

		return pinfo.userStack;
	}

	uint8_t* vmManager::mapMemory(uint8_t* preference, size_t size, utl::pageProtection prot)
	{
		const auto allocType = utl::allocationType::reservecommit;

		void* ptr = utl::allocMem(static_cast<void*>(preference), size, prot, allocType);
		if (ptr) {
			return static_cast<uint8_t*>(ptr);
		}

		return nullptr;
	}

	void vmManager::unmapRtMemory(uint8_t* ptr)
	{
		auto iter = std::find_if(
			rtPages.begin(), rtPages.end(),
			[&ptr](const auto& page) { return page.ptr == ptr; });

		rtPages.erase(iter);
	}
}