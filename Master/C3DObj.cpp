//////////////////////////////////////////////////
////
////	3Dオブジェクトクラス
////
//////////////////////////////////////////////////

#include "C3DObj.h"
#include "debug_font.h"
#include "CTexture.h"
#include "Cplayer.h"
#include "exp.h"
#include "CCamera.h"
#include "common.h"
#define _CRTDBG_MAP_ALLOC
#define VF_ANGLE (1.0f)//カリングの大体の大きさ

#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//=============================================================================
//	静的変数
//===========================================================================
C3DObj *C3DObj::p3DObj[MAX_GAMEOBJ];
int C3DObj::m_3DObjNum = 0;
int C3DObj::m_TotalScore = 0;
//THING C3DObj::Thing_Anime[5];//読み込むモデルの最大数+1
bool C3DObj::GetWCos = false;
bool C3DObj::g_CosterModeStandby = false;
C3DObj::MaterialFileData C3DObj::NORMAL_MODEL_FILES[] = {
	{ "asset/model/emi-ru2.x" },
	{ "asset/model/dish.blend.x" },
	{ "asset/model/enban.x" },
	{ "asset/model/hasira.x" },
	{ "asset/model/ferris.x" },
	{ "asset/model/jet_new.blend.x" },
	{ "asset/model/cup_blue.x" },
	{ "asset/model/cup_kiiro.x" },
	{ "asset/model/cup_midori.x" },
	{ "asset/model/pop.blend.x" },
	{ "asset/model/kensetutyuu.x" },
	{ "asset/model/ornament/lamp.x" },
	{ "asset/model/ornament/gomibako.x" },
	{ "asset/model/ornament/bench.x" },
	{ "asset/model/ornament/hunsui.x" },
	{ "asset/model/ornament/kanransya_Obj.blend.x" },
	{ "asset/model/ornament/husen.blend.x" },
	{ "asset/model/ornament/meri_dodai.blend.x" },
	{ "asset/model/ornament/Uma.blend.x" },
	{ "asset/model/ornament/kanransya_ornament.x" },
	{ "asset/model/GasBonbe.blend.x" },
	{ "asset/model/Got_PotatZou.blend.x" },
};
//	使いたいアニメモデルの数だけ書く
C3DObj::MaterialFileData2 C3DObj::ANIME_MODEL_FILES[] = {
	{ "asset/anime_model/hewplayer.x" },
	{ "asset/anime_model/small_enemy.x" },
	{ "asset/anime_model/middle_enemy.x" },
	{ "asset/anime_model/special_enemy.x" },
	{ "asset/anime_model/big_enemy.x" },
};
int C3DObj::MODEL_FILES_MAX = sizeof(C3DObj::NORMAL_MODEL_FILES) / sizeof(NORMAL_MODEL_FILES[0]);
int C3DObj::ANIME_MODEL_FILES_MAX = sizeof(C3DObj::ANIME_MODEL_FILES) / sizeof(ANIME_MODEL_FILES[0]);

bool C3DObj::boRenderSphere =false;
bool C3DObj::m_TitleFlag = true;
//モデルアニメーション関係変数
/*
#define MODEL_MAX (9)

SKIN_MESH SkinMesh;
THING_NORMAL Thing_Normal[MODEL_MAX+1];//読み込むモデルの最大数+1
THING Thing[THING_AMOUNT + 1];//読み込むモデルの最大数+1
LPD3DXANIMATIONSET pAnimSet[THING_AMOUNT][10] = { 0 };//選択したモデルに10個までのアニメーションをセット
FLOAT fAnimTime = 0.0f;
BOOL boPlayAnim = true;
D3DXTRACK_DESC TrackDesc;
*/

//SKIN_MESH C3DObj::SkinMesh;
THING_NORMAL C3DObj::Thing_Normal[(sizeof(C3DObj::NORMAL_MODEL_FILES) / sizeof(NORMAL_MODEL_FILES[0]))];//読み込むモデルの最大数+1
//THING C3DObj::Thing[sizeof(C3DObj::ANIME_MODEL_FILES) / sizeof(ANIME_MODEL_FILES[0])];//読み込むモデルの最大数+1


LPD3DXMESH C3DObj::m_pD3DXMesh[sizeof(C3DObj::NORMAL_MODEL_FILES) / sizeof(NORMAL_MODEL_FILES[0])] = {};
DWORD C3DObj::m_dwNumMaterials[sizeof(C3DObj::NORMAL_MODEL_FILES) / sizeof(NORMAL_MODEL_FILES[0])] = {};
LPD3DXBUFFER C3DObj::m_pD3DXMtrBuffer[sizeof(C3DObj::NORMAL_MODEL_FILES) / sizeof(NORMAL_MODEL_FILES[0])] = {};
LPDIRECT3DTEXTURE9 *C3DObj::m_pTexures[sizeof(C3DObj::NORMAL_MODEL_FILES) / sizeof(NORMAL_MODEL_FILES[0])] = {};
D3DMATERIAL9 *C3DObj::m_pd3dMaterials[sizeof(C3DObj::NORMAL_MODEL_FILES) / sizeof(NORMAL_MODEL_FILES[0])] = {};


//=============================================================================
//	生成
//=============================================================================
C3DObj::C3DObj()
{
	for (m_3DObjIndex = 0; m_3DObjIndex < MAX_GAMEOBJ; m_3DObjIndex++)
	{
		if (p3DObj[m_3DObjIndex] == NULL)
		{
			p3DObj[m_3DObjIndex] = this;
			m_3DObjType = TYPE_etc;
			m_3DObjNum++;
			break;
		}
	}
	// ワーク格納失敗
	if (m_3DObjIndex >= MAX_GAMEOBJ)
	{
		m_3DObjIndex = -1;
	}

	
}

C3DObj::C3DObj(int type)
{
	// ワークに自分自身(this)を格納
	for (m_3DObjIndex = 0; m_3DObjIndex < MAX_GAMEOBJ; m_3DObjIndex++)
	{
		if (p3DObj[m_3DObjIndex] == NULL)
		{
			p3DObj[m_3DObjIndex] = this;
			m_3DObjType = type;
			m_3DObjNum++;
			break;
		}
	}
	// ワーク格納失敗
	if (m_3DObjIndex >= MAX_GAMEOBJ)
	{
		m_3DObjIndex = -1;
	}

	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXMatrixIdentity(&m_mtxTranslation);
	D3DXMatrixIdentity(&m_mtxRotation);
	D3DXMatrixIdentity(&m_mtxScaling);
	m_Enable = false;
	m_DamageFlag = false;
	m_AttakFlag = false;
}
//=============================================================================
//	破棄
//=============================================================================
C3DObj::~C3DObj()
{
	if (Thing.pSphereMeshMaterials)
	{
		//delete Thing.pSphereMeshMaterials;
		for (DWORD i = 0; i < Thing.dwNumMaterials; i++)
		{
			delete[]Thing.pSphereMeshMaterials;
		}
	}
	if (Thing.pFrameRoot)
	{
		SkinMesh.cHierarchy.DestroyFrame(Thing.pFrameRoot);
	}

	/*if (Thing.pSphereMeshMaterials)
	{
		for (DWORD i = 0; i < Thing.dwNumMaterials; i++)
		{
			delete[]Thing.pSphereMeshMaterials;
		}
	}*/
	THING();
	THING_NORMAL();


	//delete &SkinMesh;

	m_3DObjNum--;
	p3DObj[m_3DObjIndex] = NULL;

}

void C3DObj::FinalizeAll()
{
	for (int i = 0; i < MAX_GAMEOBJ; i++)
	{
		// ポリモーフィズムによって派生クラスのUpdate()が呼ばれる
		if (p3DObj[i])
		{
			p3DObj[i]->Finalize();
		}
	}
}
void C3DObj::Finalize()
{

}


//=============================================================================
// 全オブジェクト更新
//=============================================================================
void C3DObj::UpdateAll()
{
	int i;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		GetWCos = CPlayer::GetCoaster_Enable();
		// ポリモーフィズムによって派生クラスのUpdate()が呼ばれる
		if (p3DObj[i])
		{
			p3DObj[i]->Update();
		}
	}
}

//=============================================================================
// 全オブジェクト描画
//=============================================================================
void C3DObj::DrawAll()
{
	int i;
	int dc = 0;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		// ポリモーフィズムによって派生クラスのDraw()が呼ばれる
		if (p3DObj[i])
		{
			if (p3DObj[i]->m_3DObjType != TYPE_STADBY)
			{
				if (!p3DObj[i]->Thing.pFrameRoot)
				{
					if (VFCulling(&p3DObj[i]->Thing_Normal_model.vPosition))
					{
						p3DObj[i]->Draw();
					}
					else
					{
						//画面外の時はテクスチャを更新
						//p3DObj[i]->SkinMesh.DrawFrame(m_pD3DDevice, p3DObj[i]->Thing_Normal_model.pFrameRoot, &p3DObj[i]->Thing, false);
					}
				}
				else
				{
					if (VFCulling(&p3DObj[i]->Thing.vPosition))
					{
						p3DObj[i]->Draw();
						//dc++;
					}
					else
					{
						//画面外の時はテクスチャを更新
						p3DObj[i]->SkinMesh.DrawFrame(m_pD3DDevice, p3DObj[i]->Thing.pFrameRoot, &p3DObj[i]->Thing, false);
					}
				}
			}
			else
			{
				p3DObj[i]->Draw();
			}
		}
	}
	
}



//=============================================================================
// 全オブジェクト削除
//=============================================================================
void C3DObj::DeleteAll()
{
	int i;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		if (p3DObj[i])
		{
			delete p3DObj[i];
			//p3DObj[i] = NULL;
		}
	}
}


//=============================================================================
// インスタンス取得
//=============================================================================
C3DObj *C3DObj::Get(int nIdx)
{
	// インデックスが範囲外
	if (p3DObj[nIdx])
	{
		return p3DObj[nIdx];
	}
	return NULL;
}


//=============================================================================
// オブジェクト削除
//=============================================================================
void C3DObj::C3DObj_delete(void)
{
	C3DObj *pobj = Get(Get_3DObjIndex());
	delete pobj;
}



//=============================================================================
// モデル読み込み
//=============================================================================
// Geometryの初期化（モデルの読み込み）
HRESULT C3DObj::InitModelLoad()
{
	int i;
	//通常モデル読み込み	
	for (i = 0; i < MODEL_FILES_MAX; i++)
	{
		InitThing(&Thing_Normal[i], NORMAL_MODEL_FILES[i].filename);
		InitSphere(m_pD3DDevice, &Thing_Normal[i]);//当たり判定の表示
	}

	//アニメーションモデル読み込み
	//THINGにxファイルを読み込む
	/*for (int i = 0; i < ANIME_MODEL_FILES_MAX; i++)
	{
		SkinMesh.InitThing(m_pD3DDevice, &Thing[i], ANIME_MODEL_FILES[i].filename);
		SkinMesh.InitSphere(m_pD3DDevice, &Thing[i]);
	}*/
	return S_OK;
}

HRESULT C3DObj::InitThing(THING_NORMAL *pThing, LPSTR szXFileName)
{
	/////////
	// Xファイルからメッシュをロードする	
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	if (FAILED(D3DXLoadMeshFromX(szXFileName, D3DXMESH_SYSTEMMEM,
		m_pD3DDevice, NULL, &pD3DXMtrlBuffer, NULL,
		&pThing->dwNumMaterials, &pThing->pMesh)))
	{
		MessageBox(NULL, "Xファイルの読み込みに失敗しました", szXFileName, MB_OK);
		return E_FAIL;
	}
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	pThing->pMeshMaterials = new D3DMATERIAL9[pThing->dwNumMaterials];
	pThing->pMeshTextures = new LPDIRECT3DTEXTURE9[pThing->dwNumMaterials];

	for (DWORD i = 0; i<pThing->dwNumMaterials; i++)
	{
		pThing->pMeshMaterials[i] = d3dxMaterials[i].MatD3D;
		pThing->pMeshMaterials[i].Ambient = pThing->pMeshMaterials[i].Diffuse;
		pThing->pMeshTextures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			if (FAILED(D3DXCreateTextureFromFile(m_pD3DDevice,
				d3dxMaterials[i].pTextureFilename,
				&pThing->pMeshTextures[i])))
			{
				MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
			}
		}
	}
	pD3DXMtrlBuffer->Release();

	return S_OK;
}

//=============================================================================
// モデル破棄
//=============================================================================
void C3DObj::Model_Finalize(void)	//	モデルデータの開放　複数化したら全部消すかどれかを消す
{
	for (int i = 0;i < MODEL_FILES_MAX;i++)
	{
		if (m_pTexures[i] != NULL)
		{
			delete[]m_pTexures[i];
		}
		if (m_pd3dMaterials[i] != NULL)
		{
			delete[]m_pd3dMaterials[i];
		}
		if (m_pD3DXMesh[i] != NULL)
		{
			m_pD3DXMesh[i]->Release();
		}
		if (Thing_Normal[i].pMeshMaterials)
		{
			//delete[]Thing_Normal[i].pMeshMaterials;
			for (DWORD k = 0; k < Thing_Normal[i].dwNumMaterials; i++)
			{
				delete[]Thing_Normal[i].pMeshMaterials;
			}
			
		}
		if (Thing_Normal[i].pMeshTextures)
		{
			//delete[]Thing_Normal[i].pMeshTextures;
			for (DWORD k = 0; k < Thing_Normal[i].dwNumMaterials; i++)
			{
				delete[]Thing_Normal[i].pMeshMaterials;
			}
		}
	}
}

void C3DObj::Model_Finalize(int index)	//	モデルデータの開放　複数化したら全部消すかどれかを消す
{
	if (m_pTexures[index] != NULL)
	{
		delete[]m_pTexures[index];
	}
	if (m_pd3dMaterials[index] != NULL)
	{
		delete[]m_pd3dMaterials[index];

	}
	if (m_pD3DXMesh[index] != NULL)
	{
		m_pD3DXMesh[index]->Release();
	}
}



//=============================================================================
// モデル描画  アニメーション有
//=============================================================================
void C3DObj::DrawDX_Anime(D3DXMATRIX mtxWorld, int type, THING* pThing)
{
	//static float fAnimTimeHold = fAnimTime;
	float fAnimTimeHold = fAnimTime;
	if (boPlayAnim)
	{
		fAnimTime += 0.01f;
	}

	SKIN_MESH::UpdateSphere(m_pD3DDevice, pThing);

	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
//	m_pD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
//	m_pD3DDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);
	//m_pD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	//m_pD3DDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);


	D3DXMATRIXA16 mat;

	m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//モデルレンダリング
	SkinMesh.UpdateFrameMatrices(pThing->pFrameRoot, &mtxWorld);
	SkinMesh.DrawFrame(m_pD3DDevice, pThing->pFrameRoot, pThing, true);
	pThing->pAnimController->AdvanceTime(fAnimTime - fAnimTimeHold, NULL);

	mtxWorld *= mat;

	m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	if (boRenderSphere)
	{
		//　バウンディングスフィアのレンダリング////////	
		D3DXMatrixTranslation(&mat, pThing->vPosition.x, pThing->vPosition.y,
			pThing->vPosition.z);
		D3DXMatrixTranslation(&mtxWorld, pThing->Sphere.vCenter.x, pThing->Sphere.vCenter.y,
			pThing->Sphere.vCenter.z);
		m_pD3DDevice->SetTexture(0, NULL);
		m_pD3DDevice->SetMaterial(pThing->pSphereMeshMaterials);
		pThing->pSphereMesh->DrawSubset(0);
	}
	////////////////////////////////
	//アニメ再生時間を+
/*	fAnimTimeHold = fAnimTime;
	if (boPlayAnim)
	{
		fAnimTime += 0.01f;
	}*/
}



//=============================================================================
// モデル描画  アニメーション無
//=============================================================================
void C3DObj::DrawDX_Normal(D3DXMATRIX mtxWorld, int type, THING_NORMAL* pThing)
{
	DWORD i;
	
	//m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	m_pD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	m_pD3DDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);


	// マトリックスのセット
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//モデルのレンダリング
	for (i = 0; i<pThing->dwNumMaterials; i++)
	{
		m_pD3DDevice->SetMaterial(&pThing->pMeshMaterials[i]);
		m_pD3DDevice->SetTexture(0, pThing->pMeshTextures[i]);
		pThing->pMesh->DrawSubset(i);
	}
	//　バウンディングスフィアのレンダリング	
	if (boRenderSphere && pThing->pSphereMeshMaterials)
	{
		m_pD3DDevice->SetTexture(0, NULL);
		m_pD3DDevice->SetMaterial(pThing->pSphereMeshMaterials);
		pThing->pSphereMesh->DrawSubset(0);
	}

}

// モデル描画  アニメーション無 コリジョン位置変更
void C3DObj::DrawDX_NormalAdd(D3DXMATRIX mtxWorld, int type, THING_NORMAL* pThing, D3DXVECTOR3 position)
{
	DWORD i;
	//m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	m_pD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	m_pD3DDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);


	// マトリックスのセット
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//モデルのレンダリング
	for (i = 0; i<pThing->dwNumMaterials; i++)
	{
		m_pD3DDevice->SetMaterial(&pThing->pMeshMaterials[i]);
		m_pD3DDevice->SetTexture(0, pThing->pMeshTextures[i]);
		pThing->pMesh->DrawSubset(i);
	}
	D3DXMATRIX mtx;
	D3DXMatrixTranslation(&mtx, mtxWorld._41 + position.x, mtxWorld._42 + position.y, mtxWorld._43 + position.z);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtx);
	//　バウンディングスフィアのレンダリング	
	if (boRenderSphere && pThing->pSphereMeshMaterials)
	{
		m_pD3DDevice->SetTexture(0, NULL);
		m_pD3DDevice->SetMaterial(pThing->pSphereMeshMaterials);
		pThing->pSphereMesh->DrawSubset(0);
	}

}

void C3DObj::DrawDX_NormalAddScale(D3DXMATRIX mtxWorld, int type, THING_NORMAL* pThing, D3DXVECTOR3 position, D3DXVECTOR3 scale)
{
	DWORD i;
	//m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	m_pD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	m_pD3DDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);


	// マトリックスのセット
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//モデルのレンダリング
	for (i = 0; i<pThing->dwNumMaterials; i++)
	{
		m_pD3DDevice->SetMaterial(&pThing->pMeshMaterials[i]);
		m_pD3DDevice->SetTexture(0, pThing->pMeshTextures[i]);
		pThing->pMesh->DrawSubset(i);
	}
	D3DXMATRIX mtxworld;
	D3DXMATRIX mtxScale;
	D3DXMATRIX mtxrotation;
	D3DXMATRIX mtx;
	D3DXMatrixTranslation(&mtx, mtxWorld._41 + position.x, mtxWorld._42 + position.y, mtxWorld._43 + position.z);
	D3DXMatrixRotationX(&mtxrotation, D3DXToRadian(0));
	D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
	mtxworld = mtxScale * mtxrotation * mtx;
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxworld);
	//　バウンディングスフィアのレンダリング	
	if (boRenderSphere && pThing->pSphereMeshMaterials)
	{
		m_pD3DDevice->SetTexture(0, NULL);
		m_pD3DDevice->SetMaterial(pThing->pSphereMeshMaterials);
		pThing->pSphereMesh->DrawSubset(0);
	}

}
void C3DObj::DrawDX_NormalCapsule(D3DXMATRIX mtxWorld, int type, THING_NORMAL* pThing, D3DXVECTOR3 position, float rotation)
{
	//m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	m_pD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	m_pD3DDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);


	// マトリックスのセット
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//モデルのレンダリング
	for (DWORD i = 0; i<pThing->dwNumMaterials; i++)
	{
		m_pD3DDevice->SetMaterial(&pThing->pMeshMaterials[i]);
		m_pD3DDevice->SetTexture(0, pThing->pMeshTextures[i]);
		pThing->pMesh->DrawSubset(i);
	}
	D3DXMATRIX mtxworld;
	D3DXMATRIX mtx;
	D3DXMATRIX mtxRotation;
	D3DXMatrixTranslation(&mtx, mtxWorld._41 + position.x, mtxWorld._42 + position.y, mtxWorld._43 + position.z);
	D3DXMatrixRotationX(&mtxRotation, D3DXToRadian(rotation));
	mtxworld = mtxRotation * mtx;
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxworld);
	//　バウンディングスフィアのレンダリング	
	if (boRenderSphere && pThing->pSphereMeshMaterials)
	{
		m_pD3DDevice->SetTexture(0, NULL);
		m_pD3DDevice->SetMaterial(pThing->pSphereMeshMaterials);
		pThing->pSphereMesh->DrawSubset(0);
	}
}

//=============================================================================
// 当たり判定
//=============================================================================

void C3DObj::HitCheck(void)
{
	//ノーマルモデル対ノーマルモデル
	/*
	if (Collision_NomalVSNormal(&Thing_Normal[0], &Thing_Normal[1]))
	{
		DebugFont_Draw(500, 300, "当たったぞ！！！！！！！！！！！");
	}
	else
	{
		DebugFont_Draw(500, 500, "当たってないぞ！！！！！！！！！！！");
	}*/
	//DebugFont_Draw(200, 50, "エネミーポジション= X= %f Y= %f Z = %f", Thing[0].vPosition.x, Thing[0].vPosition.y, Thing[0].vPosition.z);
	//DebugFont_Draw(200, 80, "プレイヤーポジション= X= %f Y= %f Z = %f", Thing_Normal[0].vPosition.x, Thing_Normal[0].vPosition.y, Thing_Normal[0].vPosition.z);
	//アニメ対ノーマル
	/*if (Collision_AnimeVSNormal(&Thing[0], &Thing_Normal[0]))
	{
		DebugFont_Draw(500, 300, "当たったぞ！！");
	}
	else
	{
		DebugFont_Draw(500, 500, "当たってないぞ！！！！！！！！！！！");
	}*/
	/*
	if (Collision_AnimeVSNormal(&Thing, &Thing_Normal[0]))
	{
	DebugFont_Draw(500, 300, "当たったぞ！！");
	}
	else
	{
	DebugFont_Draw(500, 500, "当たってないぞ！！！！！！！！！！！");
	}*/
	/*
	//アニメ対アニメ
	if (Collision_AnimeVSAnime(&Thing[0], &Thing[0]))
	{
		DebugFont_Draw(500, 300, "当たったぞ！！");
	}
	else
	{
		DebugFont_Draw(500, 500, "当たってないぞ！！！！！！！！！！！");
	}*/
}

bool C3DObj::Collision_NomalVSNormal(THING_NORMAL* pThingA, THING_NORMAL* pThingB)
{
	//２つの物体の中心間の距離を求める
	D3DXVECTOR3 vLength = pThingB->vPosition - pThingA->vPosition;
	FLOAT fLength = D3DXVec3Length(&vLength);
	// その距離が、2物体の半径を足したものより小さいということは、
	//境界球同士が重なっている（衝突している）ということ
	if (fLength <= pThingA->Sphere.fRadius + pThingB->Sphere.fRadius)
	{
		return true;
	}
	return false;
}

bool C3DObj::Collision_AnimeVSNormal(THING* pThingA, THING_NORMAL* pThingB)
{
	//２つの物体の中心間の距離を求める
	D3DXVECTOR3 vLength = pThingB->vPosition - pThingA->vPosition;
	FLOAT fLength = D3DXVec3Length(&vLength);
	// その距離が、2物体の半径を足したものより小さいということは、
	//境界球同士が重なっている（衝突している）ということ
	if (fLength <= pThingA->Sphere.fRadius + pThingB->Sphere.fRadius)
	{
		return true;
	}
	return false;
}

bool C3DObj::Collision_AnimeVSNormalCapsule(THING* pThingA, THING_NORMAL* pThingB)
{
	//２つの物体の中心間の距離を求める
	D3DXVECTOR3 vLength = pThingB->vPosition - pThingA->vPosition;
	FLOAT fLength = D3DXVec3Length(&vLength);
	// その距離が、2物体の半径を足したものより小さいということは、
	//境界球同士が重なっている（衝突している）ということ
	if (fLength <= pThingA->Sphere.fRadius + ((pThingB->Capsule.fRadius + pThingB->Capsule.fRadius2 + pThingB->Capsule.fLength) / 2))
	{
		return true;
	}
	return false;
}

float C3DObj::GetSqDistancePoint2Segment(THING* pThingA, THING_NORMAL* pThingB)
{
	/*
	const float epsilon = 1.0e-5f;	// 誤差吸収用の微小な値
	D3DXVECTOR3 SegmentSub;
	D3DXVECTOR3 SegmentPoint;
	D3DXVECTOR3 CP;

	// 線分の始点から終点へのベクトル
	SegmentSub = pThingB->Capsule.fRadius2 - pThingB->Capsule.fRadius;

	// 線分の始点から点へのベクトル
	SegmentPoint = _point - _segment.start;
	if (SegmentSub.dot(SegmentPoint) < epsilon)
	{// ２ベクトルの内積が負なら、線分の始点が最近傍
		return SegmentPoint.dot(SegmentPoint);
	}

	// 点から線分の終点へのベクトル
	SegmentPoint = _segment.end - _point;
	if (SegmentSub.dot(SegmentPoint) < epsilon)
	{// ２ベクトルの内積が負なら、線分の終点が最近傍
		return SegmentPoint.dot(SegmentPoint);
	}

	// 上記のどちらにも該当しない場合、線分上に落とした射影が最近傍
	// (本来ならサインで求めるが、外積の大きさ/線分のベクトルの大きさで求まる)
	Vec3::cross(SegmentSub, SegmentPoint, &CP);

	return CP.dot(CP) / SegmentSub.dot(SegmentSub);*/
	return 0;
}


bool C3DObj::Collision_AnimeVSAnime(THING* pThingA, THING* pThingB)
{
	//２つの物体の中心間の距離を求める
	D3DXVECTOR3 vLength = pThingB->vPosition - pThingA->vPosition;
	FLOAT fLength = D3DXVec3Length(&vLength);
	// その距離が、2物体の半径を足したものより小さいということは、
	//境界球同士が重なっている（衝突している）ということ
	if (fLength <= pThingA->Sphere.fRadius + pThingB->Sphere.fRadius)
	{
		return true;
	}
	return false;
}

THING* C3DObj::GetAnimeModel(void)
{
	return &Thing;
}

/*THING C3DObj::GetAnimeModel(int index)
{
	return Thing_Anime[index];
}*/

THING_NORMAL C3DObj::GetNormalModel(int index)
{
	return Thing_Normal[index];
}

/*THING_NORMAL C3DObj::GetNormal(int index)
{
	return Thing_Normal[index];
}*/

THING_NORMAL C3DObj::GetNormalModel(void)
{
	return Thing_Normal_model;
}

//=============================================================================
// アニメーション変更
//=============================================================================

void C3DObj::Animation_Change(int index, float speed)
{
	if (TrackDesc.Speed != speed)
	{
		TrackDesc.Speed = speed;//モーションスピード
		//Thing.pAnimController->SetTrackDesc(0, &TrackDesc);//アニメ情報セット
		Thing.pAnimController->SetTrackPosition(0, 0);
		Thing.pAnimController->SetTrackSpeed(0,TrackDesc.Speed);//アニメ情報セット
	}
	if (m_AnimationType != index)
	{
		Thing.pAnimController->SetTrackPosition(0, 0);
		Thing.pAnimController->SetTrackAnimationSet(0, pAnimSet[index]);
		m_AnimationType = index;
	}

}

void C3DObj::DamageFlag_Change(void)
{
	m_DamageFlag = true;
}

void C3DObj::Position_Keep(D3DXMATRIX mtxT)
{
	m_PosKeep = D3DXVECTOR3(mtxT._41, mtxT._42, mtxT._43);
}

void C3DObj::Add_Mp(int mp)
{
	C3DObj *pplayer = CPlayer::Get_Player();
	pplayer->m_Mp += mp;

	while (pplayer->m_Mp >= MP_MAX)
	{
		pplayer->m_Mp -= MP_MAX;
		pplayer->m_MpStock++;
	}

}

void C3DObj::Attraction_Delete(void)
{
	int i;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		if (p3DObj[i])
		{
			if ((p3DObj[i]->m_3DObjType == TYPE_ATTRACTION)|| (p3DObj[i]->m_3DObjType == TYPE_POPCORN)|| (p3DObj[i]->m_3DObjType == TYPE_STADBY)|| (p3DObj[i]->m_3DObjType == TYPE_COASTER))
			{
				delete p3DObj[i];
			}
		}
	}
}

void C3DObj::Add_Hp(void)
{
	C3DObj *pplayer = CPlayer::Get_Player();
	pplayer->m_Hp++;
	Exp_Set(HEAL, pplayer->m_mtxTranslation._41, pplayer->m_mtxTranslation._42 + 1.0f, pplayer->m_mtxTranslation._43, 3.0, 0);
	if (pplayer->m_Hp > HP_MAX)
	{
		pplayer->m_Hp = HP_MAX;
	}
}															

bool C3DObj::VFCulling(D3DXVECTOR3* pPosition)
{
	//視界内：true 外：false 
	D3DXPLANE VFLeftPlane, VFRightPlane, VFTopPlane, VFBottomPlane;
	D3DXVECTOR3 Pos = *pPosition;
	float Radius = 0;
	D3DXMATRIX pmView = CCamera::Get_ViewMtx();
	float Angle = D3DX_PI / VF_ANGLE;
	float NearClip = 0.1f;//この二ついじっても大きさ変わる
	float FarClip = 80.0f;//
	float Aspect = WINDOW_WIDTH / WINDOW_HIGHT;
	//まず、ジオメトリの位置ベクトルをワールドからビュー空間に変換
	D3DXVec3TransformCoord(&Pos, &Pos, &pmView);

	//左右、上下の平面を計算
	{
		D3DXVECTOR3 p1, p2, p3;
		//左面
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = -FarClip * ((FLOAT)tan(Angle / 2)*Aspect);
		p2.y = -FarClip * (FLOAT)tan(Angle / 2);
		p2.z = FarClip;
		p3.x = p2.x;
		p3.y = -p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFLeftPlane, &p1, &p2, &p3);
		//右面
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = FarClip * ((FLOAT)tan(Angle / 2)*Aspect);
		p2.y = FarClip * (FLOAT)tan(Angle / 2);
		p2.z = FarClip;
		p3.x = p2.x;
		p3.y = -p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFRightPlane, &p1, &p2, &p3);
		//上面
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = -FarClip * ((FLOAT)tan(Angle / 2)*Aspect);
		p2.y = FarClip * (FLOAT)tan(Angle / 2);
		p2.z = FarClip;
		p3.x = -p2.x;
		p3.y = p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFTopPlane, &p1, &p2, &p3);
		//下面
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = FarClip * ((FLOAT)tan(Angle / 2)*Aspect);
		p2.y = -FarClip * (FLOAT)tan(Angle / 2);
		p2.z = FarClip;
		p3.x = -p2.x;
		p3.y = p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFBottomPlane, &p1, &p2, &p3);
	}

	//6つの平面とジオメトリ境界球をチェック
	{
		//ニアクリップ面について
		if ((Pos.z + Radius) < NearClip)
		{
			return false;
		}
		//ファークリップ面について
		if ((Pos.z - Radius) > FarClip)
		{
			return false;
		}
		//左クリップ面について
		FLOAT Distance = (Pos.x * VFLeftPlane.a) + (Pos.z * VFLeftPlane.c);
		if (Distance>Radius)
		{
			return false;
		}
		//右クリップ面について
		Distance = (Pos.x * VFRightPlane.a) + (Pos.z * VFRightPlane.c);
		if (Distance>Radius)
		{
			return false;
		}
		//上クリップ面について
		Distance = (Pos.y * VFTopPlane.b) + (Pos.z * VFTopPlane.c);
		if (Distance>Radius)
		{
			return false;
		}
		//下クリップ面について
		Distance = (Pos.y * VFBottomPlane.b) + (Pos.z * VFBottomPlane.c);
		if (Distance>Radius)
		{
			return false;
		}
	}

	return true;
}