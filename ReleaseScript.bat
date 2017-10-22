@echo off

if not exist S2BoostRelease mkdir S2BoostRelease

echo d | xcopy "core" "S2BoostRelease/core" /s /e /y
echo d | xcopy "mods" "S2BoostRelease/mods" /s /e /y

echo f | xcopy "Stranded2Boost.dll" "S2BoostRelease/Stranded2Boost.dll"
echo f | xcopy "StrandedII.exe" "S2BoostRelease/StrandedII.exe"

echo More info at https://github.com/Aroidzap/Stranded-2-Boost > S2BoostRelease/README.txt