#include "scoreMng.h"

#include <fstream>

#include "Easing.h"	// dispScore

#include <algorithm>	// sort
#include <functional>	// greater

#include "gameManager.h"

CScoreMng::CScoreMng() :
Base("ScoreMng")
, score_(0)
, dispScore_(score_)
, scoreObj_(charabase::BaseData("img_UInumber", 32, 64))
, scoreState_(common::DispState::HIDE)
{
	std::string rankingFile = gm()->fm().GetFile("#RankingFile");
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

	//status_ = Status::update;
	start();


}


void CScoreMng::start()
{
	//gm()->GetData(this);
	__super::start();
	dispScore_ = score_ = 0;
	bonusScores_.clear();
	scoreState_ = common::DispState::HIDE;
	scoreObj_.alpha = 0.f;
	SetDrawPos(mymath::Vec3f(20.f, system::WINH - scoreObj_.size.y - 20.f, 0.f));

}

void CScoreMng::step()
{
	// スコア文字
#pragma region スコア文字
	{
		const float appearTime = 0.3f;		// 出現アニメーション秒数
		const float appearMoveX = 30.f;		// X移動量
		const float dispTime = 1.f;			// 表示秒数
		const float disappearTime = 0.2f;	// 消失アニメーション秒数
		switch (scoreState_)
		{
		case common::DispState::HIDE:
			break;

		case common::DispState::APPEARING:
			// アニメーション部
			scoreObj_.alpha = Easing::Linear(scoreAnimTime_, 0.f, 255.f, appearTime);

			// 時間判定
			if ((scoreAnimTime_ += system::FrameTime) >= appearTime)
			{
				// 出現完了
				scoreObj_.alpha = 255.f;	// 保険
				scoreState_ = common::DispState::SHOW;
				scoreAnimTime_ = 0.f;
			}
			break;
		case common::DispState::SHOW:
			scoreObj_.alpha = 255.f;	// 保険
			// スコア増加アニメーション終了後
			if (isStopAddAnimation())
			{
				// 時間判定
				if ((scoreAnimTime_ += system::ONEFRAME_TIME) >= dispTime)
				{
					// 消失開始
					scoreState_ = common::DispState::DISAPPEARING;
					scoreAnimTime_ = 0.f;
				}
			}
			break;

		case common::DispState::DISAPPEARING:
			// アニメーション
			scoreObj_.alpha = Easing::Linear(scoreAnimTime_, 255.f, -255.f, disappearTime);
			
			// 時間
			if ((scoreAnimTime_ += system::FrameTime) >= disappearTime)
			{
				// 消失完了
				scoreState_ = common::DispState::HIDE;
				//scoreObj_.pos;
				scoreObj_.alpha = 0.f;
				scoreAnimTime_ = 0.f;
			}
			break;
		}
	}
#pragma endregion // スコア文字

	if (dispScore_ < score_)
	{
		const int frameAdd = 10;	// 1フレームあたりの加算量

		dispScore_ += min(frameAdd, score_ - dispScore_);
	}
}

void CScoreMng::draw()
{
	// スコアを画像パターンに

	auto numObjs = common::GetNumberImageObjects(dispScore_, scoreObj_, common::LEFT);
	for (auto& nobj : numObjs)
		nobj.drawNC(charabase::CharBase::MODE::LeftTop);
}

void CScoreMng::SetDrawPos(const mymath::Vec3f& pos)
{
	scoreObj_.pos = pos;
	// drawStartPos_ = scoreObj_.pos = pos;
}

void CScoreMng::AddScore(int add)
{
	score_ += add;
	switch (scoreState_)
	{
	case common::DispState::HIDE:
	case common::DispState::APPEARING:
		// 出現開始
		scoreState_ = common::DispState::APPEARING;
		scoreAnimTime_ = 0.f;
		break;
	case common::DispState::SHOW:
	case common::DispState::DISAPPEARING:
		// 表示継続
		scoreState_ = common::DispState::SHOW;
		break;
	}
}

void CScoreMng::score(int s)
{
	score_ = s;
}

int CScoreMng::score() const
{
	return score_;
}

bool CScoreMng::isStopAddAnimation() const
{
	return dispScore_ == score_;
}

void CScoreMng::CalcBonusScore(const std::weak_ptr<CEnemyMng>& em)
{
	BonusScore tmp = {};
	if (em.lock()->isDestroyerBonus())
	{
		tmp.label = "Destroyer Bonus";
		tmp.score = 100000;
		bonusScores_.push_back(tmp);
	}
	if (em.lock()->isNoKillBonus())
	{
		tmp.label = "NoKill Bonus";
		tmp.score = 200000;
		bonusScores_.push_back(tmp);
	}
}
void CScoreMng::CalcBonusScore()
{
	BonusScore tmp = {};
	auto& player = gm()->GetPlayer();
	auto& sm = gm()->stageMng();
	//auto& em = gm()->enemyMng();
	if (player->isNoDamage())
	{
		tmp.label = "NoDamage Bonus";
		tmp.score = 400000;
		bonusScores_.push_back(tmp);
	}
	if (sm->isNormaTimeClear())
	{
		tmp.label = "FastClear Bonus";
		tmp.score = 100000;
		bonusScores_.push_back(tmp);
	}

}

const std::vector<CScoreMng::BonusScore>& CScoreMng::getBonusScore() const
{
	return bonusScores_;
}

void CScoreMng::RegisterRanking(const std::string& name)
{
	std::string rankingFile = gm()->fm().GetFile("#RankingFile");
	Ranking tmp = { common::StrReplace(name," ", std::to_string(Ranking::SPACE)), score_ };
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

bool CScoreMng::isRankin(int score) const
{
	return ranking_.back().score < score;
}