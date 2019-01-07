#include <iostream>
#include <string>
#define underline "\033[4m"  // useful allows all subsequent text to be underlined
#define normal "\033[0m"     // undoes permanent underlining 

using namespace std;

class Board 
{
private:
  bool turn;  // tells whose turn it is depending on true or false (false is the player)
  char array[8][8];   // the actual board
  char comp;          // tracks computer icon
  char player;        // tracks player icon
  int count;      // tracks turn count
  int col_id;         // tracks col# of potential move so it doesn't need to be passed into nested functions
  int row_id;         // tracks row# of potential move so it doesnt need to be passed into nested functions
  int end_early;  // if == 2 then raises flag that neither player can move

  Board& operator = (const Board& right);      	//shoudln't need operator, so made private
  Board(const Board& x);		       	//shouldn't need copy constructor so made private	

public:
  Board(){;}					//constructor, nothing special needed bc only one instance called
  ~Board(){;}					//destructor, nothing special needed
  void turnChoose();    			//ask player if they want to go first
  void setBoard();				//set's starting board state
  void lineRow();				//aesthetic function used for printing board
  void letterRow();				//aesthetic function used for printing board
  void print();					//print board
  void playerMove();				//user move
  void computerMind();				//Decides where computer goes
  void computerMove();				//makes computer move
  //void player2Move();				//user2 move (used for testing)
  bool validInput(string s,char user);		//overarching input validation of a move
  bool legalMove(char c);			//within above function: checks whether move is allowed
  bool adjacent(char me, char you);		//within above fucntion: checks for adjacent requirement
  bool flipCheck(int x, int y, char me);	//within above function: verifies whether a move flips a tile
  void findFlip(char me, char you);		//refind flip to then flip after valid move verified
  void flip(int x, int y, char me);		//flip all valid tiles
  bool noMove(char user);			//checks if player or computer has a valid move
  bool game();					//checks conditions for game end
  bool earlyEnd();				//within above function: checks if game is to end early bc neither player can go
  bool playerTurn();				//determines whose turn it is
  void endPrint();				//end game print statement
  int score(char c);				//within above function: returns score of that player's piece
  void winner();				//within endPrint, checks who won
  bool playAgain();				//Asks whether player wants to play again
};

//Note all functions in one class because many functions and constants are shared between potential user and computers and need to be constantly accessed, the friend and inheritance relationships could get complicated, so I decided to err on the side of caution even if the class is a little bloated. 

void Board::setBoard()
{
  count = 0;
  end_early = 0;
  turn = false; 
  int i,j;	
  for(i = 0; i < 8; i++)
  {
    for(j = 0; j < 8; j++)
    {
      array[i][j] = ' ';	    
    }	    
  }
  array[3][3] = 'O';
  array[3][4] = 'X';
  array[4][4] = 'O';
  array[4][3] = 'X';  
}
void Board::lineRow()
{	
  for(int i = 0; i < 19;i++)	  
  {
    cout << underline << ' ';	  
    //note underline makes all text underlined until 'normal' is used
  }	  
  cout << endl;
}
void Board::letterRow()
{
  char temp;	
  cout << "| |";	
  for(int i = 0; i < 8;i++)
  {
    temp = i + 'A';	  
    cout << temp << '|';
  }
  cout << endl;
	
}
void Board::print()
{
  int i,j;
  lineRow();
  letterRow();
  for(i = 0; i < 8; i++)
  {
    cout << '|' << i + 1 << '|';	  
    for(j = 0; j < 8; j++)
    {
    cout << array[i][j] << '|'; 
    }
    cout << endl; 
  }
  cout << normal << endl;
}  

void Board::turnChoose()
{
  string s;
  char c;
  do
  {
  cout << "Do you want to go first (X) or second (O)?  ";
  cin >> s;
  c = s[0];
  
  } while(c != 'X' && c != 'x' && c != 'O' && c != 'o' || s.length() != 1);
  
  if(c == 'X' || c == 'x')
  {
    turn = false;	  
    player = 'X';
    comp = 'O'; 
  }
  else
  {
    turn = true;	  
    player = 'O';
    comp = 'X';    
  }
}
bool Board::validInput(string s, char user)
{
  if(s.length() != 2)
  {
    cout << "Invalid input!" << endl;	  
    return false;	  
  }
  else if(!((s[0] >= 'A' && s[0] <= 'H') || (s[0] >= 'a' && s[0] <= 'h')))
  {
    cout << "Invalid input!" << endl;	  
    return false;	  
  } 
  else if(!(s[1] >= '1' && s[1] <= '8'))
  {
    cout << "Invalid input!" << endl;
    return false;
  }

  if(s[0] >= 'a' && s[0] <= 'h')
  {
    col_id = s[0] - 'a';
  }
  else
  {
    col_id = s[0] - 'A';
  }
  
  row_id = s[1] - '1' ;
  
  if(array[row_id][col_id]  != ' ')
  { 
    cout << "Illegal move! That tile is taken!" << endl;
    return false;
  }

  if(!legalMove(user))
  {
    cout << "Illegal move! Doesn't flip!" << endl;
    return false;
  }
  return true;
}

bool Board::legalMove(char c)
{
  char me;
  char you;	 

  if(c == player)
  {
    me = player;
    you = comp;    
  }	  
  else
  {
    me = comp;
    you = player;    
  }
  int i = row_id;
  int j = col_id;

  //adjacency check
  if(!adjacent(me,you))
  {
    return false;	  
  }
  return true; 
}

bool Board::adjacent(char me, char you)
{
  int i,j;
  for(i = 0 ; i < 8;i++)
  {
    for(j = 0; j < 8; j++)
    {
      if((i == row_id && (j == col_id+1 || j == col_id-1)) ||
         (i == row_id-1 && (j >= col_id-1 && j <= col_id + 1 ))||
         (i == row_id+1 && (j >= col_id-1 && j <= col_id + 1 )))
      {
	if(array[i][j] == you)
	{	
	  if(flipCheck(i,j,me))
	  {	  
	    return true;
	  }
	}	
      }
    }	    
  }  
  return false;	
}

bool Board::flipCheck(int x, int y, char me)
{  
  int dif_row = x - row_id;
  int dif_col = y - col_id;
  int i = x;
  int j = y;

  while(i >= 0 && i <= 7 && j >= 0 && j <= 7)
  {
    if(array[i][j] == me)
    { 
      return true;	    
    }
    else if(array[i][j] == ' ')
    {
      return false;	    
    }
    i += dif_row;
    j += dif_col;
  }
  return false;
}

void Board::playerMove()
{
  if(noMove(player))
  {
    return;	  
  }	  
  string s;	
  cout << "Player move" << endl; 
  do
  {	  
   cout << "Input in the form 'A1' or 'a1': ";
   cin >> s;
  } while(!validInput(s,player));
  array[row_id][col_id] = player;
  findFlip(player,comp);
  turn = !turn;
  count++;
  print(); 
}
void Board::computerMove()
{
  array[row_id][col_id] = comp;
  findFlip(comp,player);
  turn = !turn;
  count++;
  cout << "Computer Move: " << ((char) (col_id + 'A')) << (row_id + 1) << endl;	
  print();
}


void Board::computerMind()
{	
  if(noMove(comp))
  {
    return;	  
  }  

  //check if corners are open
  for(row_id = 0; row_id < 8; row_id += 7)
  {
    for(col_id = 0; col_id <8; col_id += 7)
    {	    
      if(array[row_id][col_id]  == ' ')
      {
        if(adjacent(comp,player))
	{
	  computerMove();
          return;	  
	}		
      }	      
    }	    
  }

  //check if top or bottom edges are open 
  for(row_id = 0; row_id < 8; row_id += 7)
  {
    for(col_id = 1; col_id < 7; col_id++)
    {	    
      if(array[row_id][col_id]  == ' ')
      {
        if(adjacent(comp,player))
	{
	  computerMove();
          return;	  
	}		
      }	      
    }	    
  }

  //check if left or right edges are open 
  for(col_id = 0; col_id < 8; col_id += 7)
  {
    for(row_id = 1; row_id < 7; row_id++)
    {	    
      if(array[row_id][col_id]  == ' ')
      {
        if(adjacent(comp,player))
	{
	  computerMove();
          return;	  
	}		
      }	      
    }	    
  }
  //check if 1st inner shell (6x6) is open excluding corners of shell
  //start with bottom and top edges of this shell
  for(row_id = 1; row_id < 7; row_id += 5)
  {
    for(col_id = 2; col_id < 6; col_id++)
    {	    
      if(array[row_id][col_id]  == ' ')
      {
        if(adjacent(comp,player))
	{
	  computerMove();
          return;	  
	}		
      }	      
    }	    
  }
  //now check left and right edges of shell
  for(col_id = 1; col_id < 7; col_id += 5)
  {
    for(row_id = 2; row_id < 6; row_id++)
    {	    
      if(array[row_id][col_id]  == ' ')
      {
        if(adjacent(comp,player))
	{
	  computerMove();
          return;	  
	}		
      }	      
    }	    
  }

  //check if middle 6x6 grid is open
  for(row_id = 2; row_id < 6; row_id++)
  {
    for(col_id = 2; col_id < 6; col_id++)
    {	    
      if(array[row_id][col_id]  == ' ')
      {
        if(adjacent(comp,player))
	{
	  computerMove();
          return;	  
	}		
      }	      
    }	    
  }

  //last resort is corners or 7x7 grid
  for(row_id = 1; row_id < 7; row_id += 5)
  {
    for(col_id = 1; col_id <7; col_id += 5)
    {	    
      if(array[row_id][col_id]  == ' ')
      {
        if(adjacent(comp,player))
	{
	  computerMove();
          return;	  
	}		
      }	      
    }	    
  }
}

/*
void Board::player2Move()
{
  if(noMove(comp))
  {
    return;	  
  }	  
  string s;	
  cout << "Player2 move" << endl; 
  do
  {	  
   cout << "Input in the form 'A1' or 'a1': ";
   cin >> s;
  } while(!validInput(s,comp));
  array[row_id][col_id] = comp;
  findFlip(comp,player);
  turn = !turn;
  count++;
  print(); 
}
*/
void Board::findFlip(char me, char you)
{
  int i,j;
  for(i = 0 ; i < 8;i++)
  {
    for(j = 0; j < 8; j++)
    {
      if(((i == row_id && (j == col_id+1 || j == col_id-1)) ||
          (i == row_id-1 && (j >= col_id-1 && j <= col_id + 1 ))||
          (i == row_id+1 && (j >= col_id-1 && j <= col_id + 1 ))) &&
	   flipCheck(i,j,me))
      {
        {
          flip(i,j,me);
        }
      }
    }
  }
  return;
}

void Board::flip(int x, int y, char me)
{
  int dif_row = x - row_id;
  int dif_col = y - col_id;
  int i = x; 
  int j = y;

  while(i >= 0 && i <= 7 && j >= 0 && j <= 7)
  {	  
    if(array[i][j] == me)
    {
      return;
    }
    else
    {
      array[i][j] = me;
    }
    i += dif_row;
    j += dif_col;
  }
  return;
}


bool Board::noMove(char user)
{
  for(row_id = 0; row_id < 8; row_id++)
  {
    for(col_id = 0; col_id < 8; col_id++)
    {
      if(array[row_id][col_id] != ' ')
      {
        continue;      
      }	      
      if(legalMove(user))
      {
        end_early = 0;	      
        return false;	      
      }	      
    }	    
  }
  end_early++;
  turn  = !turn; 
  if(user == player)
  {
    cout << "Player has no available moves! Skips turn!" << endl;    
  }
  if(user == comp)
  {
    cout << "Computer has no available moves! Skips turn!" << endl;    
  }

  return true;	
}

bool Board::game()
{
  if(earlyEnd() || count == 60)	
  {
    return false;	  
  }  
  return true;
}


bool Board::earlyEnd()
{
  if(end_early == 2)
  {
    return true;	  
  }
  return false;
}

bool Board::playerTurn()
{
  if(turn == false)
  {
    return true;	  
  }	  
  return false;
}

int Board::score(char c)
{
  int i,j;
  int points = 0;  
  for(i = 0; i < 8; i++)
  {
    for(j = 0; j < 8; j++)
    {
      if(array[i][j] == c)
      {
	 points++;     
      }	    
    }
  } 
  return points;  
}
void Board::winner()
{
  int p = score(player);
  int c = score(comp);  
  if(p > c)
  {
    cout << "Player Wins!!! Good job!" << endl << endl;	  
  }	  
  else if(c > p)
  {
    cout << "Computer Wins!!! Sorry Loser :)" << endl << endl;	  
  }	
  else
  {
    cout << "It's a tie!!!" << endl << endl;	  
  } 
}

void Board::endPrint()
{
  cout << "Neither player has any available moves left! Game ends!" << endl;
  cout << "Final Score is:" << endl;
  cout << "Player: " << score(player) << endl;
  cout << "Computer: " << score(comp) << endl;
  winner();	
}

bool Board::playAgain()
{
  string s;
  cout << "Would you like to play again? (y/n)" << endl; 
  while(1)
  {
    cin >> s;	  
    if (s == "y" || s == "Y")
    {
      cout << "You have chosen to play again" << endl;	    
      return true;	    
    }   
    else if( s == "n" || s == "N")
    {
      cout << "Thanks for Playing!" << endl;	    
      return false;	    
    }
    else
    {
      cout << "Invalid input, try again." << endl;

    }
  }  
}
