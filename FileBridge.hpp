#ifndef _QUADRO_FILEBRIDGE_HPP
#define _QUADRO_FILEBRIDGE_HPP

#include <QHash>
#include <QObject>
#include <QSharedPointer>

class FileHandle
  : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString content READ content)
  Q_PROPERTY(bool failed READ failed)
  Q_PROPERTY(QString reason READ reason)

  public:
    FileHandle(const QString & filename);
    const QString & content(void);
    bool failed(void);
    QString reason(void);

  protected:
    bool m_failed;
    QString m_reason;
    QString m_content;

    void read(const QString & filename);
}; // class FileHandle

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
