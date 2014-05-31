#include "scoreMng.h"

#include <fstream>

#include <algorithm>	// sort
#include <functional>	// greater

std::string CScoreMng::rankingFile;

CScoreMng::CScoreMng():
Base("ScoreMng")
, score_(0)
{
	// ランキング情報読み込み
	ranking_.clear();
	ranking_.reserve(RANK_NUM + 1);
	std::ifstream f(rankingFile);
	if (f.is_open())
	{
		while (!f.eof())
		{
			Ranking tmp;
			f >> tmp;
			if (f.eof())break;
			ranking_.push_back(tmp);
		}
	}

	ranking_.resize(RANK_NUM, Ranking::empty());
}

void CScoreMng::init()
{
	ranking_.clear();
}

void CScoreMng::step()
{

}

void CScoreMng::draw()
{

}


void CScoreMng::score(int s)
{
	score_ = s;
}

int CScoreMng::score() const
{
	return score_;
}

void CScoreMng::RegisterRanking(const std::string& name)
{
	Ranking tmp = { name, score_ };
	ranking_.push_back(tmp);

	// スコア降順ソート
	std::sort(ranking_.begin(), ranking_.end(), std::greater<Ranking>());

	// ランキング件数調整
	ranking_.resize(RANK_NUM, Ranking::empty());
	//ranking_.pop_back();

	// ランキングファイル上書き
	std::ofstream f(rankingFile);
	for (const auto& rank : ranking_)
	{
		f << rank;
	}


	
}