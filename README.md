# FinalSamuelMakarovskiy

*Prestated Requirements*:

The final deliverable should be a working game of Othello including
- 8x8 game board updated by turn
- Follows game rules:
  	+ Players move until game board fills up
	+ start with 4 pieces (2 xs and  2 os) in center in pattern like:
	
		o x
		x o
	+ Black (X goes first)
 	+ Must place piece adjacent to existing pieces and to make a valid move, must have your opponent's type piece be sandwiched by an exising piece of yours and the one you just placed (even applies if a few of your opponent's are sandwiched between yours and your opponents)
 	+ This sandwhich flips any piece you sandwich to your type.
	+ Player with most pieces of their type o board at the end wins.
 - Rudimentary AI because game can get very complicated very fast. (Search for most flips opportunity)
 C++ components:
 - Rudimentary input validation (rows/column entries)
- Use classes and functions including:
	+ Board class including game state and the following functions:
		> Play, Board initialization, writing, verifying valid move, printing, verifying win condtions, AI move , player move

*Compile Instructions*:

Input the following console commands:
	
	g++ main.cpp board.h -o main.exe
	./main.exe

Then folow program prompts

