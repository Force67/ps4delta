
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
		      "rsc_description=\"Delta PS4 emulation core\"" }

    includedirs {
        ".",
		"../../shared",
		"../../shared/utl",

		-- 3rd party
		"../../vendor/fmtlib/include",
		"../../vendor/capstone/include",
		"../../vendor/zlib",
		"../../vendor/xbyak/xbyak"
    }

	defines {
		'COMPILING_CORE',
		'FMT_HEADER_ONLY'
	}

	links {
        "shared",
		"capstone",
		"zlib"
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
	qtgenerateddir "qtgen"

	-- for ci
	local qtd = os.getenv("Qt5_Dir")
	if qtd then
		qtpath(qtd)
	end
	
	-- use debug versions of qt
	-- libs in debug builds
	configuration { "Debug" }
			qtsuffix "d"
	configuration {}