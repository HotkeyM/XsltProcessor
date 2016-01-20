#ifndef XSLTPROSESSOR_H
#define XSLTPROSESSOR_H

#include <QObject>

class XsltProcessor : public QObject
{
    Q_OBJECT
public:
    explicit XsltProcessor(QObject *parent = 0);
    virtual ~XsltProcessor() = 0;

    virtual QString ProcessXslt(const QString &xmlData, const QString &xsltData) = 0;
    virtual QString ProcessXsltFiles(const QString &xmlFile, const QString &xsltFile);

    //Подразумевается, что типичный процессор сохраняет в память, а сохранение в файл - дополнительная обертка
    virtual bool ProcessXsltToFile(const QString &xmlData, const QString &xsltData, const QString &dstFile);
    virtual bool ProcessXsltFilesToFile(const QString &xmlFile, const QString &xsltFile, const QString &dstFile);

    virtual QString GetLastError()
    {
        return QString();
    }

signals:

public slots:
};

#endif // XSLTPROSESSOR_H
