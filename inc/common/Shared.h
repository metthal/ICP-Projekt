#ifndef SHARED_H
#define SHARED_H

#include <boost/thread.hpp>

typedef boost::unique_lock<boost::mutex> ScopedLock;

#endif // SHARED_H
