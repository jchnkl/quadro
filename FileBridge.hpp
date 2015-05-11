#ifndef _QUADRO_FILEBRIDGE_HPP
#define _QUADRO_FILEBRIDGE_HPP

#include <fstream>
#include <QDebug>
#include <QObject>
#include <QSharedPointer>
#include <QHash>

// Q_DECLARE_METATYPE(std::string)
// Q_DECLARE_METATYPE(std::ios_base::iostate)

class FileHandle
  : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString content READ content)
  Q_PROPERTY(bool failed READ failed)
  Q_PROPERTY(QString reason READ reason)
  // Q_PROPERTY(bool good READ good)
  // Q_PROPERTY(error good READ good);
  // Q_PROPERTY(std::ios_base::iostate state READ state)

  public:
    // FileHandle(void) {}
    FileHandle(const QString & filename);
    // FileHandle(const FileHandle & other)
    //   : m_content(other.m_content)
    //   , m_state(other.m_state)
    // {}
    // ~FileHandle(void) {}
    // FileHandle &
    // operator=(const FileHandle & other)
    // {
    //   this->m_content = other.m_content;
    //   this->m_state = other.m_state;
    //   return *this;
    // }
    const QString & content(void);
    bool failed(void);
    QString reason(void);

  protected:
    bool m_failed;
    QString m_reason;
    QString m_content;
    // std::ios_base::iostate m_state;
    //   std::ios_base::iostate state(void);

    void read(const QString & filename);
    // void setFailed(const std::string & reason);

}; // class FileHandle

// Q_DECLARE_METATYPE(FileHandle)

class File
  : public QObject
{
  Q_OBJECT

  public:
    File(void);
    Q_INVOKABLE
    FileHandle * open(const QString & filename);
    void close(const QString & filename);

  private:
    QHash<QString, QSharedPointer<FileHandle>> m_FileHandles;
}; // class File

#endif // _QUADRO_FILEBRIDGE_HPP
