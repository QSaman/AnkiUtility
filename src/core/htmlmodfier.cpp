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
    minFontSize = 16;
}

//Notice: I don't need to use below function in openSUSE. I use it in Fedora 19
QString HtmlModifier::removeTagAttributes(QString tagName, QString htmlString)
{
    if (htmlString.isNull() || htmlString.isEmpty())
        return "";
    int index = htmlString.indexOf("<" + tagName);
    if (index == -1)
        return htmlString;
    int i;
    for (i = index + 1; i < htmlString.size() && htmlString[i] != '>'; ++i);
    if (i == htmlString.size())
        return htmlString;
    return htmlString.mid(0, index) + "<" + tagName + "/>" + removeTagAttributes(tagName, htmlString.mid(i + 1));
}

QString HtmlModifier::removeImageLink(QString imageString, QString htmlString)
{
//    int index = htmlString.indexOf(imageString);
//    if (index == -1)
//        return htmlString;
//    int i, j;
//    string anchor
//    for (i = index; i >= 0 && )
}

QString HtmlModifier::normalizeHtml(QString &htmlString)
{
    htmlString = removeTagAttributes("br", htmlString);
    //qDebug() << var(htmlString);
    //htmlString.replace("<img src=\"qrcx://localhost/icons/playsound.png\" border=\"0\" align=\"absmiddle\" alt=\"Play\" style=\"vertical-align: -30%;\">", "");
    //htmlString = removeTagAttributes("blockquote", htmlString);
    //htmlString.remove("<blockquote>");
    //htmlString.remove("</blockquote>");
    //I add these two lines for Fedora 19
    //htmlString.replace("<blockquote", "<span");
    //htmlString.replace("</blockquote>", "</span>");
    textDocument = new QTextDocument();
    textDocument->setHtml(htmlString);
    //textDocument->setDefaultStyleSheet(".dsl_opt { display: inline; }");
    //qDebug() << var(textDocument->toHtml());
    minFontSize = getTextMinFontSize(textDocument);
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
    int delta = 0;
    if (minFontSize < 16)
        delta = 16 - minFontSize;
    QFont font = charFormat.font();    
//    qDebug() << var(font.pixelSize()) << " - " << var(font.pointSize());
//    qDebug() << "******************************************";
    if (tf & HtmlModifier::RemoveHyperLink && charFormat.isAnchor())
        charFormat.setAnchor(false);
    if (tf & HtmlModifier::ModifyFontPointSize)
    {
        int size = font.pixelSize();
        if (size < 0)
            size = font.pointSize();
        //qDebug() << var(size + delta);
        font.setPointSize(size + delta);
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

bool HtmlModifier::removeImage(QTextCharFormat charFormat, QTextBlock::Iterator &iter, int fragmentStartPosition, int fragmentEndPosition)
{
    QTextImageFormat imageFormat = charFormat.toImageFormat();
    if (!imageFormat.isValid())
        return false;
    QString imageName = QFileInfo(imageFormat.name()).fileName();
    if (imageName != "playsound.png")
        return false;
    charFormat.setAnchor(false);
    charFormat.setAnchorHref("");
    qDebug() << charFormat.anchorHref();
    ++iter;

    QTextCursor cursor(textDocument);
    cursor.setPosition(fragmentStartPosition);
    cursor.setPosition(fragmentEndPosition, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    return true;
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

int HtmlModifier::getTextMinFontSize(QTextDocument* document)
{
    QTextBlock currentBlock = document->begin();
    int min = 16;
    for (; currentBlock.isValid(); currentBlock = currentBlock.next())
    {
        QTextBlock::iterator iter;
        for (iter = currentBlock.begin(); iter != currentBlock.end(); ++iter)
        {
            QTextFragment currentFragment = iter.fragment();
            if (!currentFragment.isValid())
                continue;
            QTextCharFormat charFormat = currentFragment.charFormat();
            int size = charFormat.font().pixelSize();
            if (size < 0)
                size = charFormat.font().pointSize();
            if (size < min && size > 0)
                min = size;
        }
    }
    return min;
}

void HtmlModifier::modifyTextFragments(HtmlModifier::TextFragments tf)
{
    QTextBlock currentBlock = textDocument->begin();
    bool removeBlock = false;
    for (; currentBlock.isValid();)
    {
        QTextBlock::Iterator iter;
        qDebug() << "Block Starts";
        for (iter = currentBlock.begin(); iter != currentBlock.end();)
        {
            QTextFragment currentFragment = iter.fragment();
            if (!currentFragment.isValid())
                continue;
            int startPosition = currentFragment.position();
            int endPosition = currentFragment.position() + currentFragment.length();
            QTextCharFormat charFormat = currentFragment.charFormat();
            if (removeImage(charFormat, iter, startPosition, endPosition))
            {
                QString blockText = currentBlock.text();
                int i;
                for (i = 0; i < blockText.length() && !QChar::isLetterOrNumber(blockText[i].toLatin1()); ++i);
                if (i >= blockText.length())
                {
                    removeBlock = true;
                    break;
                }
                continue;
            }
            if (tf & (HtmlModifier::ModifyFontPointSize | HtmlModifier::ModifyFontStretch))
                modifyFont(tf, charFormat, startPosition, endPosition);
            if (tf & (HtmlModifier::ModifyImage | HtmlModifier::ConvertImageToText))
                modifyImage(tf, charFormat, startPosition, endPosition);
            ++iter;
        }
        if (removeBlock)
        {
            removeBlock = false;
            QTextCursor cursor(currentBlock);
            currentBlock = currentBlock.next();
            cursor.select(QTextCursor::BlockUnderCursor);
            cursor.removeSelectedText();
            cursor.movePosition(QTextCursor::NextBlock);
            cursor.insertText("  ");
        }
        else
            currentBlock = currentBlock.next();
        qDebug() << "Block Ends";
    }
}
