
// Copyright (C) 2019 Force67

#include <loader/Loader.h>

int wmain()
{
	auto loader = loaders::CreateLoader(L"");
	if (loader) {
		loader->Load();
	}

	return 0;
}