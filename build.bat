@echo off

cd meshProjectfiles\uvision\lpc11u35_no_targets_if
C:\Keil_v5\UV4\UV4 -b lpc11u35_no_targets_if.uvproj -t"lpc11u35_no_targets_if" -o"output.txt"
cd ../../../

@echo "Build finished!"
EXIT /B 0
