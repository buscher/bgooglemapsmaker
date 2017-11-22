
#include "bgooglemapsmaker.h"

#include <Magick++.h>
#include <iostream>
#include <string.h>

using namespace std;
using namespace Magick;

void printHelp(const char *name)
{
    cout << "USEAGE: " << name << " INPUTFILE OUTPUTFOLDER [options]" << endl;
    cout << "HINT: you may want to set MAGICK_TMPDIR" << endl;
}

int main(int argc, char** argv)
{
    InitializeMagick(*argv);

    if (argc < 2)
    {
        printHelp(argv[0]);
        return 0;
    }

    if (argc < 3)
    {
        if (!strcmp(argv[1], "--help") || !strcmp(argv[2], "--help"))
        {
            printHelp(argv[0]);
            return 0;
        }
        cerr << "no output folder given" << endl;
        return 1;
    }

    BGooglemapsMaker builder(argv[1], argv[2]);
    for (int i = 3; i < argc; ++i)
    {
        string option = argv[i];
        if (option == "--minlevel")
        {
            if (argc >= i+1)
            {
                builder.setMinZoomLevel( atoi(argv[i+1]) );
            }
            else
            {
                cerr << "ERROR: no minlevel given" << endl;
                return 1;
            }
        }
        else if (option == "--maxlevel")
        {
            if (argc >= i+1)
            {
                builder.setMaxZoomLevel( atoi(argv[i+1]) );
            }
            else
            {
                cerr << "ERROR: no maxlevel given" << endl;
                return 1;
            }
        }
        else if (option == "--help")
        {
            printHelp(argv[0]);
            return 0;
        }
    }

    if (!builder.generateTilesMagick())
    {
        cerr << "something went wrong..." << endl;
        return 1;
    }
    else
    {
        cout << "All done, party!" << endl;
        builder.printParameters();
    }
    return 0;
}
