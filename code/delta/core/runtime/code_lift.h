#pragma once

// Copyright (C) Force67 2019

#include <capstone/capstone.h>

namespace krnl {
	struct procEnv;
}

namespace runtime
{
	// code analysis: 
	// convert unsupported code
	class codeLift
	{
	public:
		~codeLift();
		bool init();
		bool transform(uint8_t*, size_t size, uint64_t base = 0);

	private:
		void emit_syscall(uint8_t *base, uint32_t idx);

		csh handle = 0;
		cs_insn* insn = nullptr;
	};
}