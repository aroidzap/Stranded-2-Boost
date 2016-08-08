@echo off
echo --- Copying Files
echo Copying Stranded2Boost.dll
pushd Release
xcopy /y /q "Stranded2Boost.dll" "../../Stranded2Boost.dll" > nul
popd
if errorlevel 1 ( echo Failed & exit 1 )
echo Copying Stranded2Boost.decls
for /f "tokens=*" %%a in ('call ../config.bat') do set blitzfolder=%%a
xcopy /y /q "Stranded2Boost.decls" "../%blitzfolder%/userlibs/Stranded2Boost.decls" > nul
if errorlevel 1 ( echo Failed & exit 1 )
echo Done
@echo on