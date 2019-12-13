#pragma once
//////////////////////////
//ŠÛ‰e•`‰æŠÖ”
/////////////////////////
#ifndef SHADOW_H_
#define SHADOW_H_
#include <d3dx9.h>
#include <d3d9.h>
#include <windows.h>
void Shadow_Initialize(void);
void Shadow_Create(float x, float y, float scaleX, float scaleY, int shadow_number);//‰e‚ğÁ‚·‚Æ‚«‚É•K—v‚ÈŠi”[”Ô†‚ğó‚¯æ‚é‚½‚ß‚Éˆø”‚Å“n‚·
void Shadow_Finalize(void);
void Shadow_Update(void);
void Shadow_Draw(void);
void Shadow_Destroy(int number);//w’è‚µ‚½”Ô†‚Ì‰e‚ğÁ‹
#endif //SHADOW_H_