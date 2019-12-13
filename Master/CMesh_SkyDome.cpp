//////////////////////////////////////////////////
////
////	スカイドームクラス
////
//////////////////////////////////////////////////



//=============================================================================
//	インクルードファイル
//=============================================================================

#include "CMesh_SkyDome.h"
#include "Cplayer.h"
#include "C3DObj.h"
#define _CRTDBG_MAP_ALLOC

#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//=============================================================================
//	定数定義
//=============================================================================


//=============================================================================
//	静的変数
//=============================================================================

CMesh_SkyDome *CMesh_SkyDome::m_pMesh_SkyDome;

//=============================================================================
//	生成
//=============================================================================

CMesh_SkyDome::CMesh_SkyDome()
{

}

CMesh_SkyDome::CMesh_SkyDome(int TexIndex,float meshH, float radius, int meshXnum, int meshYnum) :CGameObj(CGameObj::TYPE_MESH)
{
	m_TexIndex = TexIndex;
	m_meshH = meshH;
	m_meshWnum = meshXnum;
	m_meshHnum = meshYnum;
	m_radius = radius;
	m_VertexW = meshXnum + 1;
	m_VertexH = meshYnum + 1;
	m_VertexCount = m_VertexW * m_VertexH;
	m_IndexCount = (meshXnum + 2) * (meshYnum * 2) - 2;
	m_PrimitiveCount = (meshXnum * 2 + 4) * meshYnum - 4;
	Mesh_SkyDome_Initialize();
}
//=============================================================================
//	破棄
//=============================================================================

CMesh_SkyDome::~CMesh_SkyDome()
{
	Finalize();
}

//	更新
void CMesh_SkyDome::Update(void)
{
	C3DObj *pplayer = CPlayer::Get_Player();
	m_mtxWorld = pplayer->Get_mtxTranslation();
}

//	描画
void CMesh_SkyDome::Draw(void)
{
	CMeshField::DrawSky(m_mtxWorld);
	//CMeshField::Draw();
}


//	メッシュフィールドシリンダー作成
CMesh_SkyDome *CMesh_SkyDome::Mesh_SkyDome_Create(int TexIndex,float meshH, float radius, int meshXnum, int meshYnum)
{
	CMesh_SkyDome *m_pMesh_SkyDome = new CMesh_SkyDome(TexIndex, meshH, radius, meshXnum, meshYnum);
	return m_pMesh_SkyDome;
}

//	終了処理
void CMesh_SkyDome::Finalize(void)
{
	// インデックスバッファの解放
	if (m_pIndexBuffer != NULL)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer = NULL;
	}

	// 頂点バッファの解放
	if (m_pVertexBuffer != NULL)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = NULL;
	}
	if (m_pMesh_SkyDome != NULL)
	{
		delete m_pMesh_SkyDome;
	}
}



//	メッシュフィールドシリンダー初期化(頂点設定)
void CMesh_SkyDome::Mesh_SkyDome_Initialize(void)
{
	m_pD3DDevice->CreateVertexBuffer(sizeof(MeshFieldVertex) * m_VertexCount, D3DUSAGE_WRITEONLY, FVF_MESH_FIELD_VERTEX, D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);
	// インデックスバッファの確保
	m_pD3DDevice->CreateIndexBuffer(sizeof(WORD) * m_IndexCount, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndexBuffer, NULL);

	float Subtotal = 0;

	for (int h = 0;h < m_VertexH;h++)
	{
		if (h == 0)
		{
			for (int w = 0;w < m_VertexW;w++)
			{
				int n = w + m_VertexW * h;
				m_Mfield[n].pos = D3DXVECTOR3(0, m_meshHnum * m_meshH, 0);
				//g_Mfield[n].color = D3DCOLOR(0xff0000ff);
				m_Mfield[n].color = D3DCOLOR(0xffffffff);
				m_Mfield[n].uv = D3DXVECTOR2(w * (1.0f / m_meshWnum), h * (1.0f / m_meshHnum));
			}
		}
		else
		{

			Subtotal += (h * m_meshH / m_VertexH) * 2;
			for (int w = 0;w < m_VertexW;w++)
			{
				int n = w + m_VertexW * h;
				float angle = (float)(360 / m_meshWnum * w);

				m_Mfield[n].pos = D3DXVECTOR3(m_radius / m_VertexH * h * sinf(D3DXToRadian(angle)), (m_meshHnum * m_meshH) - Subtotal, m_radius / m_VertexH * h * cosf(D3DXToRadian(angle)));
				//m_Mfield[n].color = D3DCOLOR_ARGB(250, 0, 0, 0);
				m_Mfield[n].color = D3DCOLOR(0xffffffff);
				//m_Mfield[n].color = D3DCOLOR_ARGB(255, (255 / (m_VertexH -1)) * h, (255 / (m_VertexH -1)) * h, (255 / (m_VertexH -1)) * h);
				m_Mfield[n].uv = D3DXVECTOR2(w * (1.0f / m_meshWnum), h * (1.0f / m_meshHnum));
			}
		}

	}

	Buffer_Write();
}





