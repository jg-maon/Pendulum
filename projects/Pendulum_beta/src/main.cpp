//**************************************************************************************//
//
//�@main.cpp
//
//**************************************************************************************//
#include	"define.h"

#ifndef DEF_OBJ_H
#include	"obj.h"
#endif

#ifndef DEF_CHARBASE_H
#include	"charBase.h"
#endif

#ifndef DEF_GAMEOVER_H
#include	"gameover.h"
#endif

#ifndef DEF_COMMON_H
#include	"common.h"
#endif
#ifndef DEF_SETTING_H
#include	"setting.h"
#endif

#ifndef DEF_SCENETITLE_H
#include	"sceneTitle.h"
#endif

#ifndef DEF_FADE_H
#include	"fade.h"
#endif

#include	<memory>


//3D�J����
Dx_Camera *dxCamera = nullptr;

//���b�V���S�̊Ǘ��N���X
DX3DMESHMANAGER *meshManage = nullptr;


//�Q�[���Ǘ�
CGameManager *gm = nullptr;




//**************************************************************************************//
//���C�u�������ŕK�v�ȕϐ��@
//�K�v�ɉ����ĕύX���邱��
//**************************************************************************************//
namespace gplib{
	namespace system{
		const char USERNAME[] = "Pendulum";
		const int	WINW = 1280;
		const int	WINH = 720;
		//const int	WINW = 1024;
		//const int	WINH = 576;
		//const int	WINW	=	640;
		//const int	WINH	=	480;
		bool		WindowMode = true;
		int			KeyboardMode = 1;	//pad disable:1 pad enable:0
#ifdef _DEBUG
		HINSTANCE hInstance;
		HINSTANCE hPreInst;
		LPSTR lpszCmdLine;
		int nCmdShow;
#endif
	}
}

namespace main
{

	void Step()
	{
		gm->step();
	}

	void Disp()
	{
		gm->draw();
		CFade::draw();
		debug::DFPS();
	}
}
//---------------------------------------------------------------------------------------
//�Q�[�����[�v 
//---------------------------------------------------------------------------------------
void system::GameLoop()
{
	main::Step();
	main::Disp();

}

//---------------------------------------------------------------------------------------
//�v���O�����S�̂̏�����
//�t�H���g�̍쐬�A���[�f�B���O��ʂŎg�p����摜�̓ǂݍ��݂Ȃ�
//�N�����Ɉ�񂾂��s�������͂����ōs��
//---------------------------------------------------------------------------------------
void InitGame()
{
	//2D�J�����̒����_
	camera::InitCamera(system::WINW / 2, system::WINH / 2, 0.0f, 0.0f);
	//3D�J�����ݒ�@�J�����ʒu�̓f�t�H���g�@�����_��0,0,0
	dxCamera = new Dx_Camera();
	//

	////�Q�[���Ǘ��̏�����
	gm = new CGameManager();
	{
		TempObject to(gm);
	}
	gm->start();

}

void TerminateGame()
{

	SAFE_DELETE(meshManage);
	//�J�����̔j��
	SAFE_DELETE(dxCamera);

	//�Q�[���Ǘ�����
	SAFE_DELETE(gm);
}
void system::OnCreate()
{
	setting::OnCreateSetup();
	InitGame();
}

//---------------------------------------------------------------------------------------
//�v���O�����S�̂̌�n��
//�I�����Ɉ�񂾂��s�������͂����ōs��
//---------------------------------------------------------------------------------------
void system::OnDestroy()
{
	TerminateGame();
}

/*
//----------------------
//�����\��
void Draw_TextToBmp(int x, int y, const string& msg, int size, int type)
{
	int i;
	//�t�H���g�f�[�^�̉����ѐ�
	const int GRAPHIC_CELL_LENGTH = 16;
	//�t�H���g�f�[�^�̌��T�C�Y
	const int GRAPHIC_SIZEXY = 8;
	int draw_x, draw_y;
	int font_x, font_y;
	//�����`��ʒu�쐬
	draw_x = x; draw_y = y;
	if (size <= 0)	size = 1;
	for (i = 0; i < (int)msg.length(); i++){
		//�`�敶������
		font_x = (int)msg[i] % GRAPHIC_CELL_LENGTH * GRAPHIC_SIZEXY;
		font_y = (int)msg[i] / GRAPHIC_CELL_LENGTH * GRAPHIC_SIZEXY;
		//�����`��
		graph::Draw_Graphics(draw_x, draw_y, 0.5f, "font",
			font_x, font_y, GRAPHIC_SIZEXY, GRAPHIC_SIZEXY,
			0, 0, (float)size, (float)size);
		//�����`��ʒu�X�V
		if (type == WORDBREAK && msg[i] == '\n'){
			draw_y += GRAPHIC_SIZEXY * size;
			draw_x = x;
		}
		else{
			draw_x += GRAPHIC_SIZEXY * size;
		}
	}
}
//*/
//======================================================================================//
//WINDOW�@���C��
//======================================================================================//
int WINAPI WinMain (HINSTANCE hInstance,HINSTANCE hPreInst,LPSTR lpszCmdLine,int nCmdShow)
{
#ifdef _DEBUG
	system::hInstance = hInstance;
	system::hPreInst = hPreInst;
	system::lpszCmdLine = lpszCmdLine;
	system::nCmdShow = nCmdShow;
#endif
	return system::DoWindow(hInstance,hPreInst,lpszCmdLine,nCmdShow);
}
