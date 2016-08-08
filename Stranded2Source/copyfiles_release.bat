@echo off
echo --- Copying Files
echo Copying StrandedII.exe
pushd Release
xcopy /y /q "StrandedII.exe" "../../StrandedII.exe" > nul
popd
if errorlevel 1 ( echo Failed & exit 1 )
echo Done
@echo on