/*
	ゲーム固有の設定等グローバル変数
*/
#ifndef DEF_SETTING_H
#define DEF_SETTING_H

#include <string>

namespace setting
{

extern void OnCreateSetup();

extern int GetFontID(const std::string& resname);

}	// namespace setting

#endif