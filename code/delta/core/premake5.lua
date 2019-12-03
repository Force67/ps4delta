
project "dcore"
    language "C++"
    kind "SharedLib"
	
	-- win/project resource description
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
		"../../shared",
		"../../shared/fmtlib", --temp
		"../../shared/utl"
    }

	defines {
		'COMPILING_CORE'
	}

	defines "FMT_HEADER_ONLY"
	
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
	
	-- enable qt for this project
	qt.enable()
	
	qtmodules {
		"core",
		"gui",
		"widgets",
		"opengl"
	}
	
	qtprefix "Qt5"
	qtpath "C:/Qt/Qt5.12.0/5.12.0/msvc2017_64"
	
	-- use debug versions of qt
	-- for debug builds
	configuration { "Debug" }
			qtsuffix "d"
	configuration { }