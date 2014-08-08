#ifndef DEF_GAMESTATUS_H
#define DEF_GAMESTATUS_H

#ifndef DEF_OBJ_H
#include "obj.h"
#endif

/*
	@brief	メインゲームの状態を管理
*/
class CGameStatus : public Base
{
private:




public:
	CGameStatus();
	~CGameStatus();
	
	/*
		@brief	更新処理
		@return	なし
	*/
	virtual void step() override;
	/*
		@brief	描画処理
		@return	なし
	*/
	virtual void draw() override;

	/*
		@brief	ヒットストップ開始
		@return	なし
	*/
	void StartHitStop();

};

CGameStatus::CGameStatus():
Base("GameStatus")
{
}

CGameStatus::~CGameStatus()
{
}

void CGameStatus::step()
{

}

void CGameStatus::draw()
{

}

void CGameStatus::StartHitStop()
{

}


#endif