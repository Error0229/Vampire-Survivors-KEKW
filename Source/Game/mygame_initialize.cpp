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
// �o��class���C�����C���}�Y�e������
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame *g) : CGameState(g)
{
}

void CGameStateInit::OnInit()
{
	//
	// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
	//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
	//
	ShowInitProgress(0, "Start Initialize...");	// �@�}�l��loading�i�׬�0%
	//
	// �}�l���J���
	//
	//Sleep(1000);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
	//
	// ��OnInit�ʧ@�|����CGameStaterRun::OnInit()�A�ҥH�i���٨S��100%
	//
}

void CGameStateInit::OnBeginState()
{
	 // load backgrond (�}�l����)
	CMovingBitmap& button_start_bitmap = button_start.get_skin();
	button_start_bitmap.LoadBitmapByString({ "resources/button_start.bmp" }, RGB(255, 255, 255));
	button_start_bitmap.SetTopLeft((SIZE_X - button_start_bitmap.Width())/2, (SIZE_Y - button_start_bitmap.Height()) / 2);
	
	background.LoadBitmapByString({ "resources/background.bmp "}, RGB(255, 255, 255));
	background.SetTopLeft((SIZE_X - background.Width()) / 2, (SIZE_Y - background.Height()) / 2);

}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{

}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(button_start.is_hover(point.x, point.y))
		GotoGameState(GAME_STATE_RUN);		// ������GAME_STATE_RUN
}

void CGameStateInit::OnShow()
{
	// show background
	background.ShowBitmap();
	button_start.show_button();
}
