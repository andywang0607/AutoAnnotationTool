#ifndef MOUSESELECTRESULT_H
#define MOUSESELECTRESULT_H

struct PolygonSelectResult{
    bool isSelect = false;
    int boxIdx = -1; // index of bounding box
    int polyIdx = -1; // The index of polygon
};

#endif // MOUSESELECTRESULT_H
