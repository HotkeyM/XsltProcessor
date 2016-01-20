#ifndef LIBXMLXSLTPROCESSOR_H
#define LIBXMLXSLTPROCESSOR_H

#include <QObject>

#include "xsltprocessor.h"

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

class LibXmlXsltProcessor : public XsltProcessor
{
public:
    LibXmlXsltProcessor();

    QString ProcessXslt(const QString &xmlData, const QString &xsltData);
    QString ProcessXsltFiles(const QString &xmlFile, const QString &xsltFile);

    ~LibXmlXsltProcessor();
};

#endif // LIBXMLXSLTPROCESSOR_H
