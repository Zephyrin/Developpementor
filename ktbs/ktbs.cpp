#include "ktbs.h"

#include <QDebug>
#include <sstream>

KTBS::KTBS(const QString &baseUrl,
           const int port,
           const QString &baseName,
           const QString &modelName,
           const QString &traceName,
           const QString &sessionName,
           const QString &modelCreationFile
           )
  :
    baseUrl(baseUrl),
    port(port),
    baseName(baseName),
    modelName(modelName),
    traceName(traceName),
    sessionName(sessionName),
    modelCreationFile(modelCreationFile),
    url(baseUrl),
    contentType("Content-Type"),
    jsonType("application/json"),
    turtleType("text/turtle"),
    connexionOk(false),
    fileBase(NULL),
    fileModel(NULL),
    nbSent(0),
    nbReceived(0)
{
  manager = new QNetworkAccessManager();
  url.setPort(port);
}

KTBS::~KTBS()
{
  delete manager;
}

void KTBS::init()
{
  if(baseUrl.isEmpty() || baseName.isEmpty() || modelName.isEmpty()
     || traceName.isEmpty())
    emit initDone();
  else
    isBaseExists();
}

void KTBS::isBaseExists()
{
  QNetworkRequest request;
  url.setUrl(baseUrl + "/" + baseName + "/");
  url.setPort(port);
  qDebug() << url;
  request.setUrl(url);
  request.setRawHeader(contentType, jsonType);
  QNetworkReply *reply = manager->get(request);
  connect(reply, SIGNAL(finished()), this, SLOT(isBaseExistsSlot()));
  qDebug() << "Requet base if exist ?";
  emit logSignal("Checked if base exists...");
}

void KTBS::isModelExists()
{
  QNetworkRequest request;
  url.setUrl(baseUrl + "/" + baseName + "/" + modelName);
  url.setPort(port);
  request.setUrl(url);
  request.setRawHeader(contentType, jsonType);
  QNetworkReply *reply = manager->get(request);
  connect(reply, SIGNAL(finished()), this, SLOT(isModelExistsSlot()));
  qDebug() << "Requet model if exist ?";
  emit logSignal("Checked if model exists...");
}

void KTBS::isTraceExists()
{
  QNetworkRequest request;
  url.setUrl(baseUrl + "/" + baseName + "/" + traceName + "/");
  url.setPort(port);

  request.setUrl(url);
  request.setRawHeader(contentType, jsonType);
  QNetworkReply *reply = manager->get(request);
  connect(reply, SIGNAL(finished()), this, SLOT(isTraceExistsSlot()));
  qDebug() << "Requet trace if exist ?";
  emit logSignal("Checked if trace exists...");
}

void KTBS::createBase()
{
  qDebug() << "Create base";
  QNetworkRequest request;

  url.setUrl(baseUrl + "/");
  url.setPort(port);
  request.setUrl(url);
  request.setRawHeader(contentType, jsonType);
  QByteArray data("{\"id\":\"" + baseName.toLatin1() + "/\","
                  "\"@type\":\"Base\","
                  "\"label\":\"Create by ktbs c++ lib\"}"
        );
  emit logSignal("Create a new base.");
  QNetworkReply * reply = manager->post(request, data);
  connect(reply, SIGNAL(finished()), this, SLOT(createBaseSlot()));
}

void KTBS::createModel()
{
  qDebug() << "Create model";
  QNetworkRequest request;
  url.setUrl(baseUrl + "/" + baseName + "/");
  url.setPort(port);

  request.setUrl(url);
  request.setRawHeader(contentType, turtleType);
  fileModel = new QFile(modelCreationFile);
  fileModel->open(QIODevice::ReadOnly);
  if(fileModel->isOpen())
    {
      emit logSignal("Create a new model.");
      QNetworkReply * reply = manager->post(request, fileModel);
      connect(reply, SIGNAL(finished()), this, SLOT(createModelSlot()));
    }
  else
    {
      qDebug() << "Failed to open file " << modelCreationFile;
      fileModel->deleteLater();
      emit initDone();
      emit logSignal("Failed to open file : " + modelCreationFile + ".");
    }
}

void KTBS::createTrace()
{
  qDebug() << "Create trace";
  QNetworkRequest request;
  url.setUrl(baseUrl + "/" + baseName + "/" );
  url.setPort(port);

  request.setUrl(url);
  request.setRawHeader(contentType, jsonType);
  QDateTime time = QDateTime::currentDateTime();
  QUrl urlModel(baseUrl + "/" + baseName + "/" + modelName);
  urlModel.setPort(port);
  QByteArray createStoredTraceByte(
        "{"
        "\"@id\": \"" + traceName.toLatin1() + "/\",\n"
        "\"@type\": \"StoredTrace\",\n"
        "\"hasModel\": \"" + urlModel.toString().toLatin1() + "\",\n"
        "\"origin\": \"" + time.toString("yyyy-MM-ddTHH:mm:sst").toLatin1()
        + "\",\n"
        + "\"defaultSubject\": \"" + sessionName.toLatin1() + "\"\n}"
        );
  emit logSignal("Create a new trace.");
  QNetworkReply * reply = manager->post(request, createStoredTraceByte);
  connect(reply, SIGNAL(finished()), this, SLOT(createTraceSlot()));
}

void KTBS::isBaseExistsSlot()
{
  QNetworkReply *reply = static_cast<QNetworkReply *>(this->sender());
  QVariant statusCode
      = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
  if(statusCode == 200)
    {
      qDebug() << "Base exists.";
      emit logSignal("Base exists");
      isModelExists();
    }
  else if(statusCode == 404)
    {
      qDebug() << "Base doesnot exists.";
      emit logSignal("Base does not exists.");
      createBase();
    }
  else
    {
      emit logSignal("Unknow error : " + reply->errorString() + ".");
      emit initDone();
    }
  reply->deleteLater();
}

void KTBS::isModelExistsSlot()
{
  QNetworkReply *reply = static_cast<QNetworkReply *>(this->sender());
  QVariant statusCode
      = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
  if(statusCode == 200)
    {
      qDebug() << "Mode exists.";
      emit logSignal("Model exists.");
      isTraceExists();
    }
  else if(statusCode == 404)
    {
      qDebug() << "Model doesnot exists";
      emit logSignal("Model does not exists.");
      createModel();
    }
  else
    {
      emit logSignal("Unknow error : " + reply->errorString() + ".");
      emit initDone();

    }
  reply->deleteLater();
}

void KTBS::isTraceExistsSlot()
{
  QNetworkReply *reply = static_cast<QNetworkReply *>(this->sender());
  QVariant statusCode
      = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
  if(statusCode == 200)
    {
      qDebug() << "Trace exists.";
      emit logSignal("Trace exists.");
      connexionOk = true;
      emit initDone();
    }
  else if(statusCode == 404)
    {
      emit logSignal("Trace does not exists.");
      qDebug() << "Trace doesnot exists.";
      createTrace();
    }
  else
    {
      emit logSignal("Unknow error : " + reply->errorString() + ".");
      emit initDone();
    }
  reply->deleteLater();
}

void KTBS::createBaseSlot()
{
  QNetworkReply *reply = static_cast<QNetworkReply *>(this->sender());
  QVariant statusCode
      = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
  if(statusCode == 201)
    {
      qDebug() << "Base created.";
      emit logSignal("Base created.");
      isModelExists();
    }
  else if(statusCode == 403)
    {
      emit logSignal("Base already exists.");
      isModelExists();
    }
  else
    {
      emit logSignal("Unknown error : " + reply->errorString() + ".");
      qDebug() << reply->errorString();
      emit initDone();
    }
  fileBase->close();
  fileBase->deleteLater();
  reply->deleteLater();
}

void KTBS::createModelSlot()
{
  QNetworkReply *reply = static_cast<QNetworkReply *>(this->sender());
  qDebug() << reply->request().url();

  QVariant statusCode
      = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
  qDebug() << statusCode;
  if(statusCode == 201)
    {
      emit logSignal("Model created.");
      qDebug() << "Model create.";
      isTraceExists();
    }
  else
    {
      emit logSignal("Unknown error : " + reply->errorString() + ".");
      emit initDone();
      qDebug() << reply->errorString();
    }
  fileModel->close();
  fileModel->deleteLater();
  reply->deleteLater();
}

void KTBS::createTraceSlot()
{
  QNetworkReply *reply = static_cast<QNetworkReply *>(this->sender());
  QVariant statusCode
      = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
  if(statusCode == 201)
    {
      qDebug() << "Trace create.";
      emit logSignal("Trace created.");
      connexionOk = true;
    }
  else
    {
      emit logSignal("Unknown error : " + reply->errorString() + ".");
      qDebug() << reply->errorString();
    }
  emit initDone();
  reply->deleteLater();
}

void KTBS::postToTraceSlot()
{
  QNetworkReply *reply = static_cast<QNetworkReply *>(this->sender());
  QVariant statusCode
      = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
  nbReceived ++;
  if(statusCode != 201)
    {
      qWarning() << "Data did not save on trace.";
      qDebug() << reply->readAll();
      qDebug() << reply->errorString();
    }
  else
    {
      qDebug() << "Trace save to ktbs";
    }
  if(nbSent == nbReceived)
    emit allRecieved();
  reply->deleteLater();
  emit logSignal("Obselts sent : " + QString::number(nbSent) + "; received : " +
                 QString::number(nbReceived));
}

void KTBS::getContext(std::ostream &os) const
{
  QUrl url(baseUrl + "/" + baseName + "/" + modelName);
  url.setPort(port);

  os << "@prefix : <http://liris.cnrs.fr/silex/2009/ktbs#> ." << std::endl
     << "@prefix m: <" << url.toString().toStdString() << "#> ."
     << std::endl
     << "@prefix s: <" << url.toString().toStdString() << "#Step/> ."
     << std::endl
     << "@prefix i: <" << url.toString().toStdString() << "#Interaction/> ."
     << std::endl
     << "@prefix is: <" << url.toString().toStdString() << "#Interactions/> ."
     << std::endl
     << "@prefix pr: <" << url.toString().toStdString() << "#Proposed/> ."
     << std::endl
     << "@prefix r: <" << url.toString().toStdString() << "#Result/> ."
     << std::endl
     << "@prefix e: <" << url.toString().toStdString() << "#Experience/> ."
     << std::endl

     << "<@obsels> a :StoredTraceObsels ." << std::endl
     << "<.> :hasObselCollection <@obsels> ." << std::endl;
}

void KTBS::postToTrace(const std::string &data)
{
  if(data.empty())
    return;
  std::stringstream os;
  getContext(os);
  os << data;
  QNetworkRequest request;
  url.setUrl(baseUrl + "/" + baseName + "/" + traceName + "/");
  url.setPort(port);
  qDebug() << url;
  qDebug() << os.str().c_str();
  request.setUrl(url);
  request.setRawHeader(contentType, turtleType);
  QNetworkReply *reply = manager->post(request, os.str().c_str());
  connect(reply, SIGNAL(finished()), this, SLOT(postToTraceSlot()));
  nbSent++;
  emit logSignal("Obsels sent : " + QString::number(nbSent) + "; received : " +
                 QString::number(nbReceived));
}

void KTBS::postToTrace(const std::vector<std::string> &vector)
{
  for(std::string str : vector)
    {
      postToTrace(str);
    }
}

bool KTBS::canSent() const
{
  return connexionOk;
}
