#ifndef _SCENE_H_
#define _SCENE_H_




typedef enum SCENE_INDEX 
{
	SCENE_INDEX_TITLE,
	SCENE_INDEX_CHUTO,
	SCENE_INDEX_GAME,
	SCTNE_INDEX_SCORE,
	SCENE_INDEX_RESULT,
	SCENE_INDEX_MAX
}Scene;


void Scene_Initialize(SCENE_INDEX index);
void Scene_Finalize(void);
void Scene_Update(void);
void Scene_Draw(void);

void Scene_check(void);
void Scene_Change(SCENE_INDEX index);


#endif
