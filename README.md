# bgooglemapsmaker
bgooglemapsmaker helps you to cut down you super-big-image in small map tiles for (for example) googlemaps or leaflet.

This was originally supposed to support primarily googlemaps, but as it turned out, I preferred leaflet. As googlemaps is more difficult to use in a standalone case. It was originally written in ~2012.

NOTE: Depending on your input image size, this can be VERY memory&cpu hungry. But in case of out-of-memory Imagemagick will use temp files, this behavior can be controlled with MAGICK_MEMORY_LIMIT and such.

# build
- mkdir build
- cd build
- cmake ..
- make

# usage
```
./build/bgooglemapsmaker ~/my-super-big-image.png ~/outputfolder/
cp template/leaflet.html ~/outputfolder/
xdg-open ~/outputfolder/leaflet.html
```

# Screenshot
![bgooglemapsmaker](http://www.buschinski.de/img-misc/bgooglemapsmaker.png)

# Example
[old Holarse Minecraft Map](http://buschinski.de/maps/minecraft20140723/leaflet.html)

