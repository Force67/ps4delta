
project "zlib"
    language "C"
    kind "StaticLib"
    
    includedirs
    {
        "zlib"
    }

    files
    {
        "zlib/*.h",
        "zlib/*.c"
    }
    
project "capstone"
    language "C"
    kind "StaticLib"
    
    includedirs
    {
        "capstone",
        "capstone/include"
    }
    
    defines
    {
        "CAPSTONE_HAS_X86",
        "CAPSTONE_USE_SYS_DYN_MEM" -- use OS allocators for now
    }

    files
    {
        "capstone/*.c",
        "capstone/*.h",
        "capstone/arch/X86/*.c",
        "capstone/arch/X86/*.inc",
        "capstone/arch/X86/*.h",
    }
    
project "fmtlib"
    language "C++"
    kind "StaticLib"
    
    includedirs
    {
        "fmtlib/include"
    }
    
    files
    {
        "fmtlib/include/fmt/*.h",
        "fmtlib/src/*.cc"
    }
    
project "glfw"
    language "C++"
    kind "StaticLib"
    
    includedirs
    {
        "glfw/include",
        "glfw/src"
    }
    
    defines "_GLFW_WIN32"
    
    files
    {   
        "glfw/include/*.h",
        "glfw/src/context.c",
        "glfw/src/init.c",
        "glfw/src/input.c",
        "glfw/src/monitor.c",
        "glfw/src/vulkan.c",
        "glfw/src/window.c"
    }
    
    filter "system:windows"
            files
            {
                "glfw/src/win32_platform.h",
                "glfw/src/win32_joystick.h",
                "glfw/src/wgl_context.h",
                "glfw/src/egl_context.h",
                "glfw/src/osmesa_context.h",
                "glfw/src/win32_init.c",
                "glfw/src/win32_joystick.c",
                "glfw/src/win32_monitor.c",
                "glfw/src/win32_time.c",
                "glfw/src/win32_thread.c",
                "glfw/src/win32_window.c",
                "glfw/src/wgl_context.c",
                "glfw/src/egl_context.c",
                "glfw/src/osmesa_context.c",
        
            }
    filter{}
    
    
    