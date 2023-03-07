#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "VSObject.h"
#include "Passive.h"
Passive::Passive()
{
}
Passive::~Passive()
{
}
void Passive::level_up()
{
	this->_level = _level + 1;
}
