#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

void BillBoard_Initialize(void);
void BillBoard_Finalize(void);
void BillBoard_Updata(void);
void BillBoard_Draw(void);
void BillBoard_Exp_Draw(float x, float y, float z, int texture_index, int anime_pattan, float size, float rotation, int tx, int ty, int width_patan, int counter, int animetime);
void BillBoard_Draw(D3DXMATRIX mtx);
void BillBoard_Draw(float x, float y, float z);
#endif //_CAMERA_H_

