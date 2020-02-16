#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <memory>
#include <cstdint>

/*short typedefs*/
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

template<class T>
using SharedPtr = std::shared_ptr<T>;

template<class T>
using UniquePtr = std::unique_ptr<T>;

/*fool intellisense*/
#if defined(__clang__) || defined(__GNUC__)

#define NAKED __attribute__((naked))
#define PACKED __attribute__((packed))
#define PS4ABI __attribute__((sysv_abi)) //, cdecl))
#define NORETURN __attribute__((noreturn))
#define F_INLINE __attribute__((inline_always))

#define bswap16 __builtin_bswap16
#define bswap32 __builtin_bswap32
#define bswap64 __builtin_bswap64

#define rotr16 __builtin_rotateright16
#define rotr32 __builtin_rotateright32
#define rotr64 __builtin_rotateright64

#define rotl16 __builtin_rotateleft16
#define rotl32 __builtin_rotateleft32
#define rotl64 __builtin_rotateleft64

#ifdef _WIN32
#define dbg_break() __debugbreak()
#else
#define dbg_break() asm("int $3")
#endif
#else
#define NAKED naked
#define PACKED
#define PS4ABI
#define NORETURN
#define F_INLINE __forceinline

#define bswap16 _byteswap_ushort
#define bswap32 _byteswap_ulong
#define bswap64 _byteswap_uint64

#define dbg_break() DebugBreak()

#endif

#ifdef _WIN32
#define EXPORT extern "C" __declspec(dllexport)
#elif
#define EXPORT
#endif

#define POW2_MASK (align - static_cast<T>(1))

template <typename T>
inline T align_up(const T addr, const T align) {
    return (addr + POW2_MASK) & ~POW2_MASK;
}

#define BUGCHECK dbg_break