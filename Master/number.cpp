#include "number.h"
#include "CTexture.h"
#include "CUserInterface.h"
#include "tex_animation.h"
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC

#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
void Number_Draw(float x, float y, int n)
{
	if (n >= 0 && n <= 9)
	{
		CUserInterFace *pn = new CUserInterFace();
		pn->Sprite_Draw(CTexture::TEX_NUM, x, y, Animation_UpdateX(n, 1, 10, CTexture::Texture_GetWidth(CTexture::TEX_NUM, 10), 10), 0, CTexture::Texture_GetWidth(CTexture::TEX_NUM, 9), CTexture::Texture_GetHeight(CTexture::TEX_NUM, 1));
		delete pn;
	}
}

void Number_Draw4(float x, float y, int n, int colornum)
{
	if (n >= 0 && n <= 9)
	{
		CUserInterFace *pn = new CUserInterFace();
		pn->Sprite_Draw(CTexture::TEX_UI_TIME, x, y, Animation_UpdateX(n, 1, 10, CTexture::Texture_GetWidth(CTexture::TEX_UI_TIME, 10), 10), CTexture::Texture_GetHeight(CTexture::TEX_UI_TIME, 1) * colornum, CTexture::Texture_GetWidth(CTexture::TEX_UI_TIME, 10), CTexture::Texture_GetHeight(CTexture::TEX_UI_TIME, 1));
		delete pn;
	}
}

void Number_Draw_Leader(float x, float y, int n, int colornum)
{
	if (n >= 0 && n <= 9)
	{
		CUserInterFace *pn = new CUserInterFace();
		pn->Sprite_Draw(CTexture::TEX_UI_TIME_YELLOW_150, x, y, Animation_UpdateX(n, 1, 10, CTexture::Texture_GetWidth(CTexture::TEX_UI_TIME, 10), 10), CTexture::Texture_GetHeight(CTexture::TEX_UI_TIME, 1) * colornum, CTexture::Texture_GetWidth(CTexture::TEX_UI_TIME, 10), CTexture::Texture_GetHeight(CTexture::TEX_UI_TIME, 1));
		delete pn;
	}
}

void Number_Draw_Leader_red(float x, float y, int n, int colornum)
{
	if (n >= 0 && n <= 9)
	{
		CUserInterFace *pn = new CUserInterFace();
		pn->Sprite_Draw(CTexture::TEX_UI_TIME_RED_225, x, y, Animation_UpdateX(n, 1, 10, CTexture::Texture_GetWidth(CTexture::TEX_UI_TIME_RED_225, 10), 10), CTexture::Texture_GetHeight(CTexture::TEX_UI_TIME_RED_225, 1) * colornum, CTexture::Texture_GetWidth(CTexture::TEX_UI_TIME_RED_225, 10), CTexture::Texture_GetHeight(CTexture::TEX_UI_TIME_RED_225, 1));
		delete pn;
	}
}

void Number_Draw_yellow_180(float x, float y, int n, int colornum)
{
	if (n >= 0 && n <= 9)
	{
		CUserInterFace *pn = new CUserInterFace();
		pn->Sprite_Draw(CTexture::TEX_UI_TIME_YELLOW_180, x, y, Animation_UpdateX(n, 1, 10, CTexture::Texture_GetWidth(CTexture::TEX_UI_TIME_YELLOW_180, 10), 10), CTexture::Texture_GetHeight(CTexture::TEX_UI_TIME_YELLOW_180, 1) * colornum, CTexture::Texture_GetWidth(CTexture::TEX_UI_TIME_YELLOW_180, 10), CTexture::Texture_GetHeight(CTexture::TEX_UI_TIME_YELLOW_180, 1));
		delete pn;
	}
}

void Number_Draw_Break(float x, float y, int n, int colornum)
{
	if (n >= 0 && n <= 9)
	{
		CUserInterFace *pn = new CUserInterFace();
		pn->Sprite_Draw(CTexture::TEX_UI_GEKIHA, x, y, Animation_UpdateX(n, 1, 10, CTexture::Texture_GetWidth(CTexture::TEX_UI_GEKIHA, 10), 10), CTexture::Texture_GetHeight(CTexture::TEX_UI_GEKIHA, 1) * colornum, CTexture::Texture_GetWidth(CTexture::TEX_UI_GEKIHA, 10), CTexture::Texture_GetHeight(CTexture::TEX_UI_GEKIHA, 1));
		delete pn;
	}
}


void Number_Draw5(float x, float y, int n, int colornum)
{
	if (n >= 0 && n <= 9)
	{
		CUserInterFace *pn = new CUserInterFace();
		pn->Sprite_Draw(CTexture::TEX_UI_MP, x, y, Animation_UpdateX(n, 1, 10, CTexture::Texture_GetWidth(CTexture::TEX_UI_MP, 10), 10), CTexture::Texture_GetHeight(CTexture::TEX_UI_MP, 1) * colornum, CTexture::Texture_GetWidth(CTexture::TEX_UI_MP, 10), CTexture::Texture_GetHeight(CTexture::TEX_UI_MP, 1));
		delete pn;
	}
}

void Number_Draw2(float x, float y, int n, int colornum)
{
	if (n >= 0 && n <= 9)
	{
		CUserInterFace *pn = new CUserInterFace();
		pn->Sprite_Draw(CTexture::TEX_NUM2, x, y, Animation_UpdateX(n, 1, 10, CTexture::Texture_GetWidth(CTexture::TEX_NUM2, 10), 10), CTexture::Texture_GetHeight(CTexture::TEX_NUM2, 2) * colornum, CTexture::Texture_GetWidth(CTexture::TEX_NUM2, 9), CTexture::Texture_GetHeight(CTexture::TEX_NUM2, 2));
		delete pn;
	}
}

void Number_Draw3(float x, float y, int n, int colornum)
{
	if (n >= 0 && n <= 9)
	{
		CUserInterFace *pn = new CUserInterFace();
		pn->Sprite_Draw(CTexture::TEX_UI_NUMBER, x, y, Animation_UpdateX(n, 1, 10, CTexture::Texture_GetWidth(CTexture::TEX_UI_NUMBER, 10), 10), CTexture::Texture_GetHeight(CTexture::TEX_UI_NUMBER, 1) * colornum, CTexture::Texture_GetWidth(CTexture::TEX_UI_NUMBER, 10), CTexture::Texture_GetHeight(CTexture::TEX_UI_NUMBER, 1));
		delete pn;
	}
}


void Number_Draw_Select(float x, float y, int n, int texindex, int colornum)
{
	if (n >= 0 && n <= 9)
	{
		CUserInterFace *pn = new CUserInterFace();
		pn->Sprite_Draw(texindex, x, y, Animation_UpdateX(n, 1, 10, CTexture::Texture_GetWidth(texindex, 10), 10), CTexture::Texture_GetHeight(texindex, 2) * colornum, CTexture::Texture_GetWidth(texindex, 10), CTexture::Texture_GetHeight(texindex, 2));
		delete pn;
	}
}