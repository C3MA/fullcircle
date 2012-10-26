#include "SaxHandler.h"
#include <iostream>
#include <libfullcircle/frame.hpp>
#include <libfullcircle/sprite_io.hpp>
#include <libfullcircle/sfx/frame_fader.hpp>

SaxHandler::SaxHandler() {
    //default 25 fps
    fps = 25;
    state = Doc;
}

SaxHandler::SaxHandler(int fps) {
    this->fps = fps;
    state = Doc;
}

bool SaxHandler::startElement(const QString &namespacURI,
                  const QString &localName,
                  const QString &qName,
                  const QXmlAttributes &attributes)
{
    std::cout << "startElement: qname=" << qName.toStdString() << std::endl;
    switch(state)
    {
        case Doc:
                if( qName.compare("blm") == 0)
                {
                    std::cout << "blm: " << qName.toStdString() << std::endl;
                    std::cout << "\twitdh=" << attributes.value("width").toStdString() << std::endl;
                    std::cout << "\theight=" << attributes.value("height").toStdString() << std::endl;
                    std::cout << "\tchanels=" << attributes.value("channels").toStdString() << std::endl;
                    std::cout << "\tbits=" << attributes.value("bits").toStdString() << std::endl;

                    // check if heigth and width are present

                    fullcircle::Sequence::Ptr Seq(new fullcircle::Sequence(this->fps, attributes.value("width").toInt(), attributes.value("height").toInt()));
                    sequence = Seq;
                }
                else
                {
                    //Throw exception
                }
                state = Root;
                break;
        case Root:
                if( qName.compare("header") == 0)
                {
                    state = Header;
                }
                else
                {
                    //throw
                }
                break;
        case Header:
                state = HeaderElement;
                break;
        case RootAfterHeader:
                if( qName.compare("frame") == 0)
                {
                    state = Frame;
                    std::cout << "\tbits=" << attributes.value("duration").toStdString() << std::endl;
                }
                else
                {
                    //throw
                }
                break;
        case Frame:
                if(qName.compare("row") == 0)
                {
                    state = Row;
                }
                else
                {
                   //throw
                }
                break;
        default:
                //throw
                break;
    }

    return true;
}

bool SaxHandler::endElement(const QString &namespacURI,
                            const QString &localName,
                            const QString &qName)
{
    std::cout << "endElement: qname=" << qName.toStdString() << std::endl;
    switch(state)
    {
        case HeaderElement:
                state = Header;
                break;
        case Header:
                state = RootAfterHeader;
                break;
        case RootAfterHeader:
                state = Doc;
                // Ende
                break;
        case Row:
                state = Frame;
                break;
        case Frame:
                state = RootAfterHeader;
                //add Frame to sequence
                break;
        default:
                //throw
                break;

    }
    return true;
}

bool SaxHandler::characters(const QString &str)
{
    std::cout << "chars: data=" << str.toStdString() << std::endl;
    switch (state)
    {
        case Row:
                    // Row handling
                    break;
        default:
                    break;
    }

    return true;
}
