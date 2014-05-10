/*
* Project name:
* Bludiste 2014
*
* Description:
* https://www.fit.vutbr.cz/study/courses/ICP/public/ICP-PRJ-zadani-2014-ija.html
* https://www.fit.vutbr.cz/study/courses/ICP/public/ICP-PRJ-zadani.html
*
* Project's GitHub repository:
* https://github.com/metthal/ICP-Projekt
*
* Team:
* Marek Milkovič (xmilko01)
* Ivan Ševčík (xsevci50)
*/

#ifndef OPCODE_H
#define OPCODE_H

enum Opcode
{
    FIRST_OPCODE                = 0,
    CMSG_HANDSHAKE_REQUEST      = FIRST_OPCODE,
    SMSG_HANDSHAKE_RESPONSE,
    CMSG_GAME_LIST_REQUEST,
    SMSG_GAME_LIST_RESPONSE,
    CMSG_GAME_JOIN_REQUEST,
    SMSG_GAME_JOIN_RESPONSE,
    CMSG_MAP_LIST_REQUEST,
    SMSG_MAP_LIST_RESPONSE,
    CMSG_GAME_CREATE_REQUEST,
    SMSG_GAME_CREATE_RESPONSE,
    SMSG_GAME_CREATE_OBJECT,
    SMSG_GAME_DELETE_OBJECT,
    SMSG_GAME_UPDATE_OBJECT,
    CMSG_PERFORM_ACTION_REQUEST,
    SMSG_PERFORM_ACTION_RESPONSE,
    SMSG_GAME_END,
    CMSG_PLAYER_LEFT_GAME,
    MAX_OPCODE
};

#endif // OPCODE_H
