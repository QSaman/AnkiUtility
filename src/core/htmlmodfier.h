#ifndef HTMLMODFIER_H
#define HTMLMODFIER_H

#include <QTextDocument>

class QString;
class QTextCursor;

class HtmlModifier
{
public:
    HtmlModifier();
    QString normalizeHtml(const QString & htmlString);
    bool DeleteImages(QTextCursor & textCursor);
private:
    void modifyImagePath(bool removeImages = false);
    void modifyText();

    QTextDocument textDocument;
    static const QString replacingString;
};

#endif // HTMLMODFIER_H
