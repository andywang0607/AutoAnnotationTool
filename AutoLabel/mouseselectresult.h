#ifndef MOUSESELECTRESULT_H
#define MOUSESELECTRESULT_H

struct PolygonSelectResult{
    PolygonSelectResult():
        isSelect(false),boxIdx(-1),polyIdx(-1){}
    bool isSelect;
    int boxIdx; // index of bounding box
    int polyIdx; // The index of polygon
};

#endif // MOUSESELECTRESULT_H
