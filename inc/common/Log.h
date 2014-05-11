/*
 * @file Log.h
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

#ifndef LOG_H
#define LOG_H

#include <mutex>

typedef std::lock_guard<std::recursive_mutex> OutputLock;

class Log
{
public:
    /**
     * Returns singleton class instance.
     * @return Instance of Log.
     */
    static Log& getInstance()
    {
        static Log instance;
        return instance;
    }

    /**
     * Prints out nothing and flushes the output buffer.
     */
    inline void out()
    {
        OutputLock lock(_outputMutex);

        std::cout << std::endl;
    }

    /**
     * Prints out the data and calls recursively Log::out on args.
     * @param data Data to print.
     * @param args Pack of data to print.
     */
    template <typename T, typename... Args> inline void out(const T& data, const Args&... args)
    {
        OutputLock lock(_outputMutex);

        std::cout << data;
        out(args...);
    }

    /**
     * Prints out the debug output. Prints nothing in release build.
     * @param args Pack of data to print.
     */
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
