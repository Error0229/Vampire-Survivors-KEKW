#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include "Passive.h"
using namespace game_framework;
Passive::Passive()
{
}
Passive::~Passive()
{
}
void Passive::levelup()
{
	this->_level = _level + 1;
}
