

#pragma once
#include <windows.h>
class CPoint    : public POINT
{
public:
        CPoint(){x = 0; y = 0;};
        CPoint(int x, int y){this->x = x; this->y = y;}
        CPoint(const CPoint& other){x = other.x; y = other.y;}
        ~CPoint(){};
        CPoint& operator=(const CPoint& o)
        {
            x = o.x;
            y = o.y;
            return *this;
        }
};

class CSize     : public SIZE
{
public:
        CSize(){cx = 0; cy = 0;};
        CSize(int cx, int cy){this->cx = cx; this->cy = cy;}
        CSize(const CSize& other){cx = other.cx; cy = other.cy;}
        ~CSize(){};
};

class CRect     : public RECT
{
public:
        CRect(){left = 0; top = 0; right = 0; bottom = 0;};
        CRect(int left, int top, int width, int height)
        {
                this->left = left;
                this->top = top;
                this->right = left + width;
                this->bottom = top + height;
        }
        CRect(const CRect& o){left = o.left; right = o.right; top = o.top; bottom = o.bottom;}
        CRect(CPoint point, CSize size)
        {
                left = point.x;
                top = point.y;
                right = left + size.cx;
                bottom = top + size.cy;
        }
        CRect(CPoint pt1, CPoint pt2)
        {
            left = pt1.x;
            top = pt1.y;
            right = pt2.x;
            bottom = pt2.y;
        }
        ~CRect(){};
        int Width(void){return right - left;}
        int Height(void){return bottom - top;}
        bool PtInRect(CPoint pt){return (::PtInRect((RECT*)this, (POINT)pt) != FALSE);}
        CRect& operator=(const CRect& o)
        {
            this->left = o.left;
            this->top = o.top;
            this->right = o.right;
            this->bottom = o.bottom;
            return *this;
        }
};






