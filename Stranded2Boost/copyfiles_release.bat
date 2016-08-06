@echo off
echo --- Copying Files
echo Copying Stranded2Boost.dll
cp "./Release/Stranded2Boost.dll" "../"
echo Copying Stranded2Boost.decls
cp "./Stranded2Boost.decls" "../Blitz3D/userlibs/"
echo Done
@echo on