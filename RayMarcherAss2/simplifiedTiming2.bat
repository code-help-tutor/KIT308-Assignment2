@ECHO OFF
set runs=%1
if "%1"=="" set runs=5
@ECHO ON
FastRelease\RayMarcherAss2Simplified.exe -runs %runs% -blockSize 16 -size 1024 1024 -samples 1 -input Scenes/cornell.txt     
FastRelease\RayMarcherAss2Simplified.exe -runs %runs% -blockSize 16 -size  720 1280 -samples 1 -input Scenes/juggler.txt 
FastRelease\RayMarcherAss2Simplified.exe -runs %runs% -blockSize 16 -size  240  135 -samples 1 -input Scenes/5000spheres.txt 

