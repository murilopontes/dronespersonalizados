/*
 * daemonize.cpp
 *
 *  Created on: 02/12/2013
 *      Author: mpontes
 */

#include "daemonize.h"


// C++ - C library
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfloat>
#include <ciso646>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cwchar>
#include <cwctype>

// C++ - containers
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>

//C++ - input/output
#include <fstream>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <streambuf>

//C++ - others
#include <algorithm>
#include <bitset>
#include <complex>
#include <exception>
#include <functional>
#include <iterator>
#include <limits>
#include <locale>
#include <memory>
#include <new>
#include <numeric>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>


//----- umask
#include <sys/types.h>
#include <sys/stat.h>

//----- open
#include <fcntl.h>

//----- dup2
#include <unistd.h>


#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

void murix_utils::daemonize(void)
{
    pid_t pid, sid;
    int fd;

    /* already a daemon */
    if ( getppid() == 1 ) return;

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }

    if (pid > 0)
    {
        exit(EXIT_SUCCESS); /*Killing the Parent Process*/
    }

    /* At this point we are executing as the child process */

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0)
    {
        exit(EXIT_FAILURE);
    }

    /* Change the current working directory. */
    if ((chdir("/")) < 0)
    {
        exit(EXIT_FAILURE);
    }


    fd = open("/dev/null",O_RDWR, 0);

    if (fd != -1)
    {
        dup2 (fd, STDIN_FILENO);
        dup2 (fd, STDOUT_FILENO);
        dup2 (fd, STDERR_FILENO);

        if (fd > 2)
        {
            close (fd);
        }
    }

    /*resettign File Creation Mask */
    umask(027);
}


