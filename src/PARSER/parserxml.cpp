#include <QMessageBox>
#include <QStringList>
#include "parserxml.h"

/**
 * @brief TParserXML::TParserXML
 * @param parent
 * @param ver
 */
TParserXML::TParserXML(QObject *parent, const QString ver):
                                    QObject(parent),strVersion(ver){}
/**
 * @brief TParserXML::~TParserXML
 */
TParserXML::~TParserXML(){}
/**
 * @brief TParserXML::makeEliment
 * @param domDoc
 * @param strName
 * @param strAttr
 * @param strText
 * @return
 */
QDomElement TParserXML::makeEliment(QDomDocument& domDoc,
                                    const QString& strName,
                                    const QString& strAttr="",
                                    const QString& strText="")
{
    QDomElement domElement=domDoc.createElement(strName);
    if(!strAttr.isEmpty())
    {
        QDomAttr domAttr = domDoc.createAttribute("number");
        domAttr.setValue(strAttr);
        domElement.setAttributeNode(domAttr);
    }
    if(!strText.isEmpty())
    {
        QDomText domText =domDoc.createTextNode(strText);
        domElement.appendChild(domText);
    }
    return domElement;
}
/**
 * @brief TParserXML::makeDeviceRS232
 * @param domDoc
 * @param nNumber
 * @param prm
 * @return
 */
QDomElement TParserXML::makeDeviceRS232(QDomDocument& domDoc,
                                        const int nNumber,
                                        const QStringList &prm)
{
    QDomElement domElement = makeEliment(domDoc,"RS232",QString().setNum(nNumber));
    domElement.appendChild(makeEliment(domDoc,"TimeDelay","",prm.at(0)));
    domElement.appendChild(makeEliment(domDoc,"BaudRate","",prm.at(1)));
    domElement.appendChild(makeEliment(domDoc,"ByteSize","",prm.at(2)));
    domElement.appendChild(makeEliment(domDoc,"StopBit","",prm.at(3)));
    domElement.appendChild(makeEliment(domDoc,"Parity","",prm.at(4)));
    return domElement;
}
/**
 * @brief TParserXML::makeDeviceEtherenet
 * @param domDoc
 * @param nNumber
 * @return
 */
QDomElement TParserXML::makeDeviceEtherenet(QDomDocument &domDoc,
                                            const int nNumber,
                                            const QStringList &prm)
{
    QDomElement domElement = makeEliment(domDoc,"ETHERNET",QString().setNum(nNumber));
    domElement.appendChild(makeEliment(domDoc,"TimeDelay","",prm.at(0)));
    domElement.appendChild(makeEliment(domDoc,"IP","",prm.at(1)));
    domElement.appendChild(makeEliment(domDoc,"PORT","",prm.at(2)));
    return domElement;
}
/**
 * @brief TParserXML::SaveDeviceXML
 * @param fileName
 * @param strPrmCOM
 * @param strPrmEth
 * @return
 */
bool TParserXML::SaveDeviceXML(const QString &fileName,
                               const QStringList strPrmCOM,
                               const QStringList strPrmEth)
{
    QDomDocument doc("CONFIG");
    QDomElement domElement = doc.createElement("SoundConfigurator");
    doc.appendChild(domElement);
    //set Version files
    domElement.appendChild(makeEliment(doc,"Version","",strVersion));
    // set parameters of device RS232
    domElement.appendChild(makeDeviceRS232(doc,1,strPrmCOM));
    // set parameters of device Eth
    domElement.appendChild(makeDeviceEtherenet(doc,1,strPrmEth));
    // create file device.xml
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream(&file)<<doc.toString();
        file.close();
        return true;
    }
    return false;
}
/**
 * @brief TParserXML::TraversDeviceNode
 * @param element
 * @param listPRM
 */
void TParserXML::TraversDeviceNode(const QDomElement &element, QStringList &listPRM)
{
    QDomElement child = element.firstChildElement();
    if(element.tagName()=="RS232")
    {
        while(!child.isNull())
        {
            if(child.tagName()=="TimeDelay")listPRM.append(child.text());
            if(child.tagName()=="BaudRate")listPRM.append(child.text());
            if(child.tagName()=="ByteSize")listPRM.append(child.text());
            if(child.tagName()=="Parity")listPRM.append(child.text());
            if(child.tagName()=="StopBit")listPRM.append(child.text());
            if(child.tagName()=="FlowControl")listPRM.append(child.text());
            child = child.nextSiblingElement();
        }
    }
    else if(element.tagName()=="ETHERNET")
    {
        if(child.tagName()=="TimeDelay")listPRM.append(child.text());
        if(child.tagName()=="IP")listPRM.append(child.text());
        if(child.tagName()=="PORT")listPRM.append(child.text());
    }
    child = element.nextSiblingElement();
    if(!child.isNull())
        TraversDeviceNode(child,listPRM);
}
/**
 * @brief TParserXML::OpenXML
 * @param fileName
 * @param listPRM
 * @return
 */
bool TParserXML::OpenXML(const QString &fileName,
                         QStringList &listPRM)
{
    QDomDocument doc;
    QString errorStr = "";
    int errorLine = 0;
    int errorColumn = 0;
    bool Return = false;

    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        if(doc.setContent(&file,true,&errorStr,&errorLine,&errorColumn))
        {
            QDomElement domElelment = doc.documentElement();
            if(domElelment.tagName()!="SoundConfigurator")
            {
                QMessageBox::information(0, tr("Configuration File"),
                                            tr("This file does not configure the device!"));
            }else{
                QDomElement child = domElelment.firstChildElement("Version");
                if(child.text()==strVersion)
                { // XML file is the same version
                    child = domElelment.firstChildElement("RS232");

                    TraversDeviceNode(child,listPRM);
                }
                Return = true;
            }
        }else{
            QMessageBox::information(0, tr("Configuration File"),
                                        tr("Error on line %1, column %2:\n%3!")
                                        .arg(errorLine)
                                        .arg(errorColumn)
                                        .arg(errorStr));
        }
        file.close();
    }else{
        DefaultDevicesXML("devices.xml");
        OpenXML(fileName,listPRM);
    }
return Return;
}
/**
 * @brief TParserXML::DefaultDevicesXML
 * @param fileName
 */
void TParserXML::DefaultDevicesXML(const QString file)
{
    QStringList strCom,strEth;
    strCom<<"150"<<"115200"<<"8"<<"1"<<"1"<<"0";
    strEth<<"10"<<"192.168.0.100"<<"10001";
    SaveDeviceXML(file, strCom, strEth);
}
