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

#ifndef LOG_H
#define LOG_H

#include <mutex>

typedef std::lock_guard<std::recursive_mutex> OutputLock;

class Log
{
public:
    static Log& getInstance()
    {
        static Log instance;
        return instance;
    }

    inline void out()
    {
        OutputLock lock(_outputMutex);

        std::cout << std::endl;
    }

    template <typename T, typename... Args> inline void out(const T& data, const Args&... args)
    {
        OutputLock lock(_outputMutex);

        std::cout << data;
        out(args...);
    }

#ifdef DEBUG
    template <typename... Args> inline void outDebug(const Args&... args)
    {
        out("DEBUG: ", args...);
    }
#else
    template <typename... Args> inline void outDebug(const Args&...) {}
#endif

private:
    Log() : _outputMutex() {}
    Log(const Log&) = delete;
    ~Log() {}

    Log& operator =(const Log&);

    std::recursive_mutex _outputMutex;
};

#define sLog Log::getInstance()

#endif // LOG_H
