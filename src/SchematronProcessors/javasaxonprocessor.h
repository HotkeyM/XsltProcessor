#ifndef JAVASAXONPROCESSOR_H
#define JAVASAXONPROCESSOR_H

#include "jnienv.h"
#include "xsltprocessor.h"

class Q_DECL_EXPORT JavaSaxonProcessor : public XsltProcessor
{
  public:
    JavaSaxonProcessor();
    JavaSaxonProcessor(QString jarFile);

    QString ProcessXslt(const QString &xmlData, const QString &xsltData);

    virtual QString GetLastError();

  private:
    QString lastError;
};

#endif // JAVASAXONPROCESSOR_H
