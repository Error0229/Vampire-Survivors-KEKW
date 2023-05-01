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
#include <any>
#include <unordered_map>
#include "audio.h"
#include "gameutil.h"
#include "gamecore.h"
#include "Shlwapi.h"
#include "../Game/config.h"
#include "../Game/mygame.h"
#include <filesystem>
#include <experimental/filesystem> // Header file for pre-standard implementation
#include <comdef.h>
#include "../Game/VSclass/VSObject.h"

namespace game_framework {

	/////////////////////////////////////////////////////////////////////////////
	// CMovingBitmap: Moving Bitmap class
	// 這個 class 提供可以移動的圖形
	// 要懂得怎麼呼叫(運用)其各種能力，但是可以不懂下列的程式是什麼意思
	/////////////////////////////////////////////////////////////////////////////

	CMovingBitmap::CMovingBitmap()
	{
		isBitmapLoaded = false;
		if (ENABLE_FEATURE) {
			scaler = FIX_SCALE;
		}
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
		if (!_once) isAnimation = true;
		else 
			isPause = true;
		once = _once;
		delayCount = delay;
		animation_cooldown = cooldown;
	}

	void CMovingBitmap::ShowBitmap()
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before ShowBitmap() is called !!!");
		if (last_time == -1) {
			last_time = clock();
		}
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
		if (last_time == -1) {
			last_time = clock();
		}
		CDDraw::BltBitmapToBack(SurfaceID[ selector ], location.left, location.top, factor + scaler);
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
		CDDraw::BltBitmapToBack(SurfaceID[ selector ], location.left, location.top, factor + scaler, is_mirror);
		if (last_time == -1) {
			last_time = clock();
		}
		clock_t now = clock();
		if ( animation_cooldown != 0 && ((now - last_animation_done) > animation_cooldown)  && (isAnimation == false )) {
			isAnimation = true;
			selector = 0;
			return;
		}
		if ( isAnimation == true && now - last_time >= delayCount  && !isPause) {
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
	void CMovingBitmap::SyncMirror(const CMovingBitmap& other){
		selector = other.selector;
		delayCount = other.delayCount;
		animation_cooldown = other.animation_cooldown;
		last_animation_done = other.last_animation_done;
		animationCount = other.animationCount;
		last_time = other.last_time;
		isAnimation = other.isAnimation;
		isAnimationDone = other.isAnimationDone;
		once = other.once;
		isPause = other.isPause;
		scaler = other.scaler;
	}

	void CMovingBitmap::SelectShowBitmap(int _select) {
		GAME_ASSERT(_select < ( int ) SurfaceID.size(), "choosing image out of range")
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
		return static_cast<int>(_bitmap_size[ selector ].cx * (1.0 + scaler));
	}

	int CMovingBitmap::Height()
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before Height() is called !!!");
		return static_cast<int>(_bitmap_size[selector].cy * (1.0 + scaler));
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
	void CMovingBitmap::EnableAnimation() {
		if (isPause) {
			isPause = false;
			// selector = 0;
			isAnimation = true;
			isAnimationDone = false;
		}
	}
	void CMovingBitmap::DisableAnimation() {
		if (!isPause) {
			isPause = true;
			// selector = 0;
			isAnimation = false;
			isAnimationDone = true;
		}
	}
	bool CMovingBitmap::IsAnimationDone() {
		return isAnimationDone;
	}
	bool CMovingBitmap::IsAnimation() {
		return isAnimation;
	}

	int CMovingBitmap::GetMovingBitmapFrame() {
		return ( int ) SurfaceID.size();
	}
	void CMovingBitmap::ResetBitmap() {
		this->isBitmapLoaded = false;
		this->SurfaceID.clear();
		this->_bitmap_size.clear();
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


Text::Text(string str, CPoint pos, int duration, int font_id, int align_id)
{
	this->str = str;
	this->pos = pos;
	this->font_id = font_id;
	this->align_id = align_id;
	this->duration = duration;
}
Text::~Text()
{
}
bool Text::is_remain()
{
	return duration--;
}


TextDevice::TextDevice()
{
	set_font(fonts[FONT_24x18_B], 24, 18, FW_BOLD, false, false);
	set_font(fonts[FONT_12x08], 12, 8, FW_BOLD, false, false);
	set_font(fonts[FONT_NORM], 12, 7, FW_BOLD, false, false);
}
TextDevice::~TextDevice()
{
	for (auto& i : fonts)
		i.cfont.DeleteObject();
}
void TextDevice::add_text(string str, CPoint pos, int duration, int font_id, int align_id)
{
	texts.emplace_back(Text(str, pos, duration, font_id, align_id));
}
void TextDevice::print_all()
{
	if (texts.empty())
		return;
	ptr_CDC = game_framework::CDDraw::GetBackCDC();
	ptr_CDC->SetBkMode(TRANSPARENT);
	ptr_CDC->SetTextColor(RGB(255, 255, 255));
	int x, y;
	Text* ptext;
	VS_font* pfont;
	RECT rc;
	for (int i = 0; i < (int)texts.size(); i++) {
		// offset position
		ptext = &texts.back();
		pfont = &fonts[ptext->font_id];
		if (ptext->align_id == ALIGN_LEFT)
			x = ptext->pos.x + VSObject::player_dx;
		else if (ptext->align_id == ALIGN_CENTER)
			x = ptext->pos.x - ((pfont->width * (ptext->str.size())) >> 1) + VSObject::player_dx;
		else if (ptext->align_id == ALIGN_RIGHT)
			x = ptext->pos.x - (pfont->width * ptext->str.size()) + VSObject::player_dx;
		else if (ptext->align_id == MULTILINE_LEFT)
			x = ptext->pos.x + VSObject::player_dx;
		y = ptext->pos.y - (pfont->height >> 1) + VSObject::player_dy;
		rc = { x, y, x + 250, y + 50 };

		// select font
		ptr_CDC->SelectObject(&(pfont->cfont));
		
		if (texts.back().is_remain()) {
			// textout
			if (ptext->align_id != MULTILINE_LEFT)
				ptr_CDC->ExtTextOutA(x, y, ETO_CLIPPED, NULL, ptext->str.c_str(), ptext->str.size(), NULL);
			else
				ptr_CDC->DrawText(ptext->str.c_str(), &rc, DT_LEFT | DT_EXTERNALLEADING | DT_WORDBREAK);
			texts.emplace_front(texts.back());
		}
		texts.pop_back();
	}
	if (!(clock() % 60000))
		texts.shrink_to_fit();
	game_framework::CDDraw::ReleaseBackCDC();
}
void TextDevice::set_font(VS_font& font, int height, int width, int weight, bool italic, bool underline, string font_name)
{
	font.height = height;
	font.width = width;
	font.cfont.CreateFont(
			height,						// nHeight
			width,						// nWidth
			0,							// nEscapement
			0,							// nOrientation
			weight,						// nWeight
			italic,						// bItalic
			underline,					// bUnderline
			0,							// cStrikeOut
			ANSI_CHARSET,				// nCharSet
			OUT_DEFAULT_PRECIS,			// nOutPrecision
			CLIP_DEFAULT_PRECIS,		// nClipPrecision
			PROOF_QUALITY,		// nQuality
			DEFAULT_PITCH | FF_MODERN,	// nPitchAndFamily
			font_name.c_str()			// lpszFacename
		);
}