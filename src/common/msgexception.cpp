/*
 * @file msgexception.cpp
 *
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
 * @author Marek Milkovič (xmilko01)
 * @author Ivan Ševčík (xsevci50)
 */

#include "common/msgexception.h"
#include <iostream>

MsgException::MsgException(const std::string message) noexcept
{
    _msg = std::string(message);
}

const char* MsgException::what() const noexcept
{
    return _msg.c_str();
}

void MsgException::report() noexcept
{
    std::cerr << "ERR: " << _msg << std::endl;
}
