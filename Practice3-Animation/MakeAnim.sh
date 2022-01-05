#!/bin/bash
ffmpeg -r 60 -f image2 -s 960x540 -i frame%d.ppm -vcodec libx264 -crf 25 -pix_fmt yuv420p $1
if [ $2 == "-clean" ]; then
    make ppmclean
fi