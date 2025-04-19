#!/bin/bash
ffmpeg -i output.mp4 -vf "fps=2" extracted/frame_%02d.png
