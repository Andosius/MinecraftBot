@echo off

pushd ..
Vendor\Binaries\Premake\Windows\premake5.exe --file=Build.lua --os=windows vs2022
popd
pause