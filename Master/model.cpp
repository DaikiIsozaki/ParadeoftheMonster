
//=============================================================================
//	インクルードファイル
//=============================================================================
#include <d3dx9.h>
#include "mochi_d3d.h"
#include "common.h"

#include "input.h"
#include "move.h"
//=============================================================================
//	定数定義
//=============================================================================

#define MAX_MODEL (10)

//=============================================================================
//	グローバル宣言
//=============================================================================

LPD3DXMESH g_pD3DXMesh[MAX_MODEL];	//	メッシュを受け取る変数
//LPD3DXBUFFER g_pD3DXAdjacencyBuffer;	//	隣接性情報を受け取る変数
static DWORD g_dwNumMaterials[MAX_MODEL];	//	マテリアルの総数	描画する回数
LPD3DXBUFFER g_pD3DXMtrBuffer[MAX_MODEL];	//	マテリアル情報を受け取る変数
/*
	この3つを複数化する
	リリース
		LPD3DXMESH　LPD3DXBUFFER
*/

//static LPDIRECT3DDEVICE9 g_pD3DDevice;

static LPDIRECT3DTEXTURE9 *g_pTexures[MAX_MODEL] = { NULL };
static D3DMATERIAL9 *g_pd3dMaterials[MAX_MODEL] = { NULL };

static D3DXMATRIX g_mtxTranslation[MAX_MODEL];
static D3DXMATRIX g_mtxRotation;
static float g_Angle = 0.0f;

//	モデルデータの構造体
typedef struct 
{
	char filename[256];
	
}MaterialFileData;

//	使いたいモデルの数だけ書く
static const MaterialFileData TEXTURE_FILES[] = {
	{ "bananaZ.x" },
	{"emi-ru2.x"},
	{"smart_emiru.x"}
};

static D3DXMATRIX g_mtxKeepTranslation;

//	モデル読み込み
void Model_Load(int index)		//	複数化したらindexを戻す関数にする
{
	LPDIRECT3DDEVICE9 g_pD3DDevice = mochi_GetDevice();

	HRESULT hr;
	hr = D3DXLoadMeshFromX(TEXTURE_FILES[index].filename, D3DXMESH_MANAGED, g_pD3DDevice, NULL, &g_pD3DXMtrBuffer[index], NULL, &g_dwNumMaterials[index], &g_pD3DXMesh[index]);
	if (FAILED(hr))
	{
		MessageBox(NULL, "モデルデータを読み込めませんでした", "確認", MB_OK);
	}
		/*
			第1引数　ファイル名
			第2引数　65535以上の頂点が必要なら
					 D3DXMESH_MANAGED | D3DXMESH_WRITEONLY | D3DXMESH_32BIT にする
			第3引数　デバイス
			第4引数　NULL　g_pD3DXAdjacencyBuffer
			第5引数　g_pD3DXMtrBuffer
			第6引数　dwNumMaterials
			第7引数　g_pD3DXMesh
		*/

	//	マテリアル保存用配列の確保
	g_pd3dMaterials[index] = new D3DMATERIAL9[g_dwNumMaterials[index]];
	g_pTexures[index] = new LPDIRECT3DTEXTURE9[g_dwNumMaterials[index]];
	
	//	マテリアル情報バッファのポインタ取得
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)g_pD3DXMtrBuffer[index]->GetBufferPointer();

	//	各マテリアル情報を保存
	for (int i = 0;i < g_dwNumMaterials[index];i++)
	{
		g_pd3dMaterials[index][i] = d3dxMaterials[i].MatD3D;	//	Xファイルのマテリアルカラーのコピー
		/*
			ディフーズ		
			アンビエント	
			エミッシブ		ライト関係なし　自己発光
			スペキュラ
		*/
		g_pd3dMaterials[index][i].Ambient = g_pd3dMaterials[index][i].Diffuse;

		//	テクスチャの読み込み
		if (d3dxMaterials[i].pTextureFilename == NULL)	//	テクスチャがないマテリアルはファイル名なし
		{
			g_pTexures[index][i] = NULL;
		}
		else
		{
			hr = D3DXCreateTextureFromFile(g_pD3DDevice, d3dxMaterials[i].pTextureFilename, &g_pTexures[index][i]);
			if (FAILED(hr))
			{
				g_pTexures[index][i] = NULL;
				MessageBox(NULL, "テクスチャデータを読み込めませんでした", "確認", MB_OK);
			}
		}
	}

	//	Xファイルから読み込んだマテリアル情報のバッファを解放
	if (g_pD3DXMtrBuffer)
	{
		g_pD3DXMtrBuffer[index]->Release();
	}


	D3DXMatrixTranslation(&g_mtxTranslation[index],0,1,0);
	D3DXMatrixIdentity(&g_mtxRotation);
	g_mtxKeepTranslation = g_mtxTranslation[index];
}

//	描画処理
void Model_Draw(int index)		//	複数化したら引数を変える　行列
{
	LPDIRECT3DDEVICE9 g_pD3DDevice = mochi_GetDevice();

	g_pD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	g_pD3DDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);

	//	ワールド変換行列
	D3DXMATRIX mtxWorld;		// 4*4行列　d3dx9.h必要
	D3DXMATRIX mtxScaling;
	D3DXMATRIX mtxTranslation;
	//D3DXMatrixTranslation(&mtxTranslation, 0, 1.5, 0);
	D3DXMatrixIdentity(&mtxWorld);	// アドレス送ると単位行列を作ってくれる
	D3DXMatrixScaling(&mtxScaling, 0.5f, 1.0f, 0.5f);
	mtxWorld = mtxScaling * g_mtxRotation * g_mtxTranslation[index];
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	for (int i = 0;i < g_dwNumMaterials[index];i++)
	{
		g_pD3DDevice->SetMaterial(&g_pd3dMaterials[index][i]);
		g_pD3DDevice->SetTexture(0, g_pTexures[index][i]);
		g_pD3DXMesh[index]->DrawSubset(i);
	}
}

// 描画処理
void Model_Draw(int index, D3DXMATRIX mtxWorld)		//	複数化したら引数を変える　行列
{
	LPDIRECT3DDEVICE9 g_pD3DDevice = mochi_GetDevice();

	g_pD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	g_pD3DDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	for (int i = 0;i < g_dwNumMaterials[index];i++)
	{
		g_pD3DDevice->SetMaterial(&g_pd3dMaterials[index][i]);
		g_pD3DDevice->SetTexture(0, g_pTexures[index][i]);
		g_pD3DXMesh[index]->DrawSubset(i);
	}
}

//	終了処理
void Model_Finalize(void)	//	モデルデータの開放　複数化したら全部消すかどれかを消す
{
	for (int i = 0;i < MAX_MODEL;i++)
	{
		if (g_pTexures[i] != NULL)
		{
			delete[]g_pTexures[i];
		}
		if (g_pd3dMaterials[i] != NULL)
		{
			delete[]g_pd3dMaterials[i];
			
		}
		if (g_pD3DXMesh[i] != NULL)
		{
			g_pD3DXMesh[i]->Release();
		}
	}
}

void Model_Finalize(int index)	//	モデルデータの開放　複数化したら全部消すかどれかを消す
{
	if (g_pTexures[index] != NULL)
	{
		delete[]g_pTexures[index];
	}
	if (g_pd3dMaterials[index] != NULL)
	{
		delete[]g_pd3dMaterials[index];
	}
	if (g_pD3DXMesh[index] != NULL)
	{
		g_pD3DXMesh[index]->Release();
	}
}

void Model_Updata(int index)
{
	if (Keyboard_IsPress(DIK_W))
	{
		g_mtxTranslation[index] *= Move(FLONT);

	}
	if (Keyboard_IsPress(DIK_S))
	{
		g_mtxTranslation[index] *= Move(BACK);
	}
	if (Keyboard_IsPress(DIK_D))
	{
		g_mtxTranslation[index] *= Move(RIGHT);
	}
	if (Keyboard_IsPress(DIK_A))
	{
		g_mtxTranslation[index] *= Move(LEFT);
	}

	if (Keyboard_IsPress(DIK_L) && (Keyboard_IsPress(DIK_LALT)))
	{
		g_Angle--;
		D3DXMatrixRotationY(&g_mtxRotation, D3DXToRadian(g_Angle));
	}
	if (Keyboard_IsPress(DIK_J) && (Keyboard_IsPress(DIK_LALT)))
	{
		g_Angle++;
		D3DXMatrixRotationY(&g_mtxRotation, D3DXToRadian(g_Angle));
	}

	if (45.0*45.0 < (g_mtxTranslation[index]._41*g_mtxTranslation[index]._41) + (g_mtxTranslation[index]._43 * g_mtxTranslation[index]._43))
	{
		g_mtxTranslation[index] = g_mtxKeepTranslation;
	}
	else
	{
		g_mtxKeepTranslation = g_mtxTranslation[index];
	}

}

D3DXMATRIX get_modelPos(int index)
{
	return g_mtxTranslation[index];
}