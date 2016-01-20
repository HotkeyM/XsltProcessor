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

    std::string str = Singleton<Java>::GetInstance()->CallStaticMethod<std::string, const char*, const char*>("org/xsltprocessor/processor", "ProcessXslt", xmlData.toLocal8Bit().constData(), xsltData.toLocal8Bit().constData());
    QString res = QString::fromStdString(str);
    if (res.isEmpty())
    {
        lastError = "Java code error";
    }
    return res;
}

QString JavaSaxonProcessor::GetLastError()
{
    return lastError;
}


