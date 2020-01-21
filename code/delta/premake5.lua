
project "delta"
    language "C++"
    kind "SharedLib"

    -- win/project resource description
    defines { "rsc_company=\"Dev-Force/Alpin-Dev\"",
              "rsc_copyright=\"© Force67 2019-2020. All rights reserved\"",
              "rsc_fileversion=\"1.0.0.0\"", 
              "rsc_productversion=\"1.0.0.0\"",
              "rsc_internalname=\"%{prj.name}\"",
              "rsc_productname=\"PS4Delta\"",
              "rsc_originalname=\"%{prj.name}%{prj.targetextension}\"",
              "rsc_description=\"PS4Delta core\"" }

    includedirs {
        ".",
        "../common",
        "../common/utl",
        "../core",

        -- 3rd party
        "../vendor/fmtlib/include",
        "../vendor/capstone/include",
        "../vendor/zlib",
        "../vendor/xbyak/xbyak"
    }

    links {
        "common",
        "core",

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