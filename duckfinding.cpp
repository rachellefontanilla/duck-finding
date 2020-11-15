#include "duckfinding_lib.h"

//Function Declarations
char * makeBoard(std::size_t xdim, std::size_t ydim);
void computeDucklings(char * board, std::size_t xdim, std::size_t ydim);
void concealBoard(char * board, std::size_t xdim, std::size_t ydim);
void eraseBoard(char * board);
void displayBoard(char * board, std::size_t xdim, std::size_t ydim);
int show(char * board, std::size_t xdim, std::size_t ydim, std::size_t xpos, std::size_t ypos);
int flag(char * board, std::size_t xdim, std::size_t ydim, std::size_t xpos, std::size_t ypos);
bool isWon(char * board, std::size_t xdim, std::size_t ydim);


//Function Definitions
char * makeBoard(std::size_t xdim, std::size_t ydim){
	char *board = new char[xdim*ydim] {0};
	return board;
}

void computeDucklings(char * board, std::size_t xdim, std::size_t ydim){
	// NW    N    NE
	// W   index  E
	// SW    S    SE
	
	int index{0};
	for (int row{0}; row < ydim; row++) {		// rows
		for (int col{0}; col < xdim; col++) {	//columns
			if (board [index] != 9){ 			//check if you're a duck
			
				
				int n = index - xdim;
				int s = index + xdim;
				int e = index + 1;
				int w = index - 1;
				int ne = n + 1;
				int nw = n - 1;
				int se = s + 1;
				int sw = s - 1;
				
				
				//north
				if ( (row > 0) && (board[n] == 9) ){
					board[index] = board[index] +1;
				}
				
				//south
				if ( (row < (ydim-1)) && (board[s] == 9) ){
					board[index] = board[index] +1;
				}
				
				//east
				if ( (col < (xdim-1)) && (board[e] == 9) ){
					board[index] = board[index] +1;
				}
					
				//west
				if ( (col > 0) && (board[w] == 9) ){
					board[index] = board[index] +1;
				}
				
				//north east
				if ( (row > 0) && (col < (xdim-1)) && (board[ne] == 9) ){
					board[index] = board[index] +1;
				}
				
				//north west
				if ( (row > 0) && (col > 0) && (board[nw] == 9) ){
					board[index] = board[index] +1;
				}
				
				//south east
				if ( (row < (ydim-1)) && (col < (xdim-1)) && (board[se] == 9) ){
					board[index] = board[index] +1;
				}
				
				//south west
				if ( (row < (ydim-1)) && (col > 0) && (board[sw] == 9) ){
					board[index] = board[index] +1;
				}
				
			}
				
			index++;
	}
	
}
}

void concealBoard(char * board, std::size_t xdim, std::size_t ydim){
	// turn on concealedBit(): 0x20 => 0010 0000}
	// use the bitwise OR operator to keep the pre-existing 1s & add the 1 for concealedBit
	
	for (std::size_t index{0}; index < (xdim*ydim); index++){
		board[index] = board[index] | concealedBit();
	}
	
}

void eraseBoard(char * board){
	delete[] board;
	board = nullptr;
}

void displayBoard(char * board, std::size_t xdim, std::size_t ydim){

	
	for (std::size_t index{0}; index < (xdim*ydim); index++){
		
		// FLAGGED FIELD
		if ( (board[index] & flaggedBit() ) == flaggedBit() ){
			std::cout << "M";
		}
		
		// CONCEALED FIELD
		else if ( (board[index] & concealedBit() ) == concealedBit() ){
			std::cout << "*";
		}
		
		// NUMBER OF NEIGHBOURING DUCKS
		else {
			std::cout << ( board[index] & ducklingMask() );
		}
		
		if ( (index % (xdim)) == (xdim - 1) ) {
			std::cout << std::endl;
		}
		
	}
	
	
}

int show(char * board, std::size_t xdim, std::size_t ydim, std::size_t xpos, std::size_t ypos){
	
	std::size_t index{};
	index = (ypos * xdim) + xpos;
	
	//if field is flagged
	if ( (board[ index ] & flaggedBit() ) == flaggedBit() ){
		return 1;
	}
	//if field is already shown --> means not concealed, not flagged (already checked for flagged in previous statement)
	else if  ( (board[ index ] & concealedBit() ) != concealedBit() ) {
		return 2;
	}
	//if field contains duck
	else if ( (board[ index ] & ducklingMask()) == 9) {
		board[index] = board[index] & ducklingMask();
		return 9;
	}
	else if ( (board[ index ] & ducklingMask()) == 0){
		board[index] = board[index] & ducklingMask();
		
		//show all surrounding non ducks
		int ypos_temp = (int)ypos;
		int xpos_temp = (int)xpos;
		
		//3 rows 3 columns, so start (index-1) go until (index+1)
		for (int show_y {ypos_temp -1}; show_y <= ( ypos_temp +1) ; show_y++) {
			for (int show_x {xpos_temp -1}; show_x <= ( xpos_temp +1) ; show_x++) {
				//check if still on the board
				if ( (show_x < xdim) && (show_y <ydim) && (show_x >= 0) && (show_y >= 0)) {
					//
					if ( (board[ (show_y * xdim) + show_x] & flaggedBit() ) == 0) {
						//show the space
						board[ (show_y * xdim) + show_x] = board[ (show_y* xdim) + show_x] & ducklingMask();
					}
				}
			}
		}
	}
	else {
		board[index] = board[index] & ducklingMask();
	}
	return 0;
}

int flag(char * board, std::size_t xdim, std::size_t ydim, std::size_t xpos, std::size_t ypos){
	
	// Return 2 if the field is already shown. Return 0 otherwise
	
	//   0001 0100 - Not concealed example
	// & 0010 0000 - concealed bit

	//   0000 0000 - (board[ index ] & concealedBit() )
	
	//   0010 0100 - Not flagged example
	// & 0001 0000 - flagged bit

	//   0000 0000 - (board[ index ] & flaggedBit() )
	
	int index{};
	index = (int)((ypos * xdim) + xpos);
	
	//if field is already shown --> means not concealed, not flagged
	if ( ( (board[ index ] & concealedBit() ) != concealedBit()) && ( (board[ index ] & flaggedBit() ) != flaggedBit() ) ) {
		return 2;
	}
	
	// if flaging an already flagged field --> change back to unflagged
	else if ( board[index] == ( board[index] | flaggedBit() )){
		board[index] = ( board[index] ^ flaggedBit());
		return 0;
	}
	else {
		board[index] = board[index] | flaggedBit();
		return 0;
	}
	

}


bool isWon(char * board, std::size_t xdim, std::size_t ydim){

	 bool all_nducks_shown{false};
	
	//game won bc all non-duck fields are shown
	for (std::size_t index{0}; index < (xdim*ydim); index++) {
		if ( (board[index] & ducklingMask() ) != 9) {
			if ( (board[index] & concealedBit() ) != concealedBit() ){
				all_nducks_shown = true;
			} else {
				all_nducks_shown = false;
				break;
			}
		}
	}
	
	return all_nducks_shown;
	
}
