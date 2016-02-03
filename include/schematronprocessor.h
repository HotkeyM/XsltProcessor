#ifndef SCHEMATRONPROCESSOR_H
#define SCHEMATRONPROCESSOR_H

#include <QObject>

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDomDocument>
#include <QRegExp>
#include <QDir>
#include <QList>

#include "xsltprocessor.h"




class Q_DECL_IMPORT SchematronProcessor : public QObject
{
    Q_OBJECT
public:

    struct SchematronRule
    {
        SchematronRule(){}
        SchematronRule(QString rule, QString context, QString description):rule(rule),context(context),description(description){}

        QString rule;
        QString context;
        QString description;
    };


    typedef SchematronProcessor::SchematronRule Rule;

    explicit SchematronProcessor(XsltProcessor *proc, QObject *parent = 0);

    explicit SchematronProcessor(XsltProcessor *proc, bool preprocess, QObject *parent = 0);

    ~SchematronProcessor();

    bool CheckSchematronRule(const QString &rule, const QString &context);

    bool SetDebugOutput(bool debug);

    QDomElement RuleToDomElement(const Rule &r, QDomDocument doc = QDomDocument());

    QString ExportSchematron(const QList <Rule> &rules);
    QString ExportSchematron(const Rule &r);
    QString ExportSchematron(const QString &rule, const QString &context, const QString &description);

    QString CompileExportedSchematron(const QString& schematron);

    bool ExportAndCompileSchematron(const QList <Rule> &rules, const QString exportFilename, const QString compileFilename);

    QString ProcessSchematron(const QString &xmlData);

    bool ExportSchematronToFile(const QList <Rule> &rules, const QString &filename);
    bool ExportSchematronToFile(const Rule &r, const QString &filename);
    bool ExportSchematronToFile(const QString &rule, const QString &context, const QString &description, const QString &filename);

    bool CompileSchematronToFile(const QList <Rule> &rules, QString compiledFilename);


signals:

public slots:
private:


    bool debug = false;

    QString schematronCompilerFile = "skeleton1-5.xsl";

    QString schematronCompiler = "";

    QDomElement FormBaseSchematronDocument(const QString &patternName = "pattern");

    QDomElement FillElementWithRules();

    QList <Rule> rules;

    QDomDocument *schematronDocument = nullptr;
    QDomElement rulesElement;

    XsltProcessor *proc = nullptr;

    QString PreProcessRule(QString rawRules);
    QString RemoveComments(QString rule);

    bool preprocess = false;
};

#endif // SCHEMATRONPROCESSOR_H
