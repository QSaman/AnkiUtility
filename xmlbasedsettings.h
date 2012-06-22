#ifndef XMLBASEDSETTINGS_H
#define XMLBASEDSETTINGS_H

#include <QMap>
#include <QString>
#include <QList>

class QXmlStreamReader;

class XmlBasedSettings
{
public:
    XmlBasedSettings();
    static bool loadXmlBasedSettings();
    static QList<QString> resourcePathList();
    inline static const QString & resourcePath(const int dictionaryIndex)  {return _dictionaries[dictionaryIndex].resourcePath();}
    static const QString & imageName(const QString & imageValue, int dictionaryIndex = -1);
    static const QString & imageValue(const QString & imageName, int dictionaryIndex = -1);
    inline static int dictionarySize()  {return _dictionaries.count();}
    inline static const QString & dictionaryName(int dictionaryIndex)   {return _dictionaries[dictionaryIndex].dictionaryName();}
    //static QList<Dictionary> dictionaries() {return _dictionaries;}
private:
    class Dictionary
    {
    public:
        Dictionary(QString dictionaryName, QString resourcePath, QMap<QString, QString> fileValue, QMap<QString, QString> fileName);
        inline const QString & dictionaryName() {return _dicName;}
        inline const QString & resourcePath()   {return _resourcePath;}
        inline const QString & imageValue(const QString & imageName)    {return (_fileValue.contains(imageName) ? _fileValue[imageName] : _emptyString);}
        inline const QString & imageName(const QString & imageValue)    {return (_fileName.contains(imageValue) ? _fileName[imageValue] : _emptyString);}
    private:
        QString _dicName, _resourcePath;
        QMap <QString, QString> _fileValue,  //e.g. _fileValue["foo.jpg"] = "1"
                                _fileName;   //e.g. _fileName["1"] = "foo.jpg"
    };

    static bool readImageCollectionTag(QXmlStreamReader &xsr, QMap<QString, QString> & fileValue, QMap<QString, QString> & fileName);
    static QString parsingXmlErrorMsg(const QString & errorString, const QXmlStreamReader & xsr);

    static QList<Dictionary> _dictionaries;
    static const QString _emptyString;
};

#endif // XMLBASEDSETTINGS_H
