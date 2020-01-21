#pragma once

/*
 * UTL : The universal utility library
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <cstdint>
#include <string>
#include <Windows.h>

namespace utl {
class regStream {
public:
    inline explicit regStream(const wchar_t* subKey, HKEY masterKey = HKEY_CURRENT_USER) {
        RegOpenKeyExW(masterKey, subKey, 0, KEY_READ | KEY_WRITE, &key);
    }

    inline ~regStream() {
        close();
    }
    inline bool good() const {
        return key;
    }

    inline void close() {
        if (key) {
            RegCloseKey(key);
            key = nullptr;
        }
    }

    inline void make(const wchar_t* subKey, HKEY masterKey = HKEY_CURRENT_USER) {
        RegCreateKeyW(masterKey, subKey, &key);
    }

    inline bool read(const wchar_t* name, uint8_t* data, size_t size) {
        if (key) {
            DWORD type = REG_SZ;
            DWORD rsize = static_cast<DWORD>(size);
            return RegQueryValueExW(key, name, nullptr, &type, data, &rsize /*size in bytes*/);
        }
        return false;
    }

    inline bool write(const wchar_t* name, const uint8_t* data, size_t size) {
        if (key) {
            return RegSetValueExW(key, name, 0, REG_EXPAND_SZ, data, size /*size in bytes*/);
        }
        return false;
    }

    template <typename T>
    inline bool read(const wchar_t* name, T& val) {
        return read(name, reinterpret_cast<uint8_t*>(&val), sizeof(val));
    }

    template <typename T>
    bool write(const wchar_t* name, const T& val) {
        return write(name, reinterpret_cast<const uint8_t*>(&val), sizeof(val));
    }

    template <typename T>
    inline bool write(const wchar_t* name, const std::basic_string<T>& str) {
        return write(name, reinterpret_cast<const uint8_t*>(str.c_str()),
                     str.size() * sizeof(T) /*wchar compat*/);
    }

    /*user must prepare string*/
    template <typename T>
    inline bool read(const wchar_t* name, std::basic_string<T>& str) {
        return read(name, reinterpret_cast<uint8_t*>(str.data()), str.size() * sizeof(T));
    }

private:
    HKEY key = nullptr;
};
}