@echo off
if not "%~2%"=="" ( 
	set build_dir=%~2%
) else ( 
	set build_dir=%cd%
)
echo --- Building Stranded II
set blitzpath=%build_dir%/../Blitz3D
if not exist "%build_dir%/Release/" mkdir "%build_dir%/Release/"
"%blitzpath%/bin/blitzcc.exe" -o "%build_dir%/Release/StrandedII.exe" "%build_dir%/source/StrandedII.bb"
@if not "%1"=="-q" ( pause )
@echo on
@if not %errorlevel%==0 (exit %errorlevel%)