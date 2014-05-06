#ifndef DEF_DEFINE_H
#define DEF_DEFINE_H

//ライブラリ
#include "../../../lib/gplib.h"
//3D処理外部ライブラリ
#include "../../../lib/dx_media/dx_graphics3d.h"
#include "../../../lib/dx_media/dx3dmeshmanager.h"

using namespace gplib;

//3Dカメラ
extern Dx_Camera *dxCamera;

//メッシュ全体管理クラス
extern DX3DMESHMANAGER *meshManage;

#include "gameManager.h"
//======================================================================================//
//関数記述
//======================================================================================//
enum{ TITLESCREEN, STAGESELECTSCREEN, STAGE1SCREEN, ENDSCREEN, CLEARSCREEN };

//======================================================================================//
//関数定義
//======================================================================================//
const int STRAIGHT = 0;		//まっすぐ描画
const int WORDBREAK = 1;	//\nで改行する。
void Draw_TextToBmp(int x, int y, const std::string& msg, int size, int type = STRAIGHT);


#endif