
project "core"
    language "C++"
    kind "StaticLib"

    includedirs {
        ".",
        "../common",
        "../common/utl",

        -- 3rd party
        "../vendor/fmtlib/include",
        "../vendor/capstone/include",
        "../vendor/zlib",
        "../vendor/xbyak/xbyak"
    }

    links {
        "common",
        "capstone",
        "zlib",
		"fmtlib"
    }

    files {
        "premake5.lua",
		"**.inl",
        "**.h",
        "**.hpp",
        "**.cpp",
        "**.c"
    }