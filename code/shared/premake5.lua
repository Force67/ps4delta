
project "shared"
    language "C++"
    kind "StaticLib"

    vpaths
    {
        ["*"] = "premake5.lua"
    }
	
	defines { "FMT_HEADER_ONLY",
              "XBYAK_NO_OP_NAMES" }

    includedirs
    {
        ".",
		"./utl"
    }

    files
    {
        "premake5.lua",
        "**.h",
        "**.cpp",
		"**.cc"
    }