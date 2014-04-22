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
