#ifndef PARSERXML_H
#define PARSERXML_H

#include <QMainWindow>
#include <QObject>
#include <QtXml>

class QFile;

class TParserDeviceXML
{
public:
  explicit TParserDeviceXML();
};

class TParserXML : public QObject
{
Q_OBJECT

public:
  explicit TParserXML(QObject *parent = 0,const QString ver="V1.1");
    ~TParserXML();
    //bool SaveXML(const QString &fileName, const QStringList strPRM);
    bool OpenXML(const QString &fileName,
                 QStringList &listPRM);
private:
    const QString strVersion;
    QDomElement makeEliment(QDomDocument &domDoc,
                            const QString& strName,
                            const QString& strAttr,
                            const QString& strText);
    QDomElement makeDeviceRS232(QDomDocument& domDoc,
                                const int nNumber,
                                const QStringList &);
    QDomElement makeDeviceEtherenet(QDomDocument& domDoc,
                                    const int nNumber,
                                    const QStringList &);

    void TraversDeviceNode(const QDomElement &element,
                           QStringList &listPRM);
    void DefaultDevicesXML(const QString file);
    void ConfigDefault(void);
    bool SaveDeviceXML(const QString &fileName,
                       const QStringList strPrmCOM,
                       const QStringList strPrmEth);
};

#endif // PARSERXML_H
