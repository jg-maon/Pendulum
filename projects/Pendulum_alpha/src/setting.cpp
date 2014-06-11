#include "setting.h"
#include "../../../lib/gplib.h"
#include "common.h"

#include "scoreMng.h"
#include "enemyMng.h"
#include "stageMng.h"
#include "sceneStageSelect.h"


#include <unordered_map>		// Fontテーブル用

#include <fstream>

namespace setting
{
	using std::ifstream;

struct ResData
{
	std::string resname;
	std::string path;
};

// フォント用
std::unordered_map<std::string, int> fontTable_;

//---------------------------
// リソースファイル読み込み関連
void LoadRes(ifstream&);
void LoadImg(ifstream&);
void LoadBGM(ifstream&);
void LoadSE(ifstream&);
void LoadFont(ifstream&);
//---------------------------

// OnCreateより呼ばれるsetting準備
void OnCreateSetup()
{
	ifstream iniF("res/dat/path.ini");
	if (iniF.fail())
	{
		gplib::debug::BToM("huh?OnCreateSetup");
		return;
	}
	//====================================
	//------------------------------
	// リソースファイル読み込み
	if (common::FindChunk(common::SeekSet(iniF), "#ResourceFile"))
	{
		LoadRes(iniF);
	}
	//------------------------------
	// ランキングファイルパス読み込み
	if (common::FindChunk(common::SeekSet(iniF), "#RankingFile"))
	{
		iniF >> CScoreMng::rankingFile;
	}
	//------------------------------
	// ステージファイルパス読み込み
	if (common::FindChunk(common::SeekSet(iniF), "#StageFile"))
	{
		iniF >> CStageMng::stageFile;
	}
	//------------------------------
	// 敵テーブルファイルパス読み込み
	if (common::FindChunk(common::SeekSet(iniF), "#EnemyTableFile"))
	{
		iniF >> CEnemyMng::enemyTableFile;
	}

	//------------------------------
	// ステージセレクトファイルパス読み込み
	if (common::FindChunk(common::SeekSet(iniF), "#StageSelectFile"))
	{
		iniF >> CSceneStageSelect::settingFile;
	}
}

#pragma region リソースファイル読み込み
void LoadRes(ifstream& iniF)
{
	//====================================
	// リソースファイル読み込み
	std::string buf;
	iniF >> buf;
	ifstream resF(buf);
	if (resF.fail())
	{
		gplib::debug::BToM("OnCreateSetup_resourceFile_NotFound");
		return;
	}
	
	//--------------------------
	if (common::FindChunk(resF, "#Img"))
	{
		LoadImg(resF);
	}
	common::SeekSet(resF);
	if (common::FindChunk(resF, "#Bgm"))
	{
		LoadBGM(resF);
	}
	common::SeekSet(resF);
	if (common::FindChunk(resF, "#Se"))
	{
		LoadSE(resF);
	}
	common::SeekSet(resF);
	if (common::FindChunk(resF, "#Font"))
	{
		LoadFont(resF);
	}
	common::SeekSet(resF);
}

void LoadImg(ifstream& f)
{
	/*
	struct ImgInfo
	{
		char* fileName;
		D3DCOLOR transparent;
	};
	// ImgTblの対応順に記述すること
	ImgInfo iInfo[] = 
	{
		{"res/gra/bg.jpg"},
		{"res/gra/player.png",ARGB(255,0,255,0)},
		{"res/gra/bird.png"},
		{"res/gra/lazer.png"},
		{"res/gra/circle_x96.png"},
		{"res/gra/Explosion.png"},
		{"res/gra/number.png"},
		{"res/gra/chain.png"},
	};
	*/
	std::string buf;
	f >> buf;
	if (buf != "{") return;
	while (!f.eof())
	{
		f >> buf;
		if (buf == "}") return;
		ResData data;
		D3DCOLOR transparent;
		data.resname = buf;
		f >> data.path >> std::hex >> transparent;

		graph::Draw_LoadObject(data.resname, data.path, transparent);
	}
}


// BGMロード
void LoadBGM(ifstream& f)
{
	/*
	char* bgm[] =
	{
		"res/snd/bgm/uzumaki.mp3",
		"res/snd/bgm/uzumaki.mp3",
		"res/snd/bgm/uzumaki.mp3",
		"res/snd/bgm/uzumaki.mp3",
	};
	for(const auto& b : bgm)
	{
		DShow_LoadFile(b);
	}
	*/
	std::string buf;
	f >> buf;
	if (buf != "{") return;
	while (!f.eof())
	{
		f >> buf;
		if (buf == "}") return;
		ResData data;
		data.resname = buf;
		f >> data.path;
		bgm::DShow_LoadFile(data.resname, data.path);
	}
}

// SEロード
void LoadSE(ifstream& f)
{
	/*
	char* se[] =
	{
		"res/snd/se/explode.wav",
	};
	for(const auto& s : se)
	{
		DSound_LoadFile(s);
	}
	*/
	std::string buf;
	f >> buf;
	if (buf != "{") return;
	while (!f.eof())
	{
		f >> buf;
		if (buf == "}") return;
		ResData data;
		data.resname = buf;
		f >> data.path;
		se::DSound_LoadFile(data.resname, data.path);
	}
}


void LoadFont(ifstream& f)
{
	/*
	struct FontInfo
	{
		int size;
		char* fontName;
	};
	FontInfo fInfo[] =
	{
		{ 30, "MS　ゴシック" },
		{ 30, "HG創英角ﾎﾟｯﾌﾟ体" },
		{ 25, "HG創英角ﾎﾟｯﾌﾟ体" },
		{ 12, "HG創英角ﾎﾟｯﾌﾟ体" },
		{ 15, "MS　ゴシック" },
	};
	*/
	std::string buf;
	f >> buf;
	if (buf != "{") return;
	int i = 0;
	while (!f.eof())
	{
		f >> buf;
		if (buf == "}") return;
		ResData data;
		int size;
		data.resname = buf;
		f >> data.path >> std::dec >> size;
		data.path = common::StrReplace(data.path, "＿", "");
		font::Draw_CreateFont(i, size, data.path.c_str());
		fontTable_.insert(std::unordered_map<std::string, int>::value_type(data.resname, i));
		i++;
	}
}

#pragma endregion // リソースファイル読み込み

int GetFontID(const std::string& resname)
{
	return fontTable_[resname];
}





}	// namespace setting