
#include "bgooglemapsmaker.h"

#include <Magick++.h>

#include <cmath>
#include <iostream>
#include <sstream>
#include <iomanip>


using namespace std;

BGooglemapsMaker::BGooglemapsMaker(const string& fileName, const string& outputFolder, unsigned int minZoomlevel, unsigned int maxZoomlevel)
    : m_outputBaseTilesCount(2),
      m_outputMinZoom(minZoomlevel),
      m_outputMaxZoom(maxZoomlevel),
      m_outputTileSize(256)
{
    m_fileName = fileName;
    m_outputFolder = outputFolder;
}

BGooglemapsMaker::~BGooglemapsMaker()
{}

static inline std::string fillNumber(int number)
{
    std::stringstream out;
    out << setfill('0');
    out << setw(3);
    out << number;
    return out.str();
}

bool BGooglemapsMaker::generateTilesMagick()
{
    if (m_outputFolder.empty())
    {
        cerr << "No Outputfolder given" << endl;
        return false;
    }

//     http://www.imagemagick.org/Magick++/Image.html#Image Manipulation Methods

    Magick::Image image;
    Magick::Image scaledImageOld;
    try {
        cout << "Opening image: " << m_fileName << endl;
        image.read(m_fileName);
        cout << "done opening: " << m_fileName << endl;

        for (unsigned int i = m_outputMaxZoom; i > 0; --i)
//         for (int i = 1; i <= m_outputMaxZoom; ++i)
        {
            int widthTileCount = std::pow(m_outputBaseTilesCount, i);
            double scaleToWidth = m_outputTileSize * widthTileCount;
            double scaleFactorWidth = scaleToWidth / image.columns();
            double scaleToHeight = image.rows() * scaleFactorWidth;
            int heighTileCount =  std::ceil(scaleToHeight / m_outputTileSize);

            if (i == 1)
            {
                m_ouputXTiles = m_outputBaseTilesCount * i;
                m_ouputYTiles = heighTileCount;
            }
            Magick::Image scaledImage/* = image*/;
            if (m_outputMaxZoom == i)
            {
                scaledImage = image;
            }
            else
            {
                scaledImage = scaledImageOld;
            }


            {
                cout << "tiles on level:" << i << " x-count:" << widthTileCount
                     << " y-count:" << heighTileCount
                     << " image-width:" << image.columns()
                     << " image-height:" << image.rows() << endl;
                cout << "starting scale to: " << scaleToWidth << "x" << scaleToHeight << " on level:"<< i<< endl;

    //             scaledImage.scale(
    //                 Geometry(
    //                     scaleToWidth, scaleToHeight
    //                 )
    //             );
                scaledImage.sample(
                    Magick::Geometry(
                        scaleToWidth, scaleToHeight
                    )
                );
                cout << "done scaling on zoom level:" << i << endl;
            }

            std::string zoomName = fillNumber(i);

            const int maxNumber = heighTileCount * widthTileCount;
            int run = 0;
            int last = 0;
            float tmp = 0;
            float percent = static_cast<float>(maxNumber) / 10.0f;

            for (int yTile = 0; yTile < heighTileCount; ++yTile)
            {
                std::string yTileName = fillNumber(yTile);
                int xTile;

                for (xTile = 0; xTile < widthTileCount; ++xTile)
                {
                    ++run;
                    tmp = static_cast<int>(run / percent);
                    if (tmp != last)
                    {
                        cout << tmp*10 << "% "<< run << "/" << maxNumber << " on zoomlevel:" << i << endl;
                        last = tmp;
                    }
                    Magick::Image tile(scaledImage);

                    tile.crop(
                        Magick::Geometry(
                            m_outputTileSize,
                            m_outputTileSize,
                            xTile * m_outputTileSize,
                            yTile * m_outputTileSize
                        )
                    );

                    if ((tile.columns() != m_outputTileSize) || (tile.rows() != m_outputTileSize))
                    {
                        Magick::Image newtile( Magick::Geometry(m_outputTileSize, m_outputTileSize),  Magick::Color(0, 0, 0, 1.0));
                        newtile.floodFillTexture(Magick::Geometry(tile.columns(), tile.rows()), tile);
                        tile = newtile;
                    }

                    std::string xTileName = fillNumber(xTile);

                    std::string saveName = m_outputFolder + '/' + m_outputPrefix + zoomName + xTileName + yTileName + ".png";
                    tile.write(saveName);
                }
            }
            scaledImageOld = scaledImage;
        }
    }
    catch(Magick::Exception &error_)
    {
        cerr << "Caught exception: " << error_.what() << endl;
        return false;
    }

    return true;
}

void BGooglemapsMaker::printParameters()
{
    cerr << "maxCardSizeX =" << m_ouputXTiles << endl;
    cerr << "maxCardSizeY =" << m_ouputYTiles << endl;
    cerr << "constTileSize =" << m_outputTileSize << endl;
    cerr << "maxZoomLevel =" << m_outputMaxZoom << endl;
}

void BGooglemapsMaker::setFileName(const string& fileName)
{
    m_fileName = fileName;
}

void BGooglemapsMaker::setOutputFolder(const string& outputFolder)
{
    m_outputFolder = outputFolder;
}

void BGooglemapsMaker::setMaxZoomLevel(unsigned int max)
{
    m_outputMaxZoom = max;
}

void BGooglemapsMaker::setMinZoomLevel(unsigned int min)
{
    m_outputMinZoom = min;
}
