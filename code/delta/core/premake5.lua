
project "dcore"
    language "C++"
    kind "SharedLib"
	
	defines { "rsc_company=\"AlpinDEV\"",
			  "rsc_copyright=\"© Force67. All rights reserved\"",
			  "rsc_fileversion=\"1.0.0.0\"", 
			  "rsc_productversion=\"1.0.0.0\"",
			  "rsc_internalname=\"%{prj.name}\"",
			  "rsc_productname=\"PS4Delta\"",
			  "rsc_originalname=\"%{prj.name}\"",
		      "rsc_description=\"Delta Core\"" }

    vpaths
    {
        ["*"] = { "**.hpp", "**.h" },
        ["*"] = "**.cpp",
        ["*"] = "premake5.lua"
    }

    includedirs {
        ".",
		"../../shared"
    }

	defines {
		'COMPILING_CORE'
	}
	
	links {
        "shared",
    }

    files {
        "premake5.lua",
		"_res/core.rc",
        "**.h",
        "**.hpp",
        "**.cpp",
		"**.c",
		"*.def",
    }