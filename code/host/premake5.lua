
project "host"
    language "C++"
    kind "WindowedApp"
	flags "NoManifest"
	targetname "%{wks.name}"
	staticruntime "on"

	linkoptions "/ENTRY:start /NODEFAULTLIB /LARGEADDRESSAWARE"
	buildoptions "/GS-"

	-- win/project resource description
	defines { "rsc_company=\"Dev-Force/Alpin-Dev\"",
			  "rsc_copyright=\"(C) Force67 2019-2020. All rights reserved\"",
			  "rsc_fileversion=\"1.0.0.0\"", 
			  "rsc_productversion=\"1.0.0.0\"",
			  "rsc_internalname=\"%{prj.name}\"",
			  "rsc_productname=\"PS4Delta\"",
			  "rsc_originalname=\"%{prj.name}%{prj.targetextension}\"",
		      "rsc_description=\"PS4Delta pre-loader\"" }

    includedirs
    {
        ".",
		"../core",
		"../common"
    }

    files
    {
        "premake5.lua",
		"_res/host.rc",
        "**.h",
        "**.cpp"
    }