# Chess-Engine-Core
The core of my chess engine.

# Compiling and Running:
Compiling and running `alphabeta.cpp` will create a `main` function that should be called with 3 arguments:
  * the string of the fen position
  * the depth as an unsigned interger
  * the max quietness as an unsigned interger (if unsure what value to use: use 100)

It will write "Suceess" to stdout. If it writes anything to stderr the result wouldn't be valid. The program doesn't do any checks
