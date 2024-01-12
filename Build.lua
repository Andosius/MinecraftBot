workspace "MinecraftBot"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "MineBot"

    location "build"


include "Dependencies.lua"
include "MineBot/Build-MineBot.lua"
