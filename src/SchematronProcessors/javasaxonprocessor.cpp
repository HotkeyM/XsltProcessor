#include "javasaxonprocessor.h"

JavaSaxonProcessor::JavaSaxonProcessor()
{
    Singleton<Java>::GetInstance("c:/java/saxonxslt.jar");
}

JavaSaxonProcessor::JavaSaxonProcessor(QString jarFile)
{
    Singleton<Java>::GetInstance(jarFile.toLocal8Bit().data());
}

QString JavaSaxonProcessor::ProcessXslt(const QString &xmlData, const QString &xsltData)
{

    if (xmlData.isEmpty()) qDebug() << "xmldata is empty";
    if (xsltData.isEmpty()) qDebug() << "xsltdata is empty";


    std::string str = Singleton<Java>::GetInstance()->CallStaticMethod<std::string, const char*, const char*>("org/xsltprocessor/processor", "ProcessXsltException", xmlData.toLocal8Bit().constData(), xsltData.toLocal8Bit().constData());

    if (!str.empty()) qDebug() << "processing success" << str.size();

    if (Singleton<Java>::GetInstance()->Exception())
    {
        qDebug() << "exception";
        lastError = "Java code error";
        return QString();
    }

    if (str.empty())
    {
        qDebug() << "std::string empty";
    }
    lastError = QString();
    QString res = QString::fromStdString(str);

    return res;
}

QString JavaSaxonProcessor::GetLastError()
{
    return lastError;
}


