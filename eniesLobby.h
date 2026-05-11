#ifndef ENIES_LOBBY_H
#define ENIES_LOBBY_H

#include "main.h"

// Forward declarations
class BattleContext;
class Building;
class Character;
struct TurnNode;

/*
 * Character class - Base class for all fighters
 */
class Character
{
protected:
    string name;        // character name
    int hp;             // current health
    int maxHp;          // maximum health
    int atk;            // attack power
    int def;            // defense power
    int speed;          // speed stat
    int energy;         // special skill energy
    bool alive;         // living status

public:
    Character();
    Character(string name, int hp, int atk, int def, int speed, int energy);
    virtual ~Character();

    // Combat actions
    virtual int attack(Character *target, BattleContext &context) = 0;
    virtual int specialSkill(Character *target, BattleContext &context) = 0;

    virtual int attack(Building *target, BattleContext &context);
    virtual int specialSkill(Building *target, BattleContext &context);

    virtual void endTurn(BattleContext &context);

    // Damage and healing
    int receiveDamage(int damage);
    int receiveDamageBypass(int damage, int bypassPercent);
    void receiveHeal(int amount);
    
    // Stat modifications
    void reduceDef(int amount);
    void reduceSpeed(int amount);
    
    // Getters
    bool isAlive() const;
    string getName() const;
    int getHP() const;
    int getMaxHp() const;
    int getDef() const;
    int getSpeed() const;
    int getEnergy() const;

    // Type checking
    virtual bool isStrawHat() const;
    virtual bool isCP9() const;

    // String representation
    virtual string str() const = 0;
};

/*
 * StrawHat class - Pirate side characters
 */
class StrawHat : public Character
{
protected:
    long long bounty;   // wanted bounty amount

public:
    StrawHat();
    StrawHat(string name, int hp, int atk, int def,
             int speed, int energy, long long bounty);

    virtual bool isStrawHat() const;
    virtual string str() const;
};

// ==================== STRAW HAT MEMBERS ====================

/*
 * Luffy - Captain with Gear Second ability
 */
class Luffy : public StrawHat
{
private:
    bool killedThisTurn;

public:
    Luffy(string name, int hp, int atk, int def,
          int speed, int energy, long long bounty);

    int attack(Character *target, BattleContext &context);
    int specialSkill(Character *target, BattleContext &context);

    int attack(Building *target, BattleContext &context);
    int specialSkill(Building *target, BattleContext &context);

    void endTurn(BattleContext &context);
};

/*
 * Zoro - Three-sword style swordsman
 */
class Zoro : public StrawHat
{
private:
    bool killedThisTurn;

public:
    Zoro(string name, int hp, int atk, int def,
         int speed, int energy, long long bounty);

    int attack(Character *target, BattleContext &context);
    int specialSkill(Character *target, BattleContext &context);

    int attack(Building *target, BattleContext &context);
    int specialSkill(Building *target, BattleContext &context);

    void endTurn(BattleContext &context);
};

/*
 * Sanji - Black Leg Style fighter
 */
class Sanji : public StrawHat
{
private:
    bool killedThisTurn;

public:
    Sanji(string name, int hp, int atk, int def,
          int speed, int energy, long long bounty);

    int attack(Character *target, BattleContext &context);
    int specialSkill(Character *target, BattleContext &context);

    int attack(Building *target, BattleContext &context);
    int specialSkill(Building *target, BattleContext &context);

    void endTurn(BattleContext &context);
};

/*
 * Nami - Navigator with weather abilities
 */
class Nami : public StrawHat
{
private:
    bool killedThisTurn;

public:
    Nami(string name, int hp, int atk, int def,
         int speed, int energy, long long bounty);

    int attack(Character *target, BattleContext &context);
    int specialSkill(Character *target, BattleContext &context);

    int attack(Building *target, BattleContext &context);
    int specialSkill(Building *target, BattleContext &context);

    void endTurn(BattleContext &context);
};

/*
 * Chopper - Doctor with healing abilities
 */
class Chopper : public StrawHat
{
public:
    Chopper(string name, int hp, int atk, int def,
            int speed, int energy, long long bounty);

    int attack(Character *target, BattleContext &context);
    int specialSkill(Character *target, BattleContext &context);

    int attack(Building *target, BattleContext &context);

    void endTurn(BattleContext &context);
};

/*
 * Usopp - Sniper with support skills
 */
class Usopp : public StrawHat
{
private:
    bool attackedThisTurn;

public:
    Usopp(string name, int hp, int atk, int def,
          int speed, int energy, long long bounty);

    int attack(Character *target, BattleContext &context);
    int specialSkill(Character *target, BattleContext &context);

    int attack(Building *target, BattleContext &context);
    int specialSkill(Building *target, BattleContext &context);

    void endTurn(BattleContext &context);
};

/*
 * Franky - Cyborg with heavy weapons
 */
class Franky : public StrawHat
{
public:
    Franky(string name, int hp, int atk, int def,
           int speed, int energy, long long bounty);

    int attack(Character *target, BattleContext &context);
    int specialSkill(Character *target, BattleContext &context);

    int attack(Building *target, BattleContext &context);
    int specialSkill(Building *target, BattleContext &context);

    void endTurn(BattleContext &context);
};

// ==================== CP9 AGENTS ====================

/*
 * CP9Agent class - Government assassination unit
 */
class CP9Agent : public Character
{
protected:
    int doriki;         // power level rating

public:
    CP9Agent();
    CP9Agent(string name, int hp, int atk, int def,
             int speed, int energy, int doriki);

    virtual bool isCP9() const;
    virtual string str() const;
};

/*
 * Lucci - Leopard-style strongest CP9 member
 */
class Lucci : public CP9Agent
{
public:
    Lucci(string name, int hp, int atk, int def,
          int speed, int energy, int doriki);

    int attack(Character *target, BattleContext &context);
    int specialSkill(Character *target, BattleContext &context);
    void endTurn(BattleContext &context);
};

/*
 * Kaku - Giraffe-style swordsman
 */
class Kaku : public CP9Agent
{
public:
    Kaku(string name, int hp, int atk, int def,
         int speed, int energy, int doriki);

    int attack(Character *target, BattleContext &context);
    int specialSkill(Character *target, BattleContext &context);
    void endTurn(BattleContext &context);
};

/*
 * Jabra - Wolf-style fighter
 */
class Jabra : public CP9Agent
{
public:
    Jabra(string name, int hp, int atk, int def,
          int speed, int energy, int doriki);

    int attack(Character *target, BattleContext &context);
    int specialSkill(Character *target, BattleContext &context);
    void endTurn(BattleContext &context);
};

/*
 * Blueno - Bull-style with door powers
 */
class Blueno : public CP9Agent
{
public:
    Blueno(string name, int hp, int atk, int def,
           int speed, int energy, int doriki);

    int attack(Character *target, BattleContext &context);
    int specialSkill(Character *target, BattleContext &context);
    void endTurn(BattleContext &context);
};

/*
 * Kalifa - Bubble-style soap user
 */
class Kalifa : public CP9Agent
{
public:
    Kalifa(string name, int hp, int atk, int def,
           int speed, int energy, int doriki);

    int attack(Character *target, BattleContext &context);
    int specialSkill(Character *target, BattleContext &context);
    void endTurn(BattleContext &context);
};

/*
 * Kumadori - Hair-style freestyle fighter
 */
class Kumadori : public CP9Agent
{
public:
    Kumadori(string name, int hp, int atk, int def,
             int speed, int energy, int doriki);

    int attack(Character *target, BattleContext &context);
    int specialSkill(Character *target, BattleContext &context);
    void endTurn(BattleContext &context);
};

/*
 * Fukurou - Ping-pong style with speed detection
 */
class Fukurou : public CP9Agent
{
public:
    bool isLowestTarget;  // flag for special skill bonus
    
    Fukurou(string name, int hp, int atk, int def,
            int speed, int energy, int doriki);

    int attack(Character *target, BattleContext &context);
    int specialSkill(Character *target, BattleContext &context);
    void endTurn(BattleContext &context);
};

// ==================== BATTLE CONTEXT ====================

/*
 * BattleContext - Tracks overall battle state
 */
class BattleContext
{
public:
    int turnCount;           // number of turns elapsed
    int morale;              // straw hat morale (0-100)
    int alarmLevel;          // marine alert level (0-100)
    int rescueProgress;      // robin rescue progress (0-100)
    int escapeProgress;      // escape progress (0-100)
    int busterCallTimer;     // countdown to buster call
    bool mainGateDestroyed;  // main gate status
    bool robinRescued;       // robin rescue status
    bool bridgeOpened;       // bridge connection status
    bool battleEnded;        // battle completion flag
    string resultCode;       // final battle outcome

    BattleContext();
    void nextTurn();
};

// ==================== BUILDING CLASSES ====================

/*
 * Building - Base class for all structures
 */
class Building
{
protected:
    string name;             // building identifier
    int hp;                  // current health
    int maxHP;               // maximum health
    bool destroyed;          // destruction status
    bool onDestroyedDone;    // one-time trigger flag

public:
    Building(string name, int hp);
    virtual ~Building();

    void receiveDamage(int damage);
    void forceDestroy();
    bool isDestroyed() const;
    string getName() const;

    virtual void applyEffect(BattleContext &context) = 0;
    virtual void onDestroyed(BattleContext &context);

    virtual string str() const;
};

/*
 * MainGate - Controls access to island
 */
class MainGate : public Building
{
public:
    MainGate(string name, int hp);

    void applyEffect(BattleContext &context);
    void onDestroyed(BattleContext &context);
};

/*
 * Courthouse - Raises alarm level
 */
class Courthouse : public Building
{
public:
    Courthouse(string name, int hp);

    void applyEffect(BattleContext &context);
    void onDestroyed(BattleContext &context);
};

/*
 * TowerOfJustice - Advances rescue progress
 */
class TowerOfJustice : public Building
{
public:
    TowerOfJustice(string name, int hp);

    void applyEffect(BattleContext &context);
};

/*
 * BridgeOfHesitation - Enables escape route
 */
class BridgeOfHesitation : public Building
{
public:
    BridgeOfHesitation(string name, int hp);

    void applyEffect(BattleContext &context);
};

/*
 * BusterCallShip - Countdown to bad ending
 */
class BusterCallShip : public Building
{
public:
    BusterCallShip(string name, int hp);

    void applyEffect(BattleContext &context);
    void onDestroyed(BattleContext &context);
};

// ==================== TURN ORDER NODE ====================

/*
 * TurnNode - Linked list node for turn order
 */
struct TurnNode
{
    Character *data;   // character to act
    TurnNode *next;    // next node in sequence
};

// ==================== MAIN BATTLE CONTROLLER ====================

/*
 * EniesLobbyBattle - Main battle simulation controller
 */
class EniesLobbyBattle
{
private:
    Character **strawHats;   // straw hat array
    int strawHatCount;       // number of straw hats
    
    Character **cp9Agents;   // cp9 agent array
    int cp9Count;            // number of cp9 agents
    
    Building **buildings;    // building array
    int buildingCount;       // number of buildings
    
    TurnNode *turnOrder;     // action sequence
    BattleContext context;   // battle state
    int maxTurns;            // turn limit

    // Helper methods
    Building *findBuilding(const string &name);
    Character *firstAliveCP9();
    Character *firstAliveStrawHat();
    Character *lowestHpStrawHat();
    Character *lowestHpStrawHatForCP9();
    int getSkillCost(Character *c);

public:
    EniesLobbyBattle(const string &filename);
    ~EniesLobbyBattle();

    void loadFromFile(const string &filename);

    void addStrawHat(Character *character);
    void addCP9Agent(Character *character);
    void addBuilding(Building *building);

    void buildTurnOrder();

    void runBattle();
    void processTurn(Character *character);
    void processBuildings();
    void checkEndCondition();

    string getResult() const;
};

#endif // ENIES_LOBBY_H
