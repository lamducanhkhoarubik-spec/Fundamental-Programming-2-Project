#include "debug.h"

using namespace std;
#define ceil(x) static_cast<int>(std::ceil(x))
#define elif else if

void EniesLobbyBattle::clamp() {
    this->context.escapeProgress = (0 > this->context.escapeProgress) ? 0 : ((100 < this->context.escapeProgress) ? 100 : this->context.escapeProgress);
    this->context.morale         = (0 > this->context.morale)         ? 0 : ((100 < this->context.morale)         ? 100 : this->context.morale);
    this->context.alarmLevel     = (0 > this->context.alarmLevel)     ? 0 : ((100 < this->context.alarmLevel)     ? 100 : this->context.alarmLevel);
    this->context.rescueProgress = (0 > this->context.rescueProgress) ? 0 : ((100 < this->context.rescueProgress) ? 100 : this->context.rescueProgress);
    
    if (this->context.busterCallTimer < 0) {
        this->context.busterCallTimer = 0;
    }
}

BattleContext::BattleContext() {
    this->resultCode = "";
    this->battleEnded = false;
    this->bridgeOpened = false;
    this->robinRescued = false;
    this->mainGateDestroyed = false;
    this->busterCallTimer = 0;
    this->escapeProgress = 0;
    this->rescueProgress = 0;
    this->alarmLevel = 0;
    this->morale = 0;
    this->turnCount = 0;
    this->low = INT_MAX;
}

void BattleContext::nextTurn() {
    this->turnCount = 1 + this->turnCount; 
}

Character::Character() {
    this->alive = false;
    this->energy = 0;
    this->speed = 0;
    this->def = 0;
    this->atk = 0;
    this->maxHp = 0;
    this->hp = 0;
    this->name = "";
}

Character::Character(string name, int hp, int atk, int def, int speed, int energy) {
    this->name = name;
    this->maxHp = (0 >= hp) ? 0 : hp; 
    this->hp = this->maxHp;
    this->atk = atk;
    this->def = def;
    this->speed = speed;
    this->energy = (0 >= energy) ? 0 : energy; 
    this->alive = (hp > 0);
}

Character::~Character() {}

int Character::getAtk() const   { return this->atk; }
int Character::getDef() const   { return this->def; }
int Character::getSpeed() const { return this->speed; }
int Character::getMaxHP() const { return this->maxHp; }
int Character::getHP() const    { return this->hp; }
int Character::getEnergy() const { return this->energy; }
string Character::getName() const { return this->name; }
bool Character::isAlive() const { return this->alive; }
bool Character::isDefeated() const { return this->defeated; }
bool Character::isAttacked() const { return this->attacked; }
bool Character::isStrawHat() const { return false; }
bool Character::isCP9() const      { return false; }

void Character::setDef(int def) {
    this->def = (0 > def) ? 0 : def;
}

void Character::setSpeed(int speed) {
    this->speed = (0 > speed) ? 0 : speed;
}

void Character::setHP(int hp) {
    this->hp = (hp < 0) ? 0 : hp;
    if (0 == this->hp) {
        this->alive = false;
    }
}

void Character::setEnergy(int energy) {
    this->energy = (0 > energy) ? 0 : energy;
}

int Character::attack(Building* target, BattleContext& context) { return 0; }
int Character::specialSkill(Building* target, BattleContext& context) { return 0; }
void Character::endTurn(BattleContext& context) { return; }

int Character::receiveDamageBypass(int damage, int percent) {
    int reducedDef = ceil((this->def * (100 - percent)) / 100.0); 
    if (damage <= reducedDef) return 0;
    
    this->hp = this->hp - (damage - reducedDef);
    if (0 >= this->hp) {
        this->hp = 0;
        this->alive = false;
    }
    return (damage - reducedDef);
}

void Character::receiveDamage(int damage) {
    if (this->def < damage) {
        this->hp = this->hp - (damage - this->def);
        if (0 >= this->hp) {
            this->hp = 0;
            this->alive = false;
        }
    }
}

StrawHat::StrawHat() : Character() {
    this->bounty = 0;
}

StrawHat::StrawHat(string name, int hp, int atk, int def, int speed, int energy, long long bounty) {
    this->name = name;
    this->maxHp = (hp > 0) ? hp : 0;
    this->hp = this->maxHp;
    this->atk = atk;
    this->def = def;
    this->speed = speed;
    this->energy = (energy > 0) ? energy : 0;
    this->alive = (hp > 0);
    this->bounty = bounty;
}

bool StrawHat::isStrawHat() const { return true; }

string StrawHat::str() const {
    return "StrawHat[name=" + this->name + ", hp=" + to_string(this->hp) + ", atk=" + to_string(this->atk) + 
           ", def=" + to_string(this->def) + ", speed=" + to_string(this->speed) + ", energy=" + to_string(this->energy) + 
           ", bounty=" + to_string(this->bounty) + "]";
}

/*
Luffy
*/
Luffy::Luffy(string name, int hp, int atk, int def, int speed, int energy, long long bounty) 
    : StrawHat(name, hp, atk, def, speed, energy, bounty) {}

int Luffy::attack(Character* target, BattleContext& context) {
    this->attacked = true;
    int total_dmg = this->atk;
    
    if (this->hp <= ceil(this->maxHp * 30 / 100.0)) {
        total_dmg = total_dmg + ceil(this->atk * 30 / 100.0);
    } elif (this->hp <= ceil(this->maxHp * 50 / 100.0)) {
        total_dmg = total_dmg + ceil(this->atk * 15 / 100.0);
    }
    
    target->receiveDamage(total_dmg);
    if (!target->isAlive()) {
        context.morale = std::min(100, 5 + context.morale); 
        this->defeated = true;
    }
    int effectiveDmg = total_dmg - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Luffy::specialSkill(Character* target, BattleContext& context) {
    this->attacked = true;
    if (20 > this->energy || this->hp < ceil(this->maxHp * 15 / 100.0)) {
        return 0;
    }
    
    this->energy = this->energy - 20;
    context.alarmLevel = std::min(100, 10 + context.alarmLevel);
    this->hp = this->hp - ceil(this->maxHp * 8 / 100.0);
    
    int total_dmg = this->atk + this->atk; 
    this->speed = 15 + this->speed;
    this->atk = 15 + this->atk;
    
    target->receiveDamage(total_dmg);
    if (!target->isAlive()) this->defeated = true;
    
    int effectiveDmg = total_dmg - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Luffy::attack(Building* target, BattleContext& context) {
    int total_dmg = this->atk;
    if (this->hp <= ceil(this->maxHp * 30 / 100.0)) {
        total_dmg = total_dmg + ceil(this->atk * 30 / 100.0);
    } elif (this->hp <= ceil(this->maxHp * 50 / 100.0)) {
        total_dmg = total_dmg + ceil(this->atk * 15 / 100.0);
    }
    target->receiveDamage(total_dmg);
    return total_dmg;
}

int Luffy::specialSkill(Building* target, BattleContext& context) {
    this->attacked = true;
    if (this->energy < 20 || this->hp < ceil(this->maxHp * 15 / 100.0)) {
        return 0;
    }
    this->energy = this->energy - 20;
    context.alarmLevel = std::min(100, 10 + context.alarmLevel);
    this->hp = this->hp - ceil(this->maxHp * 8 / 100.0);
    int total_dmg = this->atk + this->atk;
    this->speed = 15 + this->speed;
    this->atk = 15 + this->atk;
    target->receiveDamage(total_dmg);
    return total_dmg;
}

void Luffy::endTurn(BattleContext& context) {
    if (this->hp <= ceil(this->maxHp * 30 / 100.0)) {
        context.morale = std::min(100, 3 + context.morale);
    }
    if (this->defeated) {
        this->energy = std::min(100, 5 + this->energy);
        this->defeated = false;
    }
}

/*
Zoro
*/
Zoro::Zoro(string name, int hp, int atk, int def, int speed, int energy, long long bounty) 
    : StrawHat(name, hp, atk, def, speed, energy, bounty) {}

int Zoro::attack(Character* target, BattleContext& context) {
    int total_dmg = this->atk + ceil(this->def * 20 / 100.0);
    if (target->getHP() < ceil(target->getMaxHP() * 40 / 100.0)) {
        total_dmg = ceil(total_dmg * 115 / 100.0);
    }
    target->receiveDamage(total_dmg);
    if (!target->isAlive()) this->defeated = true;
    int effectiveDmg = total_dmg - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Zoro::specialSkill(Character* target, BattleContext& context) {
    if (this->energy >= 15) {
        int total_dmg = ceil(this->atk * 220 / 100.0);
        this->energy = this->energy - 15;
        if (target->getHP() < ceil(target->getMaxHP() * 50 / 100.0)) {
            total_dmg = ceil(total_dmg * 150 / 100.0);
        }
        target->receiveDamage(total_dmg);
        if (!target->isAlive()) {
            this->defeated = true;
            context.morale = std::min(100, 4 + context.morale);
            this->energy = std::min(100, 8 + this->energy);
        }
        int effectiveDmg = total_dmg - target->getDef();
        return (effectiveDmg > 0) ? effectiveDmg : 0;
    }
    return 0;
}

int Zoro::attack(Building* target, BattleContext& context) {
    int total_dmg = this->atk + ceil(this->def * 20 / 100.0);
    target->receiveDamage(total_dmg);
    return total_dmg;
}

int Zoro::specialSkill(Building* target, BattleContext& context) {
    if (this->energy >= 15) {
        int total_dmg = ceil(this->atk * 220 / 100.0);
        this->energy = this->energy - 15;
        target->receiveDamage(total_dmg);
        return total_dmg;
    }
    return 0;
}

void Zoro::endTurn(BattleContext& context) {
    if (this->defeated) {
        this->defeated = false;
        context.morale = std::min(100, 6 + context.morale);
        this->atk = this->atk + ceil(this->atk * 5 / 100.0);
    }
}

/*
Sanji
*/
Sanji::Sanji(string name, int hp, int atk, int def, int speed, int energy, long long bounty) 
    : StrawHat(name, hp, atk, def, speed, energy, bounty) {}

int Sanji::attack(Character* target, BattleContext& context) {
    int total_dmg = this->atk + ceil(this->speed * 50 / 100.0);
    if (this->def > target->getDef()) {
        total_dmg = total_dmg + ceil(total_dmg * 10 / 100.0);
    }
    target->receiveDamage(total_dmg);
    if (!target->isAlive()) this->defeated = true;
    int effectiveDmg = total_dmg - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Sanji::specialSkill(Character* target, BattleContext& context) {
    if (this->energy >= 18) {
        this->energy = this->energy - 18;
        int total_dmg = ceil(this->atk * 210 / 100.0);
        target->receiveDamage(total_dmg);
        if (!target->isAlive()) this->defeated = true;
        int effectiveDmg = total_dmg - target->getDef();
        target->setDef(target->getDef() - 8);
        if (target->getName() == "Jabra") {
            target->setDef(target->getDef() - 4);
        }
        return (effectiveDmg > 0) ? effectiveDmg : 0;
    }
    return 0;
}

int Sanji::attack(Building* target, BattleContext& context) {
    int total_dmg = this->atk + ceil(this->speed * 50 / 100.0);
    target->receiveDamage(total_dmg);
    return total_dmg;
}

int Sanji::specialSkill(Building* target, BattleContext& context) {
    if (this->energy >= 18) {
        this->energy = this->energy - 18;
        int total_dmg = ceil(this->atk * 210 / 100.0);
        target->receiveDamage(total_dmg);
        return total_dmg;
    }
    return 0;
}

void Sanji::endTurn(BattleContext& context) {
    if (this->defeated) {
        this->defeated = false;
        context.morale = std::min(100, 8 + context.morale);
        this->atk = this->atk + ceil(this->atk * 10 / 100.0);
    }
}

/*
Nami
*/
Nami::Nami(string name, int hp, int atk, int def, int speed, int energy, long long bounty) 
    : StrawHat(name, hp, atk, def, speed, energy, bounty) {}

int Nami::attack(Character* target, BattleContext& context) {
    int res = target->receiveDamageBypass(this->atk, 30);
    if (!target->isAlive()) {
        this->defeated = true;
        if (target->isCP9()) {
            context.morale = std::min(100, 5 + context.morale);
        }
    }
    return res;
}

int Nami::specialSkill(Character* target, BattleContext& context) {
    if (this->energy >= 20) {
        this->energy = this->energy - 20;
        target->receiveDamage(40 + this->atk);
        target->setSpeed(target->getSpeed() - 10);
        if (!target->isAlive()) {
            this->defeated = true;
            if (target->isCP9()) {
                context.morale = std::min(100, 5 + context.morale);
            }
        }
        int effectiveDmg = (this->atk + 40) - target->getDef();
        context.busterCallTimer = std::min(100, 1 + context.busterCallTimer);
        context.alarmLevel = std::max(0, context.alarmLevel - 5);
        return (effectiveDmg > 0) ? effectiveDmg : 0;
    }
    return 0;
}

int Nami::attack(Building* target, BattleContext& context) {
    target->receiveDamage(ceil(this->atk * 50 / 100.0));
    return ceil(this->atk * 50 / 100.0);
}

int Nami::specialSkill(Building* target, BattleContext& context) {
    if (this->energy >= 20) {
        this->energy = this->energy - 20;
        int total_dmg = ceil((this->atk + 40) * 150 / 100.0);
        target->receiveDamage(total_dmg);
        context.busterCallTimer = std::min(100, 1 + context.busterCallTimer);
        context.alarmLevel = std::max(0, context.alarmLevel - 5);
        return total_dmg;
    }
    return 0;
}

void Nami::endTurn(BattleContext& context) {
    if (this->defeated) {
        this->defeated = false;
        this->energy = std::min(100, 6 + this->energy);
    }
}

/*
Chopper
*/
Chopper::Chopper(string name, int hp, int atk, int def, int speed, int energy, long long bounty) 
    : StrawHat(name, hp, atk, def, speed, energy, bounty) {}

int Chopper::attack(Character* target, BattleContext& context) {
    target->receiveDamage(this->atk);
    if (!target->isAlive()) this->defeated = true;
    int effectiveDmg = this->atk - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Chopper::specialSkill(Character* target, BattleContext& context) {
    if (this->energy >= 15) {
        this->energy = this->energy - 15;
        int healAmount = 35 + ceil(this->atk * 50 / 100.0);
        int newHp = std::min(target->getMaxHP(), target->getHP() + healAmount);
        target->setHP(newHp);
        if (target->getName() == "Luffy") {
            context.morale = std::min(100, 5 + context.morale);
        }
    }
    return 0;
}

int Chopper::attack(Building* target, BattleContext& context) {
    target->receiveDamage(this->atk);
    return this->atk;
}

void Chopper::endTurn(BattleContext& context) {}

/*
Usopp
*/
Usopp::Usopp(string name, int hp, int atk, int def, int speed, int energy, long long bounty) 
    : StrawHat(name, hp, atk, def, speed, energy, bounty) {}

int Usopp::attack(Character* target, BattleContext& context) {
    this->defeated = true;
    int total_dmg = this->atk;
    if (50 > target->getSpeed()) {
        total_dmg = ceil(total_dmg * 120 / 100.0);
    }
    target->receiveDamage(total_dmg);
    int effectiveDmg = total_dmg - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Usopp::specialSkill(Character* target, BattleContext& context) {
    if (this->energy >= 16) {
        this->defeated = true;
        this->energy = this->energy - 16;
        int total_dmg = ceil(this->atk * 80 / 100.0);
        target->receiveDamage(total_dmg);
        target->setSpeed(target->getSpeed() - 12);
        context.escapeProgress = std::min(100, 8 + context.escapeProgress);
        int effectiveDmg = total_dmg - target->getDef();
        return (effectiveDmg > 0) ? effectiveDmg : 0;
    }
    return 0;
}

int Usopp::attack(Building* target, BattleContext& context) {
    this->defeated = true;
    target->receiveDamage(ceil(this->atk * 50 / 100.0));
    return ceil(this->atk * 50 / 100.0);
}

int Usopp::specialSkill(Building* target, BattleContext& context) {
    if (this->energy >= 16) {
        this->defeated = true;
        this->energy = this->energy - 16;
        int total_dmg = ceil(this->atk * 80 / 100.0);
        target->receiveDamage(total_dmg);
        context.escapeProgress = std::min(100, 8 + context.escapeProgress);
        return total_dmg;
    }
    return 0;
}

void Usopp::endTurn(BattleContext& context) {
    if (this->defeated) {
        this->defeated = false;
        context.morale = std::min(100, 10 + context.morale);
    }
}

/*
Franky
*/
Franky::Franky(string name, int hp, int atk, int def, int speed, int energy, long long bounty) 
    : StrawHat(name, hp, atk, def, speed, energy, bounty) {}

int Franky::attack(Character* target, BattleContext& context) {
    int total_dmg = ceil(this->atk + 30 * this->def / 100.0);
    if (target->isCP9()) {
        total_dmg = total_dmg + ceil(total_dmg * 10 / 100.0);
    }
    target->receiveDamage(total_dmg);
    if (target->isCP9() && !target->isAlive()) {
        context.morale = std::min(100, 5 + context.morale);
    }
    int effectiveDmg = total_dmg - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Franky::specialSkill(Character* target, BattleContext& context) {
    if (this->energy >= 30) {
        this->energy = this->energy - 30;
        int total_dmg = ceil(this->atk * 120 / 100.0);
        target->receiveDamage(total_dmg);
        if (!target->isAlive() && target->isCP9()) {
            context.morale = std::min(100, 5 + context.morale);
        }
        int effectiveDmg = total_dmg - target->getDef();
        return (effectiveDmg > 0) ? effectiveDmg : 0;
    } 
    elif (this->energy >= 20) {
        this->energy = this->energy - 20;
        int total_dmg = ceil(this->atk * 180 / 100.0);
        target->setSpeed(target->getSpeed() - 8);
        if (target->getName() == "Lucci") {
            total_dmg = total_dmg + ceil(total_dmg * 20 / 100.0);
        }
        target->receiveDamage(total_dmg);
        if (!target->isAlive() && target->isCP9()) {
            context.morale = std::min(100, 5 + context.morale);
        }
        int effectiveDmg = total_dmg - target->getDef();
        return (effectiveDmg > 0) ? effectiveDmg : 0;
    }
    return 0;
}

int Franky::attack(Building* target, BattleContext& context) {
    int total_dmg = ceil(this->atk + 30 * this->def / 100.0);
    target->receiveDamage(total_dmg);
    return total_dmg;
}

int Franky::specialSkill(Building* target, BattleContext& context) {
    if (this->energy >= 30) {
        this->energy = this->energy - 30;
        int total_dmg = target->getHp();
        target->receiveDamage(total_dmg);
        return total_dmg;
    } 
    elif (this->energy >= 20) {
        this->energy = this->energy - 20;
        int total_dmg = ceil(this->atk * 180 / 100.0);
        target->receiveDamage(total_dmg);
        return total_dmg;
    }
    return 0;
}

void Franky::endTurn(BattleContext& context) {
    if (this->hp > ceil(70 * this->maxHp / 100.0)) {
        this->def = 5 + this->def;
    } else if (this->hp < ceil(30 * this->maxHp / 100.0)) {
        this->atk = ceil(this->atk * 110 / 100.0);
    }
}

CP9Agent::CP9Agent() : Character() {
    this->doriki = 0;
}

CP9Agent::CP9Agent(string name, int hp, int atk, int def, int speed, int energy, int doriki) 
    : Character(name, hp, atk, def, speed, energy), doriki(doriki) {}

bool CP9Agent::isCP9() const { return true; }

string CP9Agent::str() const {
    return "CP9[name=" + this->name + ", hp=" + to_string(this->hp) + ", atk=" + to_string(this->atk) + ", def=" + to_string(this->def) + ", speed=" + to_string(this->speed) + ", energy=" + to_string(this->energy) + ", doriki=" + to_string(this->doriki) + "]";
}

/*
Lucci
*/
Lucci::Lucci(string name, int hp, int atk, int def, int speed, int energy, int doriki) : CP9Agent(name, hp, atk, def, speed, energy, doriki) {}

int Lucci::attack(Character* target, BattleContext& context) {
    int total_dmg = this->atk + ceil(this->doriki / 20.0);
    if (target->getHP() < ceil(50 * target->getMaxHP() / 100.0)) {
        total_dmg = total_dmg + ceil(total_dmg * 20 / 100.0);
    }
    target->receiveDamage(total_dmg);
    int effectiveDmg = total_dmg - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Lucci::specialSkill(Character* target, BattleContext& context) {
    if (this->energy >= 25) {
        this->energy = this->energy - 25;
        int res = target->receiveDamageBypass(ceil(this->atk * 280 / 100.0), 50);
        if (!target->isAlive()) {
            context.morale = std::max(0, context.morale - 10);
        }
        return res;
    }
    return 0;
}

void Lucci::endTurn(BattleContext& context) {
    if (this->hp < ceil(40 * this->maxHp / 100.0)) {
        this->atk = this->atk + ceil(this->atk * 5 / 100.0);
    }
}

/*
Kaku
*/
Kaku::Kaku(string name, int hp, int atk, int def, int speed, int energy, int doriki) 
    : CP9Agent(name, hp, atk, def, speed, energy, doriki) {}

int Kaku::attack(Character* target, BattleContext& context) {
    target->receiveDamage(this->atk);
    if (target->isStrawHat() && !target->isAlive()) {
        context.morale = std::max(0, context.morale - 5);
    }
    int effectiveDmg = this->atk - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Kaku::specialSkill(Character* target, BattleContext& context) {
    if (this->energy >= 20) {
        this->energy = this->energy - 20;
        int res = 0;
        if (target->isAlive()) {
            target->receiveDamage(ceil(this->atk * 120 / 100.0));
            res += ceil(this->atk * 120 / 100.0) > target->getDef() ? ceil(this->atk * 120 / 100.0) - target->getDef() : 0;
            if (target->isAlive()) {
                target->receiveDamage(this->atk);
                res += this->atk > target->getDef() ? this->atk - target->getDef() : 0;
                if (target->isAlive()) {
                    target->receiveDamage(ceil(this->atk * 80 / 100.0));
                    res += ceil(this->atk * 80 / 100.0) > target->getDef() ? ceil(this->atk * 80 / 100.0) - target->getDef() : 0;
                }
            }
        }
        if (!target->isAlive() && target->isStrawHat()) {
            context.morale = std::max(0, context.morale - 5);
        }
        return res;
    }
    return 0;
}

void Kaku::endTurn(BattleContext& context) {}

/*
Jabra
*/
Jabra::Jabra(string name, int hp, int atk, int def, int speed, int energy, int doriki) 
    : CP9Agent(name, hp, atk, def, speed, energy, doriki) {}

int Jabra::attack(Character* target, BattleContext& context) {
    target->receiveDamage(this->atk);
    int effectiveDmg = this->atk - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Jabra::specialSkill(Character* target, BattleContext& context) {
    if (this->energy >= 18) {
        this->energy = this->energy - 18;
        int total_dmg = ceil(this->atk * 150 / 100.0);
        if (this->hp < ceil(this->maxHp * 30 / 100.0)) {
            total_dmg = total_dmg + ceil(total_dmg * 25 / 100.0);
        }
        target->receiveDamage(total_dmg);
        if (!target->isAlive()) {
            context.morale = std::max(0, context.morale - 5);
        }
        int effectiveDmg = total_dmg - target->getDef();
        return (effectiveDmg > 0) ? effectiveDmg : 0;
    }
    return 0;
}

void Jabra::endTurn(BattleContext& context) {}

/*
Blueno
*/
Blueno::Blueno(string name, int hp, int atk, int def, int speed, int energy, int doriki) 
    : CP9Agent(name, hp, atk, def, speed, energy, doriki) {}

int Blueno::attack(Character* target, BattleContext& context) {
    target->receiveDamage(this->atk);
    if (target->isStrawHat() && !target->isAlive()) {
        context.morale = std::max(0, context.morale - 5);
    }
    int effectiveDmg = this->atk - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Blueno::specialSkill(Character* target, BattleContext& context) {
    if (this->energy >= 15) {
        this->energy = this->energy - 15;
        int total_dmg = ceil(this->atk * 130 / 100.0);
        total_dmg = total_dmg + ((this->hp <= ceil(this->maxHp * 50 / 100.0)) ? 40 : 20);
        target->receiveDamage(total_dmg);
        if (!target->isAlive() && target->isStrawHat()) {
            context.morale = std::max(0, context.morale - 5);
        }
        int effectiveDmg = total_dmg - target->getDef();
        return (effectiveDmg > 0) ? effectiveDmg : 0;
    }
    return 0;
}

void Blueno::endTurn(BattleContext& context) {}

/*
Kalifa
*/
Kalifa::Kalifa(string name, int hp, int atk, int def, int speed, int energy, int doriki) 
    : CP9Agent(name, hp, atk, def, speed, energy, doriki) {}

int Kalifa::attack(Character* target, BattleContext& context) {
    target->receiveDamage(this->atk);
    int effectiveDmg = this->atk - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Kalifa::specialSkill(Character* target, BattleContext& context) {
    if (this->energy >= 18) {
        this->energy = this->energy - 18;
        int total_dmg = ceil(this->atk * 140 / 100.0);
        target->receiveDamage(total_dmg);
        target->setSpeed(target->getSpeed() - 6);
        context.morale = std::max(0, context.morale - ((target->getName() == "Nami") ? 12 : 8));
        int effectiveDmg = total_dmg - target->getDef();
        return (effectiveDmg > 0) ? effectiveDmg : 0;
    }
    return 0;
}

void Kalifa::endTurn(BattleContext& context) {}

/*
Kumadori
*/
Kumadori::Kumadori(string name, int hp, int atk, int def, int speed, int energy, int doriki) 
    : CP9Agent(name, hp, atk, def, speed, energy, doriki) {}

int Kumadori::attack(Character* target, BattleContext& context) {
    target->receiveDamage(this->atk);
    if (target->isStrawHat() && !target->isAlive()) {
        context.morale = std::max(0, context.morale - 5);
    }
    int effectiveDmg = this->atk - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Kumadori::specialSkill(Character* target, BattleContext& context) {
    if (this->energy >= 16) {
        this->energy = this->energy - 16;
        int total_dmg = ceil(30.0 + 0.1 * this->doriki);
        if (this->hp < ceil(this->maxHp * 40 / 100.0)) {
            total_dmg = 25 + total_dmg;
        }
        target->receiveDamage(total_dmg);
        if (target->isStrawHat() && !target->isAlive()) {
            context.morale = std::max(0, context.morale - 5);
        }
        int effectiveDmg = total_dmg - target->getDef();
        return (effectiveDmg > 0) ? effectiveDmg : 0;
    }
    return 0;
}

void Kumadori::endTurn(BattleContext& context) {}

/*
Fukurou
*/
Fukurou::Fukurou(string name, int hp, int atk, int def, int speed, int energy, int doriki) 
    : CP9Agent(name, hp, atk, def, speed, energy, doriki) {}

int Fukurou::attack(Character* target, BattleContext& context) {
    target->receiveDamage(this->atk);
    int effectiveDmg = this->atk - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Fukurou::specialSkill(Character* target, BattleContext& context) {
    if (this->energy >= 14) {
        this->energy = this->energy - 14;
        int total_dmg = ceil(this->atk * 130 / 100.0);
        if (target->getHP() == context.low) {
            total_dmg = 20 + total_dmg;
        }
        target->receiveDamage(total_dmg);
        if (!target->isAlive()) {
            context.morale = std::max(0, context.morale - 6);
        }
        int effectiveDmg = total_dmg - target->getDef();
        return (effectiveDmg > 0) ? effectiveDmg : 0;
    }
    return 0;
}

void Fukurou::endTurn(BattleContext& context) {}

Building::Building(string name, int hp) {
    this->name = name;
    this->hp = (hp > 0) ? hp : 0;
    this->maxHP = this->hp;
    this->destroyed = (hp <= 0);
}

Building::~Building() {}

string Building::getName() const { return this->name; }
int Building::getMaxHP() const  { return this->maxHP; }
int Building::getHp() const     { return this->hp; }
bool Building::isDestroyed() const { return this->destroyed; }
void Building::onDestroyed(BattleContext& context) { return; }

void Building::receiveDamage(int damage) {
    this->hp = this->hp - damage;
    if (0 >= this->hp) {
        this->destroyed = true;
        this->hp = 0;
    }
}

string Building::str() const {
    return "Building[name=" + this->name + ", hp=" + to_string(this->hp) + ", maxHP=" + to_string(this->maxHP) + 
           ", destroyed=" + (this->destroyed ? "true" : "false") + "]";
}

MainGate::MainGate(string name, int hp) : Building(name, hp) {}
void MainGate::applyEffect(BattleContext& context) {}
void MainGate::onDestroyed(BattleContext& context) {
    context.mainGateDestroyed = true;
    context.rescueProgress = std::min(100, 20 + context.rescueProgress);
    context.morale = std::min(100, 5 + context.morale);
}

Courthouse::Courthouse(string name, int hp) : Building(name, hp) {}
void Courthouse::applyEffect(BattleContext& context) {
    if (!this->destroyed) {
        context.alarmLevel = std::min(100, 5 + context.alarmLevel);
    }
}
void Courthouse::onDestroyed(BattleContext& context) {
    context.alarmLevel = std::max(0, context.alarmLevel - 20);
}

TowerOfJustice::TowerOfJustice(string name, int hp) : Building(name, hp) {}
void TowerOfJustice::applyEffect(BattleContext& context) {
    if (context.mainGateDestroyed && !context.robinRescued) {
        context.rescueProgress = std::min(100, 5 + context.rescueProgress);
        if (100 <= context.rescueProgress) {
            context.robinRescued = true;
            context.morale = std::min(100, 10 + context.morale);
        }
    }
}

BridgeOfHesitation::BridgeOfHesitation(string name, int hp) : Building(name, hp) {}
void BridgeOfHesitation::applyEffect(BattleContext& context) {
    if (context.robinRescued && !this->destroyed) {
        context.bridgeOpened = true;
        context.escapeProgress = std::min(100, 5 + context.escapeProgress);
    }
}

BusterCallShip::BusterCallShip(string name, int hp) : Building(name, hp) {}
void BusterCallShip::applyEffect(BattleContext& context) {
    if (!this->destroyed) {
        context.busterCallTimer = std::max(0, context.busterCallTimer - 1);
    }
}
void BusterCallShip::onDestroyed(BattleContext& context) {
    context.busterCallTimer = 3 + context.busterCallTimer;
}

EniesLobbyBattle::EniesLobbyBattle(const string& filename) {
    this->strawHats = new Character*[7];
    this->cp9Agents = new Character*[7];
    this->buildings = new Building*[5];
    this->strawHatCount = 0;
    this->cp9Count = 0;
    this->buildingCount = 0;
    this->tailNode = nullptr;
    this->turnOrder = nullptr;
    this->loadFromFile(filename);
}

EniesLobbyBattle::~EniesLobbyBattle() {
    if (this->turnOrder != nullptr) {
        TurnNode* curr = this->turnOrder;
        while (curr->next != nullptr && this->turnOrder != curr->next) {
            curr = curr->next;
        }
        curr->next = nullptr; 
        curr = this->turnOrder;
        while (curr != nullptr) {
            TurnNode* nextNode = curr->next;
            delete curr;
            curr = nextNode;
        }
        this->turnOrder = nullptr;
    }
    for (int i = 0; this->strawHatCount > i; ++i) delete this->strawHats[i];
    for (int i = 0; this->cp9Count > i; ++i)      delete this->cp9Agents[i];
    for (int i = 0; this->buildingCount > i; ++i)  delete this->buildings[i];
    
    delete[] this->buildings;
    delete[] this->strawHats;
    delete[] this->cp9Agents;
}

void EniesLobbyBattle::addStrawHat(Character* character) {
    if (7 > this->strawHatCount) this->strawHats[this->strawHatCount++] = character;
}

void EniesLobbyBattle::addCP9Agent(Character* character) {
    if (7 > this->cp9Count) this->cp9Agents[this->cp9Count++] = character;
}

void EniesLobbyBattle::addBuilding(Building* building) {
    if (5 > this->buildingCount) this->buildings[this->buildingCount++] = building;
}

void EniesLobbyBattle::buildTurnOrder() {
    if (0 == this->strawHatCount && 0 == this->cp9Count) return;
    
    TurnNode* currentTail = nullptr;
    if (this->strawHatCount > 0) {
        this->turnOrder = new TurnNode(this->strawHats[0]);
        currentTail = this->turnOrder;
        for (int i = 1; this->strawHatCount > i; ++i) {
            currentTail->next = new TurnNode(this->strawHats[i]);
            currentTail = currentTail->next;
        }
    }
    
    if (this->cp9Count > 0) {
        if (currentTail != nullptr) {
            for (int i = 0; this->cp9Count > i; ++i) {
                currentTail->next = new TurnNode(this->cp9Agents[i]);
                currentTail = currentTail->next;
            }
        } else {
            this->turnOrder = new TurnNode(this->cp9Agents[0]);
            currentTail = this->turnOrder;
            for (int i = 1; this->cp9Count > i; ++i) {
                currentTail->next = new TurnNode(this->cp9Agents[i]);
                currentTail = currentTail->next;
            }
        }
    }
    
    if (currentTail) currentTail->next = this->turnOrder;
}

void EniesLobbyBattle::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return;
    
    string currentLine;
    while (getline(file, currentLine)) {
        if (currentLine.empty()) continue;

        stringstream parsingStream(currentLine);
        string entryType;
        parsingStream >> entryType;
        
        if (entryType == "CONTEXT") {
            parsingStream >> this->context.morale >> this->context.alarmLevel >> this->context.rescueProgress 
                          >> this->context.escapeProgress >> this->context.busterCallTimer >> this->maxTurns;
        }
        elif (entryType == "STRAW_HAT") {
            string name; int hp, atk, def, speed, energy; long long bounty;
            parsingStream >> name >> hp >> atk >> def >> speed >> energy >> bounty;

            if (name == "Luffy") {
                this->addStrawHat(new Luffy(name, hp, atk, def, speed, energy, bounty));
            }
            elif (name == "Zoro") {
                this->addStrawHat(new Zoro(name, hp, atk, def, speed, energy, bounty));
            }
            elif (name == "Nami") {
                this->addStrawHat(new Nami(name, hp, atk, def, speed, energy, bounty));
            }
            elif (name == "Usopp" || name == "Sogeking") {
                this->addStrawHat(new Usopp(name, hp, atk, def, speed, energy, bounty));
            }
            elif (name == "Sanji") {
                this->addStrawHat(new Sanji(name, hp, atk, def, speed, energy, bounty));
            }
            elif (name == "Chopper") {
                this->addStrawHat(new Chopper(name, hp, atk, def, speed, energy, bounty));
            }
            elif (name == "Franky") {
                this->addStrawHat(new Franky(name, hp, atk, def, speed, energy, bounty));
            }
        }
        elif (entryType == "CP9") {
            string name; int hp, atk, def, speed, energy, doriki;
            parsingStream >> name >> hp >> atk >> def >> speed >> energy >> doriki;
            if (name == "Lucci") {
                this->addCP9Agent(new Lucci(name, hp, atk, def, speed, energy, doriki));
            }
            elif (name == "Kaku") {
                this->addCP9Agent(new Kaku(name, hp, atk, def, speed, energy, doriki));
            }
            elif (name == "Jabra") {
                this->addCP9Agent(new Jabra(name, hp, atk, def, speed, energy, doriki));
            }
            elif (name == "Blueno") {
                this->addCP9Agent(new Blueno(name, hp, atk, def, speed, energy, doriki));
            }
            elif (name == "Kalifa") {
                this->addCP9Agent(new Kalifa(name, hp, atk, def, speed, energy, doriki));
            }
            elif (name == "Kumadori") {
                this->addCP9Agent(new Kumadori(name, hp, atk, def, speed, energy, doriki));
            }
            elif (name == "Fukurou") {
                this->addCP9Agent(new Fukurou(name, hp, atk, def, speed, energy, doriki));
            }
        }
        elif (entryType == "BUILDING") {
            string name; int hp;
            parsingStream >> name >> hp;
            if (name == "MainGate")             this->addBuilding(new MainGate(name, hp));
            elif (name == "Courthouse")          this->addBuilding(new Courthouse(name, hp));
            elif (name == "TowerOfJustice")      this->addBuilding(new TowerOfJustice(name, hp));
            elif (name == "BridgeOfHesitation")  this->addBuilding(new BridgeOfHesitation(name, hp));
            elif (name == "BusterCallShip")      this->addBuilding(new BusterCallShip(name, hp));
        }
    }
    file.close();
    this->buildTurnOrder();
}

int getSkillCost(Character* character) {
    string name = character->getName();
    if (name == "Luffy") return 20;
    if (name == "Zoro") return 15;
    if (name == "Sanji") return 18;
    if (name == "Nami") return 20;
    if (name == "Chopper") return 15;
    if (name == "Usopp" || name == "Sogeking") return 16;
    if (name == "Franky") return 20;
    if (name == "Lucci") return 25;
    if (name == "Kaku") return 20;
    if (name == "Jabra") return 18;
    if (name == "Blueno") return 15;
    if (name == "Kalifa") return 18;
    if (name == "Kumadori") return 16;
    if (name == "Fukurou") return 14;
    return 101;
}

void EniesLobbyBattle::runBattle() {
    if (this->turnOrder == nullptr) return;
    
    while (!this->context.battleEnded && this->context.turnCount < this->maxTurns) {
        Character* curActor = this->turnOrder->data;
        if (curActor && curActor->isAlive()) {
            this->processTurn(curActor);
        }
        
        this->turnOrder = this->turnOrder->next;
        this->processBuildings();
        this->context.turnCount = 1 + this->context.turnCount;
        this->checkEndCondition();
    }
}

void EniesLobbyBattle::processTurn(Character* character) {
    if (!character || !character->isAlive()) return;
    
    int lowestSHHp = INT_MAX;
    for (int i = 0; this->strawHatCount > i; ++i) {
        if (this->strawHats[i]->isAlive() && this->strawHats[i]->getHP() < lowestSHHp) {
            lowestSHHp = this->strawHats[i]->getHP();
        }
    }
    this->context.low = lowestSHHp;
    
    bool enoughEnergy = (character->getEnergy() >= getSkillCost(character));
    bool invokeSpecialSkill = enoughEnergy;
    
    if (character->getName() == "Luffy" && character->getHP() < ceil(character->getMaxHP() * 15 / 100.0)) {
        invokeSpecialSkill = false;
    }
    
    if (character->getName() == "Chopper" && enoughEnergy) {
        for (int i = 0; this->strawHatCount > i; ++i) {
            if (this->strawHats[i]->isAlive() && this->strawHats[i]->getHP() == lowestSHHp) {
                character->specialSkill(this->strawHats[i], this->context);
                break;
            }
        }
    }
    else if (character->isStrawHat()) {
        Building* gateNode = nullptr;
        Building* courtNode = nullptr;
        Building* shipNode = nullptr;
        Building* bridgeNode = nullptr;
        
        for (int i = 0; this->buildingCount > i; ++i) {
            string bName = this->buildings[i]->getName();
            if (bName == "MainGate")             gateNode = this->buildings[i];
            else if (bName == "Courthouse")      courtNode = this->buildings[i];
            else if (bName == "BusterCallShip")  shipNode = this->buildings[i];
            else if (bName == "BridgeOfHesitation") bridgeNode = this->buildings[i];
        }
        
        if (gateNode && !this->context.mainGateDestroyed && !gateNode->isDestroyed()) {
            if (invokeSpecialSkill) character->specialSkill(gateNode, this->context);
            else character->attack(gateNode, this->context);
            if (gateNode->isDestroyed()) gateNode->onDestroyed(this->context);
        }
        else if (courtNode && this->context.alarmLevel >= 50 && !courtNode->isDestroyed()) {
            if (invokeSpecialSkill) character->specialSkill(courtNode, this->context);
            else character->attack(courtNode, this->context);
            if (courtNode->isDestroyed()) courtNode->onDestroyed(this->context);
        }
        else if (shipNode && this->context.busterCallTimer <= 5 && !shipNode->isDestroyed()) {
            if (invokeSpecialSkill) character->specialSkill(shipNode, this->context);
            else character->attack(shipNode, this->context);
            if (shipNode->isDestroyed()) shipNode->onDestroyed(this->context);
        }
        else if (!this->context.robinRescued) {
            for (int i = 0; this->cp9Count > i; ++i) {
                if (this->cp9Agents[i]->isAlive()) {
                    if (invokeSpecialSkill) character->specialSkill(this->cp9Agents[i], this->context);
                    else character->attack(this->cp9Agents[i], this->context);
                    break;
                }
            }
        }
        else if (bridgeNode && !bridgeNode->isDestroyed()) {
            if (invokeSpecialSkill) character->specialSkill(bridgeNode, this->context);
            else character->attack(bridgeNode, this->context);
            if (bridgeNode->isDestroyed()) bridgeNode->onDestroyed(this->context);
        }
        else {
            for (int i = 0; this->cp9Count > i; ++i) {
                if (this->cp9Agents[i]->isAlive()) {
                    if (invokeSpecialSkill) character->specialSkill(this->cp9Agents[i], this->context);
                    else character->attack(this->cp9Agents[i], this->context);
                    break;
                }
            }
        }
    }
    else {
        for (int i = 0; this->strawHatCount > i; ++i) {
            if (this->strawHats[i]->isAlive()) {
                if (invokeSpecialSkill) character->specialSkill(this->strawHats[i], this->context);
                else character->attack(this->strawHats[i], this->context);
                break;
            }
        }
    }
    
    character->endTurn(this->context);
    this->clamp();
}

void EniesLobbyBattle::processBuildings() {
    for (int i = 0; this->buildingCount > i; ++i) {
        if (!this->buildings[i]->isDestroyed()) {
            this->buildings[i]->applyEffect(this->context);
        }
    }
}

void EniesLobbyBattle::checkEndCondition() {
    if (this->context.robinRescued && this->context.escapeProgress >= 100) {
        this->context.battleEnded = true;
        this->context.resultCode = "STRAW_HAT_WIN";
        return;
    }
    if (0 >= this->context.busterCallTimer) {
        this->context.resultCode = "BUSTER_CALL";
        this->context.battleEnded = true;
        return;
    }
    
    bool hasStrawHatsAlive = false;
    for (int i = 0; this->strawHatCount > i; ++i) {
        if (this->strawHats[i]->isAlive()) {
            hasStrawHatsAlive = true;
            break;
        }
    }
    if (!hasStrawHatsAlive) {
        this->context.resultCode = "CP9_WIN";
        this->context.battleEnded = true;
        return;
    }
    
    bool hasCp9Alive = false;
    for (int i = 0; this->cp9Count > i; ++i) {
        if (this->cp9Agents[i]->isAlive()) {
            hasCp9Alive = true;
            break;
        }
    }
    if (!hasCp9Alive) {
        this->context.resultCode = "STRAW_HAT_WIN_BY_DEFEAT_CP9";
        this->context.battleEnded = true;
        return;
    }
    if (this->context.turnCount >= this->maxTurns) {
        this->context.resultCode = "TIME_OUT";
        this->context.battleEnded = true;
        return;
    }
}

string EniesLobbyBattle::getResult() const {
    return this->context.resultCode + " " + to_string(this->context.turnCount) + " " + to_string(this->context.morale) + " " + to_string(this->context.alarmLevel) + " " + to_string(this->context.rescueProgress) + " " + to_string(this->context.escapeProgress) + " " + to_string(this->context.busterCallTimer);
}