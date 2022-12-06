#ifndef UNION_H
#define UNION_H

union U {
        struct {
                unsigned char title[20];
                float x;
                float y;
        } Area;
        unsigned char bytearr[28];
};

#endif