//////////////////////////////////////////////////
////
////	スカイドームクラス
////
//////////////////////////////////////////////////


#ifndef _CMESH_SKYDOME_H_
#define _CMESH_SKYDOME_H_

#include "CMeshField.h"


class CMesh_SkyDome :public CMeshField
{
public:

	CMesh_SkyDome();
	CMesh_SkyDome(int TexIndex,float meshH, float radius, int meshXnum, int meshYnum);
	~CMesh_SkyDome();


	void Update(void);
	void Draw(void);
	void Finalize(void);	//	終了処理
	static CMesh_SkyDome *Mesh_SkyDome_Create(int TexIndex,float meshH, float radius, int meshXnum, int meshYnum);

protected:

private:
	float m_meshH;		//	メッシュ高さ幅
	float m_radius;		//	半径
	void Mesh_SkyDome_Initialize(void);
	static CMesh_SkyDome *m_pMesh_SkyDome;

	
};
#endif // !1



