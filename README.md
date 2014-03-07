Project for ICP class (c++ seminar): Maze

Basic description:
Implement a game of maze with client and server.
Client will be used for displaying graphics and inputing commands.
Server will provide game logic and update client state. 
The game area is divided into tiles.
There are players (Max. 4 at the same time) and sentries.
Sentries are AI with random movement.
Sentry will kill player upon contact.
Each player represents one running client connected to the server.
Player can use commands to control their character.
Goal of game is to get into finish as first.

Basic blocks:
wall - Obstacle that can't be crossed.
gate - Obstacle that can be crossed only if opened.
key - Opens gate.
finish - Place where game ends.

Basic commands:
go - player will move forward until object is in front of him.
stop - stops the player movement (if not moving, does nothing).
left - player will turn left.
right - player will turn right.
take - player picks up key in front of him.
open - player opens gate in front of him (if he has a key).


Libraries used:
Boost (v. 1.53.TODO)
QT (v. 5.2)

Compilation:
GNU make
TODO

Makefile targets:
TODO

Authors:
Marek Milkovič
Ivan Ševčík

Assignment webpage:
https://www.fit.vutbr.cz/study/courses/ICP/public/ICP-PRJ-zadani.html
https://www.fit.vutbr.cz/study/courses/ICP/public/ICP-PRJ-zadani-2014-ija.html