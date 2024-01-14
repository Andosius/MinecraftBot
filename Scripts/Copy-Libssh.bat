pushd ..\Vendor\vcpkg\installed\x64-windows

REM Return back to installed, vcpkg, Vendor, wks.location
mkdir ..\..\..\..\Dependencies\libssh

REM Copy debug information
mkdir ..\..\..\..\Dependencies\libssh\Debug
mkdir ..\..\..\..\Dependencies\libssh\Debug\lib
xcopy .\debug\lib\*.lib ..\..\..\..\Dependencies\libssh\Debug\lib
mkdir ..\..\..\..\Dependencies\libssh\Debug\include
mkdir ..\..\..\..\Dependencies\libssh\Debug\include\libssh
xcopy .\include\libssh ..\..\..\..\Dependencies\libssh\Debug\include\libssh /e /i /h /y
mkdir ..\..\..\..\Dependencies\libssh\Debug\bin
xcopy .\debug\bin\*.dll ..\..\..\..\Dependencies\libssh\Debug\bin

REM Copy release information
mkdir ..\..\..\..\Dependencies\libssh\Release
mkdir ..\..\..\..\Dependencies\libssh\Release\lib
xcopy .\lib\*.lib ..\..\..\..\Dependencies\libssh\Release\lib
mkdir ..\..\..\..\Dependencies\libssh\Release\include
mkdir ..\..\..\..\Dependencies\libssh\Release\include
xcopy .\include\libssh ..\..\..\..\Dependencies\libssh\Release\include\libssh /e /i /h /y
mkdir ..\..\..\..\Dependencies\libssh\Release\bin
xcopy .\bin\*.dll ..\..\..\..\Dependencies\libssh\Release\bin

popd