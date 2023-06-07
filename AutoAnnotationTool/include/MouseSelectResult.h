#ifndef MOUSESELECTRESULT_H
#define MOUSESELECTRESULT_H

struct PolygonSelectResult
{
    PolygonSelectResult():
        isSelect(false),boxIdx(-1),polyIdx(-1){}
    bool isSelect;
    int boxIdx; // index of bounding box
    int polyIdx; // The index of polygon
};

struct RectCornerSelectResult
{
    RectCornerSelectResult():
        isSelect(false),boxIdx(-1),corner(-1){}
    bool isSelect;
    int boxIdx; // index of bounding box
    int corner; // 0:topLeft, 1:topRight, 2:bottomRight, 3:bottomLeft
};

struct RectEdgeSelectResult
{
    RectEdgeSelectResult():
        isSelect(false),boxIdx(-1),line(-1){}
    bool isSelect;
    int boxIdx;
    int line; //0: left, 1: top, 2: right, 3: bottom
};
#endif // MOUSESELECTRESULT_H
