#include "setting.h"
#include "../../../lib/gplib.h"
#include "common.h"

#include <unordered_map>		// Fontテーブル用

#include <fstream>

namespace setting
{
	using std::ifstream;

struct ResData
{
	std::string resname;
	std::string path;
	friend ifstream& operator >> (ifstream& f, ResData& data)
	{
		f >> data.resname >> data.path;
		return f;
	}
};

// フォント用
std::unordered_map<std::string, int> fontTable_;



void LoadImg(ifstream& f);
void LoadBGM(ifstream& f);
void LoadSE(ifstream& f);
void LoadFont(ifstream& f);

// OnCreateより呼ばれるsetting準備
void OnCreateSetup()
{
	ifstream f("res/txt/resdata.txt");
	if (f.fail())
	{
		gplib::debug::Dbg_BoxToMessage("huh?");
		return;
	}
	if (common::FindChunk(f, "#IMG"))
	{
		LoadImg(f);
	}
	common::SeekSet(f);
	if (common::FindChunk(f, "#BGM"))
	{
		LoadBGM(f);
	}
	common::SeekSet(f);
	if (common::FindChunk(f, "#SE"))
	{
		LoadSE(f);
	}
	common::SeekSet(f);
	if (common::FindChunk(f, "#FONT"))
	{
		LoadFont(f);
	}
	common::SeekSet(f);
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
		f >> data >> std::hex >> transparent;
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
		f >> data;
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
		f >> data;
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
		f >> data >> size;
		font::Draw_CreateFont(i, size, data.path.c_str());
		fontTable_.insert(std::unordered_map<std::string, int>::value_type(data.resname, i));
		i++;
	}
}

int GetFontID(const std::string& resname)
{
	return fontTable_[resname];
}

}	// namespace setting