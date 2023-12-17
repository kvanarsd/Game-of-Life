#include "universe.h"

#include <inttypes.h>
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "tsn:i:o:h"

//prints usage
void synopsis(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "	Conway's Game of Life\n"
        "\n"
        "USAGE\n"
        "	%s tsn:i:o:h\n"
        "\n"
        "OPTIONS\n"
        "    -t             Create your universe as a toroidal\n"
        "    -s             Silent - do not animate the evolution\n"
        "    -n {number}    Number of generations [default: 100]\n"
        "    -i {file}      Input file [default: stdin]\n"
        "    -o {file}      Output file [default: stdout]\n",
        exec);
}

//updates ncurses by one generation
void update_ncurses(Universe *u, uint32_t rows, uint32_t cols) {
    clear();
    for (uint32_t row = 0; row < rows; row++) {
        for (uint32_t col = 0; col < cols; col++) {
            if (uv_get_cell(u, row, col) == true) {
                mvprintw(row, col, "o");
            }
        }
    }
    refresh();
    usleep(50000);
    return;
}

int main(int argc, char **argv) {
    //default values
    uint32_t generations = 100;
    FILE *input = stdin;
    FILE *output = stdout;
    char *input_file = NULL;
    char *output_file = NULL;
    bool toroidal = false;
    bool s_flag = false;

    //parse arguments and change default values if called
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 't': toroidal = true; break;
        case 's': s_flag = true; break;
        case 'n': generations = atoi(optarg); break;
        case 'i': input_file = optarg; break;
        case 'o': output_file = optarg; break;
        default: synopsis(argv[0]); return 1;
        }
    }
    //open input file
    //define rows and cols to put first line in
    uint32_t rows, cols;
    FILE *infile;
    if (input_file != NULL) {
        infile = fopen(input_file, "r");
    } else {
        infile = input;
    }
    //read the first line to determine size of universe
    while (fscanf(infile, "%d %d", &rows, &cols) != 2) {
        printf("Malformed input.\n");
        return 1;
    }

    //create two arrays
    Universe *A = uv_create(rows, cols, toroidal);
    Universe *B = uv_create(rows, cols, toroidal);

    //populate universe A with rest of file contents
    if (uv_populate(A, infile) == false) {
        printf("Malformed input.\n");
        return 1;
    }
    //file is no longer needed, close file
    fclose(infile);

    //set up ncurses - referenced from asgn4.pdf
    if (!s_flag) {
        initscr();
        curs_set(FALSE);
    }

    //change B to next generation based on rules
    //if ncurses is enabled refresh screen
    for (uint32_t g = 0; g < generations; g++) {
        for (uint32_t i = 0; i < rows; i++) {
            for (uint32_t j = 0; j < cols; j++) {
                uint32_t neighbors = uv_census(A, i, j);
                if (uv_get_cell(A, i, j) == true) {
                    //live cells
                    //altering universe B for the next generation
                    if (neighbors == 2 || neighbors == 3) {
                        uv_live_cell(B, i, j);
                    } else {
                        uv_dead_cell(B, i, j);
                    }
                } else {
                    //dead cells
                    //altering universe B for the next generation
                    if (neighbors == 3) {
                        uv_live_cell(B, i, j);
                    } else {
                        uv_dead_cell(B, i, j);
                    }
                }
            }
        }
        //update ncurses
        if (!s_flag) {
            update_ncurses(A, rows, cols);
        }
        //change A to next generation
        Universe *temp = A;
        A = B;
        B = temp;
    }
    //update one last time
    //close ncurses
    if (!s_flag) {
        update_ncurses(A, rows, cols);
        endwin();
    }

    //print last generation into output file
    FILE *outfile;
    if (output_file != NULL) {
        outfile = fopen(output_file, "w");
    } else {
        outfile = output;
    }
    uv_print(A, outfile);
    fclose(outfile);

    //free allocated memory
    uv_delete(B);
    uv_delete(A);

    return 0;
}
