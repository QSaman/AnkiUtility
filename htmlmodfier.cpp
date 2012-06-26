#include "htmlmodfier.h"
#include "xmlbasedsettings.h"
#include <QDebug>
#include <QFileInfo>
#include <QTextDocument>
#include <QTextFrame>
#include <QTextFragment>
#include <QTextImageFormat>
#include <QDir>
#include <QTextCursor>
#include <QTextDocumentFragment>

#define var(x) #x << ": " << x

const QString HtmlModifier::replacingString = "30fb;";  //KATAKANA MIDDLE DOT

HtmlModifier::HtmlModifier()
{
}

QString HtmlModifier::normalizeHtml(const QString &htmlString)
{
    textDocument.setHtml(htmlString);
    modifyImagePath();
    modifyText();
    return textDocument.toHtml();
}

void HtmlModifier::modifyText()
{
    QTextCursor cursor(&textDocument);
    for (cursor = textDocument.find(replacingString, cursor); !cursor.isNull() && !cursor.atEnd();
         cursor = textDocument.find(replacingString, cursor))
    {
        //cursor.setPosition(cursor.position() + replacingString.length(), QTextCursor::KeepAnchor);
        cursor.insertText(QString(QChar(0x30FB)));
    }
}

bool HtmlModifier::DeleteImages(QTextCursor &textCursor)
{
    if (!textCursor.hasSelection())
        return false;
    textDocument.setHtml(textCursor.selection().toHtml());
    //qDebug() << textDocument.toHtml();
    modifyImagePath(true);
    int start = textCursor.selectionStart();
    textCursor.insertHtml(textDocument.toHtml());
    int end = textCursor.selectionEnd();
    textCursor.setPosition(start);
    textCursor.setPosition(end, QTextCursor::KeepAnchor);
    return true;
}

void HtmlModifier::modifyImagePath(bool removeImages)
{
//    qDebug() << htmlString;    
    QTextBlock currentBlock = textDocument.begin();
    for (; currentBlock.isValid(); currentBlock = currentBlock.next())
    {
        //qDebug() << "Block";
        QTextBlock::Iterator iter;
        for (iter = currentBlock.begin(); iter != currentBlock.end(); ++iter)
        {
            QTextFragment currentFragment = iter.fragment();
            if (!currentFragment.isValid())
                continue;
            QTextImageFormat imageFormat = currentFragment.charFormat().toImageFormat();
            if (!imageFormat.isValid())
                continue;
            QString imageName = QFileInfo(imageFormat.name()).fileName();
            QString imageValue = XmlBasedSettings::imageValue(imageName);
            //qDebug() << var(imageValue);
            QList<QString> listString = XmlBasedSettings::resourcePathList();
            for (int i = 0; i < listString.count(); ++i)
            {
                QDir dir(listString[i]);
                QFileInfo fileInfo(dir, imageName);
                if (fileInfo.exists())
                {
                    imageFormat.setName(fileInfo.absoluteFilePath());
                    QTextCursor cursor(&textDocument);
                    cursor.setPosition(currentFragment.position());
                    cursor.setPosition(currentFragment.position() + currentFragment.length(), QTextCursor::KeepAnchor);
                    if (removeImages && imageValue != "")
                    {
                        //qDebug() << var(imageName) << " - " << var(imageValue);
                        //cursor.removeSelectedText();
                        cursor.insertHtml(imageValue);
//                        if (imageValue == "bullet")
//                            cursor.insertText(replacingString);
//                        else
//                            cursor.insertText(imageValue);
                    }
                    else
                        cursor.setCharFormat(imageFormat);
                    break;
                }
            }
        }
    }
}
