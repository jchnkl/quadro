#ifndef _QUADRO_SYSTEMBRIDGE_CPP
#define _QUADRO_SYSTEMBRIDGE_CPP

#include <unistd.h>
#include <fstream>
#include <sstream>
#include "SystemBridge.hpp"

bool
System::exec(const QString & prog, const QStringList & args)
{
  pid_t child = fork();

  if (child == 0) {
    int cargs_len = args.length() + 2;
    char * cargs[cargs_len];
    cargs[cargs_len - 1] = NULL;

    QByteArrayList as;
    as.push_back(prog.toLocal8Bit());

    std::transform(args.begin(), args.end(), std::back_inserter(as),
        [](const QString & s) { return s.toLocal8Bit(); });

    for (int i = 0; i < as.length(); ++i) {
      cargs[i] = as[i].data();
    }

    execvp(cargs[0], cargs);

    // in case execvp fails, terminate the child process immediately
    _exit(EXIT_FAILURE);

  } else if (child < 0) {
    return { { "error", strerror(errno) } };

  } else {
    return {};
  }
}

#endif // _QUADRO_SYSTEMBRIDGE_CPP
