
// Copyright (C) Force67 2019

#include <xbyak.h>
#include <utl/File.h>
#include <utl/path.h>

#include "proc.h"
#include "module.h"
#include "runtime/vprx/vprx.h"

#include <filesystem>

namespace krnl
{
	using namespace runtime;

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

	static proc* g_activeProc{ nullptr };

	proc::proc()
	{
		g_activeProc = this;
	}

	proc* proc::getActive() {
		return g_activeProc;
	}

	bool proc::create(const std::string& path)
	{
		// register HLE prx modules
		runtime::vprx_init();

		auto first = modules.emplace_back(std::make_shared<elfModule>(this));
		first->handle = 0;

		if (!first->fromFile(path)) {
			LOG_ERROR("unable to load main process module");
			return false;
		}

		return true;
	}

	std::shared_ptr<elfModule> proc::getModule(std::string_view name) {
		for (auto& mod : modules) {
			if (mod->name == name)
				return mod;
		}
		return { nullptr };
	}

	std::shared_ptr<elfModule> proc::getModule(uint32_t handle) {
		for (auto& mod : modules) {

			if (mod->handle == handle)
				return mod;
		}
		return { nullptr };
	}

	std::shared_ptr<elfModule> proc::loadModule(std::string_view name)
	{
		// very hacky hack
		std::string sprxname(name);
		sprxname += ".sprx";

		auto mod = getModule(sprxname);
		if (!mod) {
			auto lib = modules.emplace_back(std::make_shared<elfModule>(this));
			lib->handle = handleCounter;
			handleCounter++;

			if (!lib->fromFile(utl::make_abs_path("modules\\" + sprxname))) {
				LOG_ERROR("unable to load module {}", name);
				return nullptr;
			}
			
			return lib;
		}
		return mod;
	}

	void proc::start()
	{
		auto invoke = [](elfModule& mod)
		{
			// todo: get rid of this mess
			entryGen callingCtx(mod.entry);
			auto func = callingCtx.getCode<void* (*)(void*)>();

			union stack_entry
			{
				const void* ptr;
				uint64_t val;
			}
			stack[128];

			stack[0].val = 1 + 0; // argc
			auto s = reinterpret_cast<stack_entry*>(&stack[1]);
			(*s++).ptr = mod.name.c_str();
			(*s++).ptr = nullptr; // arg null terminator
			(*s++).ptr = nullptr; // env null terminator
			(*s++).val = 9ull; // entrypoint type
			(*s++).ptr = (const void*)(mod.entry - mod.base);
			(*s++).ptr = nullptr; // aux null type
			(*s++).ptr = nullptr;

			func(stack);
		};

		for (int i = 1; i < modules.size(); i++) {
			invoke(*modules[i]);
		}

		auto mod = getMainModule();
		invoke(*mod);
	}
}