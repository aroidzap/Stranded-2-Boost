@echo off
echo --- Copying Files
echo Copying Stranded2Boost.dll
cp "./Release/Stranded2Boost.dll" "../"
echo Copying Stranded2Boost.decls
for /f "tokens=*" %%a in ('call ../config.bat') do set blitzfolder=%%a
cp "./Stranded2Boost.decls" "../%blitzfolder%/userlibs/"
echo Done
@echo on