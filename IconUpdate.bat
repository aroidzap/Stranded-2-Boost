@echo off
echo --- Updating Icon
if not "%~2%"=="" ( 
	set build_dir=%~2%
) else ( 
	set build_dir=%cd%
)
if not exist "%build_dir%/icon.ico" (
	echo Couldn't find "icon.ico"
	set err=1
) else if not exist "%build_dir%/StrandedII.exe" (
	echo Couldn't find "StrandedII.exe"
	set err=1
) else (
	pushd %build_dir%
	"./ResourceHacker/ResourceHacker.exe" -delete StrandedII.exe, StrandedII.exe, icongroup, ,
	"./ResourceHacker/ResourceHacker.exe" -addoverwrite StrandedII.exe, StrandedII.exe , icon.ico, icongroup, IDR_MAINFRAME,
	popd
	echo Icon Replaced /if you can't see any change, please rebuild your icon cache/
	set err=0
)
@echo on
@if not "%1"=="-q" ( pause )
@if %err%==1 ( exit 1 )
