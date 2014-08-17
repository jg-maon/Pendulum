#ifndef DEF_ENEMYBASE_HPP
#define DEF_ENEMYBASE_HPP

#ifndef DE_CHARACTERBASE_H
#include "characterBase.h"
#endif

#ifndef DEF_ATTACKBASE_HPP
#include "attackBase.hpp"
#endif

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp
#endif

#ifndef DEF_NWAYSHOT_H
#include "nWayShot.h"
#endif

#ifndef DEF_NWAYSING_H
#include "nWaySing.h"
#endif

#ifndef DEF_TACKLE_H
#include "tackle.h"
#endif

#ifndef DEF_EFFECTEXPLOSION_H
#include "effectExplosion.h"
#endif

#ifndef DEF_PICKUPJEWELY_H
#include "pickupJewely.h"
#endif

#include <memory>

/*
	@brief		敵基本クラス
	@attention	名前は必ず"E_"から始めること
*/
class IEnemy : public ICharacter
{
protected:
	AttackPtr attack_;							// 攻撃手段
protected:
	
public:
	/*	
		@brief	オブジェクト生成
		@param	[in]	name	名前("E_"から始める)
		@attention	nameは"E_"から始めること
	*/
	IEnemy(const std::string& name) :
		ICharacter(name)
		, attack_(nullptr)
	{}

	/*
		@brief	インターフェース化するための純粋仮想関数
	*/
	virtual ~IEnemy() = 0{}
	/*
		@brief	更新処理
		@return	なし
	*/
	virtual void step() = 0{ __super::step(); }
	/*
		@brief	描画処理
		@return	なし
	*/
	virtual void draw() = 0{ __super::draw(); }
	/*
		@brief	ダメージ加算
		@param	[in]	dam	ダメージ量
		@return 死亡したか
		@retval	true	死亡
		@retval	false	残存
	*/
	virtual bool ApplyDamage(int dam) = 0{ return true; }

	/*
		@brief	ワールド座標の食らい判定領域の取得
		@return	ワールド座標の当たり判定領域
	*/
	virtual Collisions GetDamageAreas() const { return __super::GetDamageAreas(); }

	/*
		@brief	攻撃の当たり判定領域の取得
		@return	攻撃の当たり判定領域
	*/
	virtual Collisions GetAttackAreas() const override { return attack_->GetDamageAreas(); }

	/*
		@brief	攻撃手段情報の格納
		@attension	ifstreamのcurrentPositionに注意
					if (FindChunk(f,"#Attack"))
						LoadAttack(f);
		@param	[in/out]	f	オープン済みファイル
		@return	読み込み成功したか
		@retval	true	読み込み成功
		@retval	false	読み込み失敗
	*/
	bool LoadAttack(std::ifstream& f)
	{
		std::string buf;
		f >> buf;
		if (buf == "NWayShot")
		{
			f >> buf;
			if (buf != "{") return false;
			else
			{
				charabase::CharBase shot_info;
				f >> buf;	// Imgラベル読み飛ばし
				f >> shot_info.resname;

				f >> buf;	// Widthラベル読み飛ばし
				LoadValue(f, obj_, shot_info.size.x);
				f >> buf;	// Heightラベル読み飛ばし
				LoadValue(f, obj_, shot_info.size.y);

				CShot shot(shot_info);
				f >> buf;	// 当たり判定ラベル読み飛ばし
				shot.LoadCollisions(f);
				f >> buf;	// ステージ当たり判定ラベル読み飛ばし
				shot.LoadStageCollisions(f);
				attack_ = AttackPtr(new CNWayShot(shot));
			}
		}
		else if (buf == "NWaySing")
		{
			f >> buf;
			if (buf != "{") return false;
			else
			{
				charabase::CharBase sing_info;
				f >> buf;	// Imgラベル読み飛ばし
				f >> sing_info.resname;

				f >> buf;	// Widthラベル読み飛ばし
				LoadValue(f, obj_, sing_info.size.x);
				f >> buf;	// Heightラベル読み飛ばし
				LoadValue(f, obj_, sing_info.size.y);

				CSing sing(sing_info);
				f >> buf;	// 当たり判定ラベル読み飛ばし
				sing.LoadCollisions(f);
				attack_ = AttackPtr(new CNWaySing(sing));
			}

		}
		else if (buf == "Tacke")
		{
			f >> buf;
			if (buf != "{") return false;
			else
			{
				attack_ = AttackPtr(new CTackle());
			}
		}
		
		return true;
	}

	/*
		@brief	攻撃手段の取得
		@return	攻撃手段
	*/
	const AttackPtr& getAttack() const{ return attack_; }

};
typedef std::shared_ptr<IEnemy> EnemyPtr;

#endif