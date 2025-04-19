#!/bin/bash
ffmpeg -framerate 2 -i encoded_imgs/frame_%02d.png -c:v mpeg4 -q:v 1 output.mp4
