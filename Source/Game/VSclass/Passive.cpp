#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "VSObject.h"
#include "Passive.h"
Passive::Passive()
{
	obj_type = PASSIVE;
}
Passive::Passive(int type) {
	_type = type;
	switch (type) {
	case POWER:
		_name = "Spinach";
		_rarity = 100;
		_level_up_msg = { "", "Raises inflicted damage by 10%" , "Base damage up by 10%" };
		_effect_by_level = { 0, 10, 20, 30, 40, 50 };
		_level = 1;
		_max_level = 5;
		_stacking = Additive;
		this->load_skin("Resources/Passive/Spinach.bmp");
		break;
	case ARMOR:
		_name = "Armor";
		_rarity = 100;
		_level_up_msg = { "", "Reduces incoming damage by 1. Increases retaliatory damage by 10%", "Reduces incoming damage by 1" };
		_effect_by_level = { 0, -1, -2, -3, -4, -5 };
		_level = 1;
		_max_level = 5;
		_stacking = Additive;
		this->load_skin("Resources/Passive/Armor.bmp");
		break;
	case MAXHEALTH:
		_name = "Hollow Heart";
		_rarity = 90;
		_level_up_msg = { "", "Augment max health by 20%", "Max health increases by 20%" };
		_effect_by_level = { 100, 120, 144, 173, 207, 249 };
		_level = 1;
		_max_level = 5;
		_stacking = Multiplicative;
		this->load_skin("Resources/Passive/Hollow Heart.bmp");
		break;
	case REGEN:
		_name = "Pummarola";
		_rarity = 90;
		_level_up_msg = { "", "Character recovers 0.2 HP per second", "Health recovery increases by 0.2 HP per second" };
		_effect_by_level = { 0, 20, 40, 60, 80, 100 };
		_level = 1;
		_max_level = 5;
		_stacking = Additive;
		this->load_skin("Resources/Passive/Pummarola.bmp");
		break;
	case COOLDOWN:
		_name = "Empty Tome";
		_rarity = 50;
		_level_up_msg = { "", "Reduces weapons cooldown by 8%", "Cooldown reduced by 8.0%" };
		_effect_by_level = { 0, -8, -16, -24, -32, -40 };
		_level = 1;
		_max_level = 5;
		_stacking = Additive;
		this->load_skin("Resources/Passive/Empty Tome.bmp");
		break;
	case AREA:
		_name = "Candelabrador";
		_rarity = 100;
		_level_up_msg = { "", "Augments area of attacks by 10%", "Base area up by 10%" };
		_effect_by_level = { 0, 10, 20, 30, 40, 50 };
		_level = 1;
		_max_level = 5;
		_stacking = Additive;
		this->load_skin("Resources/Passive/Candelabrador.bmp");
		break;
	case SPEED:
		_name = "Bracer";
		_rarity = 100;
		_level_up_msg = { "", "Increases projectiles speed by 10%", "Base speed up by 10%" };
		_effect_by_level = { 0, 10, 20, 30, 40, 50 };
		_level = 1;
		_max_level = 5;
		_stacking = Additive;
		this->load_skin("Resources/Passive/Bracer.bmp");
		break;
	case DURATION:
		_name = "Spellbinder";
		_rarity = 100;
		_level_up_msg = { "", "Increases duration of weapon effects by 10%", "Effect lasts 10% longer" };
		_effect_by_level = { 0, 10, 20, 30, 40, 50 };
		_level = 1;
		_max_level = 5;
		_stacking = Additive;
		this->load_skin("Resources/Passive/Spellbinder.bmp");
		break;
	case AMOUNT:
		_name = "Duplicator";
		_rarity = 50;
		_level_up_msg = { "", "Weapons fire more projectiles" ,"Fires 1 more projectile" };
		_effect_by_level = { 0, 1, 2 };
		_level = 1;
		_max_level = 2;
		_stacking = Additive;
		this->load_skin("Resources/Passive/Duplicator.bmp");
		break;
	case MOVESPEED:
		_name = "Wings";
		_rarity = 50;
		_level_up_msg = { "", "Character moves 10% faster",  "Movement speed increases by 10%" };
		_effect_by_level = { 0, 10, 20, 30, 40, 50 };
		_level = 1;
		_max_level = 5;
		_stacking = Additive;
		this->load_skin("Resources/Passive/Wings.bmp");
		break;
	case MAGNET:
		_name = "Attractorb";
		_rarity = 100;
		_level_up_msg = { "", "Character picks up items from further away",
			"Pickup range increased by 33%", "Pickup range increased by 25%",
			"Pickup range increased by 20%", "Pickup range increased by 33%" };
		_effect_by_level = { 0, 150, 200, 250, 300, 400 };
		_level = 1;
		_max_level = 5;
		_stacking = Additive;
		this->load_skin("Resources/Passive/Attractorb.bmp");
		break;
	case LUCK:
		_name = "Clover";
		_rarity = 100;
		_level_up_msg = { "", "Character gets 10% luckier" , "Chance to get lucky up by 10%" };
		_effect_by_level = { 0, 10, 20, 30, 40, 50 };
		_level = 1;
		_max_level = 5;
		_stacking = Additive;
		this->load_skin("Resources/Passive/Clover.bmp");
		break;
	case GROWTH:
		_name = "Crown";
		_rarity = 70;
		_level_up_msg = { "", "Character gains 8% more experience", "XP gain increases by 8%" };
		_effect_by_level = { 0, 8, 16, 24, 32, 40 };
		_level = 1;
		_max_level = 5;
		_stacking = Additive;
		this->load_skin("Resources/Passive/Crown.bmp");
		break;
	case GREED:
		_name = "Stone Mask";
		_rarity = 100;
		_level_up_msg = { "", "Character earns 10% more coins", "Coins value increases by 10%" };
		_effect_by_level = { 0, 10, 20, 30, 40, 50 };
		_level = 1;
		_max_level = 5;
		_stacking = Additive;
		this->load_skin("Resources/Passive/Mask.bmp");
		break;
	case REVIVAL:
		_name = "Tiramisu";
		_rarity = 40;
		_level_up_msg = { "", "Revives once with 50% health", "Adds 1 Revival" };
		_effect_by_level = { 0, 1, 2 };
		_level = 1;
		_max_level = 2;
		_stacking = Additive;
		this->load_skin("Resources/Passive/Tiramisu.bmp");
		break;
	case CURSE:
		_name = "Skull O\'Maniac";
		_rarity = 40;
		_level_up_msg = { "", "Increases enemy speed, health, quantity, and frequency by 10%", "Increases enemy speed, health, quantity, and frequency by 10%" };
		_effect_by_level = { 0, 10, 20, 30, 40, 50 };
		_level = 1;
		_max_level = 5;
		_stacking = Additive;
		this->load_skin("Resources/Passive/Skull OManiac.bmp");
		break;
	case SILVER:
		_name = "Silver Ring";
		_rarity = 10;
		_level_up_msg = {"", "Wear ... Clock ...", "Effect last 5% longer. Base area up by 5%" };
		_effect_by_level = { 0, 0, 5, 10, 15, 20, 25, 30, 35, 40 };
		_level = 1;
		_max_level = 9;
		_stacking = Additive;
		this->load_skin("Resources/Passive/Silver Ring.bmp");
		break;
	case GOLD:
		_name = "Gold Ring";
		_rarity = 10;
		_level_up_msg = {"", "... With ... Lancet", "Increases enemy speed, health, quantity, and frequency by 5%" };
		_effect_by_level = { 0, 0, 5, 10, 15, 20, 25, 30, 35, 40 };
		_level = 1;
		_max_level = 9;
		_stacking = Additive;
		this->load_skin("Resources/Passive/Gold Ring.bmp");
		break;
	case pLEFT:
		_name = "Metaglio Left";
		_rarity = 10;
		_level_up_msg = {"", "Channels dark powers to protect the bearer", "Health recovery increases by 0.1 HP per second. Max Health increases by 5%" };
		_effect_by_level = { 0, 0, 105, 110, 116, 122, 128, 134, 140, 148 };
		_alt_effect = { 0, 0, 10, 20, 30, 40, 50, 60, 70, 80 };
		_level = 1;
		_max_level = 9;
		_stacking = Additive | Multiplicative;
		this->load_skin("Resources/Passive/Metaglio Left.bmp");
		break;
	case pRIGHT:
		_name = "Metaglio Right";
		_rarity = 10;
		_level_up_msg = {"", "Channels dark powers to curse the bearer", "Increases enemy speed, health, quantity, and frequency by 5%" };
		_effect_by_level = { 0, 0, 5, 10, 15, 20, 25, 30, 35, 40 };
		_level = 1;
		_max_level = 9;
		_stacking = Additive;
		this->load_skin("Resources/Passive/Metaglio Right.bmp");
		break;
	case PANDORA:
		_name = "Torrona\'s Box";
		_rarity = 40;
		string msg = "Increases Might, Projectile Speed, Duration, and Area by 3%";
		_level_up_msg = {"", "Cursed item, but increases Might, Projectile Speed, Duration, and Area by 4%",
		msg, msg, msg, msg, msg, msg, msg, "Increases enemy speed, health, quantity, and frequency by 100%" };
		_effect_by_level = { 0, 4, 7, 10, 13, 16, 19, 22, 25, 100 };
		_level = 1;
		_max_level = 9;
		_stacking = Additive;
		this->load_skin("Resources/Passive/Torronas Box.bmp");
		break;
	}
}
Passive::~Passive()
{
}
void Passive::level_up()
{
	if (is_max_level()) {
		VS_ASSERT(false, "The level already max don\'t keep upgrading :(");
	}
	_level++;
}
int Passive::get_type()
{
	return _type;
}
int Passive::get_effect()
{
	return (_effect_by_level[_level] - _effect_by_level[_level - 1]);
}
int Passive::get_alt_effect()
{
	return _alt_effect[_level] - _alt_effect[_level - 1];
}
int Passive::get_level()
{
	return _level;
}
bool Passive::is_max_level()
{
	return (_level >= _max_level);
}
string Passive::get_level_up_msg(bool is_new)
{
	if (is_new)
		return _level_up_msg[1];
	if (_level + 1 > (int)_level_up_msg.size() - 1)
		return _level_up_msg[_level_up_msg.size() - 1];
	else
		return _level_up_msg[_level + 1];
}
int Passive::get_rarity()
{
	return _rarity;
}
int Passive::passive_count()
{
	return all_passive.size();
}

deque<Passive> Passive::all_passive;