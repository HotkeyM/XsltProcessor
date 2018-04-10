# XSLT processor

This repository contains XSLT processing tools. This code provides [Schematron](http://schematron.com) asserting language implementation in Qt5/C++11 environment.

Modern Schematron version requires full XSLT2.0 support, **JavaSaxonProcessor** class provides full compatibility with the newest Schematron specification. Classes **LibXmlXsltProcessor** and **MsXmlXsltProcessor** are deprecated.

General convensions:

- No exceptions are used (Qt-style). Returns empty object, zero or false if fails.

##/src/libxmlxsltprocessor.h

LibXmlXsltProcessor is *XsltProcessor* class implementation based on C-styled [libxml2](http://xmlsoft.org).
**Deprecated. Slow and has compatibility issues with Schematron 1.5.**


##/src/msxmlxsltprocessor.h

MsXmlXsltProcessor is *XsltProcessor* class implementation based on MS Windows ActiveX.
**Deprecated. Has compatibility issues with Schematron 1.5.**

##/src/javasaxonprocessor.h

JavaSaxonProcessor is *XsltProcessor* class implementation based on Java Virtual Machine that runs Saxon framework. Bulky solution, uses JNI and C++11 template magic to provide fast calls from C++ code into the JVM. The only way I found to run modern Schematron specification in Qt/C++11 environment.

**Jar-file with Saxon routine functions should be specified in constructor, or default jar-file is used instead**

##/src/xsltprocessor.h

Base class for all schematron processors.

- `explicit XsltProcessor(QObject *parent = 0);`
Ctor
- `virtual ~XsltProcessor() = 0;`
Virtual dtor
- `virtual QString ProcessXslt(const QString &xmlData, const QString &xsltData) = 0;`
Applies _xsltData_ XSLT rule to _xmlData_, returns result

- `virtual QString ProcessXsltFiles(const QString &xmlFile, const QString &xsltFile);`
Applies XSLT rule from file _xsltFile_ to _xmlData_, returns result

- `virtual bool ProcessXsltToFile(const QString &xmlData, const QString &xsltData, const QString &dstFile);`
Applies _xsltData_ XSLT rule to _xmlData_, saves result to _dstFile_ file

- `virtual bool ProcessXsltFilesToFile(const QString &xmlFile, const QString &xsltFile, const QString &dstFile);`
Applies XSLT rule from file _xsltFile_ to _xmlData_, saves result to _dstFile_ file

- `virtual QString GetLastError()`
Returns last error if any, empty string if there are no

##/src/schematronprocessor.h

Provides Schematron compilation and processing.

- `explicit SchematronProcessor(XsltProcessor *proc, QObject *parent = 0);`
- `explicit SchematronProcessor(XsltProcessor *proc, bool preprocess, QObject *parent = 0);`
Initialize Schematron processor with the given Xslt proceccor _proc_

- `~SchematronProcessor();`

- `bool CheckSchematronRule(const QString &rule, const QString &context);`
Asserts Schematron rule. Returns _true_ if rule is correct

- `bool SetDebugOutput(bool debug);`
Enables debug output to the _./debug/_ directory

- `QDomElement RuleToDomElement(const Rule &r, QDomDocument doc = QDomDocument());`
Converts rule to the QDomElement object

- `QString ExportSchematron(const QList<Rule> &rules);`
- `QString ExportSchematron(const Rule &r);`
- `QString ExportSchematron(const QString &rule, const QString &context, const QString &description);`
Forms Schematron format document from given rules. Returns QString with such document.

- `bool ExportSchematronToFile(const QList<Rule> &rules, const QString &filename);`
- `bool ExportSchematronToFile(const Rule &r, const QString &filename);`
- `bool ExportSchematronToFile(const QString &rule, const QString &context, const QString &description, const QString &filename);`
Forms Schematron format document from given rules.

- `QString CompileExportedSchematron(const QString &schematron);`
Compiles exported Schematron document

- `bool ExportAndCompileSchematron(const QList<Rule> &rules, const QString exportFilename, const QString compileFilename);`
Forms schematron document from _rules_, export uncompiled schematron file to _exportFilename_, compiles it and saves the exported compiled schematron as _compileFilename_

- `QString ProcessSchematron(const QString &xmlData);`
Processes given _xmlData_ with SchematronSkeleton compiler.

- `QString RunCompiledSchematron(const QString &xmlData, const QString &compiledSchematron);`
Runs compiled schematron _compiledSchematron_ on input _xmlData_ to be asserted. Returns assertion output.

- `bool CompileSchematronToFile(const QList<Rule> &rules, QString compiledFilename);`
Compiles schematron given by _rules_ list and saves to the _compiledFilename_ file


#Usage:#

```
#include "/src/javasaxonprocessor.h"

QString AsssertRules(QList<Rules> &rules, QString &xmlDataToAssert)
{
    // Instansing the XSLT processor
    JavaSaxonProcessor xsltProcessor("../path/to/saxonxslt.jar");
    // Instancing the Schematron compiler
    SchematronProcessor processor(&xsltProcessor);
    
    return report = processor.RunCompiledSchematron(xmlDataToAssert, processor.CompileSchematron(processor.ExportSchematron(rules)));
    // QString.isNull() == true if something is wrong 
}
```