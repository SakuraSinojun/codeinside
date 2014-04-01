

#include "preLoad.h"


namespace pl
{
	
	class HDCLINK
	{
	public:
		HDCLINK(LPCSTR filename)
		{
			HDC h=::GetDC(0);
			hdc=::CreateCompatibleDC(h);
			hBitmap=(HBITMAP)::LoadImageA(0,filename,IMAGE_BITMAP,0,0,LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			SelectObject(hdc,hBitmap);
			GetObject(hBitmap,sizeof(bitmap),&bitmap);
			next=NULL;
			prev=NULL;
			lstrcpyA(this->filename,filename);
			::ReleaseDC(NULL,h);
		}
		~HDCLINK()
		{
			DeleteObject(hBitmap);
			::DeleteDC(hdc);
			if(prev!=NULL)
				prev->next=this->next;
			if(next!=NULL)
				next->prev=this->prev;
		}
		HDCLINK * next;
		HDCLINK * prev;
		HDC hdc;
		HBITMAP hBitmap;
		BITMAP bitmap;
		char filename[255];
	};

	HDCLINK * first=NULL;
	HDCLINK * last=NULL;

	HDCLINK * findFile(LPCSTR filename)
	{
		HDCLINK * temp=first;
		while(temp!=NULL)
		{
			if(lstrcmpA(filename,temp->filename)==0)
				return temp;
			temp=temp->next;
		}
		return NULL;
	}

	HDC Load(LPCSTR filename,BITMAP * bitmap)
	{
		HDCLINK * temp=findFile(filename);
		if(temp==NULL)
		{
			temp=new HDCLINK(filename);
			if(first==NULL)
			{
				first=temp;
			}else{
				last->next=temp;
				temp->prev =last;
			}
			last=temp;
		}
		if(bitmap!=NULL)
			memcpy(bitmap,&temp->bitmap,sizeof(BITMAP));
		return temp->hdc;
	}
	void UnLoad(LPCSTR filename)
	{
		HDCLINK * temp=findFile(filename);
		if(temp!=NULL)
			delete temp;
	}
};


