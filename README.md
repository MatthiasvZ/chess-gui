# chess-gui

A simple chess game that was initally created in about half of a boring Sunday.

# Credits
Much of the underlying GL code was borrowed from The Cherno's OpenGL Tutorial series.

The following libraries were used:
- GLFW
- GLEW
- stb-image

# Features
- A graphical chequerboard with pieces moveable by mouse
- Disallowance of all illegal moves (except for those related to check)
- Castling, capturing en passant, pawn promotion (queen only)
- An alright AI opponent which should be easy to be beat for anyone 1000+ (depth = 3)
# TODO
- Disallow moves which would allow for the king to be taken
- End the game on mate instead of on capture of a king
- Improve the graphics
- Add support for windows that aren't 800 by 800
- Improve the AI
