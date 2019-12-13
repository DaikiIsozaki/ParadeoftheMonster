//////////////////////////////////////////////////
////
////	メッシュフィールドクラス
////
//////////////////////////////////////////////////

#ifndef _CMESHFIELD_H_
#define _CMESHFIELD_H_

#include "CTexture.h"

#define MAX_MESH_FIELD (5000)

class CMeshField :public CTexture
{
public:

	CMeshField();
	CMeshField(int TexIndex, float meshW, int meshXnum, int meshZnum);
	~CMeshField();


	void Update(void);
	void Draw(void);
	void Finalize(void);	//	終了処理
	static CMeshField *MeshField_Create(int TexIndex, float meshW, int meshXnum, int meshZnum);

protected:
	//メッシュフィールド頂点　構造体
	typedef struct
	{
		D3DXVECTOR3 pos;	//	座標x,y,z
		D3DCOLOR color;		//	カラー
		D3DXVECTOR2 uv;		//  テクスチャ

	}MeshFieldVertex;

	#define FVF_MESH_FIELD_VERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)	//	構造体のすぐ下に書く

	MeshFieldVertex m_Mfield[MAX_MESH_FIELD];	//	構造体宣言

	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;	//	バーテックスバッファ
	LPDIRECT3DINDEXBUFFER9 m_pIndexBuffer;		//	インデックスバッファ

	int m_VertexCount;		//	頂点数
	int m_IndexCount;		//	インデックス数
	int m_PrimitiveCount;	//	プリミティブ数
	int m_VertexW;			//	横軸の頂点数
	int m_VertexH;			//	縦軸の頂点数
	int m_meshWnum;			//	横軸のメッシュ数
	int m_meshHnum;			//	縦軸のメッシュ数
	int m_TexIndex;			//	テクスチャインデックス

	void Buffer_Write(void);	//	バッファ書き込み

	void DrawSky(D3DXMATRIX mtxw);
	D3DXMATRIX m_mtxWorld;
private:

	float m_meshW;		//	メッシュ幅
	void Mesh_Field_Initialize(void);	//	メッシュフィールド初期化(頂点設定)
	void Mesh_Field_Initialize2(void);	//	メッシュフィールド初期化(頂点設定)UV値変更

	static CMeshField *m_pMeshFielde;
};
#endif // !1
