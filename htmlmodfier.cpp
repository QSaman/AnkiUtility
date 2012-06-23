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
        cursor.setPosition(cursor.position() + replacingString.length(), QTextCursor::KeepAnchor);
        cursor.insertText(QString(QChar(0x30FB)));
    }
}

void HtmlModifier::modifyImagePath()
{
//    qDebug() << htmlString;    
    QTextBlock currentBlock = textDocument.begin();
    for (; currentBlock.isValid(); currentBlock = currentBlock.next())
    {
        qDebug() << "Block";
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
                    cursor.setCharFormat(imageFormat);
                    break;
                }
            }
        }
    }
}
