#Asignment 4: Game of Life

## Description:
This program contains an implementation of Conway's Game of Life. It creates two Universes where the second Universe is the next generation. Once the first Universe is printed the two Universes swap values.

## Build: 
Make sure universe.c, universe.h, and life.c are in the directory. If you would like to use a text file's data points include the file in the directory as well. Run $ make in order to generate executable 'life'. Running ./life with an invalid argument will print the usage.

## Cleaning:
To clean the directory after bulding run '$ make clean' to remove the executable 'life' and .o files from the directory.

## Running:
Run $./life with arguments "tsn:i:o:". -i is required to use a files data points, otherwise type data points into the command line with the first line being the size of the universe. -t specifies the universe as toroidal. -s disables ncurses. -n with any number specifies the number of generations the universe goes through. -o with an output file name will send the last generation of the universe to said file, by default the output will print on the terminal.

