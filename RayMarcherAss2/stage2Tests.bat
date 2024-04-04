Release\Stage2.exe -runs 1 -blockSize 8 -size 8 8 -samples 1 -threads 1 -output Outputs/stage2_01.bmp -input Scenes/cornell.txt           
Release\Stage2.exe -runs 1 -blockSize 8 -size 8 8 -samples 1 -threads 1 -output Outputs/stage2_02.bmp -input Scenes/juggler.txt 
Release\Stage2.exe -runs 1 -blockSize 8 -size 8 8 -samples 1 -threads 1 -output Outputs/stage2_03.bmp -input Scenes/5000spheres.txt 

magick compare -metric mae Outputs\stage2_01.bmp Outputs_REFERENCE\stage0_01.bmp Outputs\stage2diff_01.bmp
magick compare -metric mae Outputs\stage2_02.bmp Outputs_REFERENCE\stage0_02.bmp Outputs\stage2diff_02.bmp
magick compare -metric mae Outputs\stage2_03.bmp Outputs_REFERENCE\stage0_03.bmp Outputs\stage2diff_03.bmp

Release\Stage2.exe -runs 1 -blockSize 16 -size 256 256 -samples 1 -output Outputs/stage2_11.bmp -input Scenes/cornell.txt     
Release\Stage2.exe -runs 1 -blockSize 16 -size 256 256 -samples 1 -output Outputs/stage2_12.bmp -input Scenes/juggler.txt 
Release\Stage2.exe -runs 1 -blockSize 16 -size 256 256 -samples 1 -output Outputs/stage2_13.bmp -input Scenes/5000spheres.txt 

magick compare -metric mae Outputs\stage2_11.bmp Outputs_REFERENCE\stage0_11.bmp Outputs\stage2diff_11.bmp
magick compare -metric mae Outputs\stage2_12.bmp Outputs_REFERENCE\stage0_12.bmp Outputs\stage2diff_12.bmp
magick compare -metric mae Outputs\stage2_13.bmp Outputs_REFERENCE\stage0_13.bmp Outputs\stage2diff_13.bmp

Release\Stage2.exe -runs 1 -blockSize 16 -size 256 256 -samples 2 -output Outputs/stage2_21.bmp -input Scenes/cornell.txt      
Release\Stage2.exe -runs 1 -blockSize 16 -size 256 256 -samples 2 -output Outputs/stage2_22.bmp -input Scenes/juggler.txt 
Release\Stage2.exe -runs 1 -blockSize 16 -size 256 256 -samples 2 -output Outputs/stage2_23.bmp -input Scenes/5000spheres.txt 
                                                                                
magick compare -metric mae Outputs\stage2_21.bmp Outputs_REFERENCE\stage0_21.bmp Outputs\stage2diff_21.bmp
magick compare -metric mae Outputs\stage2_22.bmp Outputs_REFERENCE\stage0_22.bmp Outputs\stage2diff_22.bmp
magick compare -metric mae Outputs\stage2_23.bmp Outputs_REFERENCE\stage0_23.bmp Outputs\stage2diff_23.bmp
