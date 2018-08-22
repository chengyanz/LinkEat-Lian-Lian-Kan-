# Link and Eat (Lian Lian Kan)

## Contributors
Chengyan Zhan
Yimeng Lei

## Description
This is a game where the player links pairs of identical images in a grid of images within a time limit. The pair of images must be connected by a path that does not go through any image, and the path can have at most two turns. Once a valid pair of images are linked, they disappear. The player can shuffle the images at any stage of the game.

## Build
For instructions to install Qt on ubuntu, see https://wiki.qt.io/Install_Qt_5_on_Ubuntu
Make sure OpenGL libraries are installed following this link.
To build the game, run
> qmake -project <br />
> qmake <br />
> make

Before running make, make sure that -std=gnu++11 is specified in CXXFLAGS in Makefile. Also add the line 
> QT += widgets <br />
in directory_name.pro

## Play
> ./directory_name
