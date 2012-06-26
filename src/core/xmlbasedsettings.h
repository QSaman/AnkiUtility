#ifndef XMLBASEDSETTINGS_H
#define XMLBASEDSETTINGS_H

#include <QMap>
#include <QString>
#include <QList>
#include <QList>

class QXmlStreamReader;

class XmlBasedSettings
{
public:
    XmlBasedSettings();
    static bool loadXmlBasedSettings();
    static QList<QString> resourcePathList();
    inline static const QString & resourcePath(const int dictionaryIndex)  {return _dictionaries[dictionaryIndex].resourcePath();}
    //static const QString & imageName(const QString & imageValue, int dictionaryIndex = -1);
    static const QString imageValue(const QString & imageName, int dictionaryIndex = -1);
    inline static int dictionarySize()  {return _dictionaries.count();}
    inline static const QString & dictionaryName(int dictionaryIndex)   {return _dictionaries[dictionaryIndex].dictionaryName();}
    //static QList<Dictionary> dictionaries() {return _dictionaries;}
private:
    class Dictionary
    {
    public:
        Dictionary(const QString & dictionaryName, const QString & resourcePath, const QMap<QString, QString> &fileValue,
                   const QMap<QString, int> &fileFormatIndex, const QList<QString> & formatList);
        inline const QString & dictionaryName() {return _dicName;}
        inline const QString & resourcePath()   {return _resourcePath;}
        //inline QString formattedValue(const QString & imageValue, int formatListIndex)  {return _formatList[formatListIndex].arg(imageValue);}
        const QString imageValue(const QString & imageName);
        //inline const QString & imageName(const QString & imageValue)    {return (_fileName.contains(imageValue) ? _fileName[imageValue] : _emptyString);}
    private:
        QString _dicName, _resourcePath;        
        QMap <QString, QString> _fileValue;  //e.g. _fileValue["foo.jpg"] = "1"
        QMap<QString, int> _fileFormatIndex;    //e.g. _fileFormatIndex["foo.jpg"] = "<b>%1. </b>"
        QList<QString> _formatList;
    };

    static bool readImageCollectionTag(QXmlStreamReader &xsr, QMap<QString, QString> & fileValue, QMap<QString, int> & fileFormatValue);
    static bool readFormatCollectionTag(QXmlStreamReader &xsr, QList<QString> &formatList);
    static QString parsingXmlErrorMsg(const QString & errorString, const QXmlStreamReader & xsr);

    static QList<Dictionary> _dictionaries;
    static const QString _emptyString;
};

#endif // XMLBASEDSETTINGS_H
