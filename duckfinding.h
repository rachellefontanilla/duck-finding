#ifndef duckfinding_H
#define duckfinding_H

char * makeBoard(std::size_t xdim, std::size_t ydim);
void computeDucklings(char * board, std::size_t xdim, std::size_t ydim);
void concealBoard(char * board, std::size_t xdim, std::size_t ydim);
void eraseBoard(char * board);
void displayBoard(char * board, std::size_t xdim, std::size_t ydim);
int show(char * board, std::size_t xdim, std::size_t ydim, std::size_t xpos, std::size_t ypos);
int flag(char * board, std::size_t xdim, std::size_t ydim, std::size_t xpos, std::size_t ypos);
bool isWon(char * board, std::size_t xdim, std::size_t ydim);

#endif
