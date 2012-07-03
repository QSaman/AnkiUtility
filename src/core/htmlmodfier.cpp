#include "src/core/htmlmodfier.h"
#include "src/core/xmlbasedsettings.h"
#include <QDebug>
#include <QFileInfo>
#include <QTextDocument>
#include <QTextFrame>
#include <QTextFragment>
#include <QTextImageFormat>
#include <QDir>
#include <QTextCursor>
#include <QTextDocumentFragment>
#include <QFont>
#include <QTextCharFormat>

#define var(x) #x << ": " << x

const QString HtmlModifier::replacingString = "30fb;";  //KATAKANA MIDDLE DOT

HtmlModifier::HtmlModifier()
{
}

QString HtmlModifier::normalizeHtml(const QString &htmlString)
{
    textDocument = new QTextDocument();
    textDocument->setHtml(htmlString);
    modifyTextFragments(HtmlModifier::ModifyImage | HtmlModifier::ModifyFontPointSize | HtmlModifier::IndentExamples | HtmlModifier::RemoveHyperLink);
    modifyText();
    QString ret = textDocument->toHtml();
    delete textDocument;
    return ret;
}

void HtmlModifier::modifyText()
{
    QTextCursor cursor(textDocument);
    for (cursor = textDocument->find(replacingString, cursor); !cursor.isNull() && !cursor.atEnd();
         cursor = textDocument->find(replacingString, cursor))
    {
        int selectionStart = cursor.selectionStart();
        cursor.insertText(QString(QChar(0x30FB)));
        int selectionEnd = cursor.selectionEnd();
        cursor.setPosition(selectionStart);
        cursor.setPosition(selectionEnd, QTextCursor::KeepAnchor);
        QTextCharFormat charFormat = cursor.charFormat();
        QFont font = charFormat.font();
        font.setPointSize(14);
        charFormat.setFont(font);
        cursor.setCharFormat(charFormat);
    }
}

bool HtmlModifier::convertImageToText(QTextCursor &textCursor)
{
    if (!textCursor.hasSelection())
        return false;
    textDocument = new QTextDocument();
    textDocument->setHtml(textCursor.selection().toHtml());
    //qDebug() << textDocument.toHtml();
    modifyTextFragments(HtmlModifier::ConvertImageToText);
    int start = textCursor.selectionStart();
    textCursor.insertHtml(textDocument->toHtml());
    int end = textCursor.selectionEnd();
    textCursor.setPosition(start);
    textCursor.setPosition(end, QTextCursor::KeepAnchor);
    delete textDocument;
    return true;
}

void HtmlModifier::changeFontStretch(QTextDocument *document, int fontStretch)
{
    textDocument = document;
    _fontStretch = fontStretch;
    modifyTextFragments(HtmlModifier::ModifyFontStretch);
}

void HtmlModifier::modifyFont(HtmlModifier::TextFragments tf, QTextCharFormat charFormat, int fragmentStartPosition, int fragmentEndPosition)
{
    QFont font = charFormat.font();
    //qDebug() << var(font.pixelSize()) << " - " << var(font.pointSize());
    //qDebug() << "******************************************";
    if (tf & HtmlModifier::RemoveHyperLink && charFormat.isAnchor())
        charFormat.setAnchor(false);
    if (tf & HtmlModifier::ModifyFontPointSize)
    {
        if ((font.pixelSize() == 13 || font.pixelSize() == 14) && font.pointSize() == -1)
            font.setPointSize(14);
        else if (font.pixelSize() == -1 && font.pointSize() == 11)
            font.setPointSize(16);
    }
    else if (tf & HtmlModifier::ModifyFontStretch)
    {
        //qDebug() << var(_fontStretch);
        font.setStretch(_fontStretch);
    }
    charFormat.setFont(font);
    QTextCursor cursor(textDocument);
    cursor.setPosition(fragmentStartPosition);
    cursor.setPosition(fragmentEndPosition, QTextCursor::KeepAnchor);
    cursor.setCharFormat(charFormat);
}

void HtmlModifier::indentLine(const QString & imageName, int &fragmentStartPositin, int &fragmentEndPosition)
{
    if (imageName != "8CB0DC57.png")
        return;
    QTextCursor cursor(textDocument);
    cursor.setPosition(fragmentStartPositin);
    QString indentStr = "  ";
    cursor.insertText(indentStr);
    fragmentEndPosition += indentStr.length();
}

void HtmlModifier::modifyImage(HtmlModifier::TextFragments tf, QTextCharFormat charFormat, int fragmentStartPosition, int fragmentEndPosition)
{
    QTextImageFormat imageFormat = charFormat.toImageFormat();
    if (!imageFormat.isValid())
        return;    
    QString imageName = QFileInfo(imageFormat.name()).fileName();
    QString imageValue = XmlBasedSettings::imageValue(imageName);
    if (tf & HtmlModifier::IndentExamples)
        indentLine(imageName, fragmentStartPosition, fragmentEndPosition);
    QList<QString> listString = XmlBasedSettings::resourcePathList();
    for (int i = 0; i < listString.count(); ++i)
    {
        QDir dir(listString[i]);
        QFileInfo fileInfo(dir, imageName);
        if (fileInfo.exists())
        {
            imageFormat.setName(fileInfo.absoluteFilePath());
            QTextCursor cursor(textDocument);
            cursor.setPosition(fragmentStartPosition);
            cursor.setPosition(fragmentEndPosition, QTextCursor::KeepAnchor);
            if ((tf & HtmlModifier::ConvertImageToText) && imageValue != "")
            {
                int selectionStart = cursor.selectionStart();
                cursor.insertHtml(imageValue);
                int selectionEnd = cursor.selectionEnd();
                cursor.setPosition(selectionStart);
                cursor.setPosition(selectionEnd, QTextCursor::KeepAnchor);
                QTextCharFormat charFormat = cursor.charFormat();
                QFont font = charFormat.font();
                font.setPointSize(14);
                charFormat.setFont(font);
                cursor.setCharFormat(charFormat);
            }
            else
                cursor.setCharFormat(imageFormat);
            break;
        }
    }
}

void HtmlModifier::modifyTextFragments(HtmlModifier::TextFragments tf)
{
    QTextBlock currentBlock = textDocument->begin();
    for (; currentBlock.isValid(); currentBlock = currentBlock.next())
    {
        QTextBlock::Iterator iter;
        for (iter = currentBlock.begin(); iter != currentBlock.end(); ++iter)
        {
            QTextFragment currentFragment = iter.fragment();
            if (!currentFragment.isValid())
                continue;
            int startPosition = currentFragment.position();
            int endPosition = currentFragment.position() + currentFragment.length();
            QTextCharFormat charFormat = currentFragment.charFormat();
            if (tf & (HtmlModifier::ModifyFontPointSize | HtmlModifier::ModifyFontStretch))
                modifyFont(tf, charFormat, startPosition, endPosition);
            if (tf & (HtmlModifier::ModifyImage | HtmlModifier::ConvertImageToText))
                modifyImage(tf, charFormat, startPosition, endPosition);
        }
    }
}
