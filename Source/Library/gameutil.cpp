//#define	 INITGUID
#include "stdafx.h"
#include "../Core/StdAfx.h" // prevent sometimes intelliscense bugged
#include "../Core/game.h"
#include "../Core/MainFrm.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <direct.h>
#include <string.h>
#include "audio.h"
#include "gameutil.h"
#include "gamecore.h"
#include "Shlwapi.h"
#include "../Game/config.h"
#include "../Game/mygame.h"
#include <filesystem>
#include <experimental/filesystem> // Header file for pre-standard implementation
#include <comdef.h>

namespace game_framework {

	/////////////////////////////////////////////////////////////////////////////
	// CMovingBitmap: Moving Bitmap class
	// 這個 class 提供可以移動的圖形
	// 要懂得怎麼呼叫(運用)其各種能力，但是可以不懂下列的程式是什麼意思
	/////////////////////////////////////////////////////////////////////////////

	CMovingBitmap::CMovingBitmap()
	{
		isBitmapLoaded = false;
	}


	void CMovingBitmap::LoadBitmap(int IDB_BITMAP, COLORREF color)
	{
		const int nx = 0;
		const int ny = 0;

		// GAME_ASSERT(!isBitmapLoaded, "A bitmap has been loaded. You can not load another bitmap !!!");
		CBitmap bitmap;
		BOOL rval = bitmap.LoadBitmap(IDB_BITMAP);
		GAME_ASSERT(rval, "Load bitmap failed !!! Please check bitmap ID (IDB_XXX).");
		BITMAP bitmapSize;
		bitmap.GetBitmap(&bitmapSize);
		location.left = nx; location.top = ny;
		location.right = nx + bitmapSize.bmWidth;
		location.bottom = ny + bitmapSize.bmHeight;
		_bitmap_size.push_back({ bitmapSize.bmWidth ,bitmapSize.bmHeight });
		SurfaceID.push_back(CDDraw::RegisterBitmap(IDB_BITMAP, color));
		isBitmapLoaded = true;
	}

	void CMovingBitmap::LoadBitmap(char* filename, COLORREF color)
	{
		const int nx = 0;
		const int ny = 0;
		//  GAME_ASSERT(!isBitmapLoaded, "A bitmap has been loaded. You can not load another bitmap !!!");
		HBITMAP hbitmap = ( HBITMAP ) LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if ( hbitmap == NULL ) {
			char error_msg[ 300 ];
			sprintf(error_msg, "Loading bitmap	from file \"%s\" failed !!!", filename);
			GAME_ASSERT(false, error_msg);
		}
		CBitmap* bmp = CBitmap::FromHandle(hbitmap); // memory will be deleted automatically
		BITMAP bitmapSize;
		bmp->GetBitmap(&bitmapSize);
		location.left = nx; location.top = ny;
		location.right = nx + bitmapSize.bmWidth;
		location.bottom = ny + bitmapSize.bmHeight;
		_bitmap_size.push_back({ bitmapSize.bmWidth ,bitmapSize.bmHeight });
		SurfaceID.push_back(CDDraw::RegisterBitmap(filename, color));
		isBitmapLoaded = true;

		bmp->DeleteObject();
	}

	void CMovingBitmap::LoadBitmap(vector<char*> filename, COLORREF color)
	{
		for ( int i = 0; i < ( int ) filename.size(); i++ ) {
			LoadBitmap(filename[ i ], color);
		}
	}

	void CMovingBitmap::LoadBitmapByString(vector<string> filename, COLORREF color)
	{

		for ( int i = 0; i < ( int ) filename.size(); i++ ) {
			LoadBitmap(( char* ) filename[ i ].c_str(), color);
		}
	}

	void CMovingBitmap::UnshowBitmap()
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before SetTopLeft() is called !!!");
		isAnimation = false;
		this->ShowBitmap(0);
	}

	void CMovingBitmap::SetTopLeft(int x, int y)
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before SetTopLeft() is called !!!");
		int dx = location.left - x;
		int dy = location.top - y;
		location.left = x;
		location.top = y;
		location.right -= dx;
		location.bottom -= dy;
	}

	void CMovingBitmap::SetAnimation(int delay, bool _once, int cooldown) {
		if ( !_once ) isAnimation = true;
		once = _once;
		delayCount = delay;
		animation_cooldown = cooldown;
	}

	void CMovingBitmap::ShowBitmap()
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before ShowBitmap() is called !!!");
		CDDraw::BltBitmapToBack(SurfaceID[ selector ], location.left, location.top);
		clock_t now = clock();
		if ( animation_cooldown != 0 && ( ( now - last_animation_done ) > animation_cooldown ) && ( isAnimation == false ) ) {
			isAnimation = true;
			selector = 0;
			return;
		}
		if ( isAnimation == true && now - last_time >= delayCount ) {
			selector += 1;
			last_time = now;
			if ( selector == SurfaceID.size() && animation_cooldown != 0 ) {
				selector = SurfaceID.size() - 1;
				last_animation_done = now;
				isAnimation = false;
				return;
			}
			if ( selector == SurfaceID.size() && animationCount > 0 ) {
				animationCount -= 1;
			}
			if ( selector == SurfaceID.size() && ( once || animationCount == 0 ) ) {
				isAnimation = false;
				isAnimationDone = true;
				selector = SurfaceID.size() - 1;
				return;
			}
			selector = selector % SurfaceID.size();
		}
	}

	void CMovingBitmap::ShowBitmap(double factor)
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before ShowBitmap() is called !!!");
		CDDraw::BltBitmapToBack(SurfaceID[ selector ], location.left, location.top, factor);
		clock_t now = clock();
		if ( animation_cooldown != 0 && ( ( now - last_animation_done ) > animation_cooldown ) && ( isAnimation == false ) ) {
			isAnimation = true;
			selector = 0;
			return;
		}
		if ( isAnimation == true && now - last_time >= delayCount ) {
			selector += 1;
			last_time = now;
			if ( selector == SurfaceID.size() && animation_cooldown != 0 ) {
				selector = SurfaceID.size() - 1;
				last_animation_done = now;
				isAnimation = false;
				return;
			}
			if ( selector == SurfaceID.size() && animationCount > 0 ) {
				animationCount -= 1;
			}
			if ( selector == SurfaceID.size() && ( once || animationCount == 0 ) ) {
				isAnimation = false;
				isAnimationDone = true;
				selector = SurfaceID.size() - 1;
				return;
			}
			selector = selector % SurfaceID.size();
		}
	}
	void CMovingBitmap::ShowBitmap(double factor, bool is_mirror) //important msg!! it only work on my laptop for now
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before ShowBitmap() is called !!!");
		CDDraw::BltBitmapToBack(SurfaceID[ selector ], location.left, location.top, factor, is_mirror);
		clock_t now = clock();
		if ( animation_cooldown != 0 && ((now - last_animation_done) > animation_cooldown)  && (isAnimation == false )) {
			isAnimation = true;
			selector = 0;
			return;
		}
		if ( isAnimation == true && now - last_time >= delayCount ) {
			selector += 1;
			last_time = now;
			if ( selector == SurfaceID.size() && animation_cooldown != 0 ) {
				selector = SurfaceID.size() - 1;
				last_animation_done = now;
				isAnimation = false;
				return;
			}
			if ( selector == SurfaceID.size() && animationCount > 0 ) {
				animationCount -= 1;
			}
			if ( selector == SurfaceID.size() && ( once || animationCount == 0 ) ) {
				isAnimation = false;
				isAnimationDone = true;
				selector = SurfaceID.size() - 1;
				return;
			}
			selector = selector % SurfaceID.size();
		}
	}


	void CMovingBitmap::SelectShowBitmap(int _select) {
		GAME_ASSERT(_select < ( int ) SurfaceID.size(), "選擇圖片時索引出界");
		selector = _select;
	}

	int CMovingBitmap::GetSelectShowBitmap() {
		return selector;
	}

	int CMovingBitmap::Top()
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before Top() is called !!!");
		return location.top;
	}

	int CMovingBitmap::Width()
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before Width() is called !!!");
		return _bitmap_size[ selector ].cx;
	}

	int CMovingBitmap::Height()
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before Height() is called !!!");
		return _bitmap_size[selector].cy;
	}

	int CMovingBitmap::Left()
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before Left() is called !!!");
		return location.left;
	}
	void CMovingBitmap::ToggleAnimation() {
		selector = 0;
		isAnimation = true;
		isAnimationDone = false;
	}

	bool CMovingBitmap::IsAnimationDone() {
		return isAnimationDone;
	}

	int CMovingBitmap::GetMovingBitmapFrame() {
		return ( int ) SurfaceID.size();
	}


	/////////////////////////////////////////////////////////////////////////////
	// CTextDraw: The class provide the ability to draw the text.
	// 這個 class 提供文字的呈現
	// 要懂得怎麼呼叫(運用)其各種能力，但是可以不懂下列的程式是什麼意思
	/////////////////////////////////////////////////////////////////////////////

	void CTextDraw::Print(CDC* pDC, int x, int y, string str) {
		x = CDDraw::IsFullScreen() ? x + ( RESOLUTION_X - SIZE_X ) / 2 : x;
		y = CDDraw::IsFullScreen() ? y + ( RESOLUTION_Y - SIZE_Y ) / 2 : y;
		pDC->TextOut(x, y, str.c_str());
	}

	void CTextDraw::ChangeFontLog(CDC* pDC, CFont*& fp, int size, string fontName, int weight) {
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 255, 255));
		LOGFONT lf;
		CFont f;
		memset(&lf, 0, sizeof(lf));
		lf.lfHeight = size;
		lf.lfWeight = weight;
		strcpy(lf.lfFaceName, fontName.c_str());
		f.CreateFontIndirect(&lf);
		fp = pDC->SelectObject(&f);
	}

}