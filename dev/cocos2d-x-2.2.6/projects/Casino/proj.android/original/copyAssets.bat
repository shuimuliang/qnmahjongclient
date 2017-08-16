@echo off

if not exist ..\..\Resources md ..\..\Resources

setlocal enabledelayedexpansion

for %%i in (*.Casino) do (
	set prtofile=%%i
	set pbfile=!prtofile:Casino=..\..\Resources!
	protoc -o ..\..\Resources/!pbfile! !prtofile!
	echo convert [!prtofile!] ok
)

xcopy /s ..\..\Resources\* assets
pause