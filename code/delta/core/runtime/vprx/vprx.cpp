
// Copyright (C) Force67 2019

#include <vector>
#include "vprx.h"

namespace runtime
{
	static std::vector<const modInfo*> vprxTable;

	void vprx_init() {
		utl::init_function::init();
	}

	void vprx_reg(const modInfo* info) {
		vprxTable.push_back(info);
	}

	uintptr_t vprx_get(const char* lib, uint64_t hid) {
		const modInfo* table = nullptr;

		// find the right table
		for (const auto& t : vprxTable) {
			if (std::strcmp(lib, t->namePtr) == 0) {
				table = t;
				break;
			}
		}

		if (table) {
			// search the table
			for (int i = 0; i < table->funcCount; i++) {
				auto* f = &table->funcNodes[i];
				if (f->hashId == hid) {
					return reinterpret_cast<uintptr_t>(f->address);
				}
			}
		}

		return 0;
	}

	// base64 fast lookup
	bool decode_nid(const char* subset, size_t len, uint64_t &out)
	{
		const char lookup[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";

		for (size_t i = 0; i < len; i++) {
			auto pos = std::strchr(lookup, subset[i]);

			// invalid NID?
			if (!pos) {
				return false;
			}

			auto offset = static_cast<uint32_t>(pos - lookup);

			// max NID is 11
			if (i < 10) {
				out <<= 6;
				out |= offset;
			}
			else {
				out <<= 4;
				out |= (offset >> 2);
			}
		}

		return true;
	}
}