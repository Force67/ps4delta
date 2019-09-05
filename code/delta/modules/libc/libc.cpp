
// Copyright (C) Force67 2019

#include "../Module.h"

struct EnvInfo
{
	void* unk;
	const char* cmdl;
};

static void* _init_env(EnvInfo *info, void*, void*, void*)
{
	std::printf("init_env! %s\n", info->cmdl);

	return nullptr;
}

static int _atexit(void(*handler)())
{
	//TODO:
	return 0;
}

static void _exit(int code, void* unk)
{

}

static void _catchReturnFromMain(int code)
{
}

static const modules::FunctionInfo functions[] = {
	{0, &_init_env, "bzQExy189ZI"},
	{1, &_atexit, "8G2LB+A3rzg"},
	//ENTRY: exit
	{4, &_catchReturnFromMain, "XKRegsFpEpk"},
};

MODULE_INIT(libc)