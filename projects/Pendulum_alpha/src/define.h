#ifndef DEF_DEFINE_H
#define DEF_DEFINE_H

//���C�u����
#include "../../../lib/gplib.h"
//3D�����O�����C�u����
#include "../../../lib/dx_media/dx_graphics3d.h"
#include "../../../lib/dx_media/dx3dmeshmanager.h"

using namespace gplib;

//3D�J����
extern Dx_Camera *dxCamera;

//���b�V���S�̊Ǘ��N���X
extern DX3DMESHMANAGER *meshManage;

#include "gameManager.h"
//======================================================================================//
//�֐��L�q
//======================================================================================//
enum{ TITLESCREEN, STAGESELECTSCREEN, STAGE1SCREEN, ENDSCREEN, CLEARSCREEN };

//======================================================================================//
//�֐���`
//======================================================================================//
/*
const int STRAIGHT = 0;		//�܂������`��
const int WORDBREAK = 1;	//\n�ŉ��s����B
void Draw_TextToBmp(int x, int y, const std::string& msg, int size, int type = STRAIGHT);
//*/

#endif