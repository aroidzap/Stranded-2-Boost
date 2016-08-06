@echo off
if not "%~2%"=="" ( 
	set build_dir=%~2%
) else ( 
	set build_dir=%cd%
)
echo --- Building Stranded II
set blitzpath=%build_dir%/../Blitz3D
if not exist "%build_dir%/Debug/" mkdir "%build_dir%/Debug/"
"%blitzpath%/bin/blitzcc.exe" -d -o "%build_dir%/Debug/StrandedII.exe" "%build_dir%/source/StrandedII.bb"
@if not "%1"=="-q" ( pause )
@echo on