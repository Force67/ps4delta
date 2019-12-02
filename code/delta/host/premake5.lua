
project "host"
    language "C++"
    kind "WindowedApp"
	flags "NoManifest"
	targetname "%{wks.name}"

	linkoptions "/ENTRY:wmainCRTStartup"

	defines { "rsc_company=\"AlpinDEV\"",
			  "rsc_copyright=\"© Force67. All rights reserved\"",
			  "rsc_fileversion=\"1.0.0.0\"", 
			  "rsc_productversion=\"1.0.0.0\"",
			  "rsc_internalname=\"%{prj.name}\"",
			  "rsc_productname=\"PS4Delta\"",
			  "rsc_originalname=\"%{prj.name}\"",
		      "rsc_description=\"Delta Host\"" }

    vpaths
    {
        ["*"] = { "**.hpp", "**.h" },
        ["*"] = "**.cpp",
        ["*"] = "premake5.lua"
    }
	
	links
	{
		"shared"
	}

    includedirs
    {
        ".",
		"../../shared"
    }

    files
    {
        "premake5.lua",
		"_res/host.rc",
        "**.h",
        "**.cpp"
    }