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
		run, destroy
	};
	typedef std::vector<mymath::ShapefPtr> Collisions;
private:
	//ゲームマネージャを扱うためのクラス
	static CGameManager* gm_;
protected:
	std::string name_;
	Status status_;
protected:
	CGameManager* gm() const;
	void gm(CGameManager* gm);
public:
	Base(const std::string& name);
	virtual ~Base() = 0;
	bool isDestroy() const;
	bool FindName(const std::string& name) const;
	const std::string& getName() const;
	virtual void step() = 0;
	virtual void draw() = 0;
	virtual void kill();
	virtual void hit(const std::shared_ptr<Base>& rival);
	virtual Collisions GetCollisionAreas();
};
typedef std::shared_ptr<Base> ObjPtr;

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
protected:
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
	IObject(const std::string& name);
	virtual ~IObject() = 0;
	virtual void step() = 0;
	virtual void draw() = 0;
	virtual void kill();
	virtual void hit(const ObjPtr& rival);
	const charabase::CharBase& obj() const;
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
	Collisions collisions_;		// 当たり判定領域
public:
	const Collisions& collisions;	// 画像当たり判定領域
protected:
	/*
		@brief	当たり判定領域の削除
		@return なし
	*/
	void ClearCollisions();
public:
	IColObject(const std::string& name);
	virtual ~IColObject() = 0;
	virtual void step() = 0;
	virtual void draw() = 0;
	virtual void hit(const ObjPtr& rival);
	
	/*
		@brief	当たり判定の格納
		@attension	ifstreamのcurrentPositionに注意
					if(FindChunk(f,"#Collision"))
						LoadCollisions(f);
		@param	[in/out]	f	オープン済みファイル
		@return		EOFか
		@retval		true		EOF
		@retval		false		EOFでない
	*/
	bool LoadCollisions(std::ifstream& f);

	/*
		@brief	当たり判定領域の取得
		@return	当たり判定領域
	*/
	virtual Collisions GetCollisionAreas() const;

	/*
		@brief	当たり判定領域の代入
		@param	[in]	collisions	新しい当たり判定領域
		@return	なし
	*/
	void SetCollisionAreas(const Collisions& collisions);
};



#endif