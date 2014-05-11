/*
 * @file msgexception.h
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

#ifndef MSGEXCEPTION_H_
#define MSGEXCEPTION_H_

#include <exception>
#include <string>

class MsgException : public std::exception
{
public:
    /**
     * Creates new instance for exception with message.
     */
    MsgException(const std::string message) noexcept;

    /**
     * Returns string that was associated with exception
     * during throw.
     */
    const char* what() const noexcept;

    /**
     * Reports exception on standard error output.
     */
    void report() noexcept;

private:
    std::string _msg;
};


#endif /* NEXCEPTION_H_ */
