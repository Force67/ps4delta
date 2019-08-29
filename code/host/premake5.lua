
project "host"
    language "C++"
    kind "WindowedApp"
	optimize "Speed"
	flags "NoManifest"
	targetname "%{wks.name}"

	linkoptions "/ENTRY:wmainCRTStartup"

    vpaths
    {
        ["*"] = "premake5.lua"
    }

    includedirs
    {
        "."
    }

    files
    {
		"_res/host.rc",
        "premake5.lua",
        "**.h",
        "**.cpp"
    }