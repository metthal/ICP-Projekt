/*
 * nexception.h
 *
 *  Created on: Apr 22, 2014
 *      Author: raven
 */

#ifndef MSGEXCEPTION_H_
#define MSGEXCEPTION_H_

#include <exception>
#include <string>

class MsgException : public std::exception
{
public:
    MsgException(const std::string message) noexcept;
	const char* what() const noexcept;
	void report() noexcept;

private:
	std::string _msg;
};


#endif /* NEXCEPTION_H_ */
