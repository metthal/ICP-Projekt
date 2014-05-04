#include "client/client.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <string>

void Client::start()
{
    const unsigned int interval = 500;

    _close = false;
    std::thread loopThread(&Client::controlLoop, this);
    loopThread.detach();

    while (!_close)
    {
        auto t0 = std::chrono::high_resolution_clock::now();
        //TODO: read from server
        //TODO: print stuff like who's connected and last message etc.
        std::cout << "Loopin & wooopin" << std::endl;
        auto t1 = std::chrono::high_resolution_clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
        auto sleepTime = std::chrono::milliseconds(interval - dur);

        if (sleepTime.count() > 0)
            std::this_thread::sleep_for(sleepTime);
    }
}

void Client::controlLoop()
{
	std::string command;
	while (!std::cin.eof())
	{
        std::cin >> command;
        if (command.compare("u") == 0)
        {
            //TODO: notify server
            std::cout << "Command U" << std::endl;
        }
        else if (command.compare("d") == 0)
        {
            //TODO: notify server
            std::cout << "Command D" << std::endl;
        }
        else if (command.compare("l") == 0)
        {
            //TODO: notify server
            std::cout << "Command L" << std::endl;
        }
        else if (command.compare("r") == 0)
        {
            //TODO: notify server
            std::cout << "Command R" << std::endl;
        }
        else if (command.compare("a") == 0)
        {
            //TODO: notify server
            std::cout << "Command A" << std::endl;
        }
	}
	_close = true;
}
