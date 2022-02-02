#include "imgfuncs.h"
#include <iostream>


struct circleStuff {
    BLGradient circleRadial;
    BLCircle circle;
};


void saveBLImg(BLImage img, const char* name) {

    BLImageCodec codec;
    BLResult foundName = codec.findByName("BMP");
    if (foundName != BL_SUCCESS) {
        std::cout << "Codec name not found" << std::endl;
        return;
    }

    BLResult successSave = img.writeToFile(name, codec);
    if (successSave != BL_SUCCESS) {
        std::cout << "Failed to save image" << std::endl;
        return;
    }
    std::cout << "Successfully saved img as " << name << std::endl;
}

BLImage createImage(std::deque<int> points, int length, int pointRadius) {
    BLImage img(480, 480, BL_FORMAT_PRGB32);
    BLContext ctx(img);

    ctx.setCompOp(BL_COMP_OP_SRC_COPY);
    ctx.fillAll();

    // ctx.setFillStyle(BLRgba32(0xFFFFFFFF));

    std::deque<circleStuff> circles;
    std::deque<BLPath> lines;

    for (int i = 0; i < length; i = i + 2) {

        // init coords
        int x_coord = points[i];
        int y_coord = points[i + 1];

        // create Gradient
        BLGradient radial(BLRadialGradientValues(x_coord, y_coord, x_coord, y_coord, pointRadius));
        radial.addStop(0.0, BLRgba32(255, 255, 255));

        // first point marked red to identify start point,
        // other points blue
        if (i == 0) {
            radial.addStop(1.0, BLRgba32(100, 0, 0, 255));
        }
        else {
            radial.addStop(1.0, BLRgba32(0, 0, 100, 255));
        }

        BLCircle circle(x_coord, y_coord, pointRadius);

        circleStuff nextCircle;
        nextCircle.circleRadial = radial;
        nextCircle.circle = circle;

        circles.push_back(nextCircle);

        // ctx.setFillStyle(radial);

        // draw the point
        // ctx.fillCircle(x_coord, y_coord, PRADIUS);

        // draw line to other points
        for (int j = length - 1; j > i + 1; j = j - 2) {

            // path from point to all other points
            BLPath path;
            path.moveTo(x_coord, y_coord);
            path.lineTo(points[j - 1], points[j]);

            lines.push_back(path);

            // ctx.setStrokeStyle(BLRgba32(0xAAAAAAAA));
            // ctx.strokePath(path);

        }
    }

    // draw lines first
    ctx.setStrokeStyle(BLRgba32(150, 150, 200, 100));
    for (BLPath line : lines) {
        ctx.strokePath(line);
    }

    // draw circles over lines
    for (circleStuff circle : circles) {
        ctx.setFillStyle(circle.circleRadial);
        ctx.fillCircle(circle.circle);
    }

    ctx.end();

    return img;
}

/*
* Draws a route along the provided points given by the indices 
*/
void drawBestRoute(BLImage & img, std::deque<int> points, std::deque<int> indices) {
    
    int firstPoint;

    firstPoint = indices[0];
    indices.pop_front();

    BLContext ctx(img);
    ctx.setStrokeStyle(BLRgba32(0, 200, 50, 255));

    BLPath path;
    path.moveTo(points[2 * firstPoint], points[2 * firstPoint + 1]);

    for (int ind : indices) {
        path.lineTo(points[2 * ind], points[2 * ind + 1]);
    }

    path.lineTo(points[2 * firstPoint], points[2 * firstPoint + 1]);
    ctx.strokePath(path);
    ctx.end();

    // return img;

}