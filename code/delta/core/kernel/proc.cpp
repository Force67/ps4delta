
// Copyright (C) Force67 2019

#include <xbyak/xbyak.h>
#include <utl/File.h>
#include <utl/path.h>

#include "proc.h"
#include "module.h"
#include "runtime/vprx/vprx.h"

static void PS4ABI kek(void *idk)
{}

namespace krnl
{
	using namespace runtime;

	proc::proc()
	{
	}

	bool proc::create(const std::string& path)
	{
		// register HLE prx modules
		runtime::vprx_init();

		auto xxx = std::make_unique<elfModule>(this);
		if (!xxx->fromFile(R"(H:\.projects\ps4delta\bin\Debug\modules\libkernel.sprx)")) {
			LOG_ERROR("unable to load main process module");
			return false;
		}

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

		// generate a entry point push context
		entryGen callingContext(xxx->entry);
		auto func = callingContext.getCode<void* (*)(void*)>();

		union stack_entry
		{
			const void* ptr;
			uint64_t val;
		}
		stack[128];

		stack[0].val = 1 + 0; // argc
		auto s = reinterpret_cast<stack_entry*>(&stack[1]);
		(*s++).ptr = xxx->name.c_str();
		/*for (auto it = args.begin(); it != args.end(); ++it)
		{
			(*s++).ptr = (*it).c_str();
		}*/
		(*s++).ptr = nullptr; // arg null terminator
		(*s++).ptr = nullptr; // env null terminator
		(*s++).val = 9ull; // entrypoint type
		(*s++).ptr = xxx->entry;
		(*s++).ptr = nullptr; // aux null type
		(*s++).ptr = nullptr;

		func(stack);

		return 0;

		/*reserve slot 0 for the main process we do this, 
		because dependencies loaded by the main module 
		also allocate their own slots*/
		modules.emplace_back();

		// create our executable module
		auto mainObj = std::make_unique<elfModule>(this);
		if (!mainObj->fromFile(path)) {
			LOG_ERROR("unable to load main process module");
			return false;
		}

		// exec module is always #1
		modules[0] = std::move(mainObj);
		return true;
	}

	void proc::addObj(std::unique_ptr<kObj> obj) {
		modules.push_back(std::move(obj));
	}

	kObj* proc::getModule(std::string_view name) {
		for (auto& mod : modules) {
			if (!mod)
				continue;

			if (mod->name == name)
				return mod.get();
		}
		return nullptr;
	}

	kObj* proc::loadModule(std::string_view name)
	{
		// very hacky hack
		std::string sprxname(name);
		sprxname += ".sprx";

		auto* mod = getModule(sprxname);
		if (!mod) {
			auto lib = std::make_unique<elfModule>(this);

			// todo: gather path from user firmware installation
			if (!lib->fromFile(utl::make_abs_path("modules\\" + sprxname))) {
				LOG_ERROR("unable to load module {}", name);
				return nullptr;
			}

			auto* shit = lib.get();

			// initialize cxx
			// lib->start
			addObj(std::move(lib));

			return shit;
		}
		return mod;
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

		auto& mainx = getMainModule();
		std::printf("main name %s\n", mainx.name.c_str());

		// generate a entry point push context
		entryGen callingContext(mainx.entry);
		auto func = callingContext.getCode<void* (*)(void*)>();

		union stack_entry
		{
			const void* ptr;
			uint64_t val;
		}
		stack[128];

		stack[0].val = 1 + 0; // argc
		auto s = reinterpret_cast<stack_entry*>(&stack[1]);
		(*s++).ptr = mainx.name.c_str();
		/*for (auto it = args.begin(); it != args.end(); ++it)
		{
			(*s++).ptr = (*it).c_str();
		}*/
		(*s++).ptr = nullptr; // arg null terminator
		(*s++).ptr = nullptr; // env null terminator
		(*s++).val = 9ull; // entrypoint type
		(*s++).ptr = mainx.entry;
		(*s++).ptr = nullptr; // aux null type
		(*s++).ptr = nullptr;

		func(stack);
	}
}