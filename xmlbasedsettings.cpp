#include "xmlbasedsettings.h"
#include <QFile>
#include <QMessageBox>
#include <QXmlStreamReader>

#include <QDebug>

#define vard(x) #x << ": " << x

const QString errorTitle = "Parsing Error";
const QString errorMsg = "I expect %1 instead of %2 in line %3 in file %4";

QList<XmlBasedSettings::Dictionary> XmlBasedSettings::_dictionaries;
const QString XmlBasedSettings::_emptyString = "";

XmlBasedSettings::Dictionary::Dictionary(const QString & dictionaryName, const QString & resourcePath, const QMap<QString, QString> &fileValue, const QMap<QString, int> &fileFormatIndex, const QList<QString> & formatList) :
    _dicName(dictionaryName), _resourcePath(resourcePath), _fileValue(fileValue),_fileFormatIndex(fileFormatIndex), _formatList(formatList)
{
}

QList<QString> XmlBasedSettings::resourcePathList()
{
    QList<QString> ret;
    for (int i = 0; i < _dictionaries.count(); ++i)
        ret.push_back(_dictionaries[i].resourcePath());
    return ret;
}

//const QString & XmlBasedSettings::imageName(const QString &imageValue, int dictionaryIndex)
//{
//    if (dictionaryIndex != -1)
//        return _dictionaries[dictionaryIndex].imageName(imageValue);
//    for (int i = 0; i < _dictionaries.count(); ++i)
//        if (_dictionaries[i].imageName(imageValue) != "")
//            return _dictionaries[i].imageName(imageValue);
//    return _emptyString;
//}

const QString XmlBasedSettings::imageValue(const QString &imageName, int dictionaryIndex)
{
    if (dictionaryIndex != -1)
        return _dictionaries[dictionaryIndex].imageValue(imageName);
    for (int i = 0; i < _dictionaries.count(); ++i)
        if (_dictionaries[i].imageValue(imageName) != "")
            return _dictionaries[i].imageValue(imageName);
    return _emptyString;
}

const QString XmlBasedSettings::Dictionary::imageValue(const QString &imageName)
{
    QString format = _formatList[_fileFormatIndex[imageName]];
    if (format.indexOf('%') == -1)
        return format;
    return (_fileValue.contains(imageName) ? format.arg(_fileValue[imageName]) : _emptyString);
}

XmlBasedSettings::XmlBasedSettings()
{
}

bool XmlBasedSettings::loadXmlBasedSettings()
{    
    if (!QFile::exists("DictionaryResources.xml"))
    {
        if (!QFile::copy(":/Resources/XMLs/DictionaryResources.xml", "./DictionaryResources.xml"))
        {
            QMessageBox::critical(0, "Copy Problem", "DictionaryResources.xml doesn't exist and I can't create it");
            return false;
        }
    }

    QFile file("DictionaryResources.xml");
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(0, "Reading file problem", "Can't open DictionaryResources.xml");
        return false;
    }
    QXmlStreamReader xsr(&file);
    _dictionaries.clear();
    while (xsr.readNextStartElement())
    {
        if (!xsr.isStartElement() || (xsr.name() != "dictionary" && xsr.name() != "dictionaries"))
        {
            QMessageBox::critical(0, errorTitle, parsingXmlErrorMsg(errorMsg.arg("<dictionary> or <dictionaries>"), xsr));
            return false;
        }
        if (xsr.name() == "dictionaries")
            continue;
        QString dictionaryName, resourcePath;
        QMap<QString, QString> fileValue;
        QMap<QString, int> fileFormatIndex;
        QList<QString> formatList;
        for (int i = 0; i < 4; ++i)
        {
            xsr.readNextStartElement(); //<name> or <resource_path> or <image_collection> or <format_collection>
            if (!xsr.isStartElement() || (xsr.name() != "name" && xsr.name() != "resource_path" && xsr.name() != "image_collection"
                                          && xsr.name() != "format_collection"))
            {
                QMessageBox::critical(0, errorTitle, parsingXmlErrorMsg(errorMsg.arg("<name> or <resource_path> or <image_collection> or <format_collection>"), xsr));
                return false;
            }
            if (xsr.name() == "name")
            {
                dictionaryName = xsr.readElementText(); //</name>
                //qDebug() << vard(dictionaryName);
            }
            else if (xsr.name() == "resource_path")
            {
                resourcePath = xsr.readElementText();   //</resource_path>
                //qDebug() << vard(resourcePath);
            }
            else if (xsr.name() == "image_collection")
            {
                if (!readImageCollectionTag(xsr, fileValue, fileFormatIndex))
                    return false;
            }
            else if (xsr.name() == "format_collection")
            {
                if (!readFormatCollectionTag(xsr, formatList))
                    return false;
            }
        }
        _dictionaries.push_back(Dictionary(dictionaryName, resourcePath, fileValue, fileFormatIndex, formatList));
        xsr.readNextStartElement(); //</dictionary>
    }
    //qDebug() << vard(xsr.name()) << " " << vard(xsr.tokenString());
    return true;
}

bool XmlBasedSettings::readFormatCollectionTag(QXmlStreamReader &xsr, QList<QString> &formatList)
{
    for (xsr.readNextStartElement(); xsr.name() != "format_collection" || !xsr.isEndElement(); xsr.readNextStartElement())
    {
        if (!xsr.isStartElement() || xsr.name() != "format")
        {
            QMessageBox::critical(0, errorTitle, parsingXmlErrorMsg(errorMsg.arg("<format>"), xsr));
            return false;
        }
        QString value = xsr.readElementText();    //</format>
        formatList.push_back(value);
    }
    return true;
}

bool XmlBasedSettings::readImageCollectionTag(QXmlStreamReader &xsr, QMap<QString, QString> &fileValue, QMap<QString, int> &fileFormatIndex)
{
    for (xsr.readNextStartElement(); xsr.name() != "image_collection" || !xsr.isEndElement(); xsr.readNextStartElement())
    {
        if (!xsr.isStartElement() || xsr.name() != "image")
        {
            QMessageBox::critical(0, errorTitle, parsingXmlErrorMsg(errorMsg.arg("<image>"), xsr));
            return false;
        }
        int formatInd = 0;
        if (xsr.attributes().hasAttribute("format_id"))
        {
            bool ok = true;
            QString value = xsr.attributes().value("format_id").toString();
            formatInd = value.toInt(&ok);
            if (!ok)
            {
                QMessageBox::critical(0, "Invalid Data", QString("Can't convert %1 to int in format_id attribute in line %2").arg(value).arg(xsr.lineNumber()));
                return false;
            }
        }
        else
            formatInd = 0;
        QString imageName, imageValue;
        for (int i = 0; i < 2; ++i)
        {
            xsr.readNextStartElement(); //<image_name> or <image_value>
            if (!xsr.isStartElement() || (xsr.name() != "image_name" && xsr.name() != "image_value"))
            {
                QMessageBox::critical(0, errorTitle, parsingXmlErrorMsg(errorMsg.arg("<image_name> or <image_value>"), xsr));
                return false;
            }
            QStringRef tagName = xsr.name();
            QString str = xsr.readElementText();    //</image_name> or </image_value>
            if (tagName == "image_name")
                imageName = str;
            else
                imageValue = str;

        }
        //qDebug() << vard(imageValue) << " " << vard(imageName) << endl;
        fileValue[imageName] = imageValue;
        fileFormatIndex[imageName] = formatInd;
        //fileName[imageValue] = imageName;
        xsr.readNextStartElement(); //</image>
    }
    //qDebug() << "****************************************" << endl;
    return true;
}

QString XmlBasedSettings::parsingXmlErrorMsg(const QString & errorString, const QXmlStreamReader & xsr)
{
    QString fileName = "";
    QFile * file = qobject_cast<QFile *>(xsr.device());
    if (file != 0)
        fileName = file->fileName();

    return errorString.arg(xsr.name().toString()).arg(xsr.lineNumber()).arg(fileName);
}
