
project "shared"
    language "C++"
    kind "StaticLib"

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