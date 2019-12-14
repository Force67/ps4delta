
project "shared"
    language "C++"
    kind "StaticLib"

	defines { "FMT_HEADER_ONLY",
              "XBYAK_NO_OP_NAMES" }

    includedirs
    {
        ".",
		"./utl",
        "../vendor/fmtlib/include"
    }

    files
    {
        "premake5.lua",
        "**.h",
        "**.cpp",
		"**.cc"
    }