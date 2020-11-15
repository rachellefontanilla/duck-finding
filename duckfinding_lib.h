#ifndef duckfinding_LIB_H
#define duckfinding_LIB_H

#include <iostream>

int main();
bool game();
void beginGame(char * & board, std::size_t & xdim, std::size_t & ydim, unsigned int & numDucks);
char getAction();
void actionShow(char * & board, std::size_t & xdim, std::size_t & ydim, unsigned int & numDucks);
void actionFlag(char * board, std::size_t xdim, std::size_t ydim);
std::size_t readSizeT();
std::size_t xdim_max();
std::size_t ydim_max();
char flaggedBit();
char concealedBit();
char ducklingMask();
void scatterDucks(char * board, std::size_t xdim, std::size_t ydim, unsigned int numDucks);

#endif
