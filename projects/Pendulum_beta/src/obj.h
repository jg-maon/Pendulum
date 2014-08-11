#ifndef DEF_OBJ_H
#define DEF_OBJ_H

#ifndef DEF_CHARBASE_H
#include "CharBase.h"
#endif

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp"
#endif

//ライブラリ
#include "../../../lib/gplib.h"
using namespace gplib;

//3D処理外部ライブラリ
#include "../../../lib/dx_media/dx_graphics3d.h"
#include "../../../lib/dx_media/dx3dmeshmanager.h"

#include <memory>
#include <string>
#include <vector>

#define EraseObject(obj)	((obj).erase( \
								std::remove_if( \
									(obj).begin(), (obj).end(), \
									std::mem_fn(&Base::isDestroy)), \
								(obj).end()))
									
					


class CGameManager;

/*
	@brief	全処理系基底クラス
*/
class Base
{
public:
	enum class Status
	{
		run,		// step,drawを実行
		idle,		// step,drawを実行しない
		update,		// stepのみ実行
		disp,		// drawのみ実行
		destroy,	// 削除
	};
	typedef std::vector<mymath::ShapefPtr> Collisions;		// 当たり判定領域
	typedef std::vector<Collisions> MotionCollisions;		// モーション毎の当たり判定領域
private:
	//ゲームマネージャを扱うためのクラス
	static CGameManager* gm_;
protected:
	std::string name_;
	Status status_;
protected:
	/*
		@brief	ゲームマネージャの取得
		@return	ゲームマネージャ
	*/
	CGameManager* gm() const;
	/*
		@brief	ゲームマネージャの設定
		@param	[in]	gm	ゲームマネージャ
		@return	なし
	*/
	void gm(CGameManager* gm);
public:
	/*
		@brief	オブジェクトの生成
		@param	[in]	name	識別名
	*/
	Base(const std::string& name);
	/*
		@brief	インターフェース化するための純粋仮想関数
	*/
	virtual ~Base() = 0;
	/*
		@brief	オブジェクトが消去対象か取得
		@return	オブジェクトの消去対象状態
		@retval	true	消去対象
		@retval	false	消去対象でない
	*/
	bool isDestroy() const;
	
	
	/*
		@brief	オブジェクト名の検索
		@return	指定した名前がオブジェクト名に含まれているか
		@retval	true	含まれている
		@retval	false	含まれていない
	*/
	bool FindName(const std::string& name) const;
	/*
		@brief	オブジェクトの識別名を取得
		@return	オブジェクト識別名
	*/
	const std::string& getName() const;
	/*
		@brief	更新処理
		@return	なし
	*/
	virtual void step() = 0;
	/*
		@brief	描画処理
		@return	なし
	*/
	virtual void draw() = 0;
	/*
		@brief	オブジェクトを消す
		@return	なし
	*/
	virtual void kill();
	/*
		@brief	オブジェクトを始動させる
		@return	なし
	*/
	virtual void start();
	/*
		@brief	オブジェクトを停止させる
		@return	なし
	*/
	virtual void stop();

	/*
		@brief	オブジェクトを描画のみに設定する
		@return	なし
	*/
	virtual void SetStatusDisp();

	/*
		@brief	オブジェクトの状態を変更する
		@param	[in]	status	状態
		@return	なし
	*/
	//void setStatus(Status status);
	
	/*
		@brief	状態の取得
		@return	オブジェクトの状態
	*/
	Status getStatus() const;

	/*
		@brief	オブジェクト同士の重なり処理
		@param	[in]	rival	重なっている相手オブジェクト
		@return	なし
	*/
	virtual void hit(const std::shared_ptr<Base>& rival);
	/*
		@brief	ワールド座標の食らい判定領域の取得
		@return	ワールド座標の当たり判定領域
	*/
	virtual Collisions GetDamageAreas() const;
	/*
		@brief	ワールド座標のステージ当たり判定領域の取得
		@return	ワールド座標のステージ当たり判定領域
	*/
	virtual Collisions GetStageCollisions() const;

};
typedef std::shared_ptr<Base> ObjPtr;

/*
	@brief	Base::gm初期化用オブジェクト
*/
class TempObject : public Base
{
public:
	TempObject(CGameManager *game):
		Base("Temp")
	{
		gm(game);
	}
	~TempObject(){}
	void step() override{};
	void draw() override{};

};

/*
	@brief	オブジェクト基底クラス
*/
class IObject : public Base
{
private:
protected:
	bool turnFlag_;				// 反転描画するか
	charabase::CharBase obj_;
protected:
	/*
		@brief	objが画面内か判断
		@param	[in]	border	画面端からの余白
		@return	画面内に存在するか
		@retval	true	画面内
		@retval	false	画面外
	*/
	bool InScreen(int border = 0) const;
	
	/*
		@brief	更新をかけることができるか
		@return	更新をかけられるか
		@retval	true	更新
		@retval	false	更新できない
	*/
	bool isUpdatable() const;

	/*
		@brief		ファイルから数値を格納
					halfWidth、halfHeight対応
		@attension	ifstreamのcurrentPositionに注意
		@param		[in/out]	f		入力ファイル
		@param		[in]		obj		半分のサイズ用
		@param		[out]		value	結果格納用
		@return		EOFか
		@retval		true		EOF
		@retval		false		EOFでない
	*/
	template<class T>	bool LoadValue(std::ifstream& f, const charabase::CharBase& obj, T& value);
public:
	/*
		@brief	オブジェクトの生成
		@param	[in]	name	識別名
	*/
	IObject(const std::string& name);
	/*
		@brief	インターフェース化するための純粋仮想関数
	*/
	virtual ~IObject() = 0;
	
	/*
		@brief	更新処理
		@return	なし
	*/
	virtual void step() = 0;
	/*
		@brief	描画処理
		@return	なし
	*/
	virtual void draw();
	/*
		@brief	オブジェクトを消す
		@return	なし
	*/
	virtual void kill();
	/*
		@brief	オブジェクト同士の重なり処理
		@param	[in]	rival	重なっている相手オブジェクト
		@return	なし
	*/
	virtual void hit(const std::shared_ptr<Base>& rival);
	/*
		@brief	ワールド座標の食らい判定領域の取得
		@return	ワールド座標の当たり判定領域
	*/
	virtual Collisions GetDamageAreas() const;
	/*
		@brief	ワールド座標のステージ当たり判定領域の取得
		@return	ワールド座標のステージ当たり判定領域
	*/
	virtual Collisions GetStageCollisions() const;
	/*
		@brief	画像オブジェクトの取得
		@return	画像オブジェクト
	*/
	const charabase::CharBase& obj() const;
	/*
		@brief	画像オブジェクトの設定
		@return	なし
	*/
	void obj(const charabase::CharBase& o);

	/*
		@brief	画像反転描画させるかのフラグを返す
		@return	反転描画フラグ
		@retval	true	反転描画する
		@retval	false	反転描画しない
	*/
	bool isTurn() const;
};

/*
	@brief		ファイルから数値を格納
	halfWidth、halfHeight対応
	@attension	ifstreamのcurrentPositionに注意
	@param		[in/out]	f		入力ファイル
	@param		[in]		obj		半分のサイズ用
	@param		[out]		value	結果格納用
	@return		EOFか
	@retval		true		EOF
	@retval		false		EOFでない
*/
template<class T>	bool IObject::LoadValue(std::ifstream& f, const charabase::CharBase& obj, T& value)
{
	std::string buf;
	bool minus = false;	// マイナス
	f >> buf;
	if(buf[0] == '-')
	{
		buf.erase(buf.begin(),buf.begin()+1);	// -を取り払う
		minus = true;
	}
	// 値を格納
	if(buf == "halfWidth")
	{
		value = static_cast<T>(obj.HalfWidth());
	}
	else if(buf == "halfHeight")
	{
		value = static_cast<T>(obj.HalfHeight());
	}
	else
	{
		std::stringstream ss(buf);
		ss >> value;
	}
	if(minus)
	{
		value = -value;
	}
	return f.eof();
}


/*
	@brief	当たり判定付きオブジェクト基底クラス
*/
class IColObject : public IObject
{
protected:
	Collisions collisions_;			// 食らい判定領域
	Collisions stageCollisions_;	// ステージとの当たり判定領域
protected:
	/*
		@brief	当たり判定領域の削除
		@return なし
	*/
	void ClearCollisions();

	
	/*
		@brief	当たり判定の格納
		@attension	ifstreamのcurrentPositionに注意
					if(FindChunk(f,"#Collision"))
						LoadCollisions(f, collisions_);
					if(FindChunk(f,"#StageCollision"))
						LoadCollisions(f, stageCollisions_);
		@param	[in/out]	f			オープン済みファイル
		@param	[out]		collisions	判定を格納する配列
		@return		EOFか
		@retval		true		EOF
		@retval		false		EOFでない
	*/
	bool LoadCollisions(std::ifstream& f, Collisions& collisions);


	/*
		@brief	ワールド座標に変換した当たり判定領域の取得
		@param	[in]	collisions	ローカル当たり判定領域
		@return	ワールド座標の当たり判定領域
	*/
	Collisions GetWorldCollisions(const Collisions& collisions) const;

public:
	/*
		@brief	オブジェクトの生成
		@param	[in]	name	識別名
	*/
	IColObject(const std::string& name);
	/*
		@brief	インターフェース化するための純粋仮想関数
	*/
	virtual ~IColObject() = 0;
	/*
		@brief	更新処理
		@return	なし
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
	virtual void hit(const std::shared_ptr<Base>& rival);
	
	/*
		@brief	当たり判定の格納
		@attension	ifstreamのcurrentPositionに注意
					if(FindChunk(f,"#Collision"))
						LoadCollisions(f);
		@param	[in/out]	f			オープン済みファイル
		@return		EOFか
		@retval		true		EOF
		@retval		false		EOFでない
	*/
	bool LoadCollisions(std::ifstream& f);
	
	/*
		@brief	ステージ当たり判定の格納
		@attension	ifstreamのcurrentPositionに注意
					if(FindChunk(f,"#StageCollision"))
						LoadStageCollisions(f);
		@param	[in/out]	f			オープン済みファイル
		@return		EOFか
		@retval		true		EOF
		@retval		false		EOFでない
	*/
	bool LoadStageCollisions(std::ifstream& f);


	/*
		@brief	ワールド座標の食らい判定領域の取得
		@return	ワールド座標の当たり判定領域
	*/
	virtual Collisions GetDamageAreas() const override;
	/*
		@brief	ワールド座標のステージ当たり判定領域の取得
		@return	ワールド座標のステージ当たり判定領域
	*/
	virtual Collisions GetStageCollisions() const override;

	/*
		@brief	ローカル座標の当たり判定領域の代入
		@param	[in]	collisions	新しいローカル当たり判定領域
		@return	なし
	*/
	void SetCollisionAreas(const Collisions& collisions);

	/*
		@brief	ローカル座標の当たり判定領域の代入
		@param	[in]	obj	新しい当たり判定領域を持ったオブジェクト
		@return	なし
	*/
	void SetCollisionAreas(const IColObject& obj);

	/*
		@brief	ローカル座標のステージ当たり判定領域の代入
		@param	[in]	collisions	新しいステージ当たり判定領域
		@return	なし
	*/
	void SetStageCollisions(const Collisions& collisions);

	/*
		@brief	対象オブジェクトからステージ当たり判定領域をコピーする
		@param	[in]	obj	コピーする当たり判定領域を持ったオブジェクト
		@return	なし
	*/
	void SetStageCollisions(const IColObject& obj);

};



#endif