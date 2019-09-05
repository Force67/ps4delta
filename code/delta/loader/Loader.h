#pragma once

// Copyright (C) Force67 2019

#include <string>
#include <memory>

#include <utilities/File.h>

namespace krnl {
	class Process;
}

namespace loaders
{
	enum class LoadErrorCode
	{
		UNKNOWN,
		SUCCESS,
		BADIMP,
		BADREL,
		BADMAP,
		BADSEG,
		BADALIGN,
	};

	enum class FileType
	{
		UNKNOWN,
		ELF,
		SELF,
		SPRX
	};

	class AppLoader
	{
	public:

		explicit AppLoader(std::unique_ptr<uint8_t[]> file) :
			data(std::move(file))
		{}

		virtual ~AppLoader() = default;

		// must be implemented
		virtual LoadErrorCode Load(krnl::Process&) = 0;
		virtual LoadErrorCode Unload() = 0;

	protected:
		std::unique_ptr<uint8_t[]> data;
	};

	std::unique_ptr<AppLoader> CreateLoader(const std::wstring &);
}