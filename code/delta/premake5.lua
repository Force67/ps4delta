
project "host"
    language "C++"
    kind "WindowedApp"
	optimize "Speed"
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
        ["*"] = "premake5.lua"
    }
	
	links
	{
		"shared"
	}

    includedirs
    {
        ".",
		"../shared"
    }

    files
    {
		"_res/host.rc",
        "premake5.lua",
        "**.h",
        "**.cpp"
    }