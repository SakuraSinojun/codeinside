

#include "sw.h"
#include <QPixmap>
#include <QImage>
#include <QRgb>
#include <QColor>
#include <QDebug>
#include <stdlib.h>
#include <QString>
#include <sstream>
#include <iostream>
#include <fstream>
using namespace std;


CSW::CSW(const char * maskfile)
{

        QImage  image(maskfile);
        int     width;
        int     height;
        int     x, y;

        image.load(maskfile);

        width = image.width();
        height = image.height();


        gridwidth = width / GRID_SIZE;
        gridheight = height / GRID_SIZE;

        if(width % GRID_SIZE > 0)
        {
                gridwidth += 1;
        }
        if(height % GRID_SIZE > 0)
        {
                gridheight += 1;
        }

        this->grids = (char *)malloc(gridwidth * gridheight);
        memset(grids, 0, gridwidth * gridheight);


        for(x=0; x<width; x++)
        {
                for(y=0; y<height; y++)
                {
                        if(QColor(image.pixel(x, y)) == QColor(255, 255, 255))
                        {
                                grids[y / GRID_SIZE * gridwidth + x / GRID_SIZE] = 1;
                        }
                }
        }


        std::ifstream   filergn("map/map.rgn");
        char            buffer[1024];
        std::string     line;
        std::string     substr;

        while(filergn.getline(buffer, 1024))
        {
                line = buffer;
                substr = line;
                substr.erase(0, substr.find_first_of('"'));
                substr.erase(0, substr.find_first_not_of('"'));
                substr.erase(substr.find_last_not_of('"') + 1);
                if(line.substr(0, 7) == "minlat=")
                {
                        stringstream(substr) >> minlat;
                }
                else if(line.substr(0, 7) == "minlon=")
                {
                        stringstream(substr) >> minlon;
                }
                else if(line.substr(0, 7) == "maxlat=")
                {
                        stringstream(substr) >> maxlat;
                }
                else if(line.substr(0, 7) == "maxlon=")
                {
                        stringstream(substr) >> maxlon;
                }
                else
                {
                }
        }

}


std::vector<QPoint>& CSW::SearchWay(QPoint src, QPoint dst)
{

        std::vector<QPoint>             open_table;
        std::vector<QPoint>             close_table;
        std::map<string, int>           g_table;
        std::map<string, int>           f_table;
        std::map<string, int>           h_table;
        std::map<string, QPoint>        parent;


        open_table.clear();
        close_table.clear();
        g_table.clear();
        f_table.clear();
        h_table.clear();
        parent.clear();



        std::vector<QPoint>::iterator           otiter;
        std::vector<QPoint>::iterator           ctiter;
        std::map<string, int>::iterator         gtiter;
        std::map<string, int>::iterator         ftiter;
        std::map<string, int>::iterator         htiter;
        std::map<string, QPoint>::iterator      ptiter;

        dst.setX(dst.x() / GRID_SIZE);
        dst.setY(dst.y() / GRID_SIZE);
        src.setX(src.x() / GRID_SIZE);
        src.setY(src.y() / GRID_SIZE);


        open_table.push_back(dst);

        g_table[mkkey(dst)] = 0;
        h_table[mkkey(dst)] = abs(src.x()-dst.x()) + abs(src.y()-dst.y());
        f_table[mkkey(dst)] = g_table[mkkey(dst)] + h_table[mkkey(dst)];

        enum
        {
                QUIT_NORMAL,
                QUIT_EMPTY,
                QUIT_FOUND
        }quitflag;

        int     gd = 10;
        while(true)
        {
                if(open_table.empty())
                {
                        quitflag = QUIT_EMPTY;
                        break;
                }

                // find the point who has the min F value;
                int     minFvalue = -1;
                ctiter = open_table.end();
                for(otiter=open_table.begin(); otiter!=open_table.end(); otiter++)
                {
                        if(minFvalue == -1 || minFvalue > f_table[mkkey(*otiter)])
                        {
                                minFvalue = f_table[mkkey(*otiter)];
                                ctiter = otiter;
                        }
                }

                QPoint  ptCurrent = *ctiter;
                QPoint  ptTemp = *ctiter;

                close_table.push_back(ptCurrent);
                open_table.erase(ctiter);

                quitflag = QUIT_NORMAL;
                for(int i=0; i<4; i++)
                {
                        ptTemp = ptCurrent;
                        switch(i)
                        {
                        case 0:
                                ptTemp.setX(ptTemp.x()-1);
                                break;
                        case 1:
                                ptTemp.setX(ptTemp.x()+1);
                                break;
                        case 2:
                                ptTemp.setY(ptTemp.y()-1);
                                break;
                        case 3:
                                ptTemp.setY(ptTemp.y()+1);
                                break;
                        case 4:
                                ptTemp.setX(ptTemp.x()-1);
                                ptTemp.setY(ptTemp.y()-1);
                                break;
                        case 5:
                                ptTemp.setX(ptTemp.x()+1);
                                ptTemp.setY(ptTemp.y()-1);
                                break;
                        case 6:
                                ptTemp.setX(ptTemp.x()-1);
                                ptTemp.setY(ptTemp.y()+1);
                                break;
                        case 7:
                                ptTemp.setX(ptTemp.x()+1);
                                ptTemp.setY(ptTemp.y()+1);
                                break;
                        default:
                                break;
                        }
                        if(i<4)
                        {
                                gd=10;
                        }else{
                                gd = 14;
                        }
                        if(!PassGrid(ptTemp))
                        {
                                continue;
                        }
                        ctiter = std::find(close_table.begin(), close_table.end(), ptTemp);
                        if(ctiter != close_table.end())
                        {
                                continue;
                        }
                        otiter=std::find(open_table.begin(), open_table.end(), ptTemp);
                        if(otiter == open_table.end())
                        {
                                // not in the OPEN table
                                open_table.push_back(ptTemp);
                                parent[mkkey(ptTemp)] = ptCurrent;
                                h_table[mkkey(ptTemp)] = 10 *(abs(src.x()-ptTemp.x()) + abs(src.y()-ptTemp.y()));
                                g_table[mkkey(ptTemp)] = g_table[mkkey(ptCurrent)] + gd;
                                f_table[mkkey(ptTemp)] = g_table[mkkey(ptTemp)] + h_table[mkkey(ptTemp)];
                        }
                        else
                        {
                                // in the OPEN table
                                if(g_table[mkkey(ptCurrent)] + gd < g_table[mkkey(ptTemp)])
                                {
                                        parent[mkkey(ptTemp)] = ptCurrent;
                                        g_table[mkkey(ptTemp)] = g_table[mkkey(ptCurrent)] + gd;
                                        f_table[mkkey(ptTemp)] = g_table[mkkey(ptTemp)] + h_table[mkkey(ptTemp)];

                                }
                        }

                        otiter=std::find(open_table.begin(), open_table.end(), src);
                        if(otiter!=open_table.end())
                        {
                                quitflag = QUIT_FOUND;
                                break;
                        }
                        if(open_table.empty())
                        {
                                quitflag = QUIT_EMPTY;
                                break;
                        }
                }
                if(quitflag == QUIT_NORMAL)
                {
                        continue;
                }
                else if(quitflag == QUIT_FOUND)
                {
                        break;
                }
                else if(quitflag == QUIT_EMPTY)
                {
                        break;
                }
                else
                {
                        qDebug() << "err here. " << __FILE__ << ":" << __LINE__;
                        exit(0);
                        continue;
                }
        }

        QPoint  pt1;

        result.clear();
        if(quitflag == QUIT_FOUND)
        {
                qDebug() << "founded";
                pt1 = src;
                while(true)
                {
                        result.push_back(pt1);
                        if(pt1 == dst)
                        {
                                break;
                        }
                        pt1 = parent[mkkey(pt1)];
                }
        }
        else
        {
                qDebug() << "no way";
        }

        return this->result;

}

bool     CSW::PassGrid(int x, int y)
{
        if(x<0 || y<0 || x>=gridwidth || y>=gridheight)
        {
                return false;
        }
        return (grids[y*gridwidth + x] == 1);
}

bool    CSW::PassGrid(QPoint pt)
{
        return PassGrid(pt.x(), pt.y());
}

std::string     CSW::mkkey(QPoint& pt)
{
        stringstream    sst;
        sst.str("");
        sst << "PTKEYX"<<pt.x()<<"Y"<<pt.y();
        return sst.str();
}


float   CSW::GetMinLat()
{
        return minlat;
}


float   CSW::GetMinLon()
{
        return minlon;
}

float   CSW::GetMaxLat()
{
        return maxlat;
}

float   CSW::GetMaxLon()
{
        return maxlon;
}











