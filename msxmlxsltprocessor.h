#ifndef MSXMLXSLTPROCESSOR_H
#define MSXMLXSLTPROCESSOR_H

#include "xsltprocessor.h"
#include <QDebug>
#include <QMessageBox>
#include <ActiveQt/QAxObject>



class MsXmlXsltProcessor : public XsltProcessor
{
    Q_OBJECT
public:
    explicit MsXmlXsltProcessor();

    QString ProcessXslt(const QString &xmlData, const QString &xsltData);
    QString ProcessXsltFiles(const QString &xmlFile, const QString &xsltFile);

    QString GetLastError();

    ~MsXmlXsltProcessor();
public slots:
    void ShowError(int errcode, QString err, QString err2, QString err3);
private:

    QString lastError;

    bool exceptionFlag;

    QAxObject *xml;
    QAxObject *xslt;
};


#endif // MSXMLXSLTPROCESSOR_H
