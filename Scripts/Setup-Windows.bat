@echo off

if not exist ..\Dependencies\libssh (
    echo INFO: libssh not in the Dependencies directory. Trying to find it or build libssh using vcpkg...
    call Build-Libssh.bat
)

pushd ..
Vendor\Binaries\Premake\Windows\premake5.exe --file=Build.lua --os=windows vs2022
popd
pause