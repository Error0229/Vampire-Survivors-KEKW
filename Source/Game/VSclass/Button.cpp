#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "VSObject.h"
#include "Button.h"
Button::Button() 
{
}
Button::~Button()
{
}
bool Button::is_hover(int mouse_x, int mouse_y) 
{
	return
	(
		_skin.Left() <= mouse_x &&
		_skin.Left() + _skin.Width() >= mouse_x &&
		_skin.Top() <= mouse_y &&
		_skin.Top() + _skin.Width()  >= mouse_y
	);
}

void Button::show_button() 
{
	VSObject::show_skin();
}

