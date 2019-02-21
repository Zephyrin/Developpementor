#ifndef KTBS_H
#define KTBS_H

#include <QString>
#include <QObject>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <QFile>
#include <QTime>
#include <vector>

class KTBS :public QObject
{
  Q_OBJECT

public:
  KTBS(const QString &baseUrl,
       const int port,
       const QString &baseName,
       const QString &modelName, const QString &traceName,
       const QString &sessionName = "me",
       const QString &modelCreationFile = ":/model.ttl");

  ~KTBS();

  void init();

  void postToTrace(const std::string &data);
  void postToTrace(const std::vector<std::string> &vector);

  bool canSent() const;

signals:
  void initDone();
  void allRecieved();
  void logSignal(const QString &);
protected:

  void isBaseExists();

  void isModelExists();

  void isTraceExists();

  void createBase();

  void createModel();

  void createTrace();

  void getContext(std::ostream &os) const ;

  void getTimeId(std::ostream &os) const;

private slots:
  void isBaseExistsSlot();

  void isModelExistsSlot();

  void isTraceExistsSlot();

  void createBaseSlot();

  void createModelSlot();

  void createTraceSlot();

  void postToTraceSlot();

private:

  QString baseUrl;
  int port;
  QString baseName;
  QString modelName;
  QString traceName;
  QString sessionName;
  QString modelCreationFile;
  QNetworkAccessManager *manager;
  QUrl url;
  QByteArray contentType;
  QByteArray jsonType;
  QByteArray turtleType;
  bool connexionOk;
  QFile *fileBase;
  QFile *fileModel;
  std::size_t nbSent;
  std::size_t nbReceived;
};

#endif // KTBS_H
