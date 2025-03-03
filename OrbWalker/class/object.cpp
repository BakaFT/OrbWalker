#include "pch.hpp"

#include "object.hpp"

Object *Object::self() { return Read<Object *>(RVA(oLocalPlayer)); }

Object *Object::obj_under_mouse() { return Read<Object *>(Read<uptr>(RVA(oObjUnderMouse)) + 0x18); }

u32 Object::team() { return call_virtual<59, u32>(this); }

vec3 Object::position() { return MEMBER<vec3>(objPosition); }

bool Object::visible() { return MEMBER<bool>(objVisible); }

float Object::mana() { return MEMBER<float>(objMana); }

bool Object::targetable() { return MEMBER<bool>(objTargetable) && call_function<bool>(RVA(oTargetableToTeam), this); }

float Object::health() { return MEMBER<float>(objHealth); }

float Object::max_health() { return MEMBER<float>(objMaxHealth); }

float Object::attack_range() { return MEMBER<float>(objAttackRange); }

ActionState Object::state() { return MEMBER<ActionState>(objActionState); }

ObjectType Object::type() { return Read<ObjectType>(call_virtual<1, uptr>(this) + 0x8); }

DataStack *Object::dataStack() {
  if (type() == Inhibitor) return nullptr;
  return pMEMBER<DataStack>(objDataStack);
}

std::vector<Buff *> Object::buffs() {
  if (type() == Inhibitor) return {};
  auto begin = MEMBER<Buff *>(objBuff), end = MEMBER<Buff *>(objBuff + 0x8);
  std::vector<Buff *> res;
  for (auto i = begin; i < end; i += 2) { res.push_back(i); }
  return res;
}

std::string Object::name() { return MEMBER<RiotString16>(objName).str(); }

// float Object::attackdamage() {
//   return prop<float>(0x166C) + prop<float>(0x15D8);
// }

float Object::AttackDelay() { return call_function<float>(RVA(oAttackDelay), this); }

float Object::AttackWindup() { return call_function<float>(RVA(oAttackDelay - 0x100), this, 0x40); }

float Object::BonusRadius() { return call_virtual<37, float>(this); }

void *Object::ops_base() { return pMEMBER<void *>(0x11B8); }

bool Object::IsAlive() { return call_virtual<134, bool>(this); }

bool Object::IsEnemy() { return team() != self()->team(); }

bool Object::IsValidTarget() {
  const auto flag = visible() && targetable() && IsEnemy() && IsAlive();
  if (type() != Minion || FNV(name()) == FNV("Cherry_Plant_Powerup")) return flag;
  else return flag && max_health() > 6.f;
}

bool Object::IsCasting() { return call_virtual<248, bool>(this); }

float Object::get_mana_cost(SpellSlot slot) {
  if (slot > SpellSlot_R) return 0.f;
  return MEMBER<float>(objManaCost + slot * 0x18ull);
}

bool Object::has_buff(hash_type name) { return call_virtual<58, bool>(this, 0, name); }

Spell *Object::GetSpell(SpellSlot slot) { return pMEMBER<Spell *>(objSpell)[slot]; }

Object *Object::GetOwner() { return call_function<Object *>(RVA(oGetOwner), this); }