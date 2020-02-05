
project "delta"
    language "C++"
    kind "SharedLib"

    -- win/project resource description
    defines { "rsc_company=\"Dev-Force/Alpin-Dev\"",
              "rsc_copyright=\"(C) Force67 2019-2020. All rights reserved\"",
              "rsc_fileversion=\"1.0.0.0\"", 
              "rsc_productversion=\"1.0.0.0\"",
              "rsc_internalname=\"%{prj.name}\"",
              "rsc_productname=\"PS4Delta\"",
              "rsc_originalname=\"%{prj.name}%{prj.targetextension}\"",
              "rsc_description=\"PS4Delta - tiny core\"" }

    includedirs {
        ".",
        "../common",
        "../common/utl",
        "../core",
        "../core/orbis",
        "../video_core",

        -- 3rd party
        "../vendor/fmtlib/include",
        "../vendor/capstone/include",
        "../vendor/zlib",
        "../vendor/xbyak/xbyak",
        "../vendor/glfw/include"
    }

    links {
        "common",
        "core",
        "video_core",

        "glfw",
        "capstone",
        "zlib",
		"fmtlib"
    }

    files {
        "premake5.lua",
        "_res/core.rc",
		"**.inl",
        "**.h",
        "**.hpp",
        "**.cpp",
        "**.c"
    }