
#include "bgooglemapsmaker.h"

#include <Magick++.h>

#include <cmath>
#include <iostream>
#include <sstream>
#include <iomanip>


using namespace std;

BGooglemapsMaker::BGooglemapsMaker(const string& fileName, const string& outputFolder, unsigned int minZoomlevel, unsigned int maxZoomlevel)
    : m_outputMinZoom(minZoomlevel),
      m_outputMaxZoom(maxZoomlevel),
      m_outputTileSize(512)
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
    try {
        cout << "Opening image: " << m_fileName << endl;
        image.read(m_fileName);
        cout << "done opening: " << m_fileName << endl;


        cout << "width:" << image.columns() << " height:" << image.rows() << endl;

        m_outputImageHeight = image.rows();
        m_outputImageWidth = image.columns();

        const double imageRatio = static_cast<double>(image.rows()) / static_cast<double>(image.columns());

        for (unsigned int i = m_outputMaxZoom; i > 0; --i)
        {
            if (i < m_outputMaxZoom)
            {
                double scaledWidth = image.columns()/2.0;
                int scaleToWidth = std::floor(scaledWidth);
                int scaleToHeight = std::floor(scaledWidth*imageRatio);

                cout << "starting scale to: " << scaleToWidth << "x" << scaleToHeight << " on level:"<< i<< endl;
                image.sample(
                    Magick::Geometry(
                        scaleToWidth, scaleToHeight
                    )
                );
                cout << "done scaling on zoom level:" << i << endl;
            }

            double widthTileDoubleCount = static_cast<double>(image.columns()) / static_cast<double>(m_outputTileSize);
            int widthTileCount = std::ceil(widthTileDoubleCount);
            int heighTileCount = std::ceil((static_cast<double>(image.columns())*imageRatio) / static_cast<double>(m_outputTileSize));
            cout << "Tiles on level:" << i << " x-tiles:" << widthTileCount << " y-tiles:" << heighTileCount << endl;

            std::string zoomName = fillNumber(i);

            const int maxNumber = heighTileCount * widthTileCount;
            int run = 0;
            int last = 0;
            float tmp = 0;
            float percent = static_cast<float>(maxNumber) / 10.0f;

            for (int yTile = 0; yTile < heighTileCount; ++yTile)
            {
                std::string yTileName = fillNumber(yTile);

                for (int xTile = 0; xTile < widthTileCount; ++xTile)
                {
                    ++run;
                    tmp = static_cast<int>(run / percent);
                    if (tmp != last)
                    {
                        cout << tmp * 10 << "% "<< run << "/" << maxNumber << " on zoomlevel:" << i << endl;
                        last = tmp;
                    }
                    Magick::Image tile(image);

                    tile.crop(
                        Magick::Geometry(
                            m_outputTileSize,
                            m_outputTileSize,
                            xTile * m_outputTileSize,
                            yTile * m_outputTileSize
                        )
                    );

                    if (tile.columns() != m_outputTileSize || tile.rows() != m_outputTileSize)
                    {
                        Magick::Image newtile(Magick::Geometry(m_outputTileSize, m_outputTileSize), Magick::Color(0, 0, 0, 1.0));
//                         newtile.floodFillTexture(Magick::Geometry(tile.columns(), tile.rows()), tile);
                        newtile.composite(tile, 0, 0);
                        tile = newtile;
                    }

                    std::string xTileName = fillNumber(xTile);

                    std::string saveName = m_outputFolder + '/' + m_outputPrefix + zoomName + xTileName + yTileName + ".png";
                    tile.write(saveName);
                }
            }
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
    cerr << "m_outputImageHeight =" << m_outputImageHeight << endl;
    cerr << "m_outputImageWidth =" << m_outputImageWidth << endl;
    cerr << "constTileSize =" << m_outputTileSize << endl;
    cerr << "maxZoomLevel =" << m_outputMaxZoom << endl;

//     string outFileName = m_outputFolder + "/maps-data.js";
//
//     FILE * outFile = fopen(outFileName.data(), "w");
//     if (!outFile)
//     {
//         cerr << "WARNING: unabled to create" << m_outputFolder << "/" << "maps-data.js" << endl;
//         exit(2);
//     }
//     fprintf(outFile, "var constTileSize = %d;\n", m_outputTileSize);
//     fprintf(outFile, "var maxZoomLevel = %d;\n", m_outputMaxZoom);
//     fprintf(outFile, "var constImageHeight = %d;\n", m_outputImageHeight);
//     fprintf(outFile, "var constImageWidth = %d;\n", m_outputImageWidth);
//     fclose(outFile);
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
