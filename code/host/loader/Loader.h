#pragma once

// Copyright (C) Force67 2019

#include <string>
#include <memory>

namespace loaders
{
	enum class LoadErrorCode
	{
		UNKNOWN,
		BadSELFMagic,
	};

	enum class FileType
	{
		UNKNOWN,
		SELF,
		SPRX
	};

	class AppLoader
	{
	protected:

		bool loaded;

	public:

		explicit AppLoader();
		virtual ~AppLoader() = default;

		// must be implemented
		virtual LoadErrorCode Load() = 0;
		virtual LoadErrorCode Unload() = 0;
	};

	std::unique_ptr<AppLoader> CreateLoader(const std::wstring &);
}