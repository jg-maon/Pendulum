//**************************************************************************************//
//
//　main.cpp
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


//3Dカメラ
Dx_Camera *dxCamera = nullptr;

//メッシュ全体管理クラス
DX3DMESHMANAGER *meshManage = nullptr;


//ゲーム管理
CGameManager *gm = nullptr;

// シーン
std::unique_ptr<IScene> scene_;


//**************************************************************************************//
//ライブラリ側で必要な変数　
//必要に応じて変更すること
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
		int			KeyboardMode = 0;	//pad disable:1 pad enable:0
	}
}


void SceneStep()
{
	gm->step();
	IScene *next(scene_->step());
	if (next != scene_.get())
	{
		scene_.reset(next);
	}
}

void SceneDisp()
{
	gm->draw();
	scene_->draw();
	CFade::draw();
	debug::DFPS();
}
//---------------------------------------------------------------------------------------
//ゲームループ 
//---------------------------------------------------------------------------------------
void system::GameLoop()
{
	SceneStep();
	SceneDisp();
}

//---------------------------------------------------------------------------------------
//プログラム全体の初期化
//フォントの作成、ローディング画面で使用する画像の読み込みなど
//起動時に一回だけ行う処理はここで行う
//---------------------------------------------------------------------------------------
void InitGame()
{
	//2Dカメラの注視点
	camera::InitCamera(system::WINW / 2, system::WINH / 2, 0.0f, 0.0f);
	//3Dカメラ設定　カメラ位置はデフォルト　注視点は0,0,0
	dxCamera = new Dx_Camera();
	//

	////ゲーム管理の初期化
	gm = new CGameManager();
	{
		TempObject t(gm);
	}


	// シーン初期化
	scene_.reset(new CSceneTitle());

}

void TerminateGame()
{

	SAFE_DELETE(meshManage);
	//カメラの破棄
	SAFE_DELETE(dxCamera);

	//ゲーム管理消去
	SAFE_DELETE(gm);
}
void system::OnCreate()
{
	/*
	font::Draw_CreateFont(0, 30, "MS　ゴシック");
	font::Draw_CreateFont(1, 20, "MS　ゴシック");
	font::Draw_CreateFont(2, 10, "MS　ゴシック");

	//画像のロード
	graph::Draw_LoadObject("player", "res/gra/Act_Chara1.png");
	graph::Draw_LoadObject("mapchip", "res/gra/block.png");
	graph::Draw_LoadObject("back", "res/gra/cloud.png");
	graph::Draw_LoadObject("enemy", "res/gra/enemy.png");
	graph::Draw_LoadObject("effect", "res/gra/newEffect.png");
	
	graph::Draw_LoadObject("shot", "res/gra/shot.png");
	graph::Draw_LoadObject("effect", "res/gra/effect.png");
	graph::Draw_LoadObject("enemy2", "res/gra/enemy2.png");
	graph::Draw_LoadObject("boss", "res/gra/boss.png");
	graph::Draw_LoadObject("bomb", "res/gra/bomb.png");
	graph::Draw_LoadObject("enemyshot", "res/gra/enemyshot.png");
	//効果音のロード
	se::DSound_LoadFile("se1", "res/snd/se1.wav");
	//BGMのロード
	bgm::DShow_LoadFile("bgm1", "res/snd/bgm1.mp3");
	*/
	setting::OnCreateSetup();
	InitGame();
}

//---------------------------------------------------------------------------------------
//プログラム全体の後始末
//終了時に一回だけ行う処理はここで行う
//---------------------------------------------------------------------------------------
void system::OnDestroy()
{
	TerminateGame();
}

//----------------------
//文字表示
void Draw_TextToBmp(int x, int y, const string& msg, int size, int type)
{
	int i;
	//フォントデータの横並び数
	const int GRAPHIC_CELL_LENGTH = 16;
	//フォントデータの元サイズ
	const int GRAPHIC_SIZEXY = 8;
	int draw_x, draw_y;
	int font_x, font_y;
	//初期描画位置作成
	draw_x = x; draw_y = y;
	if (size <= 0)	size = 1;
	for (i = 0; i < (int)msg.length(); i++){
		//描画文字決定
		font_x = (int)msg[i] % GRAPHIC_CELL_LENGTH * GRAPHIC_SIZEXY;
		font_y = (int)msg[i] / GRAPHIC_CELL_LENGTH * GRAPHIC_SIZEXY;
		//文字描画
		graph::Draw_Graphics(draw_x, draw_y, 0.5f, "font",
			font_x, font_y, GRAPHIC_SIZEXY, GRAPHIC_SIZEXY,
			0, 0, (float)size, (float)size);
		//文字描画位置更新
		if (type == WORDBREAK && msg[i] == '\n'){
			draw_y += GRAPHIC_SIZEXY * size;
			draw_x = x;
		}
		else{
			draw_x += GRAPHIC_SIZEXY * size;
		}
	}
}
//======================================================================================//
//WINDOW　メイン
//======================================================================================//
int WINAPI WinMain (HINSTANCE hInstance,HINSTANCE hPreInst,LPSTR lpszCmdLine,int nCmdShow)
{
	return system::DoWindow(hInstance,hPreInst,lpszCmdLine,nCmdShow);
}
