#include "eniesLobby.h"

static int clamp(int value, int lower, int upper)
{
    if (value < lower) return lower;
    if (value > upper) return upper;
    return value;
}

static void valid(BattleContext &context)
{
    context.morale = clamp(context.morale, 0, 100);
    context.alarmLevel = clamp(context.alarmLevel, 0, 100);
    context.rescueProgress = clamp(context.rescueProgress, 0, 100);
    context.escapeProgress = clamp(context.escapeProgress, 0, 100);
    if (context.busterCallTimer < 0) context.busterCallTimer = 0;
}

static int myDiv(int numerator, int denominator)
{
    return (numerator + denominator - 1) / denominator;
}

/*
 * BattleContext
 */
BattleContext::BattleContext()
{
    turnCount = 0;
    morale = 0;
    alarmLevel = 0;
    rescueProgress = 0;
    escapeProgress = 0;
    busterCallTimer = 0;
    mainGateDestroyed = false;
    robinRescued = false;
    bridgeOpened = false;
    battleEnded = false;
    resultCode = "";
}

void BattleContext::nextTurn()
{
    turnCount = turnCount + 1;
}

/*
 * Character
 */
Character::Character()
{
    this->name = "";
    this->hp = 0;
    this->maxHp = 0;
    this->atk = 0;
    this->def = 0;
    this->speed = 0;
    this->energy = 0;
    this->alive = false;
}

Character::Character(string name, int hp, int atk, int def, int speed, int energy)
{
    this->name = name;
    this->hp = hp;
    this->maxHp = hp;
    this->atk = atk;
    this->def = def;
    this->speed = speed;
    this->energy = energy;
    this->alive = (hp > 0);
}

Character::~Character() {}

int Character::attack(Building *target, BattleContext &context)
{
    return 0;
}

int Character::specialSkill(Building *target, BattleContext &context)
{
    return 0;
}

void Character::endTurn(BattleContext &context) {}

int Character::receiveDamage(int incoming)
{
    int taken = incoming - def;
    if (taken < 0) taken = 0;
    hp = hp - taken;
    if (hp <= 0)
    {
        hp = 0;
        alive = false;
    }
    return taken;
}

int Character::receiveDamageBypass(int incoming, int ignorePercent)
{
    int reducedDef = myDiv(def * (100 - ignorePercent), 100);
    int taken = incoming - reducedDef;
    if (taken < 0) taken = 0;
    hp = hp - taken;
    if (hp <= 0)
    {
        hp = 0;
        alive = false;
    }
    return taken;
}

void Character::receiveHeal(int amount)
{
    hp = hp + amount;
    if (hp > maxHp) hp = maxHp;
    if (hp > 0) alive = true;
}

void Character::reduceSpeed(int decrement)
{
    speed = speed - decrement;
    if (speed < 0) speed = 0;
}

void Character::reduceDef(int decrement)
{
    def = def - decrement;
    if (def < 0) def = 0;
}

bool Character::isAlive() const { return this->alive; }
string Character::getName() const { return this->name; }
int Character::getHP() const { return this->hp; }
int Character::getMaxHp() const { return this->maxHp; }
int Character::getDef() const { return this->def; }
int Character::getSpeed() const { return this->speed; }
int Character::getEnergy() const { return this->energy; }
bool Character::isStrawHat() const { return false; }
bool Character::isCP9() const { return false; }

/*
 * StrawHat
 */
StrawHat::StrawHat() : Character() { bounty = 0; }

StrawHat::StrawHat(string name, int hp, int atk, int def, int speed, int energy, long long bounty)
    : Character(name, hp, atk, def, speed, energy)
{
    this->bounty = bounty;
}

bool StrawHat::isStrawHat() const { 
    return true; 
}

string StrawHat::str() const
{
    ostringstream out;
    out << "StrawHat[name=" << name << ", hp=" << hp << ", atk=" << atk<< ", def=" << def << ", speed=" << speed << ", energy=" << energy<< ", bounty=" << bounty << "]";
    return out.str();
}

/*
 * Luffy
 */
static int computeLuffyDamage(int atkVal, int currentHp, int maxHpVal)
{
    if (currentHp > maxHpVal * 50 / 100) return atkVal;
    if (currentHp > maxHpVal * 30 / 100) return myDiv(atkVal * 115, 100);
    return myDiv(atkVal * 130, 100);
}

Luffy::Luffy(string name, int hp, int atk, int def, int speed, int energy, long long bounty)
    : StrawHat(name, hp, atk, def, speed, energy, bounty)
{
    killedThisTurn = false;
}

int Luffy::attack(Character *target, BattleContext &context)
{
    int dmg = computeLuffyDamage(atk, hp, maxHp);
    int before = target->getHP();
    int dealt = target->receiveDamage(dmg);
    if (before > 0 && !target->isAlive())
    {
        context.morale = context.morale + 5;
        valid(context);
        killedThisTurn = true;
    }
    return dealt;
}

int Luffy::specialSkill(Character *target, BattleContext &context)
{
    if (energy < 20 || hp < myDiv(maxHp * 15, 100)) return 0;
    energy = energy - 20;
    int dmg = myDiv(atk * 200, 100);
    atk = atk + 15;
    speed = speed + 15;
    context.alarmLevel = context.alarmLevel + 10;
    valid(context);
    int selfHarm = myDiv(maxHp * 8, 100);
    hp = hp - selfHarm;
    if (hp <= 0)
    {
        hp = 0;
        alive = false;
    }
    int before = target->getHP();
    int dealt = target->receiveDamage(dmg);
    if (before > 0 && !target->isAlive())
    {
        context.morale = context.morale + 5;
        valid(context);
        killedThisTurn = true;
    }
    return dealt;
}

int Luffy::attack(Building *target, BattleContext &context)
{
    int dmg = computeLuffyDamage(atk, hp, maxHp);
    target->receiveDamage(dmg);
    return dmg;
}

int Luffy::specialSkill(Building *target, BattleContext &context)
{
    if (energy < 20 || hp < myDiv(maxHp * 15, 100)) return 0;
    energy = energy - 20;
    int dmg = myDiv(atk * 200, 100);
    atk = atk + 15;
    speed = speed + 15;
    context.alarmLevel = context.alarmLevel + 10;
    valid(context);
    int selfHarm = myDiv(maxHp * 8, 100);
    hp = hp - selfHarm;
    if (hp <= 0)
    {
        hp = 0;
        alive = false;
    }
    target->receiveDamage(dmg);
    return dmg;
}

void Luffy::endTurn(BattleContext &context)
{
    if (hp <= maxHp * 30 / 100)
    {
        context.morale = context.morale + 3;
        valid(context);
    }
    if (killedThisTurn) energy = energy + 5;
    killedThisTurn = false;
}

/*
 * Zoro
 */
Zoro::Zoro(string name, int hp, int atk, int def, int speed, int energy, long long bounty)
    : StrawHat(name, hp, atk, def, speed, energy, bounty)
{
    killedThisTurn = false;
}

int Zoro::attack(Character *target, BattleContext &context)
{
    int dmg = atk + myDiv(def * 20, 100);
    if (target->getHP() < myDiv(target->getMaxHp() * 40, 100))
    {
        dmg = myDiv(dmg * 115, 100);
    }
    int before = target->getHP();
    int dealt = target->receiveDamage(dmg);
    if (before > 0 && !target->isAlive())
    {
        context.morale = context.morale + 5;
        valid(context);
        killedThisTurn = true;
    }
    return dealt;
}

int Zoro::specialSkill(Character *target, BattleContext &context)
{
    if (energy < 15) return 0;
    energy = energy - 15;
    int dmg = myDiv(atk * 220, 100);
    if (target->getHP() < myDiv(target->getMaxHp() * 50, 100))
    {
        dmg = myDiv(dmg * 150, 100);
    }
    int before = target->getHP();
    int dealt = target->receiveDamage(dmg);
    if (before > 0 && !target->isAlive())
    {
        energy = energy + 8;
        context.morale = context.morale + 4;
        valid(context);
        killedThisTurn = true;
        context.morale = context.morale + 5;
        valid(context);
    }
    return dealt;
}

int Zoro::attack(Building *target, BattleContext &context)
{
    int dmg = atk + myDiv(def * 20, 100);
    target->receiveDamage(dmg);
    return dmg;
}

int Zoro::specialSkill(Building *target, BattleContext &context)
{
    if (energy < 15) return 0;
    energy = energy - 15;
    int dmg = myDiv(atk * 220, 100);
    target->receiveDamage(dmg);
    return dmg;
}

void Zoro::endTurn(BattleContext &context)
{
    if (killedThisTurn)
    {
        context.morale = context.morale + 6;
        valid(context);
        atk = myDiv(atk * 105, 100);
    }
    killedThisTurn = false;
}

/*
 * Sanji
 */
Sanji::Sanji(string name, int hp, int atk, int def, int speed, int energy, long long bounty)
    : StrawHat(name, hp, atk, def, speed, energy, bounty)
{
    killedThisTurn = false;
}

int Sanji::attack(Character *target, BattleContext &context)
{
    int dmg = atk + myDiv(speed * 50, 100);
    if (target->getDef() < def)
    {
        dmg = myDiv(dmg * 110, 100);
    }
    int before = target->getHP();
    int dealt = target->receiveDamage(dmg);
    if (before > 0 && !target->isAlive())
    {
        context.morale = context.morale + 5;
        valid(context);
        killedThisTurn = true;
    }
    return dealt;
}

int Sanji::specialSkill(Character *target, BattleContext &context)
{
    if (energy < 18) return 0;
    energy = energy - 18;
    int dmg = myDiv(atk * 210, 100);
    int before = target->getHP();
    int dealt = target->receiveDamage(dmg);
    bool slain = (before > 0 && !target->isAlive());
    if (slain)
    {
        context.morale = context.morale + 5;
        valid(context);
        killedThisTurn = true;
    }
    int reduction = 8;
    if (target->getName() == "Jabra") reduction = 12;
    target->reduceDef(reduction);
    return dealt;
}

int Sanji::attack(Building *target, BattleContext &context)
{
    int dmg = atk + myDiv(speed * 50, 100);
    target->receiveDamage(dmg);
    return dmg;
}

int Sanji::specialSkill(Building *target, BattleContext &context)
{
    if (energy < 18) return 0;
    energy = energy - 18;
    int dmg = myDiv(atk * 210, 100);
    target->receiveDamage(dmg);
    return dmg;
}

void Sanji::endTurn(BattleContext &context)
{
    if (killedThisTurn)
    {
        context.morale = context.morale + 8;
        valid(context);
        atk = myDiv(atk * 110, 100);
    }
    killedThisTurn = false;
}

/*
 * Nami
 */
Nami::Nami(string name, int hp, int atk, int def, int speed, int energy, long long bounty)
    : StrawHat(name, hp, atk, def, speed, energy, bounty)
{
    killedThisTurn = false;
}

int Nami::attack(Character *target, BattleContext &context)
{
    int before = target->getHP();
    int dealt = target->receiveDamageBypass(atk, 30);
    if (before > 0 && !target->isAlive())
    {
        context.morale = context.morale + 5;
        valid(context);
        killedThisTurn = true;
    }
    return dealt;
}

int Nami::specialSkill(Character *target, BattleContext &context)
{
    if (energy < 20) return 0;
    energy = energy - 20;
    int dmg = atk + 40;
    int before = target->getHP();
    int dealt = target->receiveDamage(dmg);
    if (before > 0 && !target->isAlive())
    {
        context.morale = context.morale + 5;
        valid(context);
        killedThisTurn = true;
    }
    target->reduceSpeed(10);
    context.busterCallTimer = context.busterCallTimer + 1;
    context.alarmLevel = context.alarmLevel - 5;
    valid(context);
    return dealt;
}

int Nami::attack(Building *target, BattleContext &context)
{
    int dmg = myDiv(atk * 50, 100);
    target->receiveDamage(dmg);
    return dmg;
}

int Nami::specialSkill(Building *target, BattleContext &context)
{
    if (energy < 20) return 0;
    energy = energy - 20;
    int dmg = myDiv((atk + 40) * 150, 100);
    target->receiveDamage(dmg);
    context.busterCallTimer = context.busterCallTimer + 1;
    context.alarmLevel = context.alarmLevel - 5;
    valid(context);
    return dmg;
}

void Nami::endTurn(BattleContext &context)
{
    if (killedThisTurn) energy = energy + 6;
    killedThisTurn = false;
}

/*
 * Chopper
 */
Chopper::Chopper(string name, int hp, int atk, int def, int speed, int energy, long long bounty)
    : StrawHat(name, hp, atk, def, speed, energy, bounty) {}

int Chopper::attack(Character *target, BattleContext &context)
{
    int before = target->getHP();
    int dealt = target->receiveDamage(atk);
    if (before > 0 && !target->isAlive())
    {
        context.morale = context.morale + 5;
        valid(context);
    }
    return dealt;
}

int Chopper::specialSkill(Character *target, BattleContext &context)
{
    if (energy < 15) return 0;
    energy = energy - 15;
    int healing = 35 + myDiv(atk * 50, 100);
    target->receiveHeal(healing);
    if (target->getName() == "Luffy")
    {
        context.morale = context.morale + 5;
        valid(context);
    }
    return 0;
}

int Chopper::attack(Building *target, BattleContext &context)
{
    target->receiveDamage(atk);
    return atk;
}

void Chopper::endTurn(BattleContext &context) {}

/*
 * Usopp
 */
Usopp::Usopp(string name, int hp, int atk, int def, int speed, int energy, long long bounty)
    : StrawHat(name, hp, atk, def, speed, energy, bounty)
{
    attackedThisTurn = false;
}

int Usopp::attack(Character *target, BattleContext &context)
{
    int dmg = atk;
    if (target->getSpeed() < 50)
    {
        dmg = myDiv(dmg * 120, 100);
    }
    int before = target->getHP();
    int dealt = target->receiveDamage(dmg);
    if (before > 0 && !target->isAlive())
    {
        context.morale = context.morale + 5;
        valid(context);
    }
    attackedThisTurn = true;
    return dealt;
}

int Usopp::specialSkill(Character *target, BattleContext &context)
{
    if (energy < 16) return 0;
    energy = energy - 16;
    int dmg = myDiv(atk * 80, 100);
    target->reduceSpeed(12);
    int before = target->getHP();
    int dealt = target->receiveDamage(dmg);
    if (before > 0 && !target->isAlive())
    {
        context.morale = context.morale + 5;
        valid(context);
    }
    context.escapeProgress = context.escapeProgress + 8;
    valid(context);
    attackedThisTurn = true;
    return dealt;
}

int Usopp::attack(Building *target, BattleContext &context)
{
    int dmg = myDiv(atk * 50, 100);
    target->receiveDamage(dmg);
    attackedThisTurn = true;
    return dmg;
}

int Usopp::specialSkill(Building *target, BattleContext &context)
{
    if (energy < 16) return 0;
    energy = energy - 16;
    int dmg = myDiv(atk * 80, 100);
    target->receiveDamage(dmg);
    context.escapeProgress = context.escapeProgress + 8;
    valid(context);
    attackedThisTurn = true;
    return dmg;
}

void Usopp::endTurn(BattleContext &context)
{
    if (attackedThisTurn)
    {
        context.morale = context.morale + 10;
        valid(context);
    }
    attackedThisTurn = false;
}

/*
 * Franky
 */
Franky::Franky(string name, int hp, int atk, int def, int speed, int energy, long long bounty)
    : StrawHat(name, hp, atk, def, speed, energy, bounty) {}

int Franky::attack(Character *target, BattleContext &context)
{
    int dmg = atk + myDiv(def * 30, 100);
    if (target->isCP9())
    {
        dmg = myDiv(dmg * 110, 100);
    }
    int before = target->getHP();
    int dealt = target->receiveDamage(dmg);
    if (before > 0 && !target->isAlive())
    {
        context.morale = context.morale + 5;
        valid(context);
    }
    return dealt;
}

int Franky::specialSkill(Character *target, BattleContext &context)
{
    if (energy >= 30)
    {
        energy -= 30;
    int dmg = myDiv(atk * 180, 100);

    if (target->getName() == "Lucci")
        dmg = myDiv(dmg * 120, 100);

    target->reduceSpeed(8);
        int before = target->getHP();
        int dealt = target->receiveDamage(dmg);
        if (before > 0 && !target->isAlive())
        {
            context.morale = context.morale + 5;
            valid(context);
        }
        return dealt;
    }
    else if (energy >= 20)
    {
        energy = energy - 20;
        int dmg = myDiv(atk * 180, 100);
        
        int before = target->getHP();
        int dealt = target->receiveDamage(dmg);
        if (before > 0 && !target->isAlive())
        {
            context.morale = context.morale + 5;
            valid(context);
        }
        return dealt;
    }
    return 0;
}

int Franky::attack(Building *target, BattleContext &context)
{
    int dmg = atk + myDiv(def * 30, 100);
    target->receiveDamage(dmg);
    return dmg;
}

int Franky::specialSkill(Building *target, BattleContext &context)
{
    if (energy < 30) return 0;
    energy = energy - 30;
    target->forceDestroy();
    return myDiv(atk * 120, 100);
}

void Franky::endTurn(BattleContext &context)
{
    if (hp > maxHp * 70 / 100)
    {
        def = def + 5;
    }
    if (hp < myDiv(maxHp * 30, 100))
    {
        atk = myDiv(atk * 110, 100);
    }
}

/*
 * CP9Agent
 */
CP9Agent::CP9Agent() : Character() { doriki = 0; }

CP9Agent::CP9Agent(string name, int hp, int atk, int def, int speed, int energy, int doriki)
    : Character(name, hp, atk, def, speed, energy)
{
    this->doriki = doriki;
}

bool CP9Agent::isCP9() const { return true; }

string CP9Agent::str() const
{
    ostringstream out;
    out << "CP9[name=" << name << ", hp=" << hp << ", atk=" << atk
        << ", def=" << def << ", speed=" << speed << ", energy=" << energy
        << ", doriki=" << doriki << "]";
    return out.str();
}

/*
 * Lucci
 */
Lucci::Lucci(string name, int hp, int atk, int def, int speed, int energy, int doriki)
    : CP9Agent(name, hp, atk, def, speed, energy, doriki) {}

int Lucci::attack(Character *target, BattleContext &context)
{
    int dmg = atk + doriki / 20;
    if (target->getHP() < myDiv(target->getMaxHp() * 50, 100))
    {
        dmg = myDiv(dmg * 120, 100);
    }
    int before = target->getHP();
    int dealt = target->receiveDamage(dmg);
    if (before > 0 && !target->isAlive())
    {
        context.morale = context.morale - 5;
        valid(context);
    }
    return dealt;
}

int Lucci::specialSkill(Character *target, BattleContext &context)
{
    if (energy < 25) return 0;
    energy = energy - 25;
    int dmg = myDiv(atk * 280, 100);
    int before = target->getHP();
    int dealt = target->receiveDamageBypass(dmg, 50);
    if (before > 0 && !target->isAlive())
    {
        context.morale = context.morale - 10;
        valid(context);
    }
    return dealt;
}

void Lucci::endTurn(BattleContext &context)
{
    if (hp < myDiv(maxHp * 40, 100))
    {
        atk = myDiv(atk * 105, 100);
    }
}

/*
 * Kaku
 */
Kaku::Kaku(string name, int hp, int atk, int def, int speed, int energy, int doriki)
    : CP9Agent(name, hp, atk, def, speed, energy, doriki) {}

int Kaku::attack(Character *target, BattleContext &context)
{
    int before = target->getHP();
    int dealt = target->receiveDamage(atk);
    if (before > 0 && !target->isAlive())
    {
        context.morale = context.morale - 5;
        valid(context);
    }
    return dealt;
}

int Kaku::specialSkill(Character *target, BattleContext &context)
{
    if (energy < 20) return 0;
    energy = energy - 20;
    int total = 0;
    int multipliers[3] = {120, 100, 80};
    for (int idx = 0; idx < 3; idx++)
    {
        if (!target->isAlive()) break;
        int dmg = myDiv(atk * multipliers[idx], 100);
        int before = target->getHP();
        int dealt = target->receiveDamage(dmg);
        total = total + dealt;
        if (before > 0 && !target->isAlive())
        {
            context.morale = context.morale - 5;
            valid(context);
        }
    }
    return total;
}

void Kaku::endTurn(BattleContext &context) {}

/*
 * Jabra
 */
Jabra::Jabra(string name, int hp, int atk, int def, int speed, int energy, int doriki)
    : CP9Agent(name, hp, atk, def, speed, energy, doriki) {}

int Jabra::attack(Character *target, BattleContext &context)
{
    int before = target->getHP();
    int dealt = target->receiveDamage(atk);
    if (before > 0 && !target->isAlive())
    {
        context.morale = context.morale - 5;
        valid(context);
    }
    return dealt;
}

int Jabra::specialSkill(Character *target, BattleContext &context)
{
    if (energy < 18) return 0;
    energy = energy - 18;
    int dmg = myDiv(atk * 150, 100);
    if (hp < myDiv(maxHp * 30, 100))
    {
        dmg = myDiv(dmg * 125, 100);
    }
    int before = target->getHP();
    int dealt = target->receiveDamage(dmg);
    if (before > 0 && !target->isAlive())
    {
        context.morale = context.morale - 5;
        valid(context);
    }
    return dealt;
}

void Jabra::endTurn(BattleContext &context) {}

/*
 * Blueno
 */
Blueno::Blueno(string name, int hp, int atk, int def, int speed, int energy, int doriki)
    : CP9Agent(name, hp, atk, def, speed, energy, doriki) {}

int Blueno::attack(Character *target, BattleContext &context)
{
    int before = target->getHP();
    int dealt = target->receiveDamage(atk);
    if (before > 0 && !target->isAlive())
    {
        context.morale = context.morale - 5;
        valid(context);
    }
    return dealt;
}

int Blueno::specialSkill(Character *target, BattleContext &context)
{
    if (energy < 15) return 0;
    energy = energy - 15;
    int dmg = myDiv(atk * 130, 100);
    if (hp > maxHp * 50 / 100)
    {
        dmg = dmg + 20;
    }
    else
    {
        dmg = dmg + 40;
    }
    int before = target->getHP();
    int dealt = target->receiveDamage(dmg);
    if (before > 0 && !target->isAlive())
    {
        context.morale = context.morale - 5;
        valid(context);
    }
    return dealt;
}

void Blueno::endTurn(BattleContext &context) {}

/*
 * Kalifa
 */
Kalifa::Kalifa(string name, int hp, int atk, int def, int speed, int energy, int doriki)
    : CP9Agent(name, hp, atk, def, speed, energy, doriki) {}

int Kalifa::attack(Character *target, BattleContext &context)
{
    int before = target->getHP();
    int dealt = target->receiveDamage(atk);
    if (before > 0 && !target->isAlive())
    {
        context.morale = context.morale - 5;
        valid(context);
    }
    return dealt;
}

int Kalifa::specialSkill(Character *target, BattleContext &context)
{
    if (energy < 18) return 0;
    energy = energy - 18;
    int dmg = myDiv(atk * 140, 100);
    int penalty = (target->getName() == "Nami") ? 12 : 8;
    int before = target->getHP();
    int dealt = target->receiveDamage(dmg);
    if (before > 0 && !target->isAlive())
    {
        context.morale = context.morale - 5;
        valid(context);
    }
    target->reduceSpeed(6);
    context.morale = context.morale - penalty;
    valid(context);
    return dealt;
}

void Kalifa::endTurn(BattleContext &context) {}

/*
 * Kumadori
 */
Kumadori::Kumadori(string name, int hp, int atk, int def, int speed, int energy, int doriki)
    : CP9Agent(name, hp, atk, def, speed, energy, doriki) {}

int Kumadori::attack(Character *target, BattleContext &context)
{
    int before = target->getHP();
    int dealt = target->receiveDamage(atk);
    if (before > 0 && !target->isAlive())
    {
        context.morale = context.morale - 5;
        valid(context);
    }
    return dealt;
}

int Kumadori::specialSkill(Character *target, BattleContext &context)
{
    if (energy < 16) return 0;
    energy = energy - 16;
    int dmg = 30 + doriki / 10;
    if (hp < myDiv(maxHp * 40, 100))
    {
        dmg = dmg + 25;
    }
    int before = target->getHP();
    int dealt = target->receiveDamage(dmg);
    if (before > 0 && !target->isAlive())
    {
        context.morale = context.morale - 5;
        valid(context);
    }
    return dealt;
}

void Kumadori::endTurn(BattleContext &context) {}

/*
 * Fukurou
 */
Fukurou::Fukurou(string name, int hp, int atk, int def, int speed, int energy, int doriki)
    : CP9Agent(name, hp, atk, def, speed, energy, doriki)
{
    isLowestTarget = false;
}

int Fukurou::attack(Character *target, BattleContext &context)
{
    int before = target->getHP();
    int dealt = target->receiveDamage(atk);
    if (before > 0 && !target->isAlive())
    {
        context.morale = context.morale - 5;
        valid(context);
    }
    return dealt;
}

int Fukurou::specialSkill(Character *target, BattleContext &context)
{
    if (energy < 14) return 0;
    energy = energy - 14;
    int dmg = myDiv(atk * 130, 100);
    if (isLowestTarget)
    {
        dmg = dmg + 20;
    }
    int before = target->getHP();
    int dealt = target->receiveDamage(dmg);
    if (before > 0 && !target->isAlive())
    {
        context.morale = context.morale - 6;
        valid(context);
    }
    return dealt;
}

void Fukurou::endTurn(BattleContext &context) {}

/*
 * Building
 */
Building::Building(string name, int hp)
{
    this->name = name;
    this->hp = hp;
    this->maxHP = hp;
    this->destroyed = (hp <= 0);
    this->onDestroyedDone = false;
}

Building::~Building() {}

void Building::receiveDamage(int amount)
{
    hp = hp - amount;
    if (hp <= 0)
    {
        hp = 0;
        destroyed = true;
    }
}

void Building::forceDestroy()
{
    hp = 0;
    destroyed = true;
}

bool Building::isDestroyed() const { return destroyed; }

string Building::getName() const { return name; }

void Building::onDestroyed(BattleContext &context) {}

string Building::str() const
{
    ostringstream out;
    out << "Building[name=" << name << ",hp=" << hp << ",maxHP=" << maxHP
        << ",destroyed=" << (destroyed ? "true" : "false") << "]";
    return out.str();
}

/*
 * MainGate
 */
MainGate::MainGate(string name, int hp) : Building(name, hp) {}

void MainGate::applyEffect(BattleContext &context) {}

void MainGate::onDestroyed(BattleContext &context)
{
    if (onDestroyedDone) return;
    onDestroyedDone = true;
    context.mainGateDestroyed = true;
    context.rescueProgress += 20;
    context.morale += 5;
    valid(context);
}

/*
 * Courthouse
 */
Courthouse::Courthouse(string name, int hp) : Building(name, hp) {}

void Courthouse::applyEffect(BattleContext &context)
{
    if (!destroyed)
    {
        context.alarmLevel = context.alarmLevel + 5;
        valid(context);
    }
}

void Courthouse::onDestroyed(BattleContext &context)
{
    if (onDestroyedDone) return;
    onDestroyedDone = true;
    context.alarmLevel = context.alarmLevel - 20;
    valid(context);
}

/*
 * TowerOfJustice
 */
TowerOfJustice::TowerOfJustice(string name, int hp) : Building(name, hp) {}

void TowerOfJustice::applyEffect(BattleContext &context)
{
    if (context.mainGateDestroyed && !context.robinRescued)
    {
        context.rescueProgress = context.rescueProgress + 5;
        valid(context);
    }
    if (context.rescueProgress >= 100)
    {
        context.robinRescued = true;
        context.morale = context.morale + 10;
        valid(context);
    }
}

/*
 * BridgeOfHesitation
 */
BridgeOfHesitation::BridgeOfHesitation(string name, int hp) : Building(name, hp) {}

void BridgeOfHesitation::applyEffect(BattleContext &context)
{
    if (context.robinRescued)
    {
        context.bridgeOpened = true;
        context.escapeProgress += 5;
        valid(context);
    }

    if (context.escapeProgress >= 100)
    {
        context.battleEnded = true;
        context.resultCode = "STRAW_HAT_WIN";
    }
}

/*
 * BusterCallShip
 */
BusterCallShip::BusterCallShip(string name, int hp) : Building(name, hp) {}

void BusterCallShip::applyEffect(BattleContext &context)
{
    if (!destroyed)
    {
        context.busterCallTimer = context.busterCallTimer - 1;
        if (context.busterCallTimer < 0) context.busterCallTimer = 0;
        if (context.busterCallTimer <= 0)
        {
            context.battleEnded = true;
            context.resultCode = "BUSTER_CALL";
        }
    }
}

void BusterCallShip::onDestroyed(BattleContext &context)
{
    if (onDestroyedDone) return;
    onDestroyedDone = true;
    context.busterCallTimer = context.busterCallTimer + 3;
}

/*
 * EniesLobbyBattle
 */
EniesLobbyBattle::EniesLobbyBattle(const string &filename)
{
    strawHats = new Character *[7];
    cp9Agents = new Character *[7];
    buildings = new Building *[5];
    strawHatCount = 0;
    cp9Count = 0;
    buildingCount = 0;
    turnOrder = nullptr;
    maxTurns = 100;
    loadFromFile(filename);
}

EniesLobbyBattle::~EniesLobbyBattle()
{
    for (int i = 0; i < strawHatCount; i++) delete strawHats[i];
    for (int i = 0; i < cp9Count; i++) delete cp9Agents[i];
    for (int i = 0; i < buildingCount; i++) delete buildings[i];
    delete[] strawHats;
    delete[] cp9Agents;
    delete[] buildings;
    TurnNode *current = turnOrder;
    while (current)
    {
        TurnNode *nextNode = current->next;
        delete current;
        current = nextNode;
    }
}

void EniesLobbyBattle::loadFromFile(const string &filename)
{
    ifstream input(filename);
    string line;
    while (getline(input, line))
    {
        if (line.empty()) continue;
        istringstream parser(line);
        string tag;
        parser >> tag;
        if (tag == "CONTEXT")
        {
            int m, a, r, e, bt, mt;
            parser >> m >> a >> r >> e >> bt >> mt;
            context.morale = m;
            context.alarmLevel = a;
            context.rescueProgress = r;
            context.escapeProgress = e;
            context.busterCallTimer = bt;
            maxTurns = mt;
        }
        else if (tag == "STRAW_HAT")
        {
            string charName;
            int hp, atk, def, spd, eng;
            long long bountyVal;
            parser >> charName >> hp >> atk >> def >> spd >> eng >> bountyVal;
            Character *newChar = nullptr;
            if (charName == "Luffy")
                newChar = new Luffy(charName, hp, atk, def, spd, eng, bountyVal);
            else if (charName == "Zoro")
                newChar = new Zoro(charName, hp, atk, def, spd, eng, bountyVal);
            else if (charName == "Sanji")
                newChar = new Sanji(charName, hp, atk, def, spd, eng, bountyVal);
            else if (charName == "Nami")
                newChar = new Nami(charName, hp, atk, def, spd, eng, bountyVal);
            else if (charName == "Chopper")
                newChar = new Chopper(charName, hp, atk, def, spd, eng, bountyVal);
            else if (charName == "Usopp")
                newChar = new Usopp(charName, hp, atk, def, spd, eng, bountyVal);
            else if (charName == "Franky")
                newChar = new Franky(charName, hp, atk, def, spd, eng, bountyVal);
            if (newChar) addStrawHat(newChar);
        }
        else if (tag == "CP9")
        {
            string charName;
            int hp, atk, def, spd, eng, dorikiVal;
            parser >> charName >> hp >> atk >> def >> spd >> eng >> dorikiVal;
            Character *newChar = nullptr;
            if (charName == "Lucci")
                newChar = new Lucci(charName, hp, atk, def, spd, eng, dorikiVal);
            else if (charName == "Kaku")
                newChar = new Kaku(charName, hp, atk, def, spd, eng, dorikiVal);
            else if (charName == "Jabra")
                newChar = new Jabra(charName, hp, atk, def, spd, eng, dorikiVal);
            else if (charName == "Blueno")
                newChar = new Blueno(charName, hp, atk, def, spd, eng, dorikiVal);
            else if (charName == "Kalifa")
                newChar = new Kalifa(charName, hp, atk, def, spd, eng, dorikiVal);
            else if (charName == "Kumadori")
                newChar = new Kumadori(charName, hp, atk, def, spd, eng, dorikiVal);
            else if (charName == "Fukurou")
                newChar = new Fukurou(charName, hp, atk, def, spd, eng, dorikiVal);
            if (newChar) addCP9Agent(newChar);
        }
        else if (tag == "BUILDING")
        {
            string buildingName;
            int hpVal;
            parser >> buildingName >> hpVal;
            Building *newBuilding = nullptr;
            if (buildingName == "MainGate")
                newBuilding = new MainGate(buildingName, hpVal);
            else if (buildingName == "Courthouse")
                newBuilding = new Courthouse(buildingName, hpVal);
            else if (buildingName == "TowerOfJustice")
                newBuilding = new TowerOfJustice(buildingName, hpVal);
            else if (buildingName == "BridgeOfHesitation")
                newBuilding = new BridgeOfHesitation(buildingName, hpVal);
            else if (buildingName == "BusterCallShip")
                newBuilding = new BusterCallShip(buildingName, hpVal);
            if (newBuilding) addBuilding(newBuilding);
        }
    }
    buildTurnOrder();
}

void EniesLobbyBattle::addStrawHat(Character *character)
{
    if (strawHatCount < 7) strawHats[strawHatCount++] = character;
}

void EniesLobbyBattle::addCP9Agent(Character *character)
{
    if (cp9Count < 7) cp9Agents[cp9Count++] = character;
}

void EniesLobbyBattle::addBuilding(Building *building)
{
    if (buildingCount < 5) buildings[buildingCount++] = building;
}

void EniesLobbyBattle::buildTurnOrder()
{
    TurnNode *walker = turnOrder;
    while (walker)
    {
        TurnNode *toDelete = walker;
        walker = walker->next;
        delete toDelete;
    }
    turnOrder = nullptr;
    TurnNode *lastNode = nullptr;
    for (int i = 0; i < strawHatCount; i++)
    {
        TurnNode *node = new TurnNode();
        node->data = strawHats[i];
        node->next = nullptr;
        if (!turnOrder)
        {
            turnOrder = node;
            lastNode = node;
        }
        else
        {
            lastNode->next = node;
            lastNode = node;
        }
    }
    for (int i = 0; i < cp9Count; i++)
    {
        TurnNode *node = new TurnNode();
        node->data = cp9Agents[i];
        node->next = nullptr;
        if (!turnOrder)
        {
            turnOrder = node;
            lastNode = node;
        }
        else
        {
            lastNode->next = node;
            lastNode = node;
        }
    }
}

Building *EniesLobbyBattle::findBuilding(const string &targetName)
{
    for (int i = 0; i < buildingCount; i++)
    {
        if (buildings[i]->getName() == targetName) return buildings[i];
    }
    return nullptr;
}

Character *EniesLobbyBattle::firstAliveCP9()
{
    for (int i = 0; i < cp9Count; i++)
    {
        if (cp9Agents[i]->isAlive()) return cp9Agents[i];
    }
    return nullptr;
}

Character *EniesLobbyBattle::firstAliveStrawHat()
{
    for (int i = 0; i < strawHatCount; i++)
    {
        if (strawHats[i]->isAlive()) return strawHats[i];
    }
    return nullptr;
}

Character *EniesLobbyBattle::lowestHpStrawHat()
{
    Character *lowest = nullptr;
    for (int i = 0; i < strawHatCount; i++)
    {
        if (strawHats[i]->isAlive())
        {
            if (!lowest || strawHats[i]->getHP() < lowest->getHP())
            {
                lowest = strawHats[i];
            }
        }
    }
    return lowest;
}

Character *EniesLobbyBattle::lowestHpStrawHatForCP9()
{
    return lowestHpStrawHat();
}

int EniesLobbyBattle::getSkillCost(Character *c)
{
    string id = c->getName();
    if (id == "Luffy") return 20;
    if (id == "Zoro") return 15;
    if (id == "Sanji") return 18;
    if (id == "Nami") return 20;
    if (id == "Chopper") return 15;
    if (id == "Usopp") return 16;
    if (id == "Franky") return 30;
    if (id == "Lucci") return 25;
    if (id == "Kaku") return 20;
    if (id == "Jabra") return 18;
    if (id == "Blueno") return 15;
    if (id == "Kalifa") return 18;
    if (id == "Kumadori") return 16;
    if (id == "Fukurou") return 14;
    return 0;
}

void EniesLobbyBattle::runBattle()
{
    while (!context.battleEnded && context.turnCount < maxTurns)
    {
        if (!turnOrder) break;
        TurnNode *current = turnOrder;
Character *activeChar = current->data;

if (activeChar->isAlive())
{
    processTurn(activeChar);
}

// rotate AFTER turn
if (current->next)
{
    turnOrder = current->next;
    current->next = nullptr;

    TurnNode *tail = turnOrder;
    while (tail->next)
        tail = tail->next;

    tail->next = current;
}
        processBuildings();

context.nextTurn();
checkEndCondition();

if (context.battleEnded)
    break;
    }
    if (!context.battleEnded)
    {
        context.battleEnded = true;
        context.resultCode = "TIME_OUT";
    }
}

void EniesLobbyBattle::processTurn(Character *activeChar)
{
    if (!activeChar->isAlive()) return;
if (activeChar->isStrawHat())
{
    Building *gate = findBuilding("MainGate");
    Building *court = findBuilding("Courthouse");
    Building *ship = findBuilding("BusterCallShip");
    Building *bridge = findBuilding("BridgeOfHesitation");
    int cost = getSkillCost(activeChar);
    bool enoughEnergy = (activeChar->getEnergy() >= cost);
    
    bool gateBroken = (!gate || gate->isDestroyed());
    bool courtBroken = (!court || court->isDestroyed());
    bool shipBroken = (!ship || ship->isDestroyed());
    
    if (!gateBroken)
{
    bool wasBroken = gate->isDestroyed();

    // Chopper không được heal cổng
    if (activeChar->getName() == "Chopper")
        activeChar->attack(gate, context);
    else if (enoughEnergy)
        activeChar->specialSkill(gate, context);
    else
        activeChar->attack(gate, context);

    if (!wasBroken && gate->isDestroyed())
    {
        gate->onDestroyed(context);
        valid(context);
    }
}
    else if (gateBroken && context.alarmLevel >= 50 && !courtBroken)
    {
        bool wasBroken = court->isDestroyed();
        if (enoughEnergy)
            activeChar->specialSkill(court, context);
        else
            activeChar->attack(court, context);
        if (!wasBroken && court->isDestroyed())
        {
            court->onDestroyed(context);
            valid(context);
        }
    }
    else if (context.busterCallTimer <= 5 && !shipBroken)
    {
        bool wasBroken = ship->isDestroyed();
        if (enoughEnergy)
            activeChar->specialSkill(ship, context);
        else
            activeChar->attack(ship, context);
        if (!wasBroken && ship->isDestroyed())
        {
            ship->onDestroyed(context);
            valid(context);
        }
    }
    else if (!context.robinRescued)
    {
        
        if (activeChar->getName() == "Chopper" && enoughEnergy)
        {
            Character *targetForHeal = lowestHpStrawHat();
            if (targetForHeal)
            {
                activeChar->specialSkill(targetForHeal, context);
                activeChar->endTurn(context);
                valid(context);
                return;
            }
        }
        
        Character *enemy = firstAliveCP9();
        if (enemy)
        {
            if (enoughEnergy)
                activeChar->specialSkill(enemy, context);
            else
                activeChar->attack(enemy, context);
            valid(context);
        }
    }
    else
    {
        if (bridge && !bridge->isDestroyed())
        {
            bool wasBroken = bridge->isDestroyed();
            if (enoughEnergy)
                activeChar->specialSkill(bridge, context);
            else
                activeChar->attack(bridge, context);
            if (!wasBroken && bridge->isDestroyed())
            {
                bridge->onDestroyed(context);
                valid(context);
            }
        }
        else
        {
            Character *enemy = firstAliveCP9();
            if (enemy)
            {
                if (enoughEnergy)
                    activeChar->specialSkill(enemy, context);
                else
                    activeChar->attack(enemy, context);
                valid(context);
            }
        }
    }
}
else if (activeChar->isCP9())
{
    Character *victim = firstAliveStrawHat();
    if (!victim) return;
    if (activeChar->getName() == "Fukurou")
    {
        Character *weakest = lowestHpStrawHat();
        ((Fukurou *)activeChar)->isLowestTarget = (victim == weakest);
    }
    int cost = getSkillCost(activeChar);
    if (activeChar->getEnergy() >= cost)
    {
        activeChar->specialSkill(victim, context);
    }
    else
    {
        activeChar->attack(victim, context);
    }
    valid(context);
}
activeChar->endTurn(context);
valid(context);
}

void EniesLobbyBattle::processBuildings()
{
     for (int i = 0; i < buildingCount; i++)
    {
        if (!buildings[i]->isDestroyed())
        {
            buildings[i]->applyEffect(context);
            valid(context);
        }

        if (context.battleEnded) return;
    }
}

void EniesLobbyBattle::checkEndCondition()
{
    if (context.battleEnded) return;
    if (context.robinRescued && context.escapeProgress >= 100)
    {
        context.battleEnded = true;
        context.resultCode = "STRAW_HAT_WIN";
        return;
    }
    if (context.busterCallTimer <= 0)
    {
        context.battleEnded = true;
        context.resultCode = "BUSTER_CALL";
        return;
    }
    bool allSHDead = true;
    for (int i = 0; i < strawHatCount; i++)
    {
        if (strawHats[i]->isAlive())
        {
            allSHDead = false;
            break;
        }
    }
    if (allSHDead)
    {
        context.battleEnded = true;
        context.resultCode = "CP9_WIN";
        return;
    }
    bool allCP9Dead = true;
    for (int i = 0; i < cp9Count; i++)
    {
        if (cp9Agents[i]->isAlive())
        {
            allCP9Dead = false;
            break;
        }
    }
    if (allCP9Dead)
    {
        context.battleEnded = true;
        context.resultCode = "STRAW_HAT_WIN_BY_DEFEAT_CP9";
        return;
    }
    if (context.turnCount >= maxTurns)
    {
        context.battleEnded = true;
        context.resultCode = "TIME_OUT";
        return;
    }
}

string EniesLobbyBattle::getResult() const
{
    ostringstream output;
    output << context.resultCode << " " << context.turnCount << " "<< context.morale << " " << context.alarmLevel << " "<< context.rescueProgress << " " << context.escapeProgress << " "<< context.busterCallTimer;
    return output.str();
}
