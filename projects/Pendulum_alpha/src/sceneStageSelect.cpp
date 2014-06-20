#include "sceneStageSelect.h"
#include "define.h"

#include "sceneMain.h"
#include "Fade.h"

#include "common.h"

#include "stageMng.h"

#include <iomanip>
#include <sstream>


//======================================
// CSceneStageSelect methods
#pragma region CSceneStageSelect methods
// コンストラクタ
CSceneStageSelect::CSceneStageSelect() :
IScene("img_stageSelect", "bgm_stageSelect")
{
	std::string settingFile = gm()->fm().GetFile("#StageSelectFile");
	std::ifstream f(settingFile);
	back_ = charabase::CharPtr(new charabase::CharBase());
	back_->resname = BACK_RESNAME;
	back_->pos.z = 1.f;
	if (f.fail())
	{
		debug::BToM("CSceneStageSelect::CSceneStageSelect path:%s", settingFile.c_str());
		fieldRect_.top = fieldRect_.left = 0;
		fieldRect_.right = system::WINW;
		fieldRect_.bottom = system::WINH;
	}
	else
	{
		LoadInfo(f);
	}
	descHeight_ = 0.f;

	//gm()->init();
	//InsertObject(ObjPtr(new CStageMng()));
	//InsertObject(ObjPtr(new CScoreMng()));
}
CSceneStageSelect::~CSceneStageSelect()
{
}

// 描画
void CSceneStageSelect::draw()
{
#ifdef _DEBUG
	const auto& mouse = gm()->GetCursorPos();
	std::stringstream ss;
	ss << "(" << std::setw(4) << (int)mouse.x
		<< "," << std::setw(4) << (int)mouse.y
		<< ")";
	font::Draw_FontText(
		(int)mouse.x,
		(int)mouse.y,
		0.f,
		ss.str(),
		-1,0);
#endif

	// 背景
	back_->draw(charabase::CharBase::MODE::LeftTop);
	// ステージ
	for (auto& stage : stages_)
	{
		// ステージ画像
		stage.first->draw();
		// ステージ説明文
		if (stage.second->CheckUse())
		{
			const auto& obj = stage.second;
			charabase::CharBase tmp(*obj);
			tmp.pos -= obj->HalfSize();
			tmp.size.y = static_cast<int>(obj->size.y * descHeight_);
			tmp.draw(charabase::CharBase::MODE::LeftTop);
		}
	}

}


// 処理
bool CSceneStageSelect::update()
{
	const mymath::Vec3f& mouse = gm()->GetCursorPos();
	POINT mouseNC = camera::GetCursorPositionNC();

	mymath::Vec3f lookAt = camera::GetLookAt();
	
	int halfWINW = system::WINW / 2;
	int halfWINH = system::WINH / 2;
	
	// カメラ移動速度
	const float cameraMoveSp = static_cast<float>(fieldRect_.right - fieldRect_.left) / 1.5f;

	// カメラ移動開始ライン(ウィンドウからの距離)
	const POINT border = { 100, 100 };
	if (mouseNC.x < border.x)
	{
		// ←
		float next = lookAt.x - (cameraMoveSp * system::FrameTime);
		lookAt.x = max(fieldRect_.left + halfWINW, next);

	}
	else if (mouseNC.x > system::WINW - border.x)
	{
		// →
		float next = lookAt.x + (cameraMoveSp * system::FrameTime);
		lookAt.x = min(fieldRect_.right - halfWINW, next);
	}
	if (mouseNC.y < border.y)
	{
		// ↑
		float next = lookAt.y - (cameraMoveSp * system::FrameTime);
		lookAt.y = max(fieldRect_.top + halfWINH, next);
	}
	else if (mouseNC.y > system::WINH - border.y)
	{
		// ↓
		float next = lookAt.y + (cameraMoveSp * system::FrameTime);
		lookAt.y = min(fieldRect_.bottom - halfWINH, next);
	}
	camera::SetLookAt(lookAt.x, lookAt.y);

	// ステージ説明、ステージ開始
	const float descSp = 1.f / 0.5f;		// 表示アニメーション速度
	int i = 0;
	bool noHover = true;	// ステージ画像上にカーソルがない
	for (auto& stage : stages_)
	{
		auto& rt = stage.first->GetRect();
		if (rt.Contains(mouse))
		{
			noHover = false;
			// ステージ説明表示
			stage.second->SetUse(true);
			//=========================================================
			// ステージ説明アニメーション
			if (descHeight_ < 1.f)
			{
				descHeight_ += descSp*system::FrameTime;
			}
			else
			{
				descHeight_ = 1.f;
			}
			//=========================================================
			// カメラとの位置関係により表示位置の変更
			stage.second->pos = stage.first->pos;
			// ステージをNC座標に変換
			mymath::Vec3f NCPos = stage.first->pos - camera::GetCameraPos();
			//-----------------------------------------
			// 説明文X位置
			if (NCPos.x < halfWINW - stage.first->HalfWidth())
				// 画面中央より左にある場合、説明文は右へ表示
				stage.second->pos.x += (stage.first->HalfWidth()+ stage.second->HalfWidth() + 10.f);
			else
				// 画面中央より右にある場合、説明文は左へ表示
				stage.second->pos.x -= (stage.first->HalfWidth() + stage.second->HalfWidth() + 10.f);
			//-----------------------------------------
			// 説明文Y位置
			if (NCPos.y < halfWINH - stage.first->HalfHeight())
				// 画面中央より上にある場合、説明文は下へ表示
				stage.second->pos.y += (stage.first->HalfHeight() + stage.second->HalfHeight() + 10.f);
			else if (NCPos.y > halfWINH + stage.first->HalfHeight())
				// 画面中央より下にある場合、説明文は上へ表示
				stage.second->pos.y -= (stage.first->HalfHeight() + stage.second->HalfHeight() + 10.f);
			else
				// 補正圏内の場合はずらさない
				stage.second->pos.y;
			//=========================================================
			// ステージ選択
			if (input::CheckPush(input::KEY_MOUSE_LBTN))
			{
				CFade::ChangeColor(255, 255, 255);
				//stageName_ = common::StrReplace(stage.first->resname, "img_ss", "stage");

				std::stringstream ss;
				ss << "Stage" << std::setw(2) << std::setfill('0') << (i + 1);
				stageName_ = ss.str();
				return true;
			}
		}
		else
		{
			stage.second->SetUse(false);
		}
		++i;
	}
	if (noHover)
		descHeight_ = 0.f;

	return false;
}

IScene* CSceneStageSelect::NextScene()
{
	const auto& sm = gm()->GetObjects("StageMng");
	if (!sm.empty())
	{
		std::dynamic_pointer_cast<CStageMng>(sm[0])->LoadStage(stageName_);
	}
	else
	{
		ObjPtr stage(new CStageMng());
		InsertObject(stage);
		std::dynamic_pointer_cast<CStageMng>(stage)->LoadStage(stageName_);
	}
	return new CSceneMain();
}



void CSceneStageSelect::LoadInfo(std::ifstream& f)
{
	//====================================================-
	// 背景画像
	//-------------------------------------
	// 画像幅
	if (common::FindChunk(f, "#BGWidth"))
	{
		f >> back_->size.x;
	}
	//-------------------------------------
	// 画像高さ
	if (common::FindChunk(common::SeekSet(f), "#BGHeight"))
	{
		f >> back_->size.y;
	}
	//-------------------------------------
	// 描画始点X
	if (common::FindChunk(common::SeekSet(f), "#BGLeft"))
	{
		f >> back_->pos.x;
	}
	//-------------------------------------
	// 描画始点Y
	if (common::FindChunk(common::SeekSet(f), "#BGTop"))
	{
		f >> back_->pos.y;
	}
	//====================================================-
	// フィールド
	//-------------------------------------
	// フィールド左端
	if (common::FindChunk(common::SeekSet(f), "#Left"))
	{
		f >> fieldRect_.left;
	}
	//-------------------------------------
	// フィールド上端
	if (common::FindChunk(common::SeekSet(f), "#Top"))
	{
		f >> fieldRect_.top;
	}
	//-------------------------------------
	// フィールド右端
	if (common::FindChunk(common::SeekSet(f), "#Right"))
	{
		f >> fieldRect_.right;
	}
	//-------------------------------------
	// フィールド下端
	if (common::FindChunk(common::SeekSet(f), "#Bottom"))
	{
		f >> fieldRect_.bottom;
	}
	//-------------------------------------
	// カメラ初期注視点
	if (common::FindChunk(common::SeekSet(f), "#StartCameraPos"))
	{
		float x, y;
		f >> x >> y;
		camera::SetLookAt(x, y);
	}
	//-------------------------------------
	// ステージ一覧
	LoadStages(f);


	//====================================================-
	// 背景画像2(始点と幅高さを初期化しておくことが条件)
	//-------------------------------------
	// 背景画像終点X
	if (common::FindChunk(common::SeekSet(f), "#BGRight"))
	{
		int right;
		f >> right;
		back_->scale.x = (static_cast<float>(right)-back_->pos.x) / static_cast<float>(back_->size.x);
	}
	//-------------------------------------
	// 背景画像終点Y
	if (common::FindChunk(common::SeekSet(f), "#BGBottom"))
	{
		int bottom;
		f >> bottom;
		back_->scale.y = (static_cast<float>(bottom)-back_->pos.y) / static_cast<float>(back_->size.y);
	}
}

void CSceneStageSelect::LoadStages(std::ifstream& f)
{
	for (int i = 1; i <= 4; ++i)
	{
		std::stringstream ss;
		ss << "#Stage" << std::setw(2) << std::setfill('0') << i;
		// ステージラベル
		if (common::FindChunk(common::SeekSet(f), ss.str()))
		{
			//---------------------------------
			// ステージ
			charabase::CharPtr stage = charabase::CharPtr(new charabase::CharBase());
			// 座標XY
			f >> stage->pos.x >> stage->pos.y;
			// ステージ画像管理名
			f >> stage->resname;
			// 画像サイズWH
			f >> stage->size.x >> stage->size.y;
			//---------------------------------
			// 説明
			charabase::CharPtr desc = charabase::CharPtr(new charabase::CharBase());
			// ステージ説明画像管理名
			f >> desc->resname;
			// 画像サイズWH
			f >> desc->size.x >> desc->size.y;

			stage->pos.z = 0.5f;
			desc->pos.z = 0.4f;

			stages_.push_back(std::make_pair(stage, desc));
		}
	}
}



#pragma endregion CSceneStageSelect methods
// CSceneStageSelect methods

