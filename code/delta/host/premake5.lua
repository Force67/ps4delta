
project "host"
    language "C++"
    kind "WindowedApp"
	flags "NoManifest"
	targetname "%{wks.name}"

	linkoptions "/ENTRY:mainCRTStartup"

	-- win/project resource description
	defines { "rsc_company=\"Dev-Force/Alpin-Dev\"",
			  "rsc_copyright=\"© Force67 2019. All rights reserved\"",
			  "rsc_fileversion=\"1.0.0.0\"", 
			  "rsc_productversion=\"1.0.0.0\"",
			  "rsc_internalname=\"%{prj.name}\"",
			  "rsc_productname=\"PS4Delta\"",
			  "rsc_originalname=\"%{prj.name}%{prj.targetextension}\"",
		      "rsc_description=\"Delta HLE PS4 emulator\"" }

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