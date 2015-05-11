#ifndef _QUADRO_FILEBRIDGE_CPP
#define _QUADRO_FILEBRIDGE_CPP

#include <fstream>
#include <sstream>
#include <iostream>
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

FileHandle::FileHandle(const QString & filename)
{
  read(filename);
}

const QString &
FileHandle::content(void)
{
  return m_content;
}

bool
FileHandle::failed(void)
{
  return m_failed;
  // return m_state != std::ios_base::goodbit;
}

QString
FileHandle::reason(void)
{
  return m_reason;
  // return QString::fromStdString(showIostate(m_state));
}

// bool
// FileHandle::good(void)
// {
//   return m_state == std::ios_base::goodbit;
// }

// std::ios_base::iostate
// FileHandle::state(void)
// {
//   return m_state;
// }


void
FileHandle::read(const QString & filename)
{
  try {
    std::fstream file(filename.toStdString(), std::ios::in);
    // m_state = file.rdstate();

    if (file.good()) {
      m_failed = false;
      std::string content;
      file >> content;
      m_content = QString::fromStdString(content);
    } else {
      m_failed = true;
      m_reason = QString::fromStdString(showIostate(file.rdstate()));
    }

  } catch (const std::exception & e) {
    m_failed = true;
    m_reason = e.what();
  }
}

// void setFailed(const std::string & reason)
// {
//   m_failed = true;
// }

File::File(void)
{
  // qRegisterMetaType<FileHandle>();
  qRegisterMetaType<FileHandle *>();
  // qRegisterMetaType<std::string>();
  // qRegisterMetaType<std::ios_base::iostate>();
}

FileHandle *
File::open(const QString & filename)
{
  if (m_FileHandles.find(filename) == m_FileHandles.end()) {
    QSharedPointer<FileHandle> handle(new FileHandle(filename));
    m_FileHandles.insert(filename, handle);
    return handle.data();
  } else {
    return m_FileHandles.value(filename).data();
  }
}

void
File::close(const QString & filename)
{
  m_FileHandles.remove(filename);
}

#endif // _QUADRO_FILEBRIDGE_CPP
