#ifndef HTMLMODFIER_H
#define HTMLMODFIER_H

#include <QTextDocument>

class QString;

class HtmlModifier
{
public:
    HtmlModifier();
    QString normalizeHtml(const QString & htmlString);
private:
    void modifyImagePath();
    void modifyText();

    QTextDocument textDocument;
    static const QString replacingString;
};

#endif // HTMLMODFIER_H
