#ifndef SaxHandler_H
#define SAXHandler_H

#include <QXmlDefaultHandler>
#include <QXmlSimpleReader>
#include <QString>
#include <libfullcircle/sequence.hpp>

typedef enum SaxHandlerStates {Doc, Root, RootAfterHeader, Header, HeaderElement, FRAME, ROW } SaxHandlerStates_t;

class SaxHandler : public QXmlDefaultHandler
{
public:
    SaxHandler();
    SaxHandler(int fps);
    SaxHandler(bool verbose);
    SaxHandler(int fps, bool verbose);

    bool startElement(const QString &namespacURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespacURI, const QString &localName,
                      const QString &qName);
    bool characters(const QString &str);

    fullcircle::Sequence::Ptr GetSequence();

private:
    bool verbose;
    fullcircle::Sequence::Ptr sequence;
    int channels;
    int bits;
    int height;
    int width;
    int fps;
    SaxHandlerStates_t state;
    fullcircle::Frame::Ptr currentFrame;
    int currentFrameDuration;
    int currentRow;
    int frameNumber;

};

#endif
