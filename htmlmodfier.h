#ifndef HTMLMODFIER_H
#define HTMLMODFIER_H

class QString;

class HtmlModifier
{
public:
    HtmlModifier();
    QString addImages(const QString & htmlString);
};

#endif // HTMLMODFIER_H
