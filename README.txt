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

Our modifications:
- Instead of gates and keys, there is water on the map where player needs to build bridge from plank which is picked up in order to get to finish
- There is only one plank present on the map in the same time. Next one is spawned as soon as bridge is built
- Player drops plank if he is killed and he was carrying it
- Player can choose number of sentries in the game while creating the game
- Players are respawned after a short time when they die
- Movement can be performed with the arrow keys and as soon as you hit the arrow key in direction you want to go, you player starts walking
- Instead of actions "take" and "open" (which are still present in the game but not recommended to use), there is action "action" which is triggered by pressing SPACE button
    and it performs action depending on what is on the tile in front of the player. When there is a plank, it is picked up, when there is water and player has plank, bridge is built.

Libraries used:
Boost (v. 1.48.0)
QT (v. 5.2)

Authors:
Marek Milkovič  <xmilko01@stud.fit.vutbr.cz>
Ivan Ševčík     <xsevci50@stud.fit.vutbr.cz>

Assignment webpage:
https://www.fit.vutbr.cz/study/courses/ICP/public/ICP-PRJ-zadani.html
https://www.fit.vutbr.cz/study/courses/ICP/public/ICP-PRJ-zadani-2014-ija.html
