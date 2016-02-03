#ifndef LIBXMLXSLTPROCESSOR_H
#define LIBXMLXSLTPROCESSOR_H

#include <QObject>

#include "xsltprocessor.h"



class Q_DECL_IMPORT LibXmlXsltProcessor : public XsltProcessor
{
public:
    LibXmlXsltProcessor();

    QString ProcessXslt(const QString &xmlData, const QString &xsltData);
    QString ProcessXsltFiles(const QString &xmlFile, const QString &xsltFile);

    QString GetLastError()
    {
            return QString();
    }

    ~LibXmlXsltProcessor();

private:
    QString lastError;
};

#endif // LIBXMLXSLTPROCESSOR_H
