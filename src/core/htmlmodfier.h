#ifndef HTMLMODFIER_H
#define HTMLMODFIER_H

#include <QTextDocument>
#include <QTextCharFormat>

class QString;
class QTextImageFormat;

class HtmlModifier
{
private:
    enum TextFragment
    {
        ConvertImageToText = 0x01,
        ModifyImage = 0x02,
        ModifyFontPointSize = 0x04,
        ModifyFontStretch = 0x08
    };
public:
    HtmlModifier();
    QString normalizeHtml(const QString & htmlString);
    bool convertImageToText(QTextCursor & textCursor);
    Q_DECLARE_FLAGS(TextFragments, TextFragment)
private:
    void modifyTextFragments(HtmlModifier::TextFragments tf);
    void modifyText();
    void modifyFontSize();
    void modifyFontPointSize(QTextCharFormat charFormat, int fragmentStartPosition, int fragmentEndPosition);
    void modifyImage(HtmlModifier::TextFragments tf, QTextCharFormat charFormat, int fragmentStartPosition, int fragmentEndPosition);

    QTextDocument textDocument;
    static const QString replacingString;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(HtmlModifier::TextFragments)

#endif // HTMLMODFIER_H
