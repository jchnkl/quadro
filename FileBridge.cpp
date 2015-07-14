#ifndef _QUADRO_FILEBRIDGE_CPP
#define _QUADRO_FILEBRIDGE_CPP

#include <fstream>
#include <sstream>
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

#endif // _QUADRO_FILEBRIDGE_CPP
