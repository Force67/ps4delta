
// Copyright (C) Force67 2019

#include <kernel/Process.h>
#include <xbyak/xbyak.h>

namespace krnl
{
	namespace
	{
		class EntryGen : public Xbyak::CodeGenerator
		{
		public:
			EntryGen(void* target)
			{
				push(rbp);
				mov(rbp, rsp);
				push(r12); push(r13); push(r14); push(r15);
				push(rdi); push(rsi); push(rbx);

				sub(rsp, 8);

				mov(rdi, rcx);
				mov(rax, (size_t)target);

				call(rax);

				add(rsp, 8);

				pop(rbx); pop(rsi); pop(rdi);
				pop(r15); pop(r14); pop(r13); pop(r12);
				pop(rbp);
				ret();
			}
		};
	}

	Process::Process() :
		creationStamp(std::chrono::steady_clock::now()),
		tlsSlots(0)
	{}

	void Process::Start(const std::vector<std::string> &args)
	{
		if (modulelist.size() == 0) {
			//TODO: assert
		}

		auto& main = GetMainModule();

		// HACK HACK HACK: initialize stack cookie this way...
		/*static uint64_t leet = 0x1337;
		static uint64_t s2 = 1234;
		*(uint64_t*)(main->base + 0x47EDE8) = (uint64_t)&leet;
		*(uint64_t*)(main->base + 0xA47D18) = (uint64_t)& s2;*/

		// generate a entry point push context
		EntryGen trampoline(main->entry);
		auto func = trampoline.getCode<void* (*)(void*)>();

		union stack_entry
		{
			const void* ptr;
			uint64_t val;
		}
		stack[128];

		stack[0].val = 1 + args.size(); // argc
		auto s = reinterpret_cast<stack_entry*>(&stack[1]);
		(*s++).ptr = main->name.c_str();
		for (auto it = args.begin(); it != args.end(); ++it)
		{
			(*s++).ptr = (*it).c_str();
		}
		(*s++).ptr = nullptr; // arg null terminator
		(*s++).ptr = nullptr; // env null terminator
		(*s++).val = 9ull; // entrypoint type
		(*s++).ptr = main->entry;
		(*s++).ptr = nullptr; // aux null type
		(*s++).ptr = nullptr;

		func(stack);
	}

	void Process::RegisterModule(ModuleHandle handle)
	{
		// fire module load callbacks
		for (auto& cb : modulenotification) {
			cb(handle);
		}

		modulelist.emplace_back(handle);
	}

	ModuleHandle Process::GetModule(const std::string& name)
	{
		for (auto& it : modulelist) {
			if (it->name == name) {
				return it;
			}
		}

		return {};
	}

	void Process::RegisterModuleNotifaction(ModuleCallback cb)
	{
		modulenotification.push_back(cb);
	}
}