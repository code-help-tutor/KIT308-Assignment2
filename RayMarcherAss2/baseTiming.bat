@ECHO OFF
set runs=%1
if "%1"=="" set runs=5
@ECHO ON
Release\RayMarcherAss2.exe -runs %runs% -blockSize 16 -size 1024 1024 -samples 1 -input Scenes/cornell.txt     
Release\RayMarcherAss2.exe -runs %runs% -blockSize 16 -size  720 1280 -samples 1 -input Scenes/juggler.txt 
Release\RayMarcherAss2.exe -runs %runs% -blockSize 16 -size  240  135 -samples 1 -input Scenes/5000spheres.txt 
Release\RayMarcherAss2.exe -runs %runs% -blockSize 16 -size 1024 1024 -samples 1 -input Scenes/all.txt 
Release\RayMarcherAss2.exe -runs %runs% -blockSize 16 -size 1024 1024 -samples 1 -input Scenes/spiral.txt
Release\RayMarcherAss2.exe -runs %runs% -blockSize 16 -size  256  256 -samples 1 -input Scenes/sponge.txt
