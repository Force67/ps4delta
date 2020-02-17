
project "common"
    language "C++"
    kind "StaticLib"

    includedirs
    {
        ".",
		"./utl",
        "../vendor/fmtlib/include",
        "../vendor/yaml-cpp/include"
    }

    files {
        "premake5.lua",
        "**.h",
        "**.cpp",
		"**.cc"
    }

    filter "system:not windows"
        removefiles {"**_win.cpp", "**_win.h"}
    filter {}
