
#ifndef BGOOGLEMAPSMAKER_H
#define BGOOGLEMAPSMAKER_H

#include <string>

class BGooglemapsMaker
{
public:
    BGooglemapsMaker(const std::string& fileName, const std::string& outputFolder, unsigned int minZoomlevel = 1, unsigned int maxZoomlevel = 8);
    virtual ~BGooglemapsMaker();

    bool generateTilesMagick();

    void printParameters();

    void setFileName(const std::string& fileName);
    void setOutputFolder(const std::string& outputFolder);
    void setMaxZoomLevel(unsigned int max);
    void setMinZoomLevel(unsigned int min);

private:
    std::string m_fileName;

    const int m_outputBaseTilesCount;
    unsigned int m_outputMinZoom;
    unsigned int m_outputMaxZoom;
    const unsigned int m_outputTileSize;
    std::string m_outputPrefix;
    std::string m_outputFolder;

    int m_ouputXTiles;
    int m_ouputYTiles;
};

#endif // BGOOGLEMAPSMAKER_H
