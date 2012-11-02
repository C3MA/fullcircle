#include "SaxHandler.h"
#include <math.h>
#include <iostream>
#include <libfullcircle/common.hpp>
#include <libfullcircle/frame.hpp>
#include <libfullcircle/sprite_io.hpp>
#include <libfullcircle/sfx/frame_fader.hpp>
#include <libfullcircle/color_scheme.hpp>

SaxHandler::SaxHandler() {
    //default 25 fps
    fps = 25;
    state = Doc;
    verbose = false;
}

SaxHandler::SaxHandler(bool verbose) {
    //default 25 fps
    fps = 25;
    state = Doc;
    this->verbose = verbose;
}

SaxHandler::SaxHandler(int fps, bool verbose) {
    //default 25 fps
    this->fps = fps;
    state = Doc;
    this->verbose = verbose;
}

SaxHandler::SaxHandler(int fps) {
    this->fps = fps;
    state = Doc;
    verbose = false;
}

fullcircle::Sequence::Ptr SaxHandler::GetSequence() {
    return sequence;
}

bool SaxHandler::startElement(const QString &namespacURI,
                  const QString &localName,
                  const QString &qName,
                  const QXmlAttributes &attributes)
{
    if (verbose)
    {
        std::cout << "startElement: qname=" << qName.toStdString() << std::endl;
    }
    switch(state)
    {
        case Doc:
                if( qName.compare("blm") == 0)
                {
                    if (verbose)
                    {
                        std::cout << "blm: " << qName.toStdString() << std::endl;
                        std::cout << "\twitdh=" << attributes.value("width").toStdString() << std::endl;
                        std::cout << "\theight=" << attributes.value("height").toStdString() << std::endl;
                        std::cout << "\tchanels=" << attributes.value("channels").toStdString() << std::endl;
                        std::cout << "\tbits=" << attributes.value("bits").toStdString() << std::endl;
                    }
                    // check if heigth and width are present
                    bool Ok;
                    width = attributes.value("width").toInt(&Ok);
                    if (!Ok)
                    {
                        throw fullcircle::DataFormatException("This is not a valid BML file. There is no width.");
                    }
                    height = attributes.value("height").toInt(&Ok);
                    if (!Ok)
                    {
                        throw fullcircle::DataFormatException("This is not a valid BML file. There is no height.");
                    }
                    bits = attributes.value("bits").toInt(&Ok);
                    if (!Ok)
                    {
                        throw fullcircle::DataFormatException("This is not a valid BML file. There is not defined how many bits are used for a channel.");
                    }
                    channels = attributes.value("channels").toInt(&Ok);
                    if (!Ok)
                    {
                        throw fullcircle::DataFormatException("This is not a valid BML file. There is no number of channles.");
                    }

                    if (bits!=8)
                    {
                        throw fullcircle::DataFormatException("The handling of BML files with more or less then 8 bits per channel is not implemented.");
                    }

                    fullcircle::Sequence::Ptr Seq(new fullcircle::Sequence(this->fps, width, height));
                    sequence = Seq;
                    frameNumber = 0;
                }
                else
                {
                    throw fullcircle::DataFormatException("This is not a BML file.");
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
                    throw fullcircle::DataFormatException("This is not a valid BML file.");
                }
                break;
        case Header:
                state = HeaderElement;
                break;
        case RootAfterHeader:
                if( qName.compare("frame") == 0)
                {
                    state = FRAME;
                    if(verbose)
                    {
                        std::cout << "\tduration=" << attributes.value("duration").toStdString() << std::endl;
                    }
                    currentFrameDuration = attributes.value("duration").toInt();
                    fullcircle::Frame::Ptr frame(new fullcircle::Frame(width, height));
                    currentFrame = frame;
                    currentRow = 0;
                }
                else
                {
                    throw fullcircle::DataFormatException("This is not a valid BML file.");
                }
                break;
        case FRAME:
                if(qName.compare("row") == 0)
                {
                    state = ROW;
                    if(verbose)
                    {
                        std::cout << "\tRownum=" << currentRow << std::endl;
                    }
                }
                else
                {
                   throw fullcircle::DataFormatException("This is not a valid BML file.");
                }
                break;
        default:
                throw fullcircle::DataFormatException("This is not a valid BML file.");
                break;
    }

    return true;
}

bool SaxHandler::endElement(const QString &namespacURI,
                            const QString &localName,
                            const QString &qName)
{
    int framesToAdd = 0;
    if(verbose)
    {
        std::cout << "endElement: qname=" << qName.toStdString() << std::endl;
    }
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
        case ROW:
                state = FRAME;
                currentRow++;
                break;
        case FRAME:
                // Ende eines Frames. Das abgeshlossene Frame wird so oft
                // zu der Sequenz hinzugefügt das es lang genug angezeigt
                // wird.
                // Danch befinden wir uns wieder im Root der XML-Datei
                framesToAdd = round(fps / 1000.00 * currentFrameDuration);
                for(int i = 0; i<framesToAdd; i++)
                {
                    sequence->add_frame(currentFrame);
                }
                frameNumber++;
                state = RootAfterHeader;
                break;
        default:
                throw fullcircle::DataFormatException("This is not a valid BML file.");
                break;

    }
    return true;
}

bool SaxHandler::characters(const QString &str)
{
    if(verbose)
    {
        std::cout << "chars: data=" << str.toStdString() << std::endl;
    }
    switch (state)
    {
        bool ok;
        fullcircle::RGB_t RGBvalue;
        case ROW:
                    //Hier wird die akutelle Zeile ausgelesen.

                    // Check ob anzahl Zeichen gleich bits * channels * width
                    if (str.length()!= channels * bits/4 * width)
                    {
                        std::cout << "Fehler in der BML-Datei: Frame " << frameNumber << " Row " << currentRow <<std::endl;
                        std::cout << "Lentgh should be " << channels * bits/4 * width << " and not " << str.length() << std::endl;
                        exit(EXIT_FAILURE);
                    }
                    switch(channels) {
                        case 3:
                                for(int i = 0; i<width;i++)
                                {
                                   RGBvalue.red = str.mid(i* channels * bits/4, 2).toUInt(&ok, 16);
                                   RGBvalue.green = str.mid(i* channels * bits/4 + 2, 2).toUInt(&ok, 16);
                                   RGBvalue.blue = str.mid(i* channels * bits/4 + 4, 2).toUInt(&ok, 16);

                                   currentFrame->set_pixel(i,currentRow, RGBvalue);
                                }
                                break;
                        default:
                                throw fullcircle::DataFormatException("The handling of BML files with more or less than 3 channals is not implemented.");
                                break;
                    }
                    break;
        default:
                    break;
    }

    return true;
}
