#pragma once

using namespace std;

struct Date
{
    Date(int _d, int _m, int _y)
    {
        d = _d;
        m = _m;
        y = _y;
    }
    int d, m, y;
};

const int monthDays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

int getDifference(Date dt1, Date dt2);
