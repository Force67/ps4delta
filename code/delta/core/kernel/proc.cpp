
// Copyright (C) Force67 2019

#include <xbyak/xbyak.h>
#include <utl/File.h>

#include "proc.h"
#include "elfModule.h"
#include "modules/vmodLinker.h"

namespace krnl
{
	using namespace modules;

	proc::proc()
	{
	}

	bool proc::create(const std::string& path)
	{
		// register HLE sprx modules
		initVMods();

		// create our executable module
		auto mainObj = std::make_unique<elfModule>(this);
		if (!mainObj->fromFile(path)) {
			LOG_ERROR("unable to load main process file");
			return false;
		}

		// exec module is always #1
		addObj(std::move(mainObj));
		return true;
	}

	void proc::addObj(std::unique_ptr<kObj> obj) {
		modules.push_back(std::move(obj));
	}

	kObj* proc::getModule(std::string_view name) {
		for (auto& mod : modules) {
			if (mod->name == name)
				return mod.get();
		}
		return nullptr;
	}

	void proc::start()
	{
		class entryGen : public Xbyak::CodeGenerator
		{
		public:
			entryGen(void* target)
			{
				push(rbp);
				mov(rbp, rsp);
				push(r12);
				push(r13);
				push(r14);
				push(r15);
				push(rdi);
				push(rsi);
				push(rbx);

				sub(rsp, 8);

				mov(rdi, rcx);
				mov(rax, (size_t)target);

				call(rax);

				add(rsp, 8);

				pop(rbx);
				pop(rsi);
				pop(rdi);
				pop(r15);
				pop(r14);
				pop(r13);
				pop(r12);
				pop(rbp);
				ret();
			}
		};

		auto& main = getMainModule();

		// generate a entry point push context
		entryGen callingContext(main.entry);
		auto func = callingContext.getCode<void* (*)(void*)>();

		union stack_entry
		{
			const void* ptr;
			uint64_t val;
		}
		stack[128];

		stack[0].val = 1 + 0; // argc
		auto s = reinterpret_cast<stack_entry*>(&stack[1]);
		(*s++).ptr = main.name.c_str();
		/*for (auto it = args.begin(); it != args.end(); ++it)
		{
			(*s++).ptr = (*it).c_str();
		}*/
		(*s++).ptr = nullptr; // arg null terminator
		(*s++).ptr = nullptr; // env null terminator
		(*s++).val = 9ull; // entrypoint type
		(*s++).ptr = main.entry;
		(*s++).ptr = nullptr; // aux null type
		(*s++).ptr = nullptr;

		func(stack);
	}
}