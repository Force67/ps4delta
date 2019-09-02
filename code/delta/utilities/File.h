#pragma once

// Copyright (C) Force67 2019

#include <cstdint>
#include <string>
#include <memory>
#include <vector>

namespace utl
{
#ifdef _WIN32
	using native_handle = void*;
#else
	using native_handle = int;
#endif

	enum class fileMode
	{
		read,
		write,
		append,
		create,
		trunc
	};

	enum class seekMode : uint32_t
	{
		seek_set,
		seek_cur,
		seek_end,
	};

	class fileBase
	{
	protected:
		native_handle handle;

	public:

		virtual ~fileBase() = default;

		virtual void Close() { handle = 0; }
		virtual uint64_t Read(void*, size_t) = 0;
		virtual uint64_t Write(const void*, size_t) = 0;
		virtual uint64_t Seek(int64_t, seekMode) = 0;
		virtual uint64_t Tell() = 0;
		virtual uint64_t GetSize() = 0;
		virtual native_handle GetNativeHandle() = 0;
	};

	// for now...
	class File : public fileBase
	{
		size_t sizeTracker;

	public:

		explicit File(const std::wstring&, fileMode mode = fileMode::read);
		~File();

		File(const File&) = delete; // non construction-copyable
		File& operator=(const File&) = delete; // non copyable

		void Close() override;
		virtual uint64_t Read(void*, size_t) override;
		virtual uint64_t Write(const void*, size_t) override;
		virtual uint64_t Seek(int64_t, seekMode) override;
		virtual uint64_t GetSize() override;
		virtual uint64_t Tell() override;
		native_handle GetNativeHandle() override;

		inline bool IsOpen() {
			return handle != nullptr;
		}

		// adapted from rpcs3:

		// POD to std::vector
		template<typename T>
		std::enable_if_t<std::is_pod<T>::value && !std::is_pointer<T>::value, bool> Read(std::vector<T>& vec, std::size_t size)
		{
			vec.resize(size);
			return this->Read(vec.data(), sizeof(T) * size) == sizeof(T) * size;
		}

		// Read POD std::vector, size must be set by resize() method
		template<typename T>
		std::enable_if_t<std::is_pod<T>::value && !std::is_pointer<T>::value, bool> Read(std::vector<T>& vec)
		{
			return this->Read(vec.data(), sizeof(T) * vec.size()) == sizeof(T) * vec.size();
		}

		// Read POD, sizeof(T) is used
		template<typename T>
		std::enable_if_t<std::is_pod<T>::value && !std::is_pointer<T>::value, bool> Read(T& data)
		{
			return Read(&data, sizeof(T)) == sizeof(T);
		}

		// Write POD unconditionally
		template<typename T>
		std::enable_if_t<std::is_pod<T>::value && !std::is_pointer<T>::value, const File&> Write(const T& data)
		{
			Write(std::addressof(data), sizeof(T));
			return *this;
		}

		// Write POD std::vector unconditionally
		template<typename T>
		std::enable_if_t<std::is_pod<T>::value && !std::is_pointer<T>::value, const File&> Write(const std::vector<T>& vec)
		{
			/*if (*/Write(vec.data(), vec.size() * sizeof(T));// != vec.size() * sizeof(T);//) //xfail();
			return *this;
		}
	};

	using FileHandle = std::shared_ptr<File>;
}