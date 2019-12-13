//////////////////////////////////////////////////
////
////	メッシュフィールドクラス
////
//////////////////////////////////////////////////



//=============================================================================
//	インクルードファイル
//=============================================================================

#include "CMeshField.h"
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC

#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//=============================================================================
//	定数定義
//=============================================================================


//=============================================================================
//	静的変数
//=============================================================================

CMeshField *CMeshField::m_pMeshFielde;

//=============================================================================
//	生成
//=============================================================================

CMeshField::CMeshField()
{
	
}
CMeshField::CMeshField(int TexIndex, float meshW, int meshXnum, int meshZnum) : CGameObj(CGameObj::TYPE_MESH)
{
	m_TexIndex = TexIndex;
	m_meshW = meshW;
	m_meshWnum = meshXnum;
	m_meshHnum = meshZnum;
	m_VertexW = meshXnum + 1;
	m_VertexH = meshZnum + 1;
	m_VertexCount = m_VertexW * m_VertexH;
	m_IndexCount = (meshXnum + 2) * (meshZnum * 2) - 2;
	m_PrimitiveCount = (meshXnum * 2 + 4) * meshZnum - 4;
	Mesh_Field_Initialize2();
}
//=============================================================================
//	破棄
//=============================================================================

CMeshField::~CMeshField()
{
	Finalize();
}

//	更新
void CMeshField::Update(void)
{

}

//	描画
void CMeshField::Draw(void)
{

	D3DXMATRIX mtxWorld;		// 4*4行列　d3dx9.h必要
	D3DXMATRIX mtxTranslation;
	

	D3DXMatrixTranslation(&mtxTranslation, 0, 0, 0);

	mtxWorld = mtxTranslation;
	//pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	m_pD3DDevice->SetFVF(FVF_MESH_FIELD_VERTEX);
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pD3DDevice->SetTexture(0, Texture_GetTexture(m_TexIndex));

	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	m_pD3DDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(MeshFieldVertex));
	m_pD3DDevice->SetIndices(m_pIndexBuffer);
	m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_VertexCount, 0, m_PrimitiveCount);

}

//	描画
void CMeshField::DrawSky(D3DXMATRIX mtxw)
{


	//pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	mtxw._42 = 0.0f;
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxw);
	m_pD3DDevice->SetFVF(FVF_MESH_FIELD_VERTEX);
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pD3DDevice->SetTexture(0, Texture_GetTexture(m_TexIndex));

	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	m_pD3DDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(MeshFieldVertex));
	m_pD3DDevice->SetIndices(m_pIndexBuffer);
	m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_VertexCount, 0, m_PrimitiveCount);

}

CMeshField *CMeshField::MeshField_Create(int TexIndex, float meshW, int meshXnum, int meshZnum)
{
	CMeshField *m_pMeshFielde = new CMeshField(TexIndex, meshW, meshXnum, meshZnum);
	return m_pMeshFielde;
}

void CMeshField::Finalize(void)
{
	if (m_pMeshFielde != NULL)
	{
		delete m_pMeshFielde;
		m_pMeshFielde = NULL;
	}
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
	if (m_pMeshFielde != NULL)
	{
		delete m_pMeshFielde;
	}
}


void CMeshField::Buffer_Write(void)
{
	// 仮想アドレス　
	MeshFieldVertex *pv;
	// 頂点バッファのロック
	m_pVertexBuffer->Lock(0, 0, (void**)&pv, 0);

	//memcpy(pv, g_Mfield, sizeof(g_Mfield[MAX_MESH_FIELD]));
	for (int i = 0;i < m_VertexCount;i++)
	{
		pv[i] = m_Mfield[i];
	}
	m_pVertexBuffer->Unlock();

	// 仮想アドレス　
	WORD *Ipv;
	// インデックスバッファのロック
	m_pIndexBuffer->Lock(0, 0, (void**)&Ipv, 0);

	for (int h = 0;h < m_meshHnum * 2 - 1;h++)
	{
		if (h < m_meshHnum)
		{
			for (int w = 0;w < m_VertexW * 2;w++)
			{
				if (w % 2 == 0)
				{
					Ipv[(m_VertexW + 1) * (2 * h) + w] = (WORD)(m_VertexW + (m_VertexW * h) + (w * 0.5));
				}
				else
				{
					Ipv[(m_VertexW + 1) * (2 * h) + w] = (WORD)((m_VertexW * h) + ((w - 1) * 0.5));
				}
			}
		}
		else
		{
			Ipv[(m_VertexW * 2)*(h - m_meshHnum + 1) + (2 * (h - m_meshHnum))] = m_VertexW * (h - m_meshHnum + 1) - 1;
			Ipv[(m_VertexW * 2)*(h - m_meshHnum + 1) + (2 * (h - m_meshHnum)) + 1] = m_VertexW * (h - m_meshHnum + 2);
		}
	}

	m_pIndexBuffer->Unlock();
}


void CMeshField::Mesh_Field_Initialize(void)
{
	// 頂点バッファの確保
	m_pD3DDevice->CreateVertexBuffer(sizeof(MeshFieldVertex) * m_VertexCount, D3DUSAGE_WRITEONLY, FVF_MESH_FIELD_VERTEX, D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);
	// インデックスバッファの確保
	m_pD3DDevice->CreateIndexBuffer(sizeof(WORD) * m_IndexCount, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndexBuffer, NULL);

	for (int h = 0;h < m_VertexH;h++)
	{
		for (int w = 0;w < m_VertexW;w++)
		{
			int n = w + m_VertexW * h;
			m_Mfield[n].pos = D3DXVECTOR3(((float)m_meshWnum * 0.5f * -m_meshW) + (m_meshW * w), 0, ((float)m_meshHnum * 0.5f * m_meshW) + (-m_meshW * h));
			m_Mfield[n].color = D3DCOLOR(0xffffffff);
			m_Mfield[n].uv = D3DXVECTOR2(w * (1.0f / m_meshWnum), h * (1.0f / m_meshHnum));
		}

	}

	Buffer_Write();
}



void CMeshField::Mesh_Field_Initialize2(void)
{
	// 頂点バッファの確保
	m_pD3DDevice->CreateVertexBuffer(sizeof(MeshFieldVertex) * m_VertexCount, D3DUSAGE_WRITEONLY, FVF_MESH_FIELD_VERTEX, D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);
	// インデックスバッファの確保
	m_pD3DDevice->CreateIndexBuffer(sizeof(WORD) * m_IndexCount, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndexBuffer, NULL);

	for (int h = 0;h < m_VertexH;h++)
	{
		for (int w = 0;w < m_VertexW;w++)
		{
			int n = w + m_VertexW * h;
			m_Mfield[n].pos = D3DXVECTOR3(((float)m_meshWnum * 0.5f * -m_meshW) + (m_meshW * w), 0.0f, ((float)m_meshHnum * 0.5f * m_meshW) + (-m_meshW * h));
			m_Mfield[n].color = D3DCOLOR(0xffffffff);
			m_Mfield[n].uv = D3DXVECTOR2((float)w, (float)h);
		}

	}

	Buffer_Write();
}

