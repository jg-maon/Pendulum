#ifndef DEF_SCENENAMEINPUT_H
#define DEF_SCENENAMEINPUT_H

#ifndef DEF_SCENEBASE_HPP
#include "sceneBase.hpp"
#endif

class CSceneNameInput : public IScene
{

private:
	//======================================
	// 定数
	//======================================
	enum kbData
	{

		KB_WIDTH = 50,
		KB_HEIGHT = 50,

	};


	enum selectOffset{

		OFFSET_W = 10,
		OFFSET_H = 10,

	};


	enum chgData
	{

		CHG_WIDTH = 150,
		CHG_HEIGHT = 50,

	};

	static const mymath::Vec3f KEYBOARD_POS;			// キーボードの座標x,y,z
	static const mymath::Vec3f CHANGE_POS;				// 文字記号変更ボタンの座標x,y,z
	static const mymath::Vec3f DISPLAY_POS;				// ディスプレイの座標x,y,z
	static const float FLASHING_TIME;
	static const std::string KEYBOARD_MARKDATA[4];		// キーボード記号データ配列
	static const std::string KEYBOARD_NAME[3];			// キーボードボタン画像名　	[0]:表示文字,[1]:選択,[2]:ボタン凸凹
	static const std::string CHANGE_NAME[3];			// 文字記号変換ボタン画像名　[0]:表示文字,[1]:選択,[2]:ボタン凸凹
	static const std::string DISPLAY_NAME[2];			// キーボードの画像名		[0]:表示文字,[1]:点滅画像


public:

	//ボタンの属性
	enum class BtnAttribute
	{
		NONE,
		KEYBOARD_DATA,		// キーボードでの文字入力
		KEYBOARD_DELETE,	// キーボードでの削除
		CHANGE_UP_LOW,		// キーボードでの大文字小文字変換
		CHANGE_LETTER,
		CHANGE_MARK,		// キーボードでの文字記号変換

	};

	//======================================
	// オブジェクトクラス
	//======================================

	class CButton : public IObject
	{

	public:

		//ボタンの状態
		enum class BtnState
		{

			NONE,			// 何もなし
			SELECT,			// マウスが乗った状態
			PRESS,			// 押されている状態
			RELEASE,		// 離れた状態

		};

		//  ボタン同士の間隔
		enum BtnInterval{

			INTERVAL_W = 5,				// 幅間隔
			INTERVAL_H = 5,				// 高さ間隔


		};

	private:
		BtnState pushState_;			// ボタンの状態
		BtnAttribute attribute_;		// ボタンの属性
		char charData_;					// 文字データ
		bool isCapital_;				// 大文字小文字用　true:大文字

		charabase::CharBase btn_;		// ボタン凸凹用
		charabase::CharBase select_;	// ボタン選択用

	private:

		/*
			@brief	ボタンが離されている
			@return	ボタンが離された瞬間か
			@retval	true	ボタンが離された
			@retval	false	ボタンが離された瞬間でない
		*/
		//bool CheckRelease();

		/*
			@brief	ボタンが押され続けている
			@return	ボタンが押され続けているか
			@retval	true	ボタンが押されている
			@retval	false	ボタンが押されていない
		*/
		//bool CheckPress();

		/*
			@brief	マウスがボタンに乗っている
			@return	なし
		*/
		bool CheckSelect();

		/*
			@brief	ボタンとマウスの関係
			@return	なし
		*/
		void CheckState();
	
	public:
		CButton();
		~CButton();
		/*
			@brief	オブジェクト初期化
			完全内包と境界線交差は内包が優先される
			@param	[in]	pos			[0][0]の座標 X,Y (一つだけの時はその座標X,Y）
			@param	[in]	w			ボタンの幅
			@param	[in]	h			ボタンの高さ
			@param	[in]	atr			ボタンの属性
			@param	[in]	resnames[3]	ボタンの画像名(要素[0]:表示文字 [1]:選択時 [2]:ボタン凸 [3]:ボタン凹)
			@param	[in]	col			ボタンが配列だった場合の行数(一つなら0)
			@param	[in]	row			ボタンが配列だった場合の列数(一つなら0)
			@return	なし
		*/
		void init(const mymath::Vec3f& pos, int w, int h, BtnAttribute atr, const string resnames[3],int col , int row);

		
		/*
			@brief	ボタンの処理
			@return	なし
		*/
		void step() override;

		/*
			@brief	ボタンの描画
			@return	なし
		*/
		void draw() override;


		/*
			@brief	大文字小文字ボタンの初期化
			@return	なし
		*/
		void UpLowInit();

		/*
			@brief	キーボードボタン大文字小文字変化
			@return	なし
		*/
		void ChangeLetterUpLow();

		/*
			@brief	キーボードボタン大文字変化
			@param	[in]	col			ボタンが配列だった場合の行数(一つなら0)
			@param	[in]	row			ボタンが配列だった場合の列数(一つなら0)
			@return	なし
		*/
		void ChangeLetter(int row, int col);

		/*
			@brief	キーボードボタン記号変化
			@param	[in]	col			ボタンが配列だった場合の行数(一つなら0)
			@param	[in]	row			ボタンが配列だった場合の列数(一つなら0)
			@return	なし
		*/
		void ChangeMark(int row, int col);

		/*
			@brief	ボタンの描画変更処理
			@return	離されたかどうか
			@retval	true	離された
			@retval	false	それ以外
		*/
		bool isReleased();

		/*
			@brief	大文字小文字フラグを反転させる
			@return	なし
		*/
		void ChangeCapital();

		/*
			@brief	割り当てられた文字取得
			@return	文字
		*/
		char getChar() const;

		/*
			@brief	ボタン属性の取得
			@return	ボタン属性
		*/
		BtnAttribute getAtt() const;
		
		/*
			@brief	ボタン属性の変更
			@return	なし
		*/
		void setAtt(BtnAttribute att);

	};


	//======================================
	// ディスプレイ用クラス
	//======================================

	class CDisplay : public IObject
	{

	public:

		//ボタンの状態
		enum class DispState
		{

			NONE,			// 何もなし
			FLASH,			// 点滅
			DISP,			// 表示

		};
	private:

		enum DispSize
		{
			DISP_WIDTH = 50,			// ディスプレイ幅
			DISP_HEIGHT = 50,			// ディスプレイ高さ
		};

	private:

		charabase::CharBase flash_;		// フラッシュ処理

		DispState dispState_;			// 表示状態
		float phaseTime_;				// ディスプレイ点滅用時間

	private:

		/*
			@brief	点滅処理
			@return	なし
		*/
		void StepFlashing();



	public:
		CDisplay();
		~CDisplay();
	

		/*
			@brief	状態変更
			@param	[in]	ds_	表示状態
			@return	なし
		*/
		void ChangeState(DispState ds_);


		/*
			@brief	データの入力
			@param	[in]	cb_	入力情報
			@return	なし
		*/
		void LoadLetter(const charabase::CharBase& cb_);

		/*
			@brief	ディスプレイの初期化
			@param	[in]	resnames	[0]:obj_ / [1]:flash_
			@param	[in]	row			列数
			@return	なし
		*/
		void init(const string resnames[2], int row);

		/*
			@brief	ディスプレイの処理
			@return	なし
		*/
		void step() override;

		/*
			@brief	ディスプレイの描画
			@return	なし
		*/
		void draw() override;


	};


private:

	enum class State
	{
		INPUT,
		CHECK,
		END,

	};

	enum
	{
		NAME_LEN = 6,	// 文字数
	};

	State state_;

	CButton keyButtons[4][7];		// キーボードボタン
	CButton changeMark[2];			// 文字記号変更ボタン

	CDisplay display[NAME_LEN];		// ディスプレイ


	//charabase::CharBase bg_;		// 背景

	std::string name_;				// 入力された名前の格納場所
	int wordcount_;					// 文字数

private:

	/*
		@brief	ボタンが離されたときの各処理
		@return	なし
	*/
	void IsRelease(const CButton& cb_);


	/*
		@brief	処理
		@return	なし
	*/
	void stepInput();


	/*
		@brief	ディスプレイの状態変更
		@return	なし
	*/
	void DisplayReflesh();


	/*
		@brief	キー入力確認
		@return	なし
	*/
	//bool InputNextKey();



protected:
	/*
		@brief	メイン更新処理
		@return	メイン終了か
		@retval	true	メイン終了(フェードアウト開始)
		@retval	false	メイン処理中
	*/
	bool update() override;


public:
	CSceneNameInput();
	~CSceneNameInput();
	/*
		@brief	描画
		@return	なし
	*/
	void draw() override;

	/*
		@brief	初期化
		@return	なし
	*/
	void start() override;
	
	/*
		@brief	次のシーンに切り替わる瞬間に呼ばれる
		@return	次のシーン
	*/
	int NextScene() const override;
};


#endif