@echo off

cd mesheven\projects\mesh-daplink
C:\Keil_v5\UV4\UV4 -b mesh-daplink.uvproj -t"MB0017-LPC11U35" -o"output.txt"
cd ../../../

@echo "Build finished!"
EXIT /B 0
