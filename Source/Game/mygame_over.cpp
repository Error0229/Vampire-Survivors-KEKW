#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

using namespace game_framework;

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame *g): CGameState(g)
{
}

void CGameStateOver::OnMove()
{
	GotoGameState(GAME_STATE_INIT); //?????
}

void CGameStateOver::OnBeginState()
{
	ifstream p_data("save/save_data.csv");
	string header, line;
	getline(p_data, header);
	getline(p_data, line);
	p_data.close();
	ofstream r_data("save/save_data.csv", std::ofstream::trunc);
	r_data << header << '\n';
	GOLD_NUM += stoi(line.substr(0, line.find_first_of(',')));
	r_data << GOLD_NUM << line.substr(line.find_first_of(','));
	r_data.close();
}

void CGameStateOver::OnInit()
{
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(66, "Initialize...");	// 接個前一個狀態的進度，此處進度視為66%
	//
	// 開始載入資料
	//
	//
	// 最終進度為100%
	//
	ShowInitProgress(100, "OK!");

	Sleep(1000);
}

void CGameStateOver::OnShow()
{

}