-- Minecraft Bot Dependencies

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"
DPP_Dir = "Vendor/Libraries/libdpp-10.0.24-win64-%{cfg.buildcfg}"
VCPKG_Dir = "C:/vcpkg/installed/x64-windows"

IncludeDir = {}
IncludeDir["spdlog"] = "../Dependencies/spdlog/include"
IncludeDir["fmt"] = "../Dependencies/fmt/include"
IncludeDir["json"] = "../Dependencies/nlohmann-json/single_include"

LibraryDir = {}

sys = os.target()

-- Windows specific DPP settings
if sys == "windows" then
    IncludeDir["dpp"] = "../%{DPP_Dir}/include/dpp-10.0"
    LibraryDir["dpp"] = "../%{DPP_Dir}/lib/dpp-10.0"
end

-- Non windows DPP settings
if sys == "macosx" or sys == "linux" then
    IncludeDir["dpp"] = os.findheader("dpp.h")
    LibraryDir["dpp"] = os.findlib("dpp")
end