
# RushHour Solver

Rush Hour solver written in C++.

![Screenshot](/screenshots/screen1.png?raw=true "Puzzle solver")

## Getting Started

The necessary libraries are SDL2 and SDL2-TTF that you can install as below:

### Installing the SDL2-TTF library:

    sudo apt-get install libsdl2-ttf-dev

### Installing the SDL2 library:

    sudo apt-get install libsdl2-dev

## Structure

The application is able to solve a given puzzle that you can add in the /res folder. The algorithm is using depth first search algorithm to find a minimal solution.

The algorithm is making use of sets to store passed already states. There is a random level constructor but also an algorithm based on clustering to find the most maximum distance within a given graph. That's possible as every possible board is generated based on backtracking and taking union of their graphs, thus each distinct cluster
is being solved by marking already passed nodes.

## Miscellaneous

Running project:

    make

For more info and interaction with test sets:

    make help

