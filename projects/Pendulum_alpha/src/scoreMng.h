#ifndef DEF_SCOREMNG_H
#define DEF_SCOREMNG_H

#ifndef DEF_OBJ_H
#include "obj.h"
#endif

#ifndef DEF_COMMON_H
#include "common.h"
#endif

#ifndef DEF_CHARBASE_H
#include "charBase.h"
#endif

#ifndef DEF_ENEMYMNG_H
#include "enemyMng.h"
#endif

#include <string>

/*
	@brief	スコア周り管理クラス
			ゲーム内でのスコア表示やランキングの作成も行う
*/
class CScoreMng : public Base
{
public:
	struct LoadInfo
	{
		std::vector<std::string> labels;
		std::vector<int> scores;
	};
	struct BonusScore
	{
		std::string label;		// ボーナス名
		int score;				// スコア量
	};
private:
	enum
	{
		RANK_NUM = 50,		// ランキング登録数
	};
	int score_;								// 現在のスコア

	int dispScore_;							// 表示用スコア
	common::DispState scoreState_;			// スコア表示状態
	//mymath::Vec3f drawStartPos_;			// スコア表示初期座標
	charabase::CharBase scoreObj_;			// スコアベース
	float scoreAnimTime_;					// スコア表示アニメーション用
	std::vector<BonusScore> bonusScores_;	// ボーナス


	struct Ranking		// ランキング登録情報
	{
		std::string name;		// 名前
		int score;				// スコア

		static const char SPACE = '@';	// 空白文字の置換

		// 空要素
		static Ranking empty()
		{
			Ranking tmp = { "NONAME", 0 };
			return tmp;
		}
		// ファイル入出力
		friend std::ifstream& operator>>(std::ifstream& f, Ranking& rank)
		{ 
			f >> rank.name >> rank.score;
			rank.name = common::StrReplace(rank.name, std::to_string(SPACE), "");
			return f;
		}
		friend std::ofstream& operator<<(std::ofstream& f, const Ranking& rank)
		{
			f << rank.name << "\t" << rank.score << std::endl;
			return f;
		}
		// sort用
		bool operator<(const Ranking& rank) const
		{
			return score < rank.score;
		}
		bool operator>(const Ranking& rank) const
		{
			return score > rank.score;
		}
	};
	std::vector<Ranking> ranking_;			// ランキング


private:

public:
	/*
		@brief	初期化、ランキング情報読み込み
		@return	なし
	*/
	CScoreMng();
	/*
		@brief	初期化
		@return	なし
	*/
	void start() override;

	/*
		@brief	更新
		@return	なし
	*/
	void step() override;
	/*
		@brief	スコアの描画
		@return	なし
	*/
	void draw() override;
	
	/*
		@brief	スコア描画始点の設定
		@param	[in]	pos	描画始点
		@return	なし
	*/
	void SetDrawPos(const mymath::Vec3f& pos);

	/*
		@brief	スコア加算
		@param	[in]	add	加算量
		@return	なし
	*/
	void AddScore(int add);
	
	/*
		@brief	スコア上書き
		@param	[in]	s	スコア
		@return	なし
	*/
	void score(int s);

	/*
		@brief	スコアの取得
		@return	なし
	*/
	int score() const;


	/*
		@brief	スコア加算アニメーションの終了判断
		@return	スコア加算アニメーションが終了しているか
		@retval	true	アニメーション終了
		@retval	false	アニメーション中
	*/
	bool isStopAddAnimation() const;

	/*
		@brief	エネミーマネージャ内ボーナススコア計算
		@param	[in]	em	エネミーマネージャ
		@return	なし
	*/
	void CalcBonusScore(const std::weak_ptr<CEnemyMng>& em);

	/*
		@brief	ボーナススコア計算
		@return	なし
	*/
	void CalcBonusScore();

	/*
		@brief	ボーナススコア取得
		@return	ボーナススコア
	*/
	const std::vector<BonusScore>& getBonusScore() const;

	/*
		@brief	現在のスコアを使いランキング登録
		@param	[in]	name	ランキング登録名
		@return	なし
	*/
	void RegisterRanking(const std::string& name);

	/*
		@brief	引数のスコアがランクインするかどうか
		@attension		同一スコアの場合ランクインしない
		@param	[in]	score	スコア
		@return	ランクインするか
		@retval	ランクインする
		@retval	ランクインしない
	*/
	bool isRankin(int score) const;
};

#endif