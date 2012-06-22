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

HtmlModifier::HtmlModifier()
{
}

QString HtmlModifier::addImages(const QString &htmlString)
{
//    qDebug() << htmlString;
    QTextDocument srcDoc;
    srcDoc.setHtml(htmlString);
    QTextBlock currentBlock = srcDoc.begin();
    for (; currentBlock.isValid(); currentBlock = currentBlock.next())
    {
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
                    QTextCursor cursor(&srcDoc);
                    cursor.setPosition(currentFragment.position());
                    cursor.setPosition(currentFragment.position() + currentFragment.length(), QTextCursor::KeepAnchor);
                    cursor.setCharFormat(imageFormat);
                    break;
                }
            }
        }
    }
    return srcDoc.toHtml();
}
