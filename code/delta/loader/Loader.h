#pragma once

// Copyright (C) Force67 2019

#include <string>
#include <memory>

#include <utilities/File.h>

namespace krnl {
	class VMAccessMgr;
}

namespace loaders
{
	enum class LoadErrorCode
	{
		UNKNOWN,
		SUCCESS,
		BADMAGIC,
		BADARCH,
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

		explicit AppLoader(std::unique_ptr<utl::File> file) :
			file(std::move(file))
		{}

		virtual ~AppLoader() = default;

		// must be implemented
		virtual LoadErrorCode Load(krnl::VMAccessMgr&) = 0;
		virtual LoadErrorCode Unload() = 0;

	protected:
		std::unique_ptr<utl::File> file;
	};

	std::unique_ptr<AppLoader> CreateLoader(const std::wstring &);
}