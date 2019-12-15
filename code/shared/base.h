#pragma once

#if defined(__clang__) || defined(__GNUC__)

#define NAKED    __attribute__((naked))
#define PACKED   __attribute__((packed))
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

#error Your compiler is not supported!

#define NAKED		naked
#define PACKED		
#define PS4ABI	
#define NORETURN	
#define F_INLINE __forceinline 

#define bswap16 _byteswap_ushort
#define bswap32 _byteswap_ulong
#define bswap64 _byteswap_uint64

#define dbg_break() DebugBreak()

#endif