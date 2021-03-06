#ifndef DEF_GAMEMANAGER_H
#define DEF_GAMEMANAGER_H

#define DEF_GM_PTR		// よく使うポインタを保持させる

//ライブラリ
#ifndef DEF_DEFINE_H
#include "define.h"
using namespace gplib;
#endif

#ifndef DEF_OBJ_H
#include "obj.h"
#endif

#ifndef DEF_CHARBASE_H
#include "charBase.h"
#endif

#ifndef DEF_FILEMNG_H
#include "fileMng.h"
#endif

#ifdef DEF_GM_PTR
//*
#ifndef DEF_PLAYER_H
#include "player.h"
#endif

#ifndef DEF_ENEMYMNG_H
#include "enemyMng.h"
#endif

#ifndef DEF_STAGEMNG_H
//#include "stageMng.h"
#endif

#ifndef DEF_SCOREMNG_H
#include "scoreMng.h"
#endif
//*/
#endif

#include	<vector>
#include	<typeinfo>

//-------------------------------------------------
//ゲームの管理を行う。
//Objectを継承しているクラスは全て管理される。
class CGameManager : public Base
{
private:
	static mymath::Recti* winRect_;		// ウィンドウサイズ

	//出現中のオブジェクトを管理
	std::vector<ObjPtr>	objs_;
	//追加予定のオブジェクトを管理
	std::vector<ObjPtr>	addObjs_;

	//============================================================
	// よく使うオブジェクトは別で確保しておく
#ifdef DEF_GM_PTR
	//*
	std::weak_ptr<CPlayer> pPlayer_;		// プレイヤーポインタ
	std::weak_ptr<CEnemyMng> pEnemyMng_;	// エネミーマネージャ
	//std::weak_ptr<CStageMng> pStageMng_;	// ステージマネージャ
	std::weak_ptr<CScoreMng> pScoreMng_;	// スコアマネージャ
	//*/
#endif
	//=============================================================

	/*
	//ゲームクリア状態を発動する。
	bool clear_;
	float count_;
	//*/

	bool showCursor_;					// true:カーソルの表示

	enum CursorSize
	{
		width = 48,
		height = 48,
	};
	charabase::CharPtr cursor_;			// カーソル
	//charabase::Anim cursorAnim_;		// カーソルアニメーション用

	
	CFileMng fileMng_;					// ファイルマネージャ

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
		@brief	ゲームマネージャ初期化、起動
		@return	なし
	*/
	void start() override;

	/*
		@brief	全オブジェクトの更新処理
		@return	なし
	*/
	void step() override;
	/*
		@brief	全オブジェクトの描画
		@return	なし
	*/
	void draw() override;

	/*
		@brief	全オブジェクトを待機状態にする
		@return	なし
	*/
	void AllStop();

	/*
		@brief		オブジェクトの追加
		@attention	オブジェクトが実際に追加されるのは1フレーム後
		@parama		[in]	obj	追加するオブジェクト
		@return		なし
	*/
	void AddObject(const ObjPtr& obj);
	/*
		@brief	オブジェクトの即時追加
		@parama	[in]	obj	追加するオブジェクト
		@return	なし
	*/
	void AddObject2(const ObjPtr& obj);
	
	/*
		@brief	指定のクラスを見つけ出して返す
		@param	[in]	objinfo	クラスのID(typeid)
		@return	オブジェクト
	*/
	ObjPtr GetObj(const type_info& objinfo);

	/*
		@brief	追加されている全オブジェクトを取得
		@return	オブジェクト群
	*/
	std::vector<ObjPtr>& GetObjects();

	/*
		@brief	与えられた名前のオブジェクトを取得
				GetObjects("Collision");
		@param	[in] taskName	探すオブジェクト名(前文一致)
		@return	オブジェクト群
	*/
	std::vector<ObjPtr> GetObjects(const std::string& taskName);
	/*
		@brief	与えられた名前のオブジェクトを取得
				GetObjects("Player,Enemy", ",");
		@param	[in] taskName	探すオブジェクト名(前文一致)
		@param	[in] delim		区切り文字
		@return	オブジェクト群
	*/
	std::vector<ObjPtr> GetObjects(const std::string& taskName, const char delim);
	
	/*
		@brief	オブジェクトの全消去
		@return	なし
	*/
	void ClearObjects();



	/*
	void setClear(bool clear);
	bool getClear() const;
	void StartClear();
	void ClearToChangeScreen(int next);
	//*/

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
		@brief	マウスカーソルの座標を取得
		@return	マウスカーソル座標
	*/
	const mymath::Vec3f& GetCursorPos() const;

	/*
		@brief	マウスカーソルのスクリーン座標を取得
		@return	マウスカーソルスクリーン内座標
	*/
	mymath::Vec3f GetCursorPosNC() const;

	/*
		@brief	プレイヤー座標の取得
		@return	プレイヤーオブジェクトの座標
	*/
	mymath::Vec3f GetPlayerPos() const;

	/*
		@brief	ファイルマネージャの取得
		@return	ファイルマネージャ
	*/
	CFileMng& fm();

#ifdef DEF_GM_PTR
	/*
		@brief	プレイヤーポインタの設定
		@param	[in]	player	プレイヤー
		@return	なし
	*/
	void SetPlayerPtr(const std::weak_ptr<CPlayer>& player);

	/*
		@brief	エネミーマネージャポインタの設定
		@param	[in]	enemymng	エネミーマネージャ
		@return	なし
	*/
	void SetEnemyMngPtr(const std::weak_ptr<CEnemyMng>& enemymng);

	/*
		@brief	ステージマネージャポインタの設定
		@param	[in]	stagemng	ステージマネージャ
		@return	なし
	*/
	//void SetStageMngPtr(const std::weak_ptr<CStageMng>& stagemng);

	/*
		@brief	スコアマネージャポインタの設定
		@param	[in]	scoremng	スコアマネージャ
		@return	なし
	*/
	void SetScoreMngPtr(const std::weak_ptr<CScoreMng>& scoremng);

	/*
		@brief	ステージマネージャの取得
		@return	ステージマネージャ
	*/
	//CStageMng& stageMng();

	/*
		@brief	スコアマネージャの取得
		@return	スコアマネージャ
	*/
	CScoreMng& scoreMng();

	/*
		@brief	プレイヤーオブジェクトの取得
		@return	プレイヤーオブジェクト
	*/
	CPlayer& GetPlayer();

	/*
		@brief	エネミーマネージャの取得
		@return	エネミーマネージャ
	*/
	CEnemyMng& enemyMng();
#endif


	/*
		@brief	情報の取得
				gm()->GetData(*this);
		@param	[in/out]	obj	格納するクラスオブジェクト
		@return	情報を設定できたか
		@retval	true	設定成功
		@retval	false	設定失敗
	*/
	template<class OBJ>	bool GetData(OBJ& obj) const
	{
		return fileMng_.GetData(obj);
	}

};

//extern void InsertObject(const ObjPtr& obj);


#endif

