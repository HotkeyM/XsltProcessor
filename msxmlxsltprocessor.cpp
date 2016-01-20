#include "msxmlxsltprocessor.h"

MsXmlXsltProcessor::MsXmlXsltProcessor():exceptionFlag(false)
{
    xml = new QAxObject("MSXML2.DOMDOCUMENT.6.0", this);

    xslt = new QAxObject("MSXML2.DOMDOCUMENT.6.0", this);

    connect(xml, SIGNAL(exception(int, QString, QString, QString)), this, SLOT(ShowError(int,QString,QString,QString)));


}



QString MsXmlXsltProcessor::ProcessXslt(const QString &xmlData, const QString &xsltData)
{
    if (xml->control().isEmpty() || xslt->control().isEmpty())
    {
        qDebug() << "No MSXML2.DOMDOCUMENT.6.0 object installed";

        return QString();
    }

    bool setProp;
    //setProp = xml->setProperty("ProhibitDTD", false);
    setProp = xml->dynamicCall("setProperty(\"ProhibitDTD\", false)").toBool();
    setProp = xml->setProperty("validateOnParse",false);
    setProp = xml->setProperty("resolveExternals", false);
    setProp = xml->setProperty("async", false);



    //////connect(xml, SIGNAL(exception(int, QString, QString, QString)), this, SLOT(ShowError(int,QString,QString,QString)));
    xml->dynamicCall("loadXML(QString)", xmlData);

    if (xml->querySubObject("parseError")->property("errorCode").toInt() != 0)
    {
        qDebug() << "Xml file parse error" << xml->querySubObject("parseError")->property("reason").toString();

        return QString();
    }

    xslt->setProperty("async", false);
    xslt->setProperty("ProhibitDTD", false);
    xslt->dynamicCall("setProperty(\"ProhibitDTD\", false)");
    //////connect(xslt, SIGNAL(exception(int, QString, QString, QString)), this, SLOT(ShowError(int,QString,QString,QString)));
    xslt->dynamicCall("loadXML(QString)", xsltData);


    //if (xsl.parseError.errorCode != 0)
   //     WScript.Echo ("XSL Parse Error : " + xsl.parseError.reason);



    if (xslt->querySubObject("parseError")->property("errorCode").toInt() != 0)
    {
        qDebug() << "Xslt file parse error" << xslt->querySubObject("parseError")->property("reason").toString();

        lastError = xslt->querySubObject("parseError")->property("reason").toString();

        return QString();
    }

    //connect(xml, QAxBase:)

    QString result = xml->dynamicCall("transformNode(QAxObject*)", xslt->querySubObject("documentElement")->asVariant()).toString();
    if (exceptionFlag)
    {
        exceptionFlag = false;
        return QString();
    }

    //QString result = xml->querySubObject("documentElement")->property("xml").toString();
    return result;
}

QString MsXmlXsltProcessor::ProcessXsltFiles(const QString &xmlFile, const QString &xsltFile)
{
    if (xml->control().isEmpty() || xslt->control().isEmpty())
    {
        qDebug() << "No MSXML2.DOMDOCUMENT.6.0 object installed";

        return QString();
    }

    bool setProp;
    //setProp = xml->setProperty("ProhibitDTD", false);
    setProp = xml->dynamicCall("setProperty(\"ProhibitDTD\", false)").toBool();
    setProp = xml->setProperty("validateOnParse",false);
    setProp = xml->setProperty("resolveExternals", false);
    setProp = xml->setProperty("async", false);

    //////connect(xml, SIGNAL(exception(int, QString, QString, QString)), this, SLOT(ShowError(int,QString,QString,QString)));
    xml->dynamicCall("load(QString)", xmlFile);

    if (xml->querySubObject("parseError")->property("errorCode").toInt() != 0)
    {
        qDebug() << "Xml file parse error" << xml->querySubObject("parseError")->property("reason").toString();

        return QString();
    }

    xslt->setProperty("async", false);
    xslt->setProperty("ProhibitDTD", false);
    xslt->dynamicCall("setProperty(\"ProhibitDTD\", false)");
    //////connect(xslt, SIGNAL(exception(int, QString, QString, QString)), this, SLOT(ShowError(int,QString,QString,QString)));
    xslt->dynamicCall("load(QString)", xsltFile);


    //if (xsl.parseError.errorCode != 0)
   //     WScript.Echo ("XSL Parse Error : " + xsl.parseError.reason);

    if (xslt->querySubObject("parseError")->property("errorCode").toInt() != 0)
    {
        qDebug() << "Xslt file parse error" << xslt->querySubObject("parseError")->property("reason").toString();

        return QString();
    }

    QAxObject *docElement = xslt->querySubObject("documentElement");

    if (!docElement)
    {
        qDebug() << "Something bad";
    }

    qDebug() << "xml" << xml->dynamicPropertyNames();
    qDebug() << "xslt" << xslt->dynamicPropertyNames();

    qDebug() << "xslt.documentElement" << docElement->dynamicPropertyNames();

    auto lst = docElement->dynamicPropertyNames();

    qDebug() << docElement->control();

    QString result = xml->dynamicCall("transformNode(QAxObject*)", docElement->asVariant()).toString();
    //xml->dynamicCall("transformNode(QAxObject*)", xslt->asVariant());

    if (exceptionFlag)
    {
        exceptionFlag = false;
        return QString();
    }

   // QString result = xml->querySubObject("documentElement")->property("xml").toString();
    return result;
}

QString MsXmlXsltProcessor::GetLastError()
{
    return lastError;
}

MsXmlXsltProcessor::~MsXmlXsltProcessor()
{
    //Деструктор не нужен, ком-объекты удалятся, т.к. они потомки данного объекта
}

void MsXmlXsltProcessor::ShowError(int errcode, QString err, QString err2, QString err3)
{
    exceptionFlag = true;

    lastError = err + ". " + err2 + ". " + err3;
    /*QMessageBox msg;

    msg.setText("Ошибка: " + err + ";" + err2);
    msg.exec();
    */
}
