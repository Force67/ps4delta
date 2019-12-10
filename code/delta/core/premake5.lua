
project "dcore"
    language "C++"
    kind "SharedLib"
	
	-- win/project resource description
	defines { "rsc_company=\"Dev-Force/Alpin-Dev\"",
			  "rsc_copyright=\"© Force67 2019. All rights reserved\"",
			  "rsc_fileversion=\"1.0.0.0\"", 
			  "rsc_productversion=\"1.0.0.0\"",
			  "rsc_internalname=\"%{prj.name}\"",
			  "rsc_productname=\"PS4Delta\"",
			  "rsc_originalname=\"%{prj.name}%{prj.targetextension}\"",
		      "rsc_description=\"Delta HLE emulation core\"" }

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
		"../../shared/utl",
		"../../shared/capstone"
    }

	defines {
		'COMPILING_CORE',
		'FMT_HEADER_ONLY'
	}

	libdirs {
		"../../shared/capstone"
	}

	links {
        "shared",
		"capstone"
    }

    files {
        "premake5.lua",
		"_res/core.rc",
		"**.ui",
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

	filter "system:windows"
		qtmodules {
			"winextras",
			"qml"
		}
	filter {}
	
	qtprefix "Qt5"

	-- TODO: use QT_ENV var
	qtpath "C:/Qt/Qt5.12.0/5.12.0/msvc2017_64"
	qtgenerateddir "qtgen"
	
	-- use debug versions of qt
	-- libs in debug builds
	configuration { "Debug" }
			qtsuffix "d"
	configuration {}