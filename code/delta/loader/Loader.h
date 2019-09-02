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
	};

	enum class FileType
	{
		UNKNOWN,
		SELF,
		SPRX
	};

	class AppLoader
	{
	public:

		explicit AppLoader(utl::FileHandle file) :
			file(std::move(file))
		{}

		virtual ~AppLoader() = default;

		// must be implemented
		virtual LoadErrorCode Load(krnl::VMAccessMgr&) = 0;
		virtual LoadErrorCode Unload() = 0;

	protected:
		utl::FileHandle file;
	};

	std::unique_ptr<AppLoader> CreateLoader(const std::wstring &);
}