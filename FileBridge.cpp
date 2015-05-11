#ifndef _QUADRO_FILEBRIDGE_CPP
#define _QUADRO_FILEBRIDGE_CPP

#include <fstream>
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
      std::string content;
      file >> content;
      return { { "content", QString::fromStdString(content) }
             , { "error", QVariant() }
             };

    } else {
      throw std::runtime_error(showIostate(file.rdstate()));
    }

  } catch (const std::exception & e) {
    return { { "content", QVariant() }, { "error",  e.what() } };
  }
}

#endif // _QUADRO_FILEBRIDGE_CPP
