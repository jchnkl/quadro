#ifndef _QUADRO_FILEBRIDGE_CPP
#define _QUADRO_FILEBRIDGE_CPP

#include <fstream>
#include <sstream>
#include <sys/vfs.h>
#include <sys/stat.h>
#include <errno.h>
#include "FileBridge.hpp"

std::string
showIostate(std::ios_base::iostate state)
{
  switch (state) {
    case std::ios_base::badbit:
      return "irrecoverable stream error";
    case std::ios_base::failbit:
      return "input/output operation failed (formatting or extraction error)";
    case std::ios_base::eofbit :
      return "associated input sequence has reached end-of-file";
    default:
      return "no error";
  };
}

QVariantMap
File::read(const QString & filename)
{
  try {
    std::fstream file(filename.toStdString(), std::ios::in);

    if (file.good()) {
      std::stringstream content;
      content << file.rdbuf();
      return { { "content", QString::fromStdString(content.str()) }
             , { "error", QVariant() }
             };

    } else {
      throw std::runtime_error(showIostate(file.rdstate()));
    }

  } catch (const std::exception & e) {
    return { { "content", QVariant() }, { "error",  e.what() } };
  }
}

QVariantMap
Stat::fs(const QString & path)
{
  struct statfs buf;
  if (statfs(path.toStdString().c_str(), &buf) == 0) {
    return { { "bsize",  QString::number(buf.f_bsize)  }
           , { "blocks", QString::number(buf.f_blocks) }
           , { "bfree",  QString::number(buf.f_bfree)  }
           , { "bavail", QString::number(buf.f_bavail) }
           , { "files",  QString::number(buf.f_files)  }
           , { "ffree",  QString::number(buf.f_ffree)  }
           };
  } else {
    return { { "error", strerror(errno) } };
  }
}

QVariantMap
Stat::file(const QString & file)
{
  struct stat buf;
  if (stat(file.toStdString().c_str(), &buf) == 0) {
    double atime = buf.st_atim.tv_sec
                 + (double)buf.st_atim.tv_nsec / 1000 / 1000 / 1000;
    double mtime = buf.st_mtim.tv_sec
                 + (double)buf.st_mtim.tv_nsec / 1000 / 1000 / 1000;
    double ctime = buf.st_ctim.tv_sec
                 + (double)buf.st_ctim.tv_nsec / 1000 / 1000 / 1000;

    return { { "dev",     QString::number(buf.st_dev)     }
           , { "ino",     QString::number(buf.st_ino)     }
           , { "mode",    QString::number(buf.st_mode)    }
           , { "nlink",   QString::number(buf.st_nlink)   }
           , { "uid",     QString::number(buf.st_uid)     }
           , { "gid",     QString::number(buf.st_gid)     }
           , { "dev",     QString::number(buf.st_rdev)    }
           , { "off",     QString::number(buf.st_size)    }
           , { "blksize", QString::number(buf.st_blksize) }
           , { "blkcnt",  QString::number(buf.st_blocks)  }
           , { "atime",   atime                           }
           , { "mtime",   mtime                           }
           , { "ctime",   ctime                           }
           };

  } else {
    return { { "error", strerror(errno) } };
  }
}

#endif // _QUADRO_FILEBRIDGE_CPP
