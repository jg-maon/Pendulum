#ifndef DE_CHARACTERBASE_H
#define DE_CHARACTERBASE_H

#ifndef DEF_OBJ_H
#include "Obj.h"
#endif

#ifndef DEF_STAGEMNG_H
#include "stageMng.h"
#endif

#ifndef DEF_MYMATH_HPP
#include "myMath.hpp"
#endif

#include <string>
#include <memory>

#define DEF_PREPOS

/*
	@brief	キャラクターオブジェクト基底クラス(プレイヤー、敵、NPC?)
	@attension	step内でICharacter::stepを呼び出す
*/
class ICharacter : public IColObject
{
private:
	static std::weak_ptr<CStageMng> sm_;			// ステージマネージャへの参照
protected:
	int health_;							// 体力
	int power_;								// 攻撃力
#ifdef DEF_PREPOS
	mymath::Vec3f prePos_;					// 1フレーム前の座標
#endif

	bool gravityF_;							// 重力処理フラグ

protected:
	/*
		@brief	ステージマネージャのポインタ取得
		@return	ステージマネージャポインタ
	*/
	std::shared_ptr<CStageMng> sm();

	/*
		@brief	ステージマネージャの設定
		@param	[in]	sm		ステージマネージャ
		@return	なし
	*/
	void sm(const std::shared_ptr<CStageMng>& sm);

public:

	/*
		@brief	オブジェクトの生成
		@param	[in]	name	識別名
	*/
	ICharacter(const std::string& name);
	/*
		@brief	インターフェース化するための純粋仮想関数
	*/
	~ICharacter() = 0;
	/*
		@brief		派生後共通処理
					1フレーム前の座標に現在座標を格納
		@attension	派生後のstep内で呼び出すこと
		@return		なし
	*/
	virtual void step() = 0;
	/*
		@brief	描画処理
		@return	なし
	*/
	virtual void draw() = 0;
	/*
		@brief	オブジェクト同士の重なり処理
		@param	[in]	rival	重なっている相手オブジェクト
		@return	なし
	*/
	virtual void hit(const ObjPtr& rival);

	/*
		@brief	ワールド座標の食らい判定領域の取得
		@return	ワールド座標の当たり判定領域
	*/
	virtual Collisions GetDamageAreas() const;

	/*
		@brief	攻撃力の取得
		@return	補正後の攻撃力
	*/
	virtual int getPower() const;

#ifdef DEF_PREPOS
	/*
		@brief	1フレーム前座標の取得
		@return	1フレーム前の座標
	*/
	const mymath::Vec3f& prePos() const;

#endif

	/*
		@brief	1フレーム後の座標の取得
		@return	1フレーム後の座標
	*/
	mymath::Vec3f nextPos() const;

};

/*
	@brief	ステージマネージャ初期化用キャラ
*/
class TempCharacter : public ICharacter
{
public:
	TempCharacter(const std::shared_ptr<CStageMng>& stageMng) :
		ICharacter("")
	{
		sm(stageMng);
	}
	void step() override{}
	void draw() override{}
};

#endif