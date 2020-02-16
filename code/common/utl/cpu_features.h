#pragma once

/*
 * UTL : The universal utility library
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <cstdint>
#include <intrin.h>

// forked from xbyak

namespace utl {
class cpu {
    uint64_t type_;

    unsigned int get32bitAsBE(const char* x) const {
        return x[0] | (x[1] << 8) | (x[2] << 16) | (x[3] << 24);
    }

    unsigned int mask(int n) const {
        return (1U << n) - 1;
    }

    unsigned int extractBit(unsigned int val, unsigned int base, unsigned int end) {
        return (val >> base) & ((1u << (end - base)) - 1);
    }

public:
    /*
        data[] = { eax, ebx, ecx, edx }
    */
    static inline void getCpuid(unsigned int eaxIn, unsigned int data[4]) {
#ifdef _MSC_VER
        __cpuid(reinterpret_cast<int*>(data), eaxIn);
#else
        __cpuid(eaxIn, data[0], data[1], data[2], data[3]);
#endif
    }

    static inline void getCpuidEx(unsigned int eaxIn, unsigned int ecxIn, unsigned int data[4]) {
#ifdef _MSC_VER
        __cpuidex(reinterpret_cast<int*>(data), eaxIn, ecxIn);
#else
        __cpuid_count(eaxIn, ecxIn, data[0], data[1], data[2], data[3]);
#endif
    }

    static inline uint64_t getXfeature() {
#ifdef _MSC_VER
        return _xgetbv(0);
#else
        unsigned int eax, edx;
        // xgetvb is not support on gcc 4.2
        //		__asm__ volatile("xgetbv" : "=a"(eax), "=d"(edx) : "c"(0));
        __asm__ volatile(".byte 0x0f, 0x01, 0xd0" : "=a"(eax), "=d"(edx) : "c"(0));
        return ((uint64_t)edx << 32) | eax;
#endif
    }

    using Type = uint64_t;

    static constexpr Type NONE = 0;
    static constexpr Type tMMX = 1 << 0;
    static constexpr Type tMMX2 = 1 << 1;
    static constexpr Type tCMOV = 1 << 2;
    static constexpr Type tSSE = 1 << 3;
    static constexpr Type tSSE2 = 1 << 4;
    static constexpr Type tSSE3 = 1 << 5;
    static constexpr Type tSSSE3 = 1 << 6;
    static constexpr Type tSSE41 = 1 << 7;
    static constexpr Type tSSE42 = 1 << 8;
    static constexpr Type tPOPCNT = 1 << 9;
    static constexpr Type tAESNI = 1 << 10;
    static constexpr Type tSSE5 = 1 << 11;
    static constexpr Type tOSXSAVE = 1 << 12;
    static constexpr Type tPCLMULQDQ = 1 << 13;
    static constexpr Type tAVX = 1 << 14;
    static constexpr Type tFMA = 1 << 15;

    static constexpr Type t3DN = 1 << 16;
    static constexpr Type tE3DN = 1 << 17;
    static constexpr Type tSSE4a = 1 << 18;
    static constexpr Type tRDTSCP = 1 << 19;
    static constexpr Type tAVX2 = 1 << 20;
    static constexpr Type tBMI1 = 1 << 21; // andn, bextr, blsi, blsmsk, blsr, tzcnt
    static constexpr Type tBMI2 = 1 << 22; // bzhi, mulx, pdep, pext, rorx, sarx, shlx, shrx
    static constexpr Type tLZCNT = 1 << 23;

    static constexpr Type tINTEL = 1 << 24;
    static constexpr Type tAMD = 1 << 25;

    static constexpr Type tENHANCED_REP = 1 << 26; // enhanced rep movsb/stosb
    static constexpr Type tRDRAND = 1 << 27;
    static constexpr Type tADX = 1 << 28;           // adcx, adox
    static constexpr Type tRDSEED = 1 << 29;        // rdseed
    static constexpr Type tSMAP = 1 << 30;          // stac
    static constexpr Type tHLE = uint64_t(1) << 31;   // xacquire, xrelease, xtest
    static constexpr Type tRTM = uint64_t(1) << 32;   // xbegin, xend, xabort
    static constexpr Type tF16C = uint64_t(1) << 33;  // vcvtph2ps, vcvtps2ph
    static constexpr Type tMOVBE = uint64_t(1) << 34; // mobve
    static constexpr Type tAVX512F = uint64_t(1) << 35;
    static constexpr Type tAVX512DQ = uint64_t(1) << 36;
    static constexpr Type tAVX512_IFMA = uint64_t(1) << 37;
    static constexpr Type tAVX512IFMA = tAVX512_IFMA;
    static constexpr Type tAVX512PF = uint64_t(1) << 38;
    static constexpr Type tAVX512ER = uint64_t(1) << 39;
    static constexpr Type tAVX512CD = uint64_t(1) << 40;
    static constexpr Type tAVX512BW = uint64_t(1) << 41;
    static constexpr Type tAVX512VL = uint64_t(1) << 42;
    static constexpr Type tAVX512_VBMI = uint64_t(1) << 43;
    static constexpr Type tAVX512VBMI = tAVX512_VBMI; // changed by Intel's manual
    static constexpr Type tAVX512_4VNNIW = uint64_t(1) << 44;
    static constexpr Type tAVX512_4FMAPS = uint64_t(1) << 45;
    static constexpr Type tPREFETCHWT1 = uint64_t(1) << 46;
    static constexpr Type tPREFETCHW = uint64_t(1) << 47;
    static constexpr Type tSHA = uint64_t(1) << 48;
    static constexpr Type tMPX = uint64_t(1) << 49;
    static constexpr Type tAVX512_VBMI2 = uint64_t(1) << 50;
    static constexpr Type tGFNI = uint64_t(1) << 51;
    static constexpr Type tVAES = uint64_t(1) << 52;
    static constexpr Type tVPCLMULQDQ = uint64_t(1) << 53;
    static constexpr Type tAVX512_VNNI = uint64_t(1) << 54;
    static constexpr Type tAVX512_BITALG = uint64_t(1) << 55;
    static constexpr Type tAVX512_VPOPCNTDQ = uint64_t(1) << 56;
    static constexpr Type tAVX512_BF16 = uint64_t(1) << 57;
    static constexpr Type tAVX512_VP2INTERSECT = uint64_t(1) << 58;

    cpu()
        : type_(NONE) {

        unsigned int data[4] = {};
        const unsigned int& EAX = data[0];
        const unsigned int& EBX = data[1];
        const unsigned int& ECX = data[2];
        const unsigned int& EDX = data[3];
        getCpuid(0, data);
        const unsigned int maxNum = EAX;
        static const char intel[] = "ntel";
        static const char amd[] = "cAMD";
        if (ECX == get32bitAsBE(amd)) {
            type_ |= tAMD;
            getCpuid(0x80000001, data);
            if (EDX & (1U << 31))
                type_ |= t3DN;
            if (EDX & (1U << 15))
                type_ |= tCMOV;
            if (EDX & (1U << 30))
                type_ |= tE3DN;
            if (EDX & (1U << 22))
                type_ |= tMMX2;
            if (EDX & (1U << 27))
                type_ |= tRDTSCP;
        }
        if (ECX == get32bitAsBE(intel)) {
            type_ |= tINTEL;
            getCpuid(0x80000001, data);
            if (EDX & (1U << 27))
                type_ |= tRDTSCP;
            if (ECX & (1U << 5))
                type_ |= tLZCNT;
            if (ECX & (1U << 8))
                type_ |= tPREFETCHW;
        }
        getCpuid(1, data);
        if (ECX & (1U << 0))
            type_ |= tSSE3;
        if (ECX & (1U << 9))
            type_ |= tSSSE3;
        if (ECX & (1U << 19))
            type_ |= tSSE41;
        if (ECX & (1U << 20))
            type_ |= tSSE42;
        if (ECX & (1U << 22))
            type_ |= tMOVBE;
        if (ECX & (1U << 23))
            type_ |= tPOPCNT;
        if (ECX & (1U << 25))
            type_ |= tAESNI;
        if (ECX & (1U << 1))
            type_ |= tPCLMULQDQ;
        if (ECX & (1U << 27))
            type_ |= tOSXSAVE;
        if (ECX & (1U << 30))
            type_ |= tRDRAND;
        if (ECX & (1U << 29))
            type_ |= tF16C;

        if (EDX & (1U << 15))
            type_ |= tCMOV;
        if (EDX & (1U << 23))
            type_ |= tMMX;
        if (EDX & (1U << 25))
            type_ |= tMMX2 | tSSE;
        if (EDX & (1U << 26))
            type_ |= tSSE2;

        if (type_ & tOSXSAVE) {
            // check XFEATURE_ENABLED_MASK[2:1] = '11b'
            uint64_t bv = getXfeature();
            if ((bv & 6) == 6) {
                if (ECX & (1U << 28))
                    type_ |= tAVX;
                if (ECX & (1U << 12))
                    type_ |= tFMA;
                if (((bv >> 5) & 7) == 7) {
                    getCpuidEx(7, 0, data);
                    if (EBX & (1U << 16))
                        type_ |= tAVX512F;
                    if (type_ & tAVX512F) {
                        if (EBX & (1U << 17))
                            type_ |= tAVX512DQ;
                        if (EBX & (1U << 21))
                            type_ |= tAVX512_IFMA;
                        if (EBX & (1U << 26))
                            type_ |= tAVX512PF;
                        if (EBX & (1U << 27))
                            type_ |= tAVX512ER;
                        if (EBX & (1U << 28))
                            type_ |= tAVX512CD;
                        if (EBX & (1U << 30))
                            type_ |= tAVX512BW;
                        if (EBX & (1U << 31))
                            type_ |= tAVX512VL;
                        if (ECX & (1U << 1))
                            type_ |= tAVX512_VBMI;
                        if (ECX & (1U << 6))
                            type_ |= tAVX512_VBMI2;
                        if (ECX & (1U << 8))
                            type_ |= tGFNI;
                        if (ECX & (1U << 9))
                            type_ |= tVAES;
                        if (ECX & (1U << 10))
                            type_ |= tVPCLMULQDQ;
                        if (ECX & (1U << 11))
                            type_ |= tAVX512_VNNI;
                        if (ECX & (1U << 12))
                            type_ |= tAVX512_BITALG;
                        if (ECX & (1U << 14))
                            type_ |= tAVX512_VPOPCNTDQ;
                        if (EDX & (1U << 2))
                            type_ |= tAVX512_4VNNIW;
                        if (EDX & (1U << 3))
                            type_ |= tAVX512_4FMAPS;
                        if (EDX & (1U << 8))
                            type_ |= tAVX512_VP2INTERSECT;
                    }
                    // EAX=07H, ECX=1
                    getCpuidEx(7, 1, data);
                    if (type_ & tAVX512F) {
                        if (EAX & (1U << 5))
                            type_ |= tAVX512_BF16;
                    }
                }
            }
        }
        if (maxNum >= 7) {
            getCpuidEx(7, 0, data);
            if (type_ & tAVX && (EBX & (1U << 5)))
                type_ |= tAVX2;
            if (EBX & (1U << 3))
                type_ |= tBMI1;
            if (EBX & (1U << 8))
                type_ |= tBMI2;
            if (EBX & (1U << 9))
                type_ |= tENHANCED_REP;
            if (EBX & (1U << 18))
                type_ |= tRDSEED;
            if (EBX & (1U << 19))
                type_ |= tADX;
            if (EBX & (1U << 20))
                type_ |= tSMAP;
            if (EBX & (1U << 4))
                type_ |= tHLE;
            if (EBX & (1U << 11))
                type_ |= tRTM;
            if (EBX & (1U << 14))
                type_ |= tMPX;
            if (EBX & (1U << 29))
                type_ |= tSHA;
            if (ECX & (1U << 0))
                type_ |= tPREFETCHWT1;
        }
    }

    bool has(Type type) const {
        return (type & type_) != 0;
    }
};
}