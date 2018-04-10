#include "libxmlxsltprocessor.h"

#include <libxml/xmlmemory.h>
#include <libxml/debugXML.h>
#include <libxml/HTMLtree.h>
#include <libxml/xmlIO.h>
#include <libxml/DOCBparser.h>
#include <libxml/xinclude.h>
#include <libxml/catalog.h>

#include <libxslt/libxslt.h>
#include <libxslt/transform.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/xsltutils.h>

#include <QFile>
#include <QDebug>
#include <QTextStream>

LibXmlXsltProcessor::LibXmlXsltProcessor()
{
}

QString LibXmlXsltProcessor::ProcessXslt(const QString &xmlData, const QString &xsltData)
{
    const char *TMP_XSLT_FILENAME = "schema.xsl";
    const char *TMP_XML_NAMESPACE = "doc.xml";
    const char *TMP_RESULT_FILENAME = "res.xml";

    QFile f(TMP_XML_NAMESPACE);
    f.open(QIODevice::WriteOnly);

    QTextStream stream(&f);
    stream << xmlData;

    f.close();

    f.setFileName(TMP_XSLT_FILENAME);
    f.open(QIODevice::WriteOnly);
    stream.setDevice(&f);
    stream << xsltData;
    f.close();

    xmlDocPtr style, doc, res;

    xsltStylesheetPtr cur;

    xmlSubstituteEntitiesDefault(1);
    xmlLoadExtDtdDefaultValue = 1;

    style = xmlReadFile(TMP_XSLT_FILENAME, NULL, 0);
    if (!style)
    {
        qDebug() << tr("Ошибка парсинга файла xslt");
        return QString(tr("Ошибка парсинга файла xslt"));
    }
    doc = xmlReadFile(TMP_XML_NAMESPACE, NULL, 0);
    if (!doc)
    {
        qDebug() << tr("Ошибка парсинга файла xml");
        xmlFreeDoc(style);
        return QString(tr("Ошибка парсинга файла xml"));
    }

    //cur = xsltLoadStylesheetPI(style);
    cur = xsltParseStylesheetDoc(style);

    if (!cur)
    {
        qDebug() << tr("Ошибка загрузка страницы стилей!");
        xmlFreeDoc(doc);
        xmlFreeDoc(style);
        return QString(tr("Ошибка загрузка страницы стилей!"));
    }

    res = xsltApplyStylesheet(cur, doc, 0);

    if (!res)
    {
        qDebug() << tr("Невозможо применить таблицу стилей!");

        xsltFreeStylesheet(cur);
        xmlFreeDoc(doc);
        //xmlFreeDoc(style);
        return QString(tr("Невозможо применить таблицу стилей!"));
    }

    FILE *f2 = fopen(TMP_RESULT_FILENAME, "w");
    xmlDocDump(f2, res);
    fclose(f2);

    f.setFileName(TMP_RESULT_FILENAME);
    f.open(QIODevice::ReadOnly);
    stream.setDevice(&f);
    QString result = stream.readAll();
    f.close();

    xsltFreeStylesheet(cur);
    xmlFreeDoc(doc);
    xmlFreeDoc(res);
    //xmlFreeDoc(style);

    return result;
}

QString LibXmlXsltProcessor::ProcessXsltFiles(const QString &xmlFile, const QString &xsltFile)
{
    xmlDocPtr style, doc, res;

    xsltStylesheetPtr cur;

    xmlSubstituteEntitiesDefault(1);
    xmlLoadExtDtdDefaultValue = 1;

    style = xmlReadFile(xsltFile.toLocal8Bit(), NULL, 0);
    if (!style)
    {
        qDebug() << tr("Ошибка парсинга файла xslt");

        lastError = "Ошибка парсинга файла xslt";

        return QString(tr("Ошибка парсинга файла xslt"));
    }
    doc = xmlReadFile(xmlFile.toLocal8Bit(), NULL, 0);
    if (!doc)
    {
        qDebug() << tr("Ошибка парсинга файла xml");
        xmlFreeDoc(style);

        lastError = "Ошибка парсинга файла xml";

        return QString(tr("Ошибка парсинга файла xml"));
    }

    //cur = xsltLoadStylesheetPI(style);
    cur = xsltParseStylesheetDoc(style);

    if (!cur)
    {
        qDebug() << tr("Ошибка загрузка страницы стилей!");
        xmlFreeDoc(doc);
        xmlFreeDoc(style);

        return QString();
    }

    res = xsltApplyStylesheet(cur, doc, 0);

    if (!res)
    {
        qDebug() << tr("Невозможо применить таблицу стилей!");

        lastError = "Ошибка загрузка страницы стилей!";

        xsltFreeStylesheet(cur);
        xmlFreeDoc(doc);
        return QString();
    }

    uchar *mem;

    int size;

    xmlDocDumpMemory(res, (xmlChar **)&mem, &size);

    QString resString = QString::fromUtf8((const char *)mem, size);

    
    xsltFreeStylesheet(cur);
    xmlFreeDoc(doc);
    xmlFreeDoc(res);

    lastError = QString();

    return resString;
}

LibXmlXsltProcessor::~LibXmlXsltProcessor()
{
    //No dtor neede - C-styled structs are destoyed before
}
