
project "delta"
    language "C++"
    kind "SharedLib"

    defines { 
        -- WinRC description
        "rsc_company=\"Dev-Force/Alpin-Dev\"",
        "rsc_copyright=\"(C) Force67 2019-2020. All rights reserved\"",
        "rsc_fileversion=\"1.0.0.0\"", 
        "rsc_productversion=\"1.0.0.0\"",
        "rsc_internalname=\"%{prj.name}\"",
        "rsc_productname=\"PS4Delta\"",
        "rsc_originalname=\"%{prj.name}%{prj.targetextension}\"",
        "rsc_description=\"PS4Delta - tiny core\""
    }

    filter "system:windows"
        defines "GLFW_EXPOSE_NATIVE_WIN32"

    includedirs {
        ".",
        "../common",
        "../common/utl",
        "../core",
        "../core/orbis",
        "../video_core",

        -- 3rd party
        "../vendor/fmtlib/include",
        "../vendor/glfw/include",
        "../vendor/yaml-cpp/include"
    }

    links {
        "common",
        "core",
        "video_core",

        "glfw",
        "capstone",
        "zlib",
		"fmtlib",
        "yaml-cpp",
        "simdjson"
    }

    files {
        "premake5.lua",
        "_res/core.rc",
        "**.h",
        "**.cpp",
    }