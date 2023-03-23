/*
 * gamelib.h: 本檔案儲遊戲相關的class的interface
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * History:
 *   2002-03-04 V3.1
 *		1. Add ENABLE_AUDIO flag.
 *   2004-03-02 V4.0
 *		1. Add CInteger class.
 *      2. Add CGameState abstract class and adjust CGame to apply
 *         state pattern for switching game states.
 *   2004-03-08 V4.1
 *      1. Add OPEN_AS_FULLSCREEN flag.
 *      2. The Game Engine now becomes a framework.
 *   2005-07-28
 *      1. Add GAME_ASSERT macro to display run time errors gracefully.
 *      2. Remove CDDarw::CheckSystem(). It is useless.
 *   2005-09-08
 *      1. Eliminate CSpecialEffect::Abort. It is useless now.
 *      2. Add SHOW_GAME_CYCLE_TIME flag.
 *      3. Add DEFAULT_BG_COLOR flag;
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add MOUSEMOVE Handler for CGame and CGameState.
 *      2. Add _TRACE preprocessor flag for VC++.net.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Remove constantness of CAnimation::DELAY_COUNT.
 *      3. Enhance CAnimation to support SetDelayCount(), Reset(), and IsFinalBitmap().
 *      4. Remove CAnimation::GetLocation() and CMovingBitmap::GetLocation().
 *      5. Bitmap coordinate can no longer be set by CMovingBitmap::LoadBitmap().
 *         Defauts to (0,0).
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *      2. Fix bug: audio is now correctly recovered after a sleep or suspension of windows.
 *      3. Support ENABLE_GAME_PAUSE.
 *   2008-02-15 V4.4
 *      1. Add setup project for Visual studio 2005.
 *      2. Support bitmap scaling when ShowBitmap(scale) is called.
 *      3. Add namespace game_framework.
 *      4. Make the class CGame a singleton so that MFC can access it easily.
 *      5. Support loading of bitmap from bmp file.
 *      6. Support ShowInitProgress(percent) to display loading progress.
 *   2010-03-23 V4.6
 *      1. Rewrite CAudio with MCI commands to eliminate dependency with DirectMusic.
*/

/////////////////////////////////////////////////////////////////////////////
// Header for STL (Standard Template Library)
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "../Core/StdAfx.h" // prevent sometimes intelliscense bugged
#include <utility>
#include <deque>
#include <queue>
#include <set>
#include <list>
#include <vector>
#include <map>
using namespace std;

namespace game_framework {

	/////////////////////////////////////////////////////////////////////////////
	// 這個class提供動態(可以移動)的圖形
	// 每個Public Interface的用法都要懂，Implementation可以不懂
	/////////////////////////////////////////////////////////////////////////////

	class CMovingBitmap {
	public:
		CMovingBitmap();
		int   Height();						// 取得圖形的高度
		int   Left();						// 取得圖形的左上角的 x 座標
		void  SetAnimation(int delay, bool _once, int cooldown = 0);
		void  LoadBitmap(int, COLORREF = CLR_INVALID);		// 載入圖，指定圖的編號(resource)及透明色
		void  LoadBitmap(char*, COLORREF = CLR_INVALID);	// 載入圖，指定圖的檔名及透明色
		void  LoadBitmap(vector<char*>, COLORREF = CLR_INVALID);	// 載入圖，指定圖的檔名及透明色
		void  LoadBitmapByString(vector<string>, COLORREF = CLR_INVALID);	// 載入圖，指定圖的檔名及透明色
		void  UnshowBitmap();
		void  SetTopLeft(int, int);			// 將圖的左上角座標移至 (x,y)
		void  ShowBitmap();					// 將圖貼到螢幕
		void  ShowBitmap(double factor);	// 將圖貼到螢幕 factor < 1時縮小，>1時放大。注意：需要VGA卡硬體的支援，否則會很慢
		void  ShowBitmap(double factor, bool is_mirror);	
		void  SelectShowBitmap(int select);
		int   GetSelectShowBitmap();
		void  ToggleAnimation();
		int   Top();						// 取得圖形的左上角的 y 座標
		int   Width();						// 取得圖形的寬度
		bool  IsAnimationDone();
		int   GetMovingBitmapFrame();
	protected:
		int selector = 0;
		int delayCount = 10;
		int animation_cooldown  = 0;
		clock_t last_animation_done = 0;
		int animationCount = -1;
		clock_t last_time = -1;
		bool isAnimation = false;
		bool isAnimationDone = false;
		bool once = false;
		vector<unsigned> SurfaceID;
		bool     isBitmapLoaded = false;	// whether a bitmap has been loaded
		CRect    location;			// location of the bitmap
		vector<CSize>	_bitmap_size;		// size of the bitmap
	};

	class CTextDraw {
	public:
		void static Print(CDC *pDC, int x, int y, string str);
		void static ChangeFontLog(CDC* pDC, CFont* &fp, int size, string fontName, int weight = 500);
	};

}