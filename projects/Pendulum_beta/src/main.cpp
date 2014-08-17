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
		int			KeyboardMode = 1;	//pad disable:1 pad enable:0
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
//ゲームループ 
//---------------------------------------------------------------------------------------
void system::GameLoop()
{
	main::Step();
	main::Disp();

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
		TempObject to(gm);
	}
	gm->start();

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

//======================================================================================//
//WINDOW　メイン
//======================================================================================//
int WINAPI WinMain (HINSTANCE hInstance,HINSTANCE hPreInst,LPSTR lpszCmdLine,int nCmdShow)
{
	return system::DoWindow(hInstance,hPreInst,lpszCmdLine,nCmdShow);
}
