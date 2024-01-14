project "MineBot"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	--targetdir "Binaries/%{cfg.buildcfg}"
	staticruntime "off"

    -- Output directories
	targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
	objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

    -- Files to build and configurations
    files { 
        "Source/**.cpp", 
        "Source/Include/**.hpp"
    }

    includedirs {
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.fmt}",
        "%{IncludeDir.json}",
        "%{IncludeDir.dpp}",
		"Source/Include"
    }

    defines {
        "SPDLOG_HEADER_ONLY",
        "FMT_HEADER_ONLY",
        "SPDLOG_FMT_EXTERNAL",
        "DPP_BUILD",
        "DPP_USE_EXTERNAL_JSON",
		"FD_SETSIZE=1024",
        "JSON_USE_GLOBAL_UDLS=0",
        "JSON_USE_IMPLICIT_CONVERSIONS=0"
    }

    -- Libraries
    libdirs { 
        "%{LibraryDir.dpp}"
    }

    links { 
        "dpp"
    }


    -- Cross platform settings
        filter "configurations:Debug"
        defines { "DEBUG" }

    filter "configurations:Release"
        defines { "NDEBUG" }

    -- Windows
        -- Configure Debug
    filter { "system:windows", "configurations:Debug" }
        runtime "Debug"
        
        buildoptions { "/bigobj", "/sdl", "/Zc:preprocessor", "/MP", "/Od", "/DEBUG" }
        linkoptions { "/DEBUG" }
        defines { "DEBUG" }

    -- Configure Release
    filter { "system:windows", "configurations:Release" }
        runtime "Release"
        optimize "On"

        buildoptions { "/bigobj", "/sdl", "/Zc:preprocessor", "/MP", "/O2", "/Oi", "/Oy", "/GL", "/Gy" }
        defines { "NDEBUG" }

    -- Copy .dll dependencies to directory on Windows only
    filter { "system:windows" }
        postbuildcommands { "{COPYFILE} %[../%{Binarydir.dpp}/**] %[%{!cfg.targetdir}]" }
    
    -- Linux & macOS
    filter "system:not windows"
        buildoptions { "-pthread", "-fPIC" }

    filter { "system:not windows", "configurations:Debug" }
        buildoptions { "-g", "-Og" }

    filter { "system:not windows", "configurations:Release" }
        buildoptions { "-O3" }
