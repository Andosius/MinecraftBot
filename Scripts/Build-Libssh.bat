@echo off

if not exist ..\Dependencies\libssh (

	pushd ..\Vendor

	REM 
	if not exist vcpkg (
		echo [GITHUB] Cloning vcpkg from github...
		git clone https://github.com/microsoft/vcpkg vcpkg --quiet
		
		pushd vcpkg
		echo [VCPKG] Disabling vcpkg metrics
		type NUL > vcpkg.disable-metrics
		call bootstrap-vcpkg.bat >NUL
		popd
	)

	REM Not the best way to check for packages being installed but it works...
	if not exist vcpkg\installed\x64-windows\include\libssh (
		pushd vcpkg
		echo [VCPKG/BUILD] Building libssh! This may take a while...
		.\vcpkg.exe install libssh:x64-windows >NUL
		popd
	)

	REM Return to Scripts
	popd
	
	echo [COPY] Copy libssh files to Dependencies/libssh...
	call Copy-Libssh.bat >NUL
	
	echo Done setting up libssh!
)