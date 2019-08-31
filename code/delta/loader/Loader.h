#pragma once

// Copyright (C) Force67 2019

#include <string>
#include <memory>

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
	protected:

		bool loaded;
		uint8_t* data;

		inline void SetLoaded() {
			loaded = true;
		}

	public:

		explicit AppLoader(uint8_t*);
		virtual ~AppLoader() = default;

		// must be implemented
		virtual LoadErrorCode Load(krnl::VMAccessMgr&) = 0;
		virtual LoadErrorCode Unload() = 0;
	};

	std::unique_ptr<AppLoader> CreateLoader(const std::wstring &);
}