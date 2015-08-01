#ifndef _QUADRO_SYSTEMBRIDGE_CPP
#define _QUADRO_SYSTEMBRIDGE_CPP

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include "SystemBridge.hpp"

QVariantMap
System::exec(const QString & prog, const QStringList & args)
{
  pid_t pid = fork();

  if (pid == 0) {
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
    _exit(errno);

  } else if (pid < 0) {
    goto fail;

  } else {

    sigset_t mask;
    sigset_t orig_mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);

    if (sigprocmask(SIG_BLOCK, &mask, &orig_mask) < 0) {
      goto fail;
    }

    struct timespec timeout;
    timeout.tv_sec = 0;
    timeout.tv_nsec = 10 * 1000 * 1000;

    while (true) {
      int ret = sigtimedwait(&mask, NULL, &timeout);

      if (ret < 0) {
        if (errno == EAGAIN) {
          // timeout
          goto win;
        } else {
          // error
          goto fail;
        }

      } else {
        if (errno == EINTR) {
          // not SIGCHLD
          continue;
        } else {
          siginfo_t si;
          if (waitid(P_PID, pid, &si, WEXITED) < 0) {
            goto fail;
          } else {
            return { { "errno", si.si_status }
                   , { "error", strerror(si.si_status) }
                   };
          }
        }
      }
    }
  }

win:
  return {};

fail:
  return { { "errno", errno }, { "error", strerror(errno) } };
}

#endif // _QUADRO_SYSTEMBRIDGE_CPP
