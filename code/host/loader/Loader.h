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

		const std::wstring& file;
		bool loaded;

	public:

		explicit AppLoader(const std::wstring&);

		// must be implemented
		virtual LoadErrorCode Load() = 0;
		virtual LoadErrorCode Unload() = 0;
	};

	std::unique_ptr<AppLoader> CreateLoader();
}