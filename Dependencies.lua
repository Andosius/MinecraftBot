-- Minecraft Bot Dependencies

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

IncludeDir = {}
IncludeDir["spdlog"] = "../Dependencies/spdlog/include"
IncludeDir["fmt"] = "../Dependencies/fmt/include"
IncludeDir["json"] = "../Dependencies/nlohmann-json/single_include"

LibraryDir = {}

sys = os.target()

-- Windows specific DPP settings
if sys == "windows" then
    IncludeDir["dpp"] = "../Dependencies/DPP/%{cfg.buildcfg}/include/dpp-10.0"
    LibraryDir["dpp"] = "../Dependencies/DPP/%{cfg.buildcfg}/lib/dpp-10.0"

    BinaryDir = {}
    BinaryDir["dpp"] = "../Dependencies/DPP/%{cfg.buildcfg}/bin"
end

-- Non windows DPP settings
if sys == "macosx" or sys == "linux" then
    IncludeDir["dpp"] = os.findheader("dpp.h")
    LibraryDir["dpp"] = os.findlib("dpp")
end