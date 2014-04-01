
#ifndef SW_H
#define SW_H



#include <map>
#include <vector>
#include <stack>
#include <algorithm>
#include <QPoint>
#include <string>

#define GRID_SIZE       10


class CSW
{
public:
        CSW(const char * maskfile);


        float   GetMinLat();
        float   GetMinLon();
        float   GetMaxLat();
        float   GetMaxLon();

        std::vector<QPoint>& SearchWay(QPoint src, QPoint dst);

        bool    PassGrid(int x, int y);
        bool    PassGrid(QPoint pt);

private:

        char    *       grids;
        int             gridwidth;
        int             gridheight;

        std::string     mkkey(QPoint& pt);

        std::vector<QPoint>        result;


        float   minlat;
        float   maxlat;
        float   minlon;
        float   maxlon;



};

#endif // SW_H
