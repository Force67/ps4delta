
// Copyright (C) Force67 2019

#include <logger/logger.h>

#include "code_lift.h"
#include <cstdio>

#include <xbyak.h>

namespace runtime
{
	uintptr_t lv2_get(uint32_t sysIndex);

	// for debugging
	static void printOpInfo(const cs_x86_op& op) {
		std::printf("Operand: Type %d, Reg %d, (Mem: base %d)\n", op.type, op.reg, op.mem.base);
	}

	codeLift::~codeLift()
	{
		if (handle) {
			cs_free(insn, 1);
			cs_close(&handle);

			// just to be sure...
			handle = 0;
		}
	}

	bool codeLift::init()
	{
		auto err = cs_open(CS_ARCH_X86, CS_MODE_64, &handle);
		if (err == CS_ERR_MEM) {
			LOG_ERROR("vabiPass: not enough mem for disasembler");
			return false;
		}

		// setup disasm config
		cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);
		return true;
	}

	bool codeLift::transform(uint8_t* data, size_t size, uint64_t base)
	{
		insn = cs_malloc(handle);

		const uint8_t* codePtr = data; // iterator
		while (cs_disasm_iter(handle, &codePtr, &size, &base, insn)) {

			auto detail = insn->detail->x86;
			uint32_t dest = static_cast<uint32_t>(X86_REL_ADDR(*insn));

			auto getOps = [&](int32_t ofs) {
				//return codePtr + ofs;
				return &data[insn->address + ofs];
			};

			if (insn->id == X86_INS_SYSCALL) {
				emit_syscall(getOps(-10), *(uint32_t*)(getOps(-7)));
			}

#if 0
			if (insn->id == X86_INS_INT) {
				auto op = detail.operands[0];

				/*abort interrupt*/
				if (op.type == X86_OP_IMM && *(uint8_t*)(getOps(1)) == 0x44) {
					auto* target = getOps(1);
					*target = 3;
				}
			}
#endif

		}

		return false;
	}

	void codeLift::emit_syscall(uint8_t* base, uint32_t idx)
	{
		auto address = lv2_get(idx);
		if (address) {
			*(uint16_t*)base = 0xB848;
			*(uint64_t*)(base + 2) = address;
			*(uint16_t*)(base + 10) = 0xE0FF;
		}
	}
}