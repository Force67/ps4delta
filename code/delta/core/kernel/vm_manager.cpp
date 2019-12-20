
// Copyright (C) Force67 2019

#include <Windows.h>

#include "vm_manager.h"

namespace krnl
{
	/*TODO: make platform independent*/
	uint8_t* vmManager::mapCodeMemory(uint8_t* preference, size_t size)
	{
		void* ptr = VirtualAlloc(preference, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (ptr) {
			codeMemTotal += size;
			codePages.emplace_back(static_cast<uint8_t*>(ptr), size);

			return static_cast<uint8_t*>(ptr);
		}

		return nullptr;
	}

	uint8_t* vmManager::mapRtMemory(size_t size)
	{
		/*todo: track which module alloc'd*/

		uint8_t* ptr = static_cast<uint8_t*>(malloc(size));
		if (ptr) {
			rtMemTotal += size;
			rtPages.emplace_back(ptr, size);

			return ptr;
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