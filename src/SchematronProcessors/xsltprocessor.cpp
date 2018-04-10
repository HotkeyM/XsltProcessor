#include "xsltprocessor.h"
#include <QFile>
#include <QTextStream>

XsltProcessor::XsltProcessor(QObject *parent) : QObject(parent)
{
}

QString XsltProcessor::ProcessXsltFiles(const QString &xmlFile, const QString &xsltFile)
{
    QFile xml(xmlFile);
    xml.open(QIODevice::ReadOnly);
    QString xmlData = xml.readAll();
    xml.close();

    QFile xslt(xsltFile);
    xml.open(QIODevice::ReadOnly);
    QString xsltData = xslt.readAll();
    xslt.close();

    return ProcessXslt(xmlData, xsltData);
}

XsltProcessor::~XsltProcessor()
{
}

bool XsltProcessor::ProcessXsltToFile(const QString &xmlData, const QString &xsltData, const QString &dstFile)
{
    QFile f(dstFile);
    if (!f.open(QIODevice::WriteOnly))
        return false;
    QTextStream str(&f);
    QString data = ProcessXslt(xmlData, xsltData);
    if (data.isEmpty())
    {
        f.close();
        return false;
    }
    str << data;
    f.close();

    return true;
}

bool XsltProcessor::ProcessXsltFilesToFile(const QString &xmlFile, const QString &xsltFile, const QString &dstFile)
{

    QFile f(dstFile);
    if (!f.open(QIODevice::WriteOnly))
        return false;
    QTextStream str(&f);
    QString data = ProcessXsltFiles(xmlFile, xsltFile);
    if (data.isEmpty())
    {
        f.close();
        return false;
    }
    str << data;
    f.close();

    return true;
}
