
// Copyright (C) Force67 2019

#include <utl/mem.h>
#include "vm_manager.h"

namespace krnl
{
	uint8_t* vmManager::mapMemory(uint8_t* preference, size_t size, bool code)
	{
		auto prot = code ? utl::pageProtection::rwx : utl::pageProtection::write;

		void* ptr = utl::allocMem(static_cast<void*>(preference), size, prot,
			utl::allocationType::reservecommit);
		if (ptr) {
			if (code) {
				codeMemTotal += size;
				codePages.emplace_back(static_cast<uint8_t*>(ptr), size);
			}
			else {
				rtMemTotal += size;
				rtPages.emplace_back(static_cast<uint8_t*>(ptr), size);
			}

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