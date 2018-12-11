#include "board.h"

using namespace std;

int main()
{	
  Board a;
  cout << "Welcome to Othello!" << endl;
  do
  {
    a.setBoard();
    a.turnChoose();
    a.print();
    while(a.game())
    {	    
      if(a.playerTurn())	
      {  
        a.playerMove();
      }
      else	
      {   
        a.computerMind();
      }
    }
    a.endPrint();
  } while(a.playAgain());
  return 0;	
}
