#ifndef DEF_GAMEMANAGER_H
#define DEF_GAMEMANAGER_H

//ライブラリ
#ifndef DEF_DEFINE_H
#include "define.h"
#endif
using namespace gplib;

#ifndef DEF_OBJ_H
#include "obj.h"
#endif

#include	<vector>
#include	<typeinfo>

//-------------------------------------------------
//ゲームの管理を行う。
//Objectを継承しているクラスは全て管理される。
class CGameManager
{
private:
	//出現中のオブジェクトを管理
	std::vector<ObjPtr>	objs_;
	//追加予定のオブジェクトを管理
	std::vector<ObjPtr>	addObjs_;
	//ゲームクリア状態を発動する。
	bool clear_;
	float count_;

	static mymath::Recti* winRect_;		// ウィンドウサイズ


private:

	/*
		@brief	オブジェクトの追加、削除を行う
		@return	なし
	*/
	void MargeObjects();

public:

	CGameManager();
	~CGameManager();
	/*
		@brief	初期化	
		@return	なし
	*/
	void init();
	/*
		@brief	全オブジェクトの更新処理
		@return	なし
	*/
	void step();
	/*
		@brief	全オブジェクトの描画
		@return	なし
	*/
	void draw();

	/*
		@brief		オブジェクトの追加
		@attention	オブジェクトが実際に追加されるのは1フレーム後
		@parama		[in]	obj	追加するオブジェクト
		@return		なし
	*/
	void AddObject(const ObjPtr& obj);
	/*
		@brief		オブジェクトの即時追加
		@parama		[in]	obj	追加するオブジェクト
		@return		なし
	*/
	void AddObject2(const ObjPtr& obj);

	//指定のクラスを見つけ出して返す
	ObjPtr GetObj(const type_info& objinfo);

	/*
		@brief	追加されている全オブジェクトを取得
		@return	オブジェクト群
	*/
	std::vector<ObjPtr>& GetObjects();

	/*
		@brief	与えられた名前を含むオブジェクトを取得
				GetObjects("Collision");
		@param	[in] taskName	探すオブジェクト名
		@return	オブジェクト群
	*/
	std::vector<ObjPtr> GetObjects(const std::string& taskName);
	/*
		@brief	与えられた名前を含むオブジェクトを取得
				GetObjects("Player,Enemy", ",");
		@param	[in] taskName	探すオブジェクト名
		@param	[in] delim		区切り文字
		@return	オブジェクト群
	*/
	std::vector<ObjPtr> GetObjects(const std::string& taskName, const char delim);
	
	/*
		@brief	オブジェクトの全消去
		@return	なし
	*/
	void ClearObjects();



	void setClear(bool clear);
	bool getClear() const;
	void StartClear();
	void ClearToChangeScreen(int next);

	/*
		@brief	ウィンドウサイズの取得
		@return	ウィンドウサイズ
	*/
	const mymath::Recti* winRect() const;
	/*
		@brief	新しいウィンドウサイズの代入
		@param	[in]	newRect	新しいウィンドウサイズ
		@return	なし
	*/
	void winRect(const mymath::Recti& newRect);
	/*
		@brief	ウィンドウサイズの参照先を変更
		@param	[in]	newRect	新しいウィンドウサイズ参照先
		@return	なし
	*/
	void winRect(mymath::Recti* newRect);

	/*
		@brief	プレイヤー座標の取得
		@return	プレイヤーオブジェクトの座標
	*/
	mymath::Vec3f GetPlayerPos() const;

};

extern void InsertObject(const ObjPtr& obj);


#endif

