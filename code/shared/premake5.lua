
project "shared"
    language "C++"
    kind "StaticLib"

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
        "premake5.lua",
        "**.h",
        "**.cpp"
    }