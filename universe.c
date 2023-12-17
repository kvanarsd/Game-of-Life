#include "universe.h"

#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>

struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

//Creating and allocating memory for the universe
Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    Universe *u = (Universe *) calloc(1, sizeof(Universe));
    assert(u);
    u->rows = rows;
    u->cols = cols;
    u->toroidal = toroidal;

    u->grid = (bool **) calloc(rows, sizeof(bool *));
    for (uint32_t i = 0; i < rows; i++) {
        u->grid[i] = (bool *) calloc(cols, sizeof(bool));
    }

    return u;
}

//free universe
void uv_delete(Universe *u) {
    for (uint32_t i = 0; i < uv_rows(u); i++) {
        free(u->grid[i]);
    }
    free(u->grid);
    free(u);
    return;
}

//return columns
uint32_t uv_rows(Universe *u) {
    uint32_t rows = u->rows;
    return rows;
}

//return columns
uint32_t uv_cols(Universe *u) {
    uint32_t cols = u->cols;
    return cols;
}

//changes cell value to true
void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r < uv_rows(u) && c < uv_cols(u)) {
        u->grid[r][c] = true;
    }
    return;
}

//changes cell value to false
void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r < uv_rows(u) && c < uv_cols(u)) {
        u->grid[r][c] = false;
    }
    return;
}

//returns cell at r, c or false if out of bounds
bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r < uv_rows(u) && c < uv_cols(u)) {
        return u->grid[r][c];
    } else {
        return false;
    }
}

//reads live cell pairs and sets them to true
//if out of bounds return false
bool uv_populate(Universe *u, FILE *infile) {
    uint32_t r, c;
    //read line by line and set cells to live
    while (fscanf(infile, "%" PRIu32 " %" PRIu32, &r, &c) == 2) {
        if (r < uv_rows(u) && c < uv_cols(u)) {
            uv_live_cell(u, r, c);
        } else {
            return false;
        }
    }
    return true;
}

//returns number of live cells adjacent to cell r,c
//valid neighbors are dependent on whether or not
//the universe is toroidal or not
//Loop through r-1 to r+1 and c-1 to r+1
//Don't count r,c as a neighbor to itself
//Using modular arithmetic to find all valid neighbors if toroidal
uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    uint32_t neighbors = 0;
    if (u->toroidal == true) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                int ir = (r + i + uv_rows(u)) % uv_rows(u);
                int jc = (c + j + uv_cols(u)) % uv_cols(u);
                if (i == 0 && j == 0) {
                    continue;
                } else {
                    if (u->grid[ir][jc] == true) {
                        neighbors += 1;
                    }
                }
            }
        }
    } else {
        uint32_t left = r - 1;
        uint32_t top = c - 1;
        if (r == 0) {
            left = 0;
        }
        if (c == 0) {
            top = 0;
        }
        for (uint32_t i = left; i <= r + 1; i++) {
            for (uint32_t j = top; j <= c + 1; j++) {
                if (i < uv_rows(u) && j < uv_cols(u)) {
                    if (i == r && j == c) {
                        continue;
                    } else {

                        if (uv_get_cell(u, i, j) == true) {
                            neighbors += 1;
                        }
                    }
                }
            }
        }
    }
    return neighbors;
}

//print 'o' for a live cell and '.' for a dead cell into an outfile
void uv_print(Universe *u, FILE *outfile) {
    for (uint32_t i = 0; i < uv_rows(u); i++) {
        for (uint32_t j = 0; j < uv_cols(u); j++) {
            if (uv_get_cell(u, i, j) == true) {
                fprintf(outfile, "o");
            } else if (uv_get_cell(u, i, j) == false) {
                fprintf(outfile, ".");
            }
        }
        fprintf(outfile, "\n");
    }
    return;
}
