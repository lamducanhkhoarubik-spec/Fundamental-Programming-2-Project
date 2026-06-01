#include "eniesLobby.h"

#define elif else if



void EniesLobbyBattle::clamp() {
    this->context.escapeProgress  = (this->context.escapeProgress > 100) ? 100 : ((this->context.escapeProgress < 0) ? 0 : this->context.escapeProgress);
    this->context.morale          = (this->context.morale > 100)         ? 100 : ((this->context.morale < 0)         ? 0 : this->context.morale);
    this->context.alarmLevel      = (this->context.alarmLevel > 100)     ? 100 : ((this->context.alarmLevel < 0)     ? 0 : this->context.alarmLevel);
    this->context.rescueProgress  = (this->context.rescueProgress > 100) ? 100 : ((this->context.rescueProgress < 0) ? 0 : this->context.rescueProgress);
    
    if (this->context.busterCallTimer < 0) {
        this->context.busterCallTimer = 0;
    }
}

BattleContext::BattleContext() {
    this->low = INT_MAX;
    this->turnCount = 0;
    this->morale = 0;
    this->alarmLevel = 0;
    this->rescueProgress = 0;
    this->escapeProgress = 0;
    this->busterCallTimer = 0;
    this->mainGateDestroyed = false;
    this->robinRescued = false;
    this->bridgeOpened = false;
    this->battleEnded = false;
    this->resultCode = "";
}

void BattleContext::nextTurn() {
    this->turnCount++;
}


Character::Character() {
    this->name = "";
    this->hp = 0;
    this->maxHp = 0;
    this->atk = 0;
    this->def = 0;
    this->speed = 0;
    this->energy = 0;
    this->alive = false;
}

Character::Character(string name, int hp, int atk, int def, int speed, int energy) {
    this->name = name;
    this->hp = (hp < 0) ? 0 : hp;
    this->maxHp = this->hp;
    this->atk = atk;
    this->def = def;
    this->speed = speed;
    this->energy = (energy < 0) ? 0 : energy;
    this->alive = (this->hp > 0);
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
    this->def = (def < 0) ? 0 : def;
}

void Character::setSpeed(int speed) {
    this->speed = (speed < 0) ? 0 : speed;
}

void Character::setHP(int hp) {
    this->hp = (hp < 0) ? 0 : hp;
    if (this->hp == 0) this->alive = false;
}

void Character::setEnergy(int energy) {
    this->energy = (energy < 0) ? 0 : energy;
}

int Character::attack(Building* target, BattleContext& context) { return 0; }
int Character::specialSkill(Building* target, BattleContext& context) { return 0; }
void Character::endTurn(BattleContext& context) { return; }

void Character::receiveDamage(int damage) {
    int finalDamage = damage - this->def;
    if (finalDamage < 0) finalDamage = 0;
    
    this->hp = (this->hp - finalDamage > 0) ? this->hp - finalDamage : 0;
    if (this->hp == 0) this->alive = false;
}



StrawHat::StrawHat() : Character() {
    this->bounty = 0;
}

StrawHat::StrawHat(string name, int hp, int atk, int def, int speed, int energy, long long bounty) {
    this->name = name;
    this->hp = (hp < 0) ? 0 : hp;
    this->maxHp = this->hp;
    this->atk = atk;
    this->def = def;
    this->speed = speed;
    this->energy = (energy < 0) ? 0 : energy;
    this->alive = (this->hp > 0);
    this->bounty = bounty;
}

bool StrawHat::isStrawHat() const { return true; }

string StrawHat::str() const {
    return "StrawHat[name=" + this->name + ", hp=" + to_string(this->hp) + ", atk=" + to_string(this->atk) + 
           ", def=" + to_string(this->def) + ", speed=" + to_string(this->speed) + ", energy=" + to_string(this->energy) + 
           ", bounty=" + to_string(this->bounty) + "]";
}

// --- LUFFY ---
Luffy::Luffy(string name, int hp, int atk, int def, int speed, int energy, long long bounty) 
    : StrawHat(name, hp, atk, def, speed, energy, bounty) {}

int Luffy::attack(Character* target, BattleContext& context) {
    this->attacked = true;
    double ratio = (double)this->hp / this->maxHp;
    double dmgDealt = this->atk;
    
    if (ratio < 0.3)      dmgDealt = ceil(this->atk * 1.3);
    elif (ratio < 0.5)    dmgDealt = ceil(this->atk * 1.15);

    target->receiveDamage((int)ceil(dmgDealt));
    if (!target->isAlive()) {
        this->defeated = true;
        context.morale += 5;
    }
    int effectiveDmg = (int)ceil(dmgDealt) - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Luffy::specialSkill(Character* target, BattleContext& context) {
    this->attacked = true;
    if (this->energy < 20 || (double)this->hp < this->maxHp * 0.15) return 0;
    
    this->energy -= 20;
    int dmgDealt = ceil(this->atk * 2);
    this->speed += 15;
    this->atk += 15;
    
    int effectiveDmg = dmgDealt - target->getDef();
    this->hp -= (int)ceil(0.08 * this->maxHp);
    if (this->hp <= 0) { this->hp = 0; this->alive = false; }
    
    target->receiveDamage(dmgDealt);
    if (!target->isAlive()) this->defeated = true;
    
    context.alarmLevel += 10;
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Luffy::attack(Building* target, BattleContext& context) {
    double ratio = (double)this->hp / this->maxHp;
    double dmgDealt = this->atk;
    
    if (ratio < 0.3)      dmgDealt = ceil(this->atk * 1.3);
    elif (ratio < 0.5)    dmgDealt = ceil(this->atk * 1.15);

    target->receiveDamage((int)ceil(dmgDealt));
    return (dmgDealt > 0) ? (int)ceil(dmgDealt) : 0;
}

int Luffy::specialSkill(Building* target, BattleContext& context) {
    this->attacked = true;
    if (this->energy < 20 || (double)this->hp < this->maxHp * 0.15) return 0;
    
    this->energy -= 20;
    int dmgDealt = ceil(this->atk * 2);
    this->speed += 15;
    this->atk += 15;
    
    this->hp -= (int)ceil(0.08 * this->maxHp);
    if (this->hp <= 0) { this->hp = 0; this->alive = false; }
    
    context.alarmLevel += 10;
    target->receiveDamage(dmgDealt);
    return (dmgDealt > 0) ? dmgDealt : 0;
}

void Luffy::endTurn(BattleContext& context) {
    if (((double)this->hp / this->maxHp) <= 0.3) context.morale += 3;
    if (this->defeated) this->energy += 5;
    this->defeated = false;
}

// --- ZORO ---
Zoro::Zoro(string name, int hp, int atk, int def, int speed, int energy, long long bounty) 
    : StrawHat(name, hp, atk, def, speed, energy, bounty) {}

int Zoro::attack(Character* target, BattleContext& context) {
    double dmgDealt = this->atk + ceil(0.2 * this->def);
    if (target->getHP() < 0.4 * this->maxHp) dmgDealt = ceil(dmgDealt * 1.15);
    
    target->receiveDamage((int)ceil(dmgDealt));
    if (!target->isAlive()) this->defeated = true;
    
    int effectiveDmg = (int)ceil(dmgDealt) - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Zoro::specialSkill(Character* target, BattleContext& context) {
    if (this->energy < 15) return 0;
    this->energy -= 15;
    
    double dmgDealt = ceil(2.2 * this->atk);
    if (target->getHP() < 0.5 * this->maxHp) dmgDealt = ceil(dmgDealt * 1.5);
    
    target->receiveDamage((int)ceil(dmgDealt));
    if (!target->isAlive()) {
        this->defeated = true;
        context.morale += 4;
        this->energy += 8;
    }
    int effectiveDmg = (int)ceil(dmgDealt) - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Zoro::attack(Building* target, BattleContext& context) {
    double dmgDealt = this->atk + ceil(0.2 * this->def);
    if (target->getHp() < 0.4 * target->getMaxHP()) dmgDealt = ceil(dmgDealt * 1.15);
    
    target->receiveDamage((int)ceil(dmgDealt));
    return (dmgDealt > 0) ? (int)ceil(dmgDealt) : 0;
}

int Zoro::specialSkill(Building* target, BattleContext& context) {
    if (this->energy < 15) return 0;
    this->energy -= 15;
    
    double dmgDealt = ceil(2.2 * this->atk);
    if (target->getHp() < 0.5 * target->getMaxHP()) dmgDealt = ceil(dmgDealt * 1.5);
    
    target->receiveDamage((int)ceil(dmgDealt));
    return (dmgDealt > 0) ? (int)ceil(dmgDealt) : 0;
}

void Zoro::endTurn(BattleContext& context) {
    if (this->defeated) {
        context.morale += 6;
        this->atk *= 1.05;
    }
    this->defeated = false;
}

// --- SANJI ---
Sanji::Sanji(string name, int hp, int atk, int def, int speed, int energy, long long bounty) 
    : StrawHat(name, hp, atk, def, speed, energy, bounty) {}

int Sanji::attack(Character* target, BattleContext& context) {
    double dmgDealt = this->atk + ceil(0.5 * this->speed);
    if (this->def > target->getDef()) dmgDealt = ceil(dmgDealt * 1.1);
    
    target->receiveDamage((int)ceil(dmgDealt));
    if (!target->isAlive()) this->defeated = true;
    
    int effectiveDmg = (int)ceil(dmgDealt) - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Sanji::specialSkill(Character* target, BattleContext& context) {
    if (this->energy < 18) return 0;
    this->energy -= 18;
    
    int baseDmg = (int)ceil(2.1 * this->atk);            
    int effectiveDmg = baseDmg - target->getDef();       
    if (effectiveDmg < 0) effectiveDmg = 0;
    
    target->receiveDamage(baseDmg);
    if (!target->isAlive()) this->defeated = true;
    
    if (target->getName() == "Jabra") {
        target->setDef(target->getDef() - 12);
    } else {
        target->setDef(target->getDef() - 8);
    }
    
    return effectiveDmg;
}

int Sanji::attack(Building* target, BattleContext& context) {
    double dmgDealt = this->atk + ceil(0.5 * this->speed);
    target->receiveDamage((int)ceil(dmgDealt));
    return (dmgDealt > 0) ? (int)ceil(dmgDealt) : 0;
}

int Sanji::specialSkill(Building* target, BattleContext& context) {
    if (this->energy < 18) return 0;
    this->energy -= 18;
    
    int baseDmg = (int)ceil(2.1 * this->atk);
    target->receiveDamage(baseDmg);
    
    return (baseDmg > 0) ? baseDmg : 0;
}

void Sanji::endTurn(BattleContext& context) {
    if (this->defeated) {
        context.morale += 8;
        this->atk = ceil(this->atk * 1.1);
    }
    this->defeated = false;
}

// --- NAMI ---
Nami::Nami(string name, int hp, int atk, int def, int speed, int energy, long long bounty) 
    : StrawHat(name, hp, atk, def, speed, energy, bounty) {}

int Nami::attack(Character* target, BattleContext& context) {
    double dmgDealt = this->atk + floor(0.3 * target->getDef());
    target->receiveDamage((int)ceil(dmgDealt));
    if (!target->isAlive()) this->defeated = true;
    
    int effectiveDmg = (int)ceil(dmgDealt) - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Nami::specialSkill(Character* target, BattleContext& context) {
    if (this->energy < 20) return 0;
    this->energy -= 20;
    
    double dmgDealt = this->atk + 40;
    target->receiveDamage((int)ceil(dmgDealt));
    target->setSpeed(target->getSpeed() - 10);
    
    if (!target->isAlive()) this->defeated = true;
    
    context.busterCallTimer++;
    context.alarmLevel -= 5;
    
    int effectiveDmg = (int)ceil(dmgDealt) - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Nami::attack(Building* target, BattleContext& context) {
    double dmgDealt = ceil(this->atk * 0.5);
    target->receiveDamage((int)ceil(dmgDealt));
    return (dmgDealt > 0) ? (int)ceil(dmgDealt) : 0;
}

int Nami::specialSkill(Building* target, BattleContext& context) {
    if (this->energy < 20) return 0;
    this->energy -= 20;
    
    double dmgDealt = (this->atk + 40) + ceil((this->atk + 40) * 0.5);
    target->receiveDamage((int)ceil(dmgDealt));
    
    context.busterCallTimer++;
    context.alarmLevel -= 5;
    return (dmgDealt > 0) ? (int)ceil(dmgDealt) : 0;
}

void Nami::endTurn(BattleContext& context) {
    if (this->defeated) {
        this->energy += 6;
        context.morale += 5;
    }
    this->defeated = false;
}

// --- CHOPPER ---
Chopper::Chopper(string name, int hp, int atk, int def, int speed, int energy, long long bounty) 
    : StrawHat(name, hp, atk, def, speed, energy, bounty) {}

int Chopper::attack(Character* target, BattleContext& context) {
    double dmgDealt = this->atk;
    target->receiveDamage((int)ceil(dmgDealt));
    if (!target->isAlive()) this->defeated = true;
    
    int effectiveDmg = (int)ceil(dmgDealt) - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Chopper::specialSkill(Character* target, BattleContext& context) {
    if (this->energy < 15) return 0;
    this->energy -= 15;
    
    double pointsToHeal = 35 + ceil(0.5 * this->atk);
    double capHeal = (target->getHP() + pointsToHeal > target->getMaxHP()) ? target->getMaxHP() - target->getHP() : pointsToHeal;
    
    target->setHP(target->getHP() + (int)ceil(capHeal));
    if (target->getName() == "Luffy") context.morale += 5;
    
    return 0;
}

int Chopper::attack(Building* target, BattleContext& context) {
    double dmgDealt = this->atk;
    target->receiveDamage((int)ceil(dmgDealt));
    return (dmgDealt > 0) ? (int)ceil(dmgDealt) : 0;
}

void Chopper::endTurn(BattleContext& context) {
    if (this->defeated) context.morale += 5;
    this->defeated = false;
}

// --- USOPP ---
Usopp::Usopp(string name, int hp, int atk, int def, int speed, int energy, long long bounty) 
    : StrawHat(name, hp, atk, def, speed, energy, bounty) {}

int Usopp::attack(Character* target, BattleContext& context) {
    double dmgDealt = this->atk;
    if (target->getSpeed() < 50) dmgDealt = ceil(dmgDealt * 1.2);
    
    target->receiveDamage((int)ceil(dmgDealt));
    if (!target->isAlive()) this->defeated = true;
    
    int effectiveDmg = (int)ceil(dmgDealt) - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Usopp::specialSkill(Character* target, BattleContext& context) {
    if (this->energy < 16) return 0;
    this->energy -= 16;
    
    double dmgDealt = ceil(this->atk * 0.8);
    target->setSpeed(target->getSpeed() - 12);
    target->receiveDamage((int)ceil(dmgDealt));
    
    if (!target->isAlive()) this->defeated = true;
    context.escapeProgress += 8;
    
    int effectiveDmg = (int)ceil(dmgDealt) - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Usopp::attack(Building* target, BattleContext& context) {
    double dmgDealt = this->atk * 0.5;
    target->receiveDamage((int)ceil(dmgDealt));
    return (dmgDealt > 0) ? (int)ceil(dmgDealt) : 0;
}

int Usopp::specialSkill(Building* target, BattleContext& context) {
    if (this->energy < 16) return 0;
    this->energy -= 16;
    
    double dmgDealt = ceil(this->atk * 0.8);
    target->receiveDamage((int)ceil(dmgDealt));
    context.escapeProgress += 8;
    return (dmgDealt > 0) ? (int)ceil(dmgDealt) : 0;
}

void Usopp::endTurn(BattleContext& context) {
    context.morale += 10;
    this->defeated = false;
}

// --- FRANKY ---
Franky::Franky(string name, int hp, int atk, int def, int speed, int energy, long long bounty) 
    : StrawHat(name, hp, atk, def, speed, energy, bounty) {}

int Franky::attack(Character* target, BattleContext& context) {
    double dmgDealt = this->atk + ceil(0.3 * this->def);
    if (target->isCP9()) dmgDealt += ceil(dmgDealt * 0.1);
    
    target->receiveDamage((int)ceil(dmgDealt));
    if (!target->isAlive()) this->defeated = true;
    
    int effectiveDmg = (int)ceil(dmgDealt) - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Franky::specialSkill(Character* target, BattleContext& context) {
    if (this->energy < 20) return 0;
    
    if (this->energy < 30) {
        this->energy -= 20;
        double dmgDealt = this->atk + ceil(0.8 * this->atk);
        if (target->getName() == "Lucci") dmgDealt += ceil(dmgDealt * 0.2);
        
        target->receiveDamage((int)ceil(dmgDealt));
        if (!target->isAlive()) this->defeated = true;
        
        target->setSpeed(target->getSpeed() - 8);
        int effectiveDmg = (int)ceil(dmgDealt) - target->getDef();
        return (effectiveDmg > 0) ? effectiveDmg : 0;
    }
    
    this->energy -= 30;
    double dmgDealt = this->atk + ceil(0.2 * this->atk);
    target->receiveDamage((int)ceil(dmgDealt));
    if (!target->isAlive()) this->defeated = true;
    
    int effectiveDmg = (int)ceil(dmgDealt) - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Franky::attack(Building* target, BattleContext& context) {
    double dmgDealt = this->atk + ceil(0.3 * this->def);
    target->receiveDamage((int)ceil(dmgDealt));
    return (dmgDealt > 0) ? (int)ceil(dmgDealt) : 0;
}

int Franky::specialSkill(Building* target, BattleContext& context) {
    if (this->energy < 20) return 0;
    
    if (this->energy < 30) {
        this->energy -= 20;
        double dmgDealt = this->atk + ceil(0.8 * this->atk);
        target->receiveDamage((int)ceil(dmgDealt));
        return (dmgDealt > 0) ? (int)ceil(dmgDealt) : 0;
    }
    
    this->energy -= 30;
    int buildingCurrentHp = target->getHp();
    target->receiveDamage(buildingCurrentHp);
    return (buildingCurrentHp > 0) ? buildingCurrentHp : 0;
}

void Franky::endTurn(BattleContext& context) {
    if (this->defeated) context.morale += 5;
    
    if (this->hp > 0.7 * this->maxHp)       this->def += 5;
    elif (this->hp < 0.3 * this->maxHp)    this->atk += ceil(this->atk * 0.1);
    
    this->defeated = false;
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

// --- LUCCI ---
Lucci::Lucci(string name, int hp, int atk, int def, int speed, int energy, int doriki) : CP9Agent(name, hp, atk, def, speed, energy, doriki) {}

int Lucci::attack(Character* target, BattleContext& context) {
    double dmgDealt = this->atk + ceil(this->doriki / 20.0);
    if (target->getHP() < 0.5 * target->getMaxHP()) dmgDealt += ceil(dmgDealt * 0.2);
    
    target->receiveDamage((int)ceil(dmgDealt));
    if (!target->isAlive()) this->defeated = true;
    
    int effectiveDmg = (int)ceil(dmgDealt) - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Lucci::specialSkill(Character* target, BattleContext& context) {
    if (this->energy < 25) return 0;
    this->energy -= 25;
    
    double dmgDealt = this->atk * 2 + ceil(0.8 * this->atk) + floor(target->getDef() * 0.5);
    target->receiveDamage((int)ceil(dmgDealt));
    
    if (!target->isAlive()) {
        this->defeated = true;
        context.morale -= 10;
    }
    int effectiveDmg = (int)ceil(dmgDealt) - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

void Lucci::endTurn(BattleContext& context) {
    if (this->hp < 0.4 * this->maxHp) this->atk += ceil(0.05 * this->atk);
    this->defeated = false;
}

// --- KAKU ---
Kaku::Kaku(string name, int hp, int atk, int def, int speed, int energy, int doriki) 
    : CP9Agent(name, hp, atk, def, speed, energy, doriki) {}

int Kaku::attack(Character* target, BattleContext& context) {
    double dmgDealt = this->atk;
    target->receiveDamage((int)ceil(dmgDealt));
    if (!target->isAlive()) this->defeated = true;
    
    int effectiveDmg = (int)ceil(dmgDealt) - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Kaku::specialSkill(Character* target, BattleContext& context) {
    if (this->energy < 20) return 0;
    this->energy -= 20;
    
    double totalDmg = 0;
    int multiHitCount = 0;
    double multipliers[] = {1.2, 1.0, 0.8};
    
    for (int i = 0; i < 3; ++i) {
        ++multiHitCount;
        double currentHitDmg = ceil(multipliers[i] * this->atk);
        totalDmg += currentHitDmg;
        target->receiveDamage((int)ceil(currentHitDmg));
        if (!target->isAlive()) {
            this->defeated = true;
            break;
        }
    }
    int effectiveDmg = (int)ceil(totalDmg) - multiHitCount * target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

void Kaku::endTurn(BattleContext& context) {
    if (this->defeated) context.morale -= 5;
    this->defeated = false;
}

// --- JABRA ---
Jabra::Jabra(string name, int hp, int atk, int def, int speed, int energy, int doriki) 
    : CP9Agent(name, hp, atk, def, speed, energy, doriki) {}

int Jabra::attack(Character* target, BattleContext& context) {
    target->receiveDamage((int)ceil(this->atk));
    if (!target->isAlive()) this->defeated = true;
    
    int effectiveDmg = (int)ceil(this->atk) - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Jabra::specialSkill(Character* target, BattleContext& context) {
    if (this->energy < 18) return 0;
    this->energy -= 18;
    
    double dmgDealt = this->atk + ceil(this->atk * 0.5);
    if (this->hp < 0.35 * this->maxHp) dmgDealt += ceil(dmgDealt * 0.25);
    
    target->receiveDamage((int)ceil(dmgDealt));
    if (!target->isAlive()) {
        this->defeated = true;
        context.morale -= 5;
    }
    int effectiveDmg = (int)ceil(dmgDealt) - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

void Jabra::endTurn(BattleContext& context) {
    this->defeated = false;
}

// --- BLUENO ---
Blueno::Blueno(string name, int hp, int atk, int def, int speed, int energy, int doriki) 
    : CP9Agent(name, hp, atk, def, speed, energy, doriki) {}

int Blueno::attack(Character* target, BattleContext& context) {
    target->receiveDamage((int)ceil(this->atk));
    if (!target->isAlive()) this->defeated = true;
    
    int effectiveDmg = (int)ceil(this->atk) - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Blueno::specialSkill(Character* target, BattleContext& context) {
    if (this->energy < 15) return 0;
    this->energy -= 15;
    
    double dmgDealt = this->atk + ceil(this->atk * 0.3);
    dmgDealt += (this->hp > 0.5 * this->maxHp) ? 20 : 40;
    
    target->receiveDamage((int)ceil(dmgDealt));
    if (!target->isAlive()) this->defeated = true;
    
    int effectiveDmg = (int)ceil(dmgDealt) - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

void Blueno::endTurn(BattleContext& context) {
    if (this->defeated) context.morale -= 5;
    this->defeated = false;
}

// --- KALIFA ---
Kalifa::Kalifa(string name, int hp, int atk, int def, int speed, int energy, int doriki) 
    : CP9Agent(name, hp, atk, def, speed, energy, doriki) {}

int Kalifa::attack(Character* target, BattleContext& context) {
    target->receiveDamage((int)ceil(this->atk));
    if (!target->isAlive()) {
        this->defeated = true;
        context.morale -= 5;
    }
    int effectiveDmg = (int)ceil(this->atk) - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Kalifa::specialSkill(Character* target, BattleContext& context) {
    if (this->energy < 18) return 0;
    this->energy -= 18;
    
    double dmgDealt = this->atk + ceil(this->atk * 0.4);
    target->receiveDamage((int)ceil(dmgDealt));
    if (!target->isAlive()) this->defeated = true;
    
    context.morale -= (target->getName() == "Nami") ? 12 : 8;
    target->setSpeed(target->getSpeed() - 6);
    
    int effectiveDmg = (int)ceil(dmgDealt) - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

void Kalifa::endTurn(BattleContext& context) {
    this->defeated = false;
}

// --- KUMADORI ---
Kumadori::Kumadori(string name, int hp, int atk, int def, int speed, int energy, int doriki) 
    : CP9Agent(name, hp, atk, def, speed, energy, doriki) {}

int Kumadori::attack(Character* target, BattleContext& context) {
    target->receiveDamage((int)ceil(this->atk));
    if (!target->isAlive()) this->defeated = true;
    
    int effectiveDmg = (int)ceil(this->atk) - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Kumadori::specialSkill(Character* target, BattleContext& context) {
    if (this->energy < 16) return 0;
    this->energy -= 16;
    
    double dmgDealt = 30 + ceil(0.1 * this->doriki);
    if (this->hp < 0.4 * this->maxHp) dmgDealt += 25;
    
    target->receiveDamage((int)ceil(dmgDealt));
    if (!target->isAlive()) this->defeated = true;
    
    int effectiveDmg = (int)ceil(dmgDealt) - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

void Kumadori::endTurn(BattleContext& context) {
    if (this->defeated) context.morale -= 5;
    this->defeated = false;
}

// --- FUKUROU ---
Fukurou::Fukurou(string name, int hp, int atk, int def, int speed, int energy, int doriki) 
    : CP9Agent(name, hp, atk, def, speed, energy, doriki) {}

int Fukurou::attack(Character* target, BattleContext& context) {
    target->receiveDamage((int)ceil(this->atk));
    if (!target->isAlive()) this->defeated = true;
    
    int effectiveDmg = (int)ceil(this->atk) - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

int Fukurou::specialSkill(Character* target, BattleContext& context) {
    if (this->energy < 14) return 0;
    this->energy -= 14;
    
    double dmgDealt = this->atk + ceil(this->atk * 0.3);
    if (target->getHP() == context.low) dmgDealt += 20;
    
    target->receiveDamage((int)ceil(dmgDealt));
    if (!target->isAlive()) {
        this->defeated = true;
        context.morale -= 6;
    }
    int effectiveDmg = (int)ceil(dmgDealt) - target->getDef();
    return (effectiveDmg > 0) ? effectiveDmg : 0;
}

void Fukurou::endTurn(BattleContext& context) {
    this->defeated = false;
}

// ==========================================
// BUILDING IMPLEMENTATIONS
// ==========================================

Building::Building(string name, int hp) {
    this->name = name;
    this->hp = (hp < 0) ? 0 : hp;
    this->maxHP = hp;
    this->destroyed = (hp <= 0);
}

Building::~Building() {}

string Building::getName() const { return this->name; }
int Building::getMaxHP() const  { return this->maxHP; }
int Building::getHp() const     { return this->hp; }
bool Building::isDestroyed() const { return this->destroyed; }
void Building::onDestroyed(BattleContext& context) { return; }

void Building::receiveDamage(int damage) {
    this->hp = (this->hp - damage > 0) ? this->hp - damage : 0;
    if (this->hp == 0) this->destroyed = true;
}

string Building::str() const {
    return "Building[name=" + this->name + ", hp=" + to_string(this->hp) + ", maxHP=" + to_string(this->maxHP) + 
           ", destroyed=" + to_string(this->destroyed) + "]";
}


MainGate::MainGate(string name, int hp) : Building(name, hp) {}
void MainGate::applyEffect(BattleContext& context) {}
void MainGate::onDestroyed(BattleContext& context) {
    context.mainGateDestroyed = true;
    context.rescueProgress += 20;
    context.morale += 5;
}


Courthouse::Courthouse(string name, int hp) : Building(name, hp) {}
void Courthouse::applyEffect(BattleContext& context) {
    if (!this->destroyed) context.alarmLevel += 5;
}
void Courthouse::onDestroyed(BattleContext& context) {
    context.alarmLevel -= 20;
    if (context.alarmLevel < 0) context.alarmLevel = 0;
}

// --- TOWER OF JUSTICE ---
TowerOfJustice::TowerOfJustice(string name, int hp) : Building(name, hp) {}
void TowerOfJustice::applyEffect(BattleContext& context) {
    if (context.mainGateDestroyed && !context.robinRescued) {
        context.rescueProgress += 5;
        if (context.rescueProgress >= 100) {
            context.rescueProgress = 100;
            context.morale += 10;
            context.robinRescued = true;
        }
    }
}

// --- BRIDGE OF HESITATION ---
BridgeOfHesitation::BridgeOfHesitation(string name, int hp) : Building(name, hp) {}
void BridgeOfHesitation::applyEffect(BattleContext& context) {
    if (this->destroyed) return;
    
    if (context.robinRescued) {
        context.bridgeOpened = true;
        context.escapeProgress = (context.escapeProgress + 5 > 100) ? 100 : context.escapeProgress + 5;
        
        if (context.escapeProgress == 100) {
            context.battleEnded = true;
            context.resultCode = "STRAW_HAT_WIN";
        }
    }
}

// --- BUSTER CALL SHIP ---
BusterCallShip::BusterCallShip(string name, int hp) : Building(name, hp) {}
void BusterCallShip::applyEffect(BattleContext& context) {
    if (!this->destroyed) {
        context.busterCallTimer -= 1;
        if (context.busterCallTimer <= 0) {
            context.battleEnded = true;
            context.busterCallTimer = 0;
            
            context.resultCode = "BUSTER_CALL";
        }
    }
}
void BusterCallShip::onDestroyed(BattleContext& context) {
    context.busterCallTimer += 3;
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
        TurnNode* nextNode = nullptr;
        while (curr->next != this->turnOrder) {
            nextNode = curr->next;
            curr->next = nextNode->next;
            delete nextNode;
        }
        delete curr;
        this->turnOrder = nullptr;
    }
    
    for (int i = 0; i < this->strawHatCount; ++i) delete this->strawHats[i];
    for (int i = 0; i < this->cp9Count; ++i)      delete this->cp9Agents[i];
    for (int i = 0; i < this->buildingCount; ++i)  delete this->buildings[i];
    
    delete[] this->buildings;
    delete[] this->strawHats;
    delete[] this->cp9Agents;
}

void EniesLobbyBattle::addStrawHat(Character* character) {
    if (this->strawHatCount < 7) this->strawHats[this->strawHatCount++] = character;
}

void EniesLobbyBattle::addCP9Agent(Character* character) {
    if (this->cp9Count < 7) this->cp9Agents[this->cp9Count++] = character;
}

void EniesLobbyBattle::addBuilding(Building* building) {
    if (this->buildingCount < 5) this->buildings[this->buildingCount++] = building;
}

void EniesLobbyBattle::buildTurnOrder() {
    if (this->strawHatCount == 0 && this->cp9Count == 0) return;
    
    TurnNode* currentTail = nullptr;
    this->turnOrder = new TurnNode(this->strawHats[0]);
    currentTail = this->turnOrder;
    
    for (int i = 1; i < this->strawHatCount; ++i) {
        currentTail->next = new TurnNode(this->strawHats[i]);
        currentTail = currentTail->next;
    }
    for (int i = 0; i < this->cp9Count; ++i) {
        currentTail->next = new TurnNode(this->cp9Agents[i]);
        currentTail = currentTail->next;
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

            if (name == "Luffy")       {
                this->addStrawHat(new Luffy(name, hp, atk, def, speed, energy, bounty));
            }
            elif (name == "Zoro")    {
                this->addStrawHat(new Zoro(name, hp, atk, def, speed, energy, bounty));
            }
            elif (name == "Nami")    {
                this->addStrawHat(new Nami(name, hp, atk, def, speed, energy, bounty));
            }
            elif (name == "Usopp")   {
                this->addStrawHat(new Usopp(name, hp, atk, def, speed, energy, bounty));
            }
            elif (name == "Sanji")  {
                this->addStrawHat(new Sanji(name, hp, atk, def, speed, energy, bounty));
            }
            elif (name == "Chopper") {
                this->addStrawHat(new Chopper(name, hp, atk, def, speed, energy, bounty));
            }
            elif (name == "Franky")  {
                this->addStrawHat(new Franky(name, hp, atk, def, speed, energy, bounty));

        }
    }
        elif (entryType == "CP9") {
            string name; int hp, atk, def, speed, energy, doriki;
            parsingStream >> name >> hp >> atk >> def >> speed >> energy >> doriki;
            if (name == "Lucci")       {
                this->addCP9Agent(new Lucci(name, hp, atk, def, speed, energy, doriki));
            }
            elif (name == "Kaku")    {
                this->addCP9Agent(new Kaku(name, hp, atk, def, speed, energy, doriki));
            }
            elif (name == "Jabra")   {
                this->addCP9Agent(new Jabra(name, hp, atk, def, speed, energy, doriki));
            }
            elif (name == "Blueno")  {
                this->addCP9Agent(new Blueno(name, hp, atk, def, speed, energy, doriki));
            }
            elif (name == "Kalifa")  {
                this->addCP9Agent(new Kalifa(name, hp, atk, def, speed, energy, doriki));
            }
            elif (name == "Kumadori") {
                this->addCP9Agent(new Kumadori(name, hp, atk, def, speed, energy, doriki));
            }
            elif (name == "Fukurou")  {
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

void EniesLobbyBattle::runBattle() {
    if (this->turnOrder == nullptr) return;
    
    while (!this->context.battleEnded && this->context.turnCount < this->maxTurns) {
        Character* curActor = this->turnOrder->data;
        if (curActor && curActor->isAlive()) {
            this->processTurn(curActor);
        }
        
        this->turnOrder = this->turnOrder->next;
        this->processBuildings();
        this->context.turnCount++;
        this->checkEndCondition();
    }
}


void EniesLobbyBattle::processTurn(Character* character) {
    if (!character || !character->isAlive()) return;
    
    Character* tgtChar = nullptr;
    Building* tgtBld = nullptr;
    
    string cName = character->getName();
    int curEnergy = character->getEnergy();
    int curHp = character->getHP();
    int mHp = character->getMaxHP();
    bool invokeSpecialSkill = false;
    
    if (character->isStrawHat()) {
        bool hasHealed = false;
        if (cName == "Chopper" && curEnergy >= 15) {
            int absoluteMinHp = INT_MAX;
            for (int i = 0; i < this->strawHatCount; ++i) {
                if (this->strawHats[i]->isAlive() && this->strawHats[i]->getHP() < absoluteMinHp) {
                    absoluteMinHp = this->strawHats[i]->getHP();
                    tgtChar = this->strawHats[i];
                }
            }
            if (tgtChar) hasHealed = true;
        }
        
        if (!hasHealed) {
            Building* gateNode = nullptr;
            Building* courtNode = nullptr;
            Building* shipNode = nullptr;
            Building* bridgeNode = nullptr;
            
            for (int i = 0; i < this->buildingCount; ++i) {
                string bName = this->buildings[i]->getName();
                if (bName == "MainGate")             gateNode = this->buildings[i];
                elif (bName == "Courthouse")          courtNode = this->buildings[i];
                elif (bName == "BusterCallShip")      shipNode = this->buildings[i];
                elif (bName == "BridgeOfHesitation")  bridgeNode = this->buildings[i];
            }
            
            // Ép kiểm tra an toàn nullptr lên đầu điều kiện nhánh để tránh crash
            if (gateNode && !this->context.mainGateDestroyed && !gateNode->isDestroyed()) {
                tgtBld = gateNode;
            } 
            elif (courtNode && this->context.alarmLevel >= 50 && !courtNode->isDestroyed()) {
                tgtBld = courtNode;
            } 
            elif (shipNode && this->context.busterCallTimer <= 5 && !shipNode->isDestroyed()) {
                tgtBld = shipNode;
            } 
            elif (!this->context.robinRescued) {
                for (int i = 0; i < this->cp9Count; ++i) {
                    if (this->cp9Agents[i]->isAlive()) {
                        tgtChar = this->cp9Agents[i];
                        break;
                    }
                }
            } 
            else {
                if (bridgeNode && this->context.robinRescued && !bridgeNode->isDestroyed()) {
                    tgtBld = bridgeNode;
                } else {
                    for (int i = 0; i < this->cp9Count; ++i) {
                        if (this->cp9Agents[i]->isAlive()) {
                            tgtChar = this->cp9Agents[i];
                            break;
                        }
                    }
                }
            }
        }
    } 
    else {
        for (int i = 0; i < this->strawHatCount; ++i) {
            if (this->strawHats[i]->isAlive()) {
                tgtChar = this->strawHats[i];
                break;
            }
        }
        int lowestSHHp = INT_MAX;
        for (int i = 0; i < this->strawHatCount; ++i) {
            if (this->strawHats[i]->isAlive() && this->strawHats[i]->getHP() < lowestSHHp) {
                lowestSHHp = this->strawHats[i]->getHP();
            }
        }
        this->context.low = lowestSHHp;
    }
    
    if (!tgtChar && !tgtBld) {
        character->endTurn(this->context);
        this->clamp();
        return;
    }
    
   
    if (cName == "Luffy")        invokeSpecialSkill = (curEnergy >= 20 && curHp >= ceil(mHp * 0.15));
    elif (cName == "Zoro")     invokeSpecialSkill = (curEnergy >= 15);
    elif (cName == "Sanji")    invokeSpecialSkill = (curEnergy >= 18);
    elif (cName == "Nami")     invokeSpecialSkill = (curEnergy >= 20);
    elif (cName == "Chopper")  invokeSpecialSkill = (curEnergy >= 15);
    elif (cName == "Usopp")    invokeSpecialSkill = (curEnergy >= 16);
    elif (cName == "Franky")   invokeSpecialSkill = (curEnergy >= 20);
    elif (cName == "Lucci")    invokeSpecialSkill = (curEnergy >= 25);
    elif (cName == "Kaku")     invokeSpecialSkill = (curEnergy >= 20);
    elif (cName == "Jabra")    invokeSpecialSkill = (curEnergy >= 18);
    elif (cName == "Blueno")   invokeSpecialSkill = (curEnergy >= 15);
    elif (cName == "Kalifa")   invokeSpecialSkill = (curEnergy >= 18);
    elif (cName == "Kumadori")  invokeSpecialSkill = (curEnergy >= 16);
    elif (cName == "Fukurou")   invokeSpecialSkill = (curEnergy >= 14);
    
    if (tgtChar) {
        if (invokeSpecialSkill) character->specialSkill(tgtChar, this->context);
        else                    character->attack(tgtChar, this->context);
    } 
    elif (tgtBld) {
        bool isDestroyedBefore = tgtBld->isDestroyed();
        if (invokeSpecialSkill) character->specialSkill(tgtBld, this->context);
        else                    character->attack(tgtBld, this->context);
        
        if (!isDestroyedBefore && tgtBld->isDestroyed()) {
            tgtBld->onDestroyed(this->context);
        }
    }
    
    character->endTurn(this->context);
    this->clamp();
}

void EniesLobbyBattle::processBuildings() {
    for (int i = 0; i < this->buildingCount; ++i) {
        this->buildings[i]->applyEffect(this->context);
    }
}

void EniesLobbyBattle::checkEndCondition() {
    if (this->context.robinRescued && this->context.escapeProgress >= 100) {
        this->context.battleEnded = true;
        this->context.resultCode = "STRAW_HAT_WIN";
        return;
    }
    if (this->context.busterCallTimer <= 0) {
        this->context.resultCode = "BUSTER_CALL";
        this->context.battleEnded = true;
        return;
    }
    
    bool hasStrawHatsAlive = false;
    for (int i = 0; i < this->strawHatCount; ++i) {
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
    for (int i = 0; i < this->cp9Count; ++i) {
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
