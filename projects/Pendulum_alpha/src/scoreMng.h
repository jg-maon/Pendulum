#ifndef DEF_SCOREMNG_H
#define DEF_SCOREMNG_H

#ifndef DEF_OBJ_H
#include "obj.h"
#endif

#include <string>

/*
	@brief	スコア周り管理クラス
			ゲーム内でのスコア表示やランキングの作成も行う
*/
class CScoreMng : public Base
{
private:
	enum
	{
		RANK_NUM = 50,		// ランキング登録数
	};

	int score_;			// 現在のスコア

	struct Ranking		// ランキング登録情報
	{
		std::string name;		// 名前
		int score;				// スコア
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
	std::vector<Ranking> ranking_;		// ランキング
public:
	static std::string rankingFile;		// ランキングファイルパス名

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
	void init();
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
		@brief	スコア上書き
		@return	なし
	*/
	void score(int s);

	/*
		@brief	スコアの取得
		@return	なし
	*/
	int score() const;

	/*
		@brief	現在のスコアを使いランキング登録
		@param	[in]	name	ランキング登録名
		@return	なし
	*/
	void RegisterRanking(const std::string& name);


};

#endif