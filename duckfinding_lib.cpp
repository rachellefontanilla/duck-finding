#include <cstdlib>
#include <iostream>
#include <cctype>
#include <ctime>      // for time(NULL)
#include "duckfinding_lib.h"
#include "duckfinding.h"

int main()
{
  srand(time(NULL));
  game();
  return 0;
}

bool game()
{
  std::size_t xdim {0};
  std::size_t ydim {0};
  unsigned int numDucks {0};
  char * gameBoard {NULL};

  beginGame(gameBoard, xdim, ydim, numDucks);

  char currAction {0};
  while(currAction != 'Q')
  {
    switch (currAction)
    {
      case 'S' :  // show
      {
        actionShow(gameBoard, xdim, ydim, numDucks);
        break;
      }
      case 'F' :  // flag
      {
        actionFlag(gameBoard, xdim, ydim);
        break;
      }
      case 'R' :  // restart
      {
        std::cout << "Restarting the game." << std::endl;
        beginGame(gameBoard, xdim, ydim, numDucks);
        break;
      }
    }
    displayBoard(gameBoard, xdim, ydim);
    if (isWon(gameBoard, xdim, ydim))
    {
      std::cout << "You have shown all the fields without disturbing a duck!" << std::endl;
      std::cout << "YOU WON!!!" << std::endl;
      for (std::size_t show_row {0}; show_row < ydim; show_row++)
      {
        for (std::size_t show_col {0}; show_col < xdim; show_col++)
        {
          gameBoard[show_row*xdim + show_col] = (gameBoard[show_row*xdim + show_col] & ducklingMask());
        }
      }
      displayBoard(gameBoard, xdim, ydim);
      std::cout << "Resetting the game board." << std::endl;
      beginGame(gameBoard, xdim, ydim, numDucks);
    }
    currAction = getAction();
  }

  eraseBoard(gameBoard);
  return true;
}

void beginGame(char * & board, std::size_t & xdim, std::size_t & ydim, unsigned int & numDucks)
{
  std::cout << "Welcome to Duck Finding!" << std::endl;
  do {
    std::cout << "Please enter the x dimension (max " << xdim_max() << "): ";
    std::cin >> xdim;
  } while(xdim < 1 || xdim > xdim_max());
  do {
    std::cout << "Please enter the y dimension (max " << ydim_max() << "): ";
    std::cin >> ydim;
  } while(ydim < 1 || ydim > ydim_max());

  std::cout << "Please enter the number of ducks: ";
  std::cin >> numDucks;
  while (numDucks > xdim * ydim)
  {
    std::cout << "That's too many ducks!" << std::endl;
    std::cout << "Please enter the number of ducks: ";
    std::cin >> numDucks;
  }

  eraseBoard(board);
  board = makeBoard(xdim, ydim);
  scatterDucks(board, xdim, ydim, numDucks);
  computeDucklings(board, xdim, ydim);
  concealBoard(board, xdim, ydim);
}

char getAction()
{
  char action {0};

  std::cout << "Please enter the action ([S]how, [M]ark, [R]estart, [Q]uit): ";
  std::cin >> action;

  if (islower(action))
    action = toupper(action);

  return action;
}

void actionShow(char * & board, std::size_t & xdim, std::size_t & ydim, unsigned int & numDucks)
{
  std::size_t show_x {0};
  std::size_t show_y {0};
  std::cout << "Please enter the x position to show: ";
  std::cin >> show_x;
  std::cout << "Please enter the y position to show: ";
  std::cin >> show_y;

  if (show_x >= xdim || show_y >= ydim)
  {
    std::cout << "Position entered is not on the board." << std::endl;
  }
  else if (board[xdim*show_y + show_x] & flaggedBit())
  {
    std::cout << "Position is flagged, and therefore cannot be shown." << std::endl;
    std::cout << "Use flag on position to unflag." << std::endl;
  }
  else if (show(board, xdim, ydim, show_x, show_y) == 9)
  {
    std::cout << "You disturbed a duck! Your game has ended." << std::endl;
    displayBoard(board, xdim, ydim);
    std::cout << "Starting a new game." << std::endl;
    beginGame(board, xdim, ydim, numDucks);
  }
}

void actionFlag(char * board, std::size_t xdim, std::size_t ydim)
{
  std::size_t flag_x {0};
  std::size_t flag_y {0};
  std::cout << "Please enter the x position to flag: ";
  std::cin >> flag_x;
  std::cout << "Please enter the y position to flag: ";
  std::cin >> flag_y;

  if (flag_x >= xdim || flag_y >= ydim)
  {
    std::cout << "Position entered is not on the board." << std::endl;
  }
  else if (flag(board, xdim, ydim, flag_x, flag_y) == 2)
  {
    std::cout << "Position already shown, so cannot be flagged." << std::endl;
  }
}

std::size_t readSizeT()
{
  return 0;
}

std::size_t xdim_max()
{
  return 60;
}

std::size_t ydim_max()
{
  return 20;
}

char flaggedBit()
{
  return 0x10;
}

char concealedBit()
{
  return 0x20;
}

char ducklingMask()
{
  return 0x0F;
}

void scatterDucks(char * board, std::size_t xdim, std::size_t ydim, unsigned int numDucks)
{
  if (board != NULL)
  {
    for (unsigned int gen_duck {0}; gen_duck < numDucks; gen_duck++)
    {
      std::size_t try_position {0};
      do {
         try_position = rand() % (xdim * ydim);
      } while (board[try_position] != 0);

      board[try_position] = 9;
    }
  }
}
