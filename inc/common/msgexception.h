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
