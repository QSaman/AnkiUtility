#ifndef HTMLMODFIER_H
#define HTMLMODFIER_H

#include <QTextCharFormat>
#include <QTextBlock>

class QString;
class QTextImageFormat;
class QTextDocument;
class QTextFragment;

class HtmlModifier
{
private:
    enum TextFragment
    {
        ConvertImageToText = 0x01,
        ModifyImage = 0x02,
        ModifyFontPointSize = 0x04,
        ModifyFontStretch = 0x08,
        IndentExamples = 0x10,
        RemoveHyperLink = 0x20
    };
public:
    HtmlModifier();
    QString normalizeHtml(QString & htmlString);
    //Notice: I don't need to use below function in openSUSE. I use it in Fedora 19
    QString removeTagAttributes(QString tagName, QString htmlString);
    QString removeImageLink(QString imageString, QString htmlString);
    bool convertImageToText(QTextCursor & textCursor);
    void changeFontStretch(QTextDocument * document, int fontStretch);
    static int getTextMinFontSize(QTextDocument* document);
    Q_DECLARE_FLAGS(TextFragments, TextFragment)
private:
    void modifyTextFragments(HtmlModifier::TextFragments tf);
    void modifyText();
    void modifyFontSize();
    void modifyFont(HtmlModifier::TextFragments tf, QTextCharFormat charFormat, int fragmentStartPosition, int fragmentEndPosition);
    void modifyImage(HtmlModifier::TextFragments tf, QTextCharFormat charFormat, int fragmentStartPosition, int fragmentEndPosition);
    bool removeImage(QTextCharFormat charFormat, QTextBlock::Iterator & iter, int fragmentStartPosition, int fragmentEndPosition);
    void indentLine(const QString &imageName, int &fragmentStartPositin, int &fragmentEndPosition);   //both input integer will be modified by function    

    QTextDocument * textDocument;
    int _fontStretch;
    int minFontSize;
    static const QString replacingString;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(HtmlModifier::TextFragments)

#endif // HTMLMODFIER_H
