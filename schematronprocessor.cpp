#include "schematronprocessor.h"

#include <QDateTime>

#include "msxmlxsltprocessor.h"

SchematronProcessor::SchematronProcessor(XsltProcessor *proc, QObject *parent) : QObject(parent), proc(proc)
{

    QFile skeleton(schematronCompilerFile);
    if (skeleton.open(QIODevice::ReadOnly))
    {
        schematronCompiler =  skeleton.readAll();
    }
    skeleton.close();
}

SchematronProcessor::~SchematronProcessor()
{
    if (proc) delete proc;


}

bool SchematronProcessor::CompileSchematronToFile(const QList <Rule> &rules, QString compiledFilename)
{
    QFile f(schematronCompilerFile);
    if (!f.open(QIODevice::ReadOnly)) return false;

    QString skelet = f.readAll();

    f.close();

    QString compiled = proc->ProcessXslt(ExportSchematron(rules), skelet);

    //QString result = proc->ProcessXslt(compiled,compiled);
    if (compiled.isEmpty())
    {
        QDir().mkdir("debuglog");

        QFile deb("debuglog/" + QString::number(QDateTime::currentMSecsSinceEpoch()) + ".xml");
        if (deb.open(QIODevice::WriteOnly))
        {
            QTextStream out (&deb);
            out << compiled;
        }

        deb.close();
        return false;
    }

    QFile compiledFile (compiledFilename);
    if (!compiledFile.open(QIODevice::WriteOnly)) return false;

    QTextStream str(&compiledFile);
    str << compiled;

    return true;
}

bool SchematronProcessor::CheckSchematronRule(const QString &rule, const QString &context)
{

    QFile f(schematronCompilerFile);
    if (!f.open(QIODevice::ReadOnly)) return false;

    QString skelet = f.readAll();

    f.close();

    QString exported = ExportSchematron(rule, context ,"test");

    QString compiled = proc->ProcessXslt(exported, skelet);

    QString result = proc->ProcessXslt(compiled,compiled);
    if (result.isEmpty())
    {
        QDir().mkdir("debuglog");

        QString filename = QString::number(QDateTime::currentMSecsSinceEpoch());

        QFile deb("debuglog/" + filename + ".xml");
        if (deb.open(QIODevice::WriteOnly))
        {
            QTextStream out (&deb);
            out << compiled;
        }

        deb.close();

        deb.setFileName("debuglog/" + filename + "raw.xml");
        if (deb.open(QIODevice::WriteOnly))
        {

            QTextStream out (&deb);
            out << exported;
            out << "\n";
            out << proc->GetLastError();
        }


        return false;
    }

    else return true;
}

bool SchematronProcessor::SetDebugOutput(bool debug)
{
    this->debug = debug;
    return debug;
}

QDomElement SchematronProcessor::RuleToDomElement(const Rule &r, QDomDocument doc)
{
    if (doc.isNull()) doc = *schematronDocument;

    QDomElement domRule = doc.createElement("rule");
    domRule.setAttribute("context", r.context);
    //pattern.appendChild(domRule);

    QDomElement assert = doc.createElement("assert");
    assert.setAttribute("test", r.rule);
    domRule.appendChild(assert);
    assert.appendChild(doc.createTextNode(r.description));

    QDomElement objectName = doc.createElement("value-of");
    objectName.setAttribute("select","name()");
    assert.appendChild(objectName);
    assert.appendChild(doc.createTextNode("gml:id="));
    objectName = doc.createElement("value-of");
    objectName.setAttribute("select","@gml:id");
    assert.appendChild(objectName);

    return domRule;
}

QString SchematronProcessor::ExportSchematron(const QList<Rule> &rules)
{
   if (!proc)
   {
       return QString();
   }

   FormBaseSchematronDocument();

   this->rules = rules;

   FillElementWithRules();

   QString exportedSchematron;
   QTextStream str(&exportedSchematron);

   schematronDocument->save(str, 1);

   return exportedSchematron;

}

QString SchematronProcessor::ExportSchematron(const Rule &r)
{
    QList <Rule> rules;

    rules.append(r);
    return ExportSchematron(rules);

}


QString SchematronProcessor::ExportSchematron(const QString &rule, const QString &context, const QString &description)
{
    Rule r{rule, context, description};
    return ExportSchematron(r);
}

QString SchematronProcessor::CompileExportedSchematron(const QString &schematron)
{
    //QFile f(schematronCompilerFile);
    QFile f("skeleton1-5.xsl");
    if (!f.open(QIODevice::ReadOnly)) return false;

    QString skelet = f.readAll();

    f.close();


    if (dynamic_cast<XsltProcessor*>(proc) == nullptr)
    {
        qDebug() << "Something wrong";
    }

    QString compiled = proc->ProcessXslt(schematron, skelet);
    return compiled;

}

bool SchematronProcessor::ExportAndCompileSchematron(const QList<SchematronProcessor::Rule> &rules, const QString exportFilename, const QString compileFilename)
{

    //QFile compiledFile(compileFilename);
    //compiledFile.open(QIODevice::WriteOnly);
    //QTextStream strCompile(&compiledFile);


    QString compiled;
    if (false)
    {
        QString exportedSchematron = ExportSchematron(rules);

        if (exportedSchematron.isEmpty()) return false;

        if (!exportFilename.isEmpty())
        {
            QFile exportFile(exportFilename);
            exportFile.open(QIODevice::WriteOnly);

            QTextStream strExport(&exportFile);
            strExport << exportedSchematron;
            exportFile.close();
        }

        /*
    if (schematronCompiler.isEmpty())
    {
        QFile skeleton(schematronCompilerFile);
        if (skeleton.open(QIODevice::ReadOnly))
        {
            schematronCompiler = skeleton.readAll();
        }
        else return false;

    }
    */

        QString comp = proc->ProcessXslt(exportedSchematron, schematronCompiler);

        //QString comp = proc->ProcessXslt("Const1", "Const2");

        if (!comp.isEmpty())
        {
            QFile compiledFile(compileFilename);
            compiledFile.open(QIODevice::WriteOnly);

            QTextStream strCompile(&compiledFile);
            strCompile << comp;
            compiledFile.close();
        }
        else return false;

        //QString compiledSchematron = CompileExportedSchematron(exportedSchematron);

        /*
    if (compiledSchematron.isEmpty()) return false;

    if (!compileFilename.isEmpty())
    {
        QFile compiledFile(compileFilename);
        compiledFile.open(QIODevice::WriteOnly);

        QTextStream strCompile(&compiledFile);
        strCompile << compiledSchematron;
        compiledFile.close();
    }

    */

    }
    else
    {
        QString exported = ExportSchematron(rules);

        if (!exported.isEmpty())
        {



           compiled = proc->ProcessXslt(exported, schematronCompiler);

           /// Почему нельзя открыть файл здесь???
           /// сложный прикол
           QFile compiledFile(compileFilename);
           compiledFile.open(QIODevice::WriteOnly);
           QTextStream strCompile(&compiledFile);
           strCompile << compiled;
           compiledFile.close();
        }
        else return false;


    }



    return true;
}

QString SchematronProcessor::ProcessSchematron(const QString &xmlData)
{
    if (schematronCompiler.isEmpty())
    {
        qDebug() << "Schematron compiler (skeleton) is not loaded";
        return QString();
    }
    QString compiled = proc->ProcessXslt(xmlData, schematronCompiler);
    return compiled;
}

bool SchematronProcessor::ExportSchematronToFile(const QList<SchematronProcessor::Rule> &rules, const QString &filename)
{
    QString text = ExportSchematron(rules);
    if (text.isEmpty()) return false;

    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly)) return false;

    QTextStream str(&f);
    str << text;
    f.close();
}

bool SchematronProcessor::ExportSchematronToFile(const SchematronProcessor::Rule &r, const QString &filename)
{
    QString text = ExportSchematron(r);
    if (text.isEmpty()) return false;

    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly)) return false;

    QTextStream str(&f);
    str << text;
    f.close();
}

bool SchematronProcessor::ExportSchematronToFile(const QString &rule, const QString &context, const QString &description, const QString &filename)
{
    QString text = ExportSchematron(rule,context,description);
    if (text.isEmpty()) return false;

    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly)) return false;

    QTextStream str(&f);
    str << text;
    f.close();

}

QString SchematronProcessor::RemoveComments( QString rule)
{
    QRegExp reg;
    //("(\\(:).*?(:\\))");
    reg.setMinimal(true);
    reg.setPattern("(\\(:).*(:\\))");
    return rule.remove(reg);
    //return rule.remove(QRegExp("(\\(:).*?(:\\))"));
}

QString SchematronProcessor::PreProcessRule(QString rawRules)
{
    QRegExp reg("\\([ ]*(('[A-Za-z0-9]+')[ ]*,[ ]*)+'[A-Za-z0-9]+'[ ]*\\)");
    while (reg.indexIn(rawRules) != -1)
    {
        QString substr = reg.cap();
        QString origsubstr = rawRules;
        substr.replace("," ," or ");
        rawRules.replace(origsubstr, substr);
    }

    rawRules = RemoveComments(rawRules);

    return rawRules;
}


QDomElement SchematronProcessor::FillElementWithRules()
{
    for (auto &r: rules)
    {

        Rule modifiedRule{PreProcessRule(r.rule), PreProcessRule(r.context), r.description};
        rulesElement.appendChild(RuleToDomElement(modifiedRule));
        //QString compiled = ProcessXlstFromFileHddBuffer(preprocessed, SCHEMATRON_COMPILER_FILE);

    }
}

QDomElement SchematronProcessor::FormBaseSchematronDocument(const QString &patternName)
{
    //QDomDocument doc("schema");
    if (schematronDocument)
    {
        delete schematronDocument;
    }
    schematronDocument = new QDomDocument("schema");


    schematronDocument->appendChild(schematronDocument->createElement("schema"));

    schematronDocument->documentElement().setAttribute("xmlns","http://www.ascc.net/xml/schematron");
    schematronDocument->documentElement().setAttribute("xmlns:aixm","http://www.aixm.aero/schema/5.1");
    schematronDocument->documentElement().setAttribute("xmlns:gml","http://www.opengis.net/gml/3.2");


    QDomElement ns = schematronDocument->createElement("ns");
    ns.setAttribute("prefix","aixm");
    ns.setAttribute("uri","http://www.aixm.aero/schema/5.1");
    schematronDocument->documentElement().appendChild(ns);

    QDomElement nsGml = schematronDocument->createElement("ns");
    nsGml.setAttribute("prefix","gml");
    nsGml.setAttribute("uri","http://www.opengis.net/gml/3.2");
    schematronDocument->documentElement().appendChild(nsGml);

    // <pattern name="Predefined values">
    QDomElement pattern = schematronDocument->createElement("pattern");
    pattern.setAttribute("name", patternName);
    schematronDocument->documentElement().appendChild(pattern);
    this->rulesElement = pattern;
    return pattern;

}
