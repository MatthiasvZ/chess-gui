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
- Castling, pawn promotion (queen only)
# TODO
- Allow for taking en passant
- Disallow moves which would allow for the king to be taken
- End the game on mate instead of on capture of a king
- Improve the graphics
- Add support for windows that aren't 800 by 800
- Add a somewhat decent AI opponent
