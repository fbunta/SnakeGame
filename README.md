# Snake Game

This repo was developed for the final project of Udacity's C++ Nanodegree

<img src="snake_game.gif"/>

## Rules of the Game
Snake is a classic arcade game of directing a snake to eat blocks of food which increases the score but subsequently increases the size and speed of the snake. Therefore, each time the score goes up the game gets slightly harder. The only way to lose was to direct the head of the snake into a part of its tail.

In this expansion there are two additional blocks: superfood (green) and danger (red).

The superfood will appear once every few levels and is valuable because it slows down the snake instead of speeding it up. The only catch is that superfood changes position on the board every few seconds making it tricky to capture.

Danger blocks are another way of killing your snake then the traditional running into the tail problem described above. Danger blocks will appear every few levels and never will disappear, making the game harder and harder as the score increases.

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  * Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source.
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.
