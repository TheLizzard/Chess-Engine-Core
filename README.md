# Chess-Engine-Core
The core of my chess engine.

# Compiling and Running:
Compiling and running `alphabeta.cpp` will create a `main` function that should be called with 3 arguments:
  * the string of the fen position
  * the depth as an unsigned interger
  * the max quietness as an unsigned interger (if unsure what value to use: use 100)

# Notes:
The program returns an unsigned integer where:
  * 9999999 means that the position is about even
  * 0 means black is winning
  * 19999998 white is winning

It will write "Suceess" to stdout. If it writes anything to stderr the result wouldn't be valid. The program doesn't do any checks
