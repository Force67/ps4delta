
project "zlib"
    language "C"
    kind "StaticLib"
    
    includedirs
    {
        "zlib"
    }

    files
    {
        "zlib/*.h",
        "zlib/*.c"
    }
    
project "capstone"
    language "C"
    kind "StaticLib"
    
    includedirs
    {
        "capstone",
        "capstone/include"
    }
    
    defines
    {
        "CAPSTONE_HAS_X86",
        "CAPSTONE_USE_SYS_DYN_MEM" -- use OS allocators for now
    }

    files
    {
        "capstone/*.c",
        "capstone/*.h",
        "capstone/arch/X86/*.c",
        "capstone/arch/X86/*.inc",
        "capstone/arch/X86/*.h",
    }
    
project "fmtlib"
    language "C++"
    kind "StaticLib"
    
    includedirs
    {
        "fmtlib/include"
    }
    
    files
    {
        "fmtlib/include/fmt/*.h",
        "fmtlib/src/*.cc"
    }
    
    