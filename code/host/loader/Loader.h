#pragma once

// Copyright (C) Force67 2019

#include <string>
#include <memory>

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
	protected:

		bool loaded;
		uint8_t* data;

	public:

		explicit AppLoader(uint8_t*);
		virtual ~AppLoader() = default;

		// must be implemented
		virtual LoadErrorCode Load() = 0;
		virtual LoadErrorCode Unload() = 0;
	};

	std::unique_ptr<AppLoader> CreateLoader(const std::wstring &);
}