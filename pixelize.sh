#!/bin/bash

vid()
{
  ./enc "$1";
  ./create_vid.sh
}
unvid(){
  ./dec
  ./extract_imgs.sh
}
