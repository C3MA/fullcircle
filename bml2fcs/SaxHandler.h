#ifndef SaxHandler_H
#define SAXHandler_H

#include <QXmlDefaultHandler>
#include <QXmlSimpleReader>
#include <QString>
#include <libfullcircle/sequence.hpp>

typedef enum SaxHandlerStates {Doc, Root, RootAfterHeader, Header, HeaderElement, Frame, Row } SaxHandlerStates_t;

class SaxHandler : public QXmlDefaultHandler
{
public:
    SaxHandler();
    SaxHandler(int fps);

    bool startElement(const QString &namespacURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespacURI, const QString &localName,
                      const QString &qName);
    bool characters(const QString &str);

    fullcircle::Sequence::Ptr GetSequence();

private:
    fullcircle::Sequence::Ptr sequence;
    int height;
    int width;
    int fps;
    SaxHandlerStates_t state;
    fullcircle::Frame::Ptr currentFrame;
    int currentFrameDuration;
    int currentRow;

};

#endif
