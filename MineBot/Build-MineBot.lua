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
        "%{IncludeDir.libssh}",
        "%{IncludeDir.asio}",
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
        "JSON_USE_IMPLICIT_CONVERSIONS=0",
        "ASIO_STANDALONE"
    }

    -- Libraries
    libdirs { 
        "%{LibraryDir.dpp}",
        "%{LibraryDir.libssh}"
    }

    links { 
        "dpp",
        "ssh"
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
        
        buildoptions { "/bigobj", "/sdl", "/Zc:preprocessor", "/MP", "/Od", "/DEBUG", "/utf-8" }
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
        postbuildcommands {
            "{COPYFILE} %[%{BinaryDir.dpp}/**] %[%{!cfg.targetdir}]",
            --"{COPYFILE} %[%{BinaryDir.libssh}/**] %[%{!cfg.targetdir}]"
        }
    
    -- Linux & macOS
    filter { "system:not windows", "configurations:Debug" }
        buildoptions { "-pthread", "-fPIC", "-g", "-Og" }

    filter { "system:not windows", "configurations:Release" }
        buildoptions { "-pthread", "-fPIC", "-O3" }

    filter{}
    vpaths {
        ["Commands"] = { 
            "Source/commands/*.cpp", "Source/Include/commands/*.hpp",
            "Source/CommandMutex.cpp", "Source/Include/CommandMutex.hpp",
            "Source/Include/command.hpp", "Source/Include/Commands.hpp"
        },
        ["Models"] = { 
            "Source/Operator.cpp", "Source/Include/Operator.hpp",
            "Source/WhitelistUser.cpp", "Source/Include/WhitelistUser.hpp",
            "Source/DiscordUser.cpp", "Source/Include/DiscordUser.hpp",
            "Source/Config.cpp", "Source/Include/Config.hpp"
        },
        ["Utility"] = {
            "Source/Log.cpp", "Source/Include/Log.hpp",
            "Source/Utility.cpp", "Source/Include/Utility.hpp"
        },
        ["Networking"] = {
            "Source/SourceRcon.cpp", "Source/Include/SourceRcon.hpp"
        },
        ["Core"] = {
            "Source/Application.cpp", "Source/Include/Application.hpp",
            "Source/Core.cpp", "Source/Include/Core.hpp",
            "Source/main.cpp"
        }
    }