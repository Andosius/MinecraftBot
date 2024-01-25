-- Minecraft Bot Dependencies

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

IncludeDir = {}
IncludeDir["spdlog"] = "../Dependencies/spdlog/include"
IncludeDir["fmt"] = "../Dependencies/fmt/include"
IncludeDir["json"] = "../Dependencies/nlohmann-json/single_include"
IncludeDir["asio"] = "../Dependencies/asio-1.28.0/include"

LibraryDir = {}

sys = os.target()

-- Windows specific DPP settings
if sys == "windows" then
    IncludeDir["dpp"] = "../Dependencies/DPP/%{cfg.buildcfg}/include/dpp-10.0"
    IncludeDir["libssh"] = "../Dependencies/libssh/%{cfg.buildcfg}/include"

    LibraryDir["dpp"] = "../Dependencies/DPP/%{cfg.buildcfg}/lib/dpp-10.0"
    LibraryDir["libssh"] = "../Dependencies/libssh/%{cfg.buildcfg}/lib"

    BinaryDir = {}
    BinaryDir["dpp"] = "../Dependencies/DPP/%{cfg.buildcfg}/bin"
    BinaryDir["libssh"] = "../Dependencies/libssh/%{cfg.buildcfg}/bin"
end

-- Non windows DPP settings
if sys == "macosx" or sys == "linux" then
    IncludeDir["dpp"] = os.findheader("dpp.h")
    IncludeDir["libssh"] = os.findheader("libsshpp.hpp")

    LibraryDir["dpp"] = os.findlib("dpp")
    LibraryDir["libssh"] = os.findlib("ssh")
end