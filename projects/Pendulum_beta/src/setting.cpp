#include "setting.h"

#include "gameManager.h"

extern CGameManager* gm;

namespace setting
{

// OnCreateより呼ばれるsetting準備
void OnCreateSetup()
{

}

int GetFontID(const std::string& resname)
{
	return gm->fm().GetFontID(resname);
}


}	// namespace setting