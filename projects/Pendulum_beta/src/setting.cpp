#include "setting.h"

#include "gameManager.h"

extern CGameManager* gm;

namespace setting
{

// OnCreate‚æ‚èŒÄ‚Î‚ê‚ésetting€”õ
void OnCreateSetup()
{

}

int GetFontID(const std::string& resname)
{
	return gm->fm().GetFontID(resname);
}


}	// namespace setting