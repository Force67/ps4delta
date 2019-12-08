
// Copyright (c) Nomad Group 2019

#include <Windows.h>
#include <utl/path.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

namespace utl
{
	/*
	 * These functions are REQUIRED to return a canonicalized path!
	 * CEF requirement! Otherwise these will result in weird path errors
	 */
	std::wstring make_abs_path(const std::wstring &rel_to, void *handle)
	{
		static wchar_t executable_path[MAX_PATH] = { '\0' };

		if (executable_path[0] == '\0' || handle) {
			wchar_t buf[MAX_PATH];
			GetModuleFileNameW((HMODULE)handle, buf, MAX_PATH);
			_wsplitpath(buf, &executable_path[0], &executable_path[_MAX_DRIVE - 1], nullptr, nullptr);
		}

		wchar_t buf[MAX_PATH];
		lstrcpyW(buf, executable_path);
		lstrcatW(buf, rel_to.c_str());

		wchar_t final_buf[MAX_PATH] = { '\0' };
		PathCanonicalizeW(final_buf, buf);

		return final_buf;
	}

	std::string make_abs_path(const std::string &rel_to, void *handle)
	{
		static char executable_path[MAX_PATH] = { '\0' };

		if (executable_path[0] == '\0' || handle) {
			char buf[MAX_PATH];
			GetModuleFileNameA((HMODULE)handle, buf, MAX_PATH);
			_splitpath(buf, &executable_path[0], &executable_path[_MAX_DRIVE - 1], nullptr, nullptr);
		}

		char buf[MAX_PATH];
		strcpy(buf, executable_path);
		strcat(buf, rel_to.c_str());

		char final_buf[MAX_PATH] = { '\0' };
		PathCanonicalizeA(final_buf, buf);

		return final_buf;
	}

} // namespace utl