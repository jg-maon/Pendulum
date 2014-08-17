
#include "GameManager.h"

#include "Collision.h"


#include	<algorithm>
#include	<functional>
#include	<sstream>
#include	<fstream>

mymath::Recti* CGameManager::winRect_ = nullptr;

CGameManager::CGameManager() :
Base("GameManager")
, showCursor_(false)
//, cursor_.anim(360.f,10.f)
{
	ShowCursor(showCursor_ == true);
	init();


	cursor_.obj = charabase::CharBase(
		mymath::Vec3f(), mymath::Vec3f(),
		"img_cursor0",
		CursorSize::width,
		CursorSize::height);
	cursor_.animTbl.resize(11);
	for (size_t i = 0; i < cursor_.animTbl.size(); ++i)
		cursor_.animTbl[i] = (static_cast<float>(i));
	cursor_.anim.set(10, 0.1f);
	cursor_.animType = Cursor::AnimType::ANIM_X;

	if (!winRect_)
		winRect_ = new mymath::Recti(0, 0, system::WINW, system::WINH);

	fileMng_ = std::shared_ptr<CFileMng>(new CFileMng("res/dat/path.ini"));

}

CGameManager::~CGameManager()
{
	objs_.clear();
	addObjs_.clear();
	if (winRect_)
		delete winRect_;
}

void CGameManager::start()
{
	__super::start();

	fileMng_->Load();
	
	sceneMng_ = std::unique_ptr<CSceneMng>(new CSceneMng());


	if (!GetObj(typeid(CStageMng)))
	{
		auto sm = std::shared_ptr<CStageMng>(new CStageMng());
		AddObject2(sm);
		TempCharacter t(sm);
		sm->load();
#ifdef DEF_GM_PTR
		SetStageMngPtr(sm);
#endif
	}

	if (!GetObj(typeid(CScoreMng)))
	{
		auto sm = std::shared_ptr<CScoreMng>(new CScoreMng());
		AddObject2(sm);
#ifdef DEF_GM_PTR
		SetScoreMngPtr(sm);
#endif
	}

	if (!GetObj(typeid(CCollision)))
		AddObject2(ObjPtr(new CCollision()));

	if (!GetObj(typeid(CGameStatus)))
	{
		auto gs = std::shared_ptr<CGameStatus>(new CGameStatus());
		AddObject2(gs);
#ifdef DEF_GM_PTR
		SetGameStatusPtr(gs);
#endif
	}
		

}

void CGameManager::init()
{
	objs_.clear();
	addObjs_.clear();
	//clear_ = false;
}

void CGameManager::step()
{
	//=======================================================
	// カーソル処理
	if (input::CheckPush(input::KEY_F1))
	{
		showCursor_ ^= 1;
		ShowCursor(showCursor_ == true);
	}
	//-------------------------------------
	// カーソルアニメーション
	if (cursor_.obj.CheckUse())
		cursor_.step();
	//-------------------------------------
	// ウィンドウ外へ出ないように
	mymath::Vec3f& cursorPos = cursor_.obj.pos;
	const float& halfWidth = cursor_.obj.HalfWidth();
	const float& halfHeight = cursor_.obj.HalfHeight();
	cursorPos = camera::GetCursorPosition();
	RECT rt = camera::GetScreenRect();
	if (cursorPos.x - halfWidth < rt.left)
		cursorPos.x = rt.left + halfWidth;
	else if (cursorPos.x + halfWidth > rt.right)
		cursorPos.x = rt.right - halfWidth;
	if (cursorPos.y - halfHeight < rt.top)
		cursorPos.y = rt.top + halfHeight;
	else if (cursorPos.y + halfHeight > rt.bottom)
		cursorPos.y = rt.bottom - halfHeight;

	//=======================================================
	// シーン
	sceneMng_->step();

	//=======================================================
	//各種更新
	for (const auto& obj : objs_)
	{
		const auto& status = obj->getStatus();
		if (status == Status::run || status == Status::update)
			obj->step();
	}

	MargeObjects();


}

void CGameManager::draw()
{
	//-------------------------------------
	// シーン
	sceneMng_->draw();
	//-------------------------------------
	// オブジェクト
	for (const auto& obj : objs_)
	{
		const auto& status = obj->getStatus();
		if (status == Status::run || status == Status::disp)
			obj->draw();
	}
	//-------------------------------------
	// カーソル
	if (cursor_.obj.CheckUse())
		cursor_.obj.draw();
}



void CGameManager::MargeObjects()
{
	objs_.erase(
		std::remove_if(objs_.begin(), objs_.end(),
		std::mem_fn(&Base::isDestroy)),
		objs_.end());

	if (!addObjs_.empty())
	{
		objs_.insert(objs_.end(), addObjs_.begin(), addObjs_.end());
		addObjs_.clear();
	}

}

void CGameManager::AllStop()
{
	// 現在登録中
	for (auto& obj : objs_)
		obj->stop();

	//*
	// 追加予定
	for (auto& obj : addObjs_)
		obj->stop();
	//*/
}

void CGameManager::AddObject(const ObjPtr& obj)
{
	addObjs_.push_back(obj);
}

void CGameManager::AddObject2(const ObjPtr& obj)
{
	addObjs_.push_back(obj);
	//追加要素配列から管理配列へ追加する。
	objs_.insert(objs_.end(), addObjs_.begin(), addObjs_.end());
	//追加配列をクリア
	addObjs_.clear();
}



//指定のクラスを見つけ出して返す
ObjPtr CGameManager::GetObj(const type_info& objinfo) const
{
	for (auto& obj : objs_)
	{
		const type_info& info = typeid(*obj);
		if (info == objinfo)
		{
			return obj;
		}
	}
	//追加前のものもチェック
	for (auto& obj : addObjs_)
	{
		const type_info& info = typeid(*obj);
		if (info == objinfo)
		{
			return obj;
		}
	}
	return nullptr;
}

std::vector<ObjPtr>& CGameManager::GetObjects()
{
	return objs_;
}

std::vector<ObjPtr> CGameManager::GetObjects(const std::string& taskName) const
{
	std::vector<ObjPtr> ret;
	for (const auto& obj : objs_)
	{
		if (!obj->FindName(taskName)) continue;
		ret.push_back(obj);
	}
	// 追加予定のオブジェクトも探査
	for (const auto& obj : addObjs_)
	{
		if (!obj->FindName(taskName)) continue;
		ret.push_back(obj);
	}
	return ret;
}

std::vector<ObjPtr> CGameManager::GetObjects(const std::string& taskName, const char delim) const
{
	std::vector<ObjPtr> ret;
	std::vector<std::string> taskNames;
	// taskNamesの作成
	{
		std::stringstream ss(taskName);
		std::string temp;
		while (std::getline(ss, temp, delim))
		{
			taskNames.push_back(temp);
		}
	}
	for (const auto& obj : objs_)
	{
		for (const auto& taskName : taskNames)
		{
			if (!obj->FindName(taskName)) continue;
			ret.push_back(obj);
		}
	}
	// 追加予定のオブジェクトも探査
	for (const auto& obj : addObjs_)
	{
		for (const auto& taskName : taskNames)
		{
			if (!obj->FindName(taskName)) continue;
			ret.push_back(obj);
		}
	}
	return ret;
}

void CGameManager::ClearObjects()
{
	// 現在登録中
	for (auto& obj : objs_)
		obj->kill();
	/*
	// 追加予定
	for (auto& obj : addObjs_)
	obj->kill();
	//*/
}


/*
void CGameManager::setClear(bool clear)
{
	clear_ = clear;
}
bool CGameManager::getClear() const
{
	return clear_;
}


void CGameManager::StartClear()
{
	setClear(true);
	count_ = 3.f;
}

void CGameManager::ClearToChangeScreen(int next)
{
	if (getClear())
	{
		count_ -= system::ONEFRAME_TIME;
		debug::SToMNC(0, 100, "GameClearCount:%0.2f", count_);
		if (count_ <= 0)
		{
			stage::ChangeStage(next);
		}
	}
}
//*/

const mymath::Recti* CGameManager::winRect() const
{
	return winRect_;
}

void CGameManager::winRect(const mymath::Recti& newRect)
{
	*winRect_ = newRect;
}

void CGameManager::winRect(mymath::Recti* newRect)
{
	winRect_ = newRect;
}

const mymath::Vec3f& CGameManager::GetCursorPos() const
{
	return cursor_.obj.pos;
}

mymath::Vec3f CGameManager::GetCursorPosNC() const
{
	RECT rt = camera::GetScreenRect();
	mymath::Vec3f pos = cursor_.obj.pos;
	pos.x -= rt.left;
	pos.y -= rt.top;
	return pos;
}

const CGameManager::Cursor& CGameManager::cursor() const
{
	return cursor_;
}

void CGameManager::cursor(const CGameManager::Cursor& newCursor)
{
	cursor_ = newCursor;
}

mymath::Vec3f CGameManager::GetPlayerPos() const
{
#ifdef DEF_GM_PTR
	if (pPlayer_.lock())
	{
		// プレイヤーポインタが設定されているならばポインタから座標を取得
		return pPlayer_.lock()->obj().pos;
	}
#endif
	// オブジェクト配列内から探し出す
	mymath::Vec3f v = { -1, -1, -1 };
	for (const auto& s : objs_){
		if (s->FindName("Player")){
			v = std::dynamic_pointer_cast<IObject>(s)->obj().pos;
			return v;
		}
	}
	return v;
}

std::shared_ptr<CFileMng> CGameManager::fm()
{
	return fileMng_;
}

#ifdef DEF_GM_PTR
//*

void CGameManager::SetPlayerPtr(const std::weak_ptr<CPlayer>& player)
{
	pPlayer_ = player;
	//std::swap(pPlayer_, player);
	//pPlayer_.swap(player);
}

void CGameManager::SetEnemyMngPtr(const std::weak_ptr<CEnemyMng>& enemymng)
{
	pEnemyMng_ = enemymng;
}

void CGameManager::SetStageMngPtr(const std::weak_ptr<CStageMng>& stagemng)
{
	pStageMng_ = stagemng;
}

void CGameManager::SetScoreMngPtr(const std::weak_ptr<CScoreMng>& scoremng)
{
	pScoreMng_ = scoremng;
}

void CGameManager::SetGameStatusPtr(const std::weak_ptr<CGameStatus>& gamestatus)
{
	pGameStatus_ = gamestatus;
}


std::shared_ptr<CStageMng> CGameManager::stageMng() const
{
	if (!pStageMng_.lock())
	{
		// ポインタに登録がない場合、オブジェクト群のチェック
		return std::dynamic_pointer_cast<CStageMng>(GetObj(typeid(CStageMng)));
	}
	return pStageMng_.lock();
}

std::shared_ptr<CScoreMng> CGameManager::scoreMng() const
{
	if (!pScoreMng_.lock())
	{
		// ポインタに登録がない場合、オブジェクト群のチェック
		return std::dynamic_pointer_cast<CScoreMng>(GetObj(typeid(CScoreMng)));
	}
	return pScoreMng_.lock();
}

std::shared_ptr<CPlayer> CGameManager::GetPlayer() const
{
	if (!pPlayer_.lock())
	{
		// ポインタに登録がない場合、オブジェクト群のチェック
		return std::dynamic_pointer_cast<CPlayer>(GetObj(typeid(CPlayer)));
	}
	return pPlayer_.lock();
}

std::shared_ptr<CEnemyMng> CGameManager::enemyMng() const
{
	if (!pEnemyMng_.lock())
	{
		// ポインタに登録がない場合、オブジェクト群のチェック
		return std::dynamic_pointer_cast<CEnemyMng>(GetObj(typeid(CEnemyMng)));
	}
	return pEnemyMng_.lock();
}

std::shared_ptr<CGameStatus> CGameManager::gameStatus() const
{
	if (!pGameStatus_.lock())
	{
		// ポインタに登録がない場合、オブジェクト群のチェック
		return std::dynamic_pointer_cast<CGameStatus>(GetObj(typeid(CGameStatus)));
	}
	return pGameStatus_.lock();
}

//*/
#endif

/*
void InsertObject(const ObjPtr& obj)
{
extern CGameManager* gm;
gm->AddObject(obj);
}
//*/