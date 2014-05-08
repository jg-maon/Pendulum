#ifndef DEF_ENEMYBASE_HPP
#define DEF_ENEMYBASE_HPP

#ifndef DEF_CHARAOBJBASE_HPP
#include "charaObjBase.hpp"
#endif

#ifndef DEF_ATTACKBASE_HPP
#include "attackBase.hpp"
#endif

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp
#endif

#ifndef DEF_DEFINE_H
#include "define.h"
#endif

#ifndef DEF_NWAYSHOT_T
#include "nWayShot.h"
#endif

/*
	@brief		敵基本クラス
	@attention	名前は必ず"E_"から始めること
*/
class IEnemy : public ICharacter
{
protected:
	AttackPtr attack_;							// 攻撃手段
public:
	const AttackPtr& attack;
protected:
	
public:
	/*
		@brief	コンストラクタ
		@param	[in]	name	名前("E_"から始める)
		@param	[in]	attack	攻撃用クラスポインタ
		@attention	nameは"E_"から始めること
	*/
	IEnemy(const std::string& name):
		ICharacter(name)
		,attack(attack_)
	{}
	virtual ~IEnemy() = 0{}
	virtual void step() = 0{}
	virtual void draw() = 0{}
	/*
		@brief	ダメージ加算
		@param	[in]	dam	ダメージ量
		@return 死亡したか
		@retval	true	死亡
		@retval	false	残存
	*/
	virtual bool ApplyDamage(int dam) = 0{}

	/*
		@brief	攻撃の当たり判定領域の取得
		@return	攻撃の当たり判定領域
	*/
	virtual Collisions GetAttackCollisions(){return attack_->GetCollisionAreas();}

	/*
		@brief	攻撃手段情報の格納
		@attension	ifstreamのcurrentPositionに注意
					if(FindChunk(f,"#Attack"))
						LoadAttack(f);
		@param	[in/out]	f	オープン済みファイル
		@return		EOFか
		@retval		true		EOF
		@retval		false		EOFでない
	*/
	bool LoadAttack(std::ifstream& f)
	{
		std::string buf;
		f >> buf;
		if(buf == "NWayShot")
		{
			f >> buf;
			if(buf != "{") return f.eof();
			else
			{
				CharBase shot_info;
				f >> buf;	// Imgラベル読み飛ばし
				std::string img_path;
				f >> img_path;
				D3DCOLOR transparent;
				f >> std::hex >> transparent;
				shot_info.img = Draw_LoadObject(img_path,transparent);

				f >> buf;	// Widthラベル読み飛ばし
				LoadValue(f, obj_, shot_info.src.x);
				f >> buf;	// Heightラベル読み飛ばし
				LoadValue(f, obj_, shot_info.src.y);

				CShot shot(shot_info);
				shot.LoadCollisions(f);
				attack_ = AttackPtr(new CNWayShot(shot));
			}
		}
		
		return f.eof();
	}

};
typedef std::shared_ptr<IEnemy> EnemyPtr;

#endif