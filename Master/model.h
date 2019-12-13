#ifndef _MODEL_H_
#define _MODEL_H_



void Model_Load(int index);		//	複数化したらindexを戻す関数にする
void Model_Draw(int index);		//	複数化したら引数を変える　行列
void Model_Finalize(void);	//	モデルデータの開放　複数化したら全部消すかどれかを消す
void Model_Updata(int index);
void Model_Draw(int index, D3DXMATRIX mtxWorld);
void Model_Finalize(int index);

D3DXMATRIX get_modelPos(int index);

#endif //_MODEL_H_