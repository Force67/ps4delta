
project "imputil"
    language "C++"
    kind "ConsoleApp"

    vpaths
    {
        ["*"] = "premake5.lua"
    }

    includedirs
    {
        ".",
		"../../shared",
    }
	
	links "shared"

    files
    {
        "premake5.lua",
        "*.h",
        "*.cpp",
		"*.c"
    }