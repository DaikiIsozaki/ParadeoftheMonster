//////////////////////////////////////////////////
////
////	テクスチャクラス
////
//////////////////////////////////////////////////

//=============================================================================
//	インクルードファイル
//=============================================================================

#include "CTexture.h"

//=============================================================================
//	静的変数
//=============================================================================

CTexture::TextureFileData CTexture::TEXTURE_FILES[] = {
	{ "asset/floor.png",600,600 },
	{ "asset/bullet000.png",80,80 },
	{ "asset/sky01.png",4000,992 },
	{ "asset/title.png",1920,1080 },
	{ "asset/gameover.jpg",2480,3508 },
	{ "asset/HP_Gage.png",200,50 },
	{ "asset/HP_Gage_2.png",200,50 },
	{ "asset/MP_Gage.png",200,50 },
	{ "asset/stock.png",60,60 },
	{ "asset/number2.png",320,32 },
	{ "asset/score.png",2250,450 },
	{ "asset/map.png",300,300 },			//11
	{ "asset/Arrow.png",20,20 },		//12
	{ "asset/Enemy_mark.png",12,12 },
	{ "asset/hit1.png",3360,960 },
	{ "asset/eff.jpg",80,80 },
	{ "asset/gauge.png",100,20 },
	{ "asset/whitebar.png",100,20 },
	{ "asset/ui/ber.png",1024,157 },
	{ "asset/ui/memori.png",43,38 },
	{ "asset/ui/ui_bg_normal.png",600,600 },
	{ "asset/ui/ui_coldtime.png",600,600 },
	{ "asset/ui/ui_available.png",600,600 },
	{ "asset/ui/ui_futi.png",600,600 },
	{ "asset/ui/futi_aka.png",600,600 },
	{ "asset/ui/futi_kiiro.png",600,600 },
	{ "asset/ui/ui_coffee.png",600,600 },
	{ "asset/ui/ui_freefall.png",600,600 },
	{ "asset/ui/ui_popcorn.png",600,600 },
	{ "asset/ui/ui_wheel.png",600,600 },
	{ "asset/ui/ui_coaster.png",600,600 },
	{ "asset/ui/ui_x.png",81,94 },
	{ "asset/ui/ui_diamond.png",111,121 },
	{ "asset/ui/ui_mp_font.png",450,45 },
	{ "asset/ui/time01.png",1500,150 },
	{ "asset/anime_model/outUV1.jpg",256, 256 },
	{ "asset/anime_model/outUV2.jpg",256, 256 },
	{ "asset/anime_model/zago1-end.jpg",256, 256 },
	{ "asset/anime_model/zago2-1.jpg",256, 256 },
	{ "asset/anime_model/zago2-1_2.jpg",256, 256 },
	{ "asset/anime_model/zago2-end.jpg",256, 256 },
	{ "asset/anime_model/zago3-1.jpg",256, 256 },
	{ "asset/anime_model/zago3-1_2.jpg",256, 256 },
	{ "asset/anime_model/zago3-end.jpg",256, 256 },
	{ "asset/anime_model/zago4-1.jpg",256, 256 },
	{ "asset/anime_model/zago4-1_2.jpg",256, 256 },
	{ "asset/anime_model/zago4-end.jpg",256, 256 },
	{ "asset/ui/ui_bar.png",1024,222 },
	{ "asset/ui/ui_graybar.png",1024,222 },
	{ "asset/ui/ui_hpbar.png",1024,222 },
	{ "asset/ui/ui_mpbar.png",1024,222 },
	{ "asset/ui/ui_mp_font.png",800,80 },
	{ "asset/Bar.png",200,50 },
	{ "asset/score2.png",1350,270 },
	{ "asset/kusa_renga2.png",760,760 },
	{ "asset/result01.png",340, 170 },
	{ "asset/result02.png",340, 170 },
	{ "asset/Score_result03.png",1824, 10 },
	{ "asset/result03.png",340, 220 },
	{ "asset/a.png", 30,144 },
	{ "asset/GameClear.png", 912,200 },
	{ "asset/Clear_bonus03.png", 500, 350 },
	{ "asset/HP_Gage_2.png",400,100 },
	{ "asset/MP_Gage.png",400,100 },
	{ "asset/a.png", 30,144 },
	{ "asset/screenshot.bmp",1920,1016 },
	{ "asset/Blue.jpg",14,14 },
	{ "asset/HP_Gage_2.png",762, 40 },
	{ "asset/MP_Gage.png",762,40 },
	{ "asset/mountain.png",8268,1772 },
	{ "asset/ui/ui_a.png",200,200 },
	{ "asset/ui/ui_b.png",200,200 },
	{ "asset/ui/ui_l1.png",200,200 },
	{ "asset/ui/ui_r1.png",200,200 },
	{ "asset/number_yellow.png",1350,270 },
	{ "asset/effect/hikkaki.png",1200,480 },
	{ "asset/effect/star2.png",1200,720 },
	{ "asset/effect/shine.png",2400,240 },
	{ "asset/effect/heal_17.png",960,384 },
	{ "asset/effect/hit.png",4000,2000 },
	{ "asset/effect/impact_13.png",960,384 },
	{ "asset/effect/Effect_p020.png",1600,200 },
	{ "asset/effect/heal.png",960,384 },
	{ "asset/tutorial01.png",1920,1016 },
	{ "asset/tutorial02.png",1920,1016 },
	{ "asset/tutorial03.png",1920,1016 },
	{ "asset/push_button.png",1027,156 },
	{ "asset/title_logo.png",1200,600 },
	{ "asset/ui/time01.png",1500,150 },
	{ "asset/ui/time02.png",1800,180 },
	{ "asset/ui/time03.png",2250,450 },
	{ "asset/ui/gekiha.png",2250,450 },
	{ "asset/ui/time03.png",1500,300 },
	{ "asset/ui/time01.png",1800,180 },
	{ "asset/ui/time04.png",2250,225 },
	{ "asset/ui/ui_a.png",130,130 },
	{ "asset/effect/maru_fadeout.png",9600,7112 },
	{ "asset/effect/maru_fade_in.png",9600,7112 },
};

int CTexture::TEXTURE_MAX = sizeof(CTexture::TEXTURE_FILES) / sizeof(TEXTURE_FILES[0]);

LPDIRECT3DTEXTURE9 CTexture::m_pTextures[sizeof(CTexture::TEXTURE_FILES) / sizeof(TEXTURE_FILES[0])] = {};




//=============================================================================
//	生成
//=============================================================================

CTexture::CTexture()
{

}

//=============================================================================
//	破棄
//=============================================================================

CTexture::~CTexture()
{
	//CTexture::Texture_Release();

}


//	テクスチャの読み込み
int CTexture::Texture_Load(void)
{

	HRESULT hr;
	int i;
	int failed_count = 0;
	for (i = 0; i < TEXTURE_MAX; i++)
	{
		if (m_pTextures[i] == NULL)
		{
			hr = D3DXCreateTextureFromFile(m_pD3DDevice, TEXTURE_FILES[i].filename, &m_pTextures[i]);
			if (FAILED(hr))
			{
				failed_count++;
				MessageBox(NULL, "テクスチャデータを読み込めませんでした", "確認", MB_OK);
			}
		}
	}

	return failed_count;
}


// テクスチャの開放
void CTexture::Texture_Release(void)
{
	int i;
	for (i = 0; i < TEXTURE_MAX; i++)
	{
		if (m_pTextures[i] != NULL)
		{
			m_pTextures[i]->Release();
			m_pTextures[i] = NULL;
		}
	}
}

void CTexture::Texture_Load(int index)
{
	HRESULT hr;
	if (m_pTextures[index] == NULL)
	{
		hr = D3DXCreateTextureFromFile(m_pD3DDevice, TEXTURE_FILES[index].filename, &m_pTextures[index]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "テクスチャデータを読み込めませんでした", "確認", MB_OK);
		}
	}
}

void CTexture::Texture_Release(int index)
{
	if (m_pTextures[index] != NULL)
	{
		m_pTextures[index]->Release();
		m_pTextures[index] = NULL;
	}
}













