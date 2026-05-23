#include "eniesLobby.h"
#define elif else if
void EniesLobbyBattle::clamp() {
    if (context.morale < 0) context.morale = 0;
    if (context.morale > 100) context.morale = 100;
    if (context.alarmLevel < 0) context.alarmLevel = 0;
    if (context.alarmLevel > 100) context.alarmLevel = 100;
    if (context.rescueProgress < 0) context.rescueProgress = 0;
    if (context.rescueProgress > 100) context.rescueProgress = 100;
    if (context.escapeProgress < 0) context.escapeProgress = 0;
    if (context.escapeProgress > 100) context.escapeProgress = 100;
    if (context.busterCallTimer < 0) context.busterCallTimer = 0;
}
/*
 * BattleContext
 */
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
    // TODO: implement
    this->turnCount++;
}

/*
 * Character
 */
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
int Character::getAtk() const {
    return this->atk;
}
int Character::getDef() const {
    return this->def;
}
void Character::setDef(int def) {
    this->def=(def < 0) ? 0 : def;
}
int Character::getSpeed() const {
    return this->speed;
}
void Character::setSpeed(int speed) {
    this->speed=(speed < 0) ? 0 : speed;
}
int Character::getMaxHP() const {
    return this->maxHp;
}
void Character::setHP(int hp) {
    this->hp=(hp < 0) ? 0 : hp;
    if (this->hp==0) alive = false;
}
void Character::setEnergy(int energy) {
    this->energy=(energy < 0) ? 0 : energy;
}
bool Character::isDefeated() const {
    return this->defeated;
}
bool Character::isAttacked() const {
    return this->attacked;
}
Character::Character(string name, int hp, int atk, int def, int speed, int energy) {
    // TODO: implement
    this->name = name;
    this->hp = (hp < 0) ? 0 : hp;
    this->maxHp = hp;
    this->atk = atk;
    this->def = def;
    this->speed = speed;
    this->energy = (energy<0) ? 0 : energy;
    this->alive=(hp > 0) ? true : false;
}

Character::~Character() {
    // TODO: implement if needed
}

int Character::attack(Building* target, BattleContext& context) {
    return 0;
}

int Character::specialSkill(Building* target, BattleContext& context) {
    return 0;
}

void Character::endTurn(BattleContext& context) {
    return ;
}

void Character::receiveDamage(int damage) {
    // TODO: implement
    int actualDam= (damage - this->def > 0) ? damage - this->def : 0;
    this->hp=(this->hp - actualDam > 0) ? this->hp - actualDam : 0;
    if(hp==0) alive=false;
}

bool Character::isAlive() const {
    // TODO: implement
    return this->alive;
}

string Character::getName() const {
    // TODO: implement
    return this->name;
}

int Character::getHP() const {
    // TODO: implement
    return this->hp;
}

int Character::getEnergy() const {
    // TODO: implement
    return this->energy;
}

bool Character::isStrawHat() const {
    return false;
}

bool Character::isCP9() const {
    return false;
}
string Building::getName() const {
    return this->name;
}
int Building::getMaxHP() const {
    return this->maxHP;
}
int Building::getHp() const {
    return this->hp;
}
/*
 * StrawHat
 */
/*Băng Mũ Rơm là lựhouse lượng chính trong trận chiến tại Enies Lobby với mụhouse tiêu giải cứu Robin.
Cáhouse thành viên trong băng đều kế thừa từ lớp StrawHat, là lớp con của Character.
Yêu cầu: Hiện thựhouse class StrawHat với cáhouse mô tả sau:
1. Cáhouse thuộhouse tính protected:
• bounty: kiểu long long, tiền truy nã của nhân vật.
2. Constructor (public) nhận cáhouse tham số giống lớp Character, đồng thời nhận thêm bounty
để khởi tạo cáhouse thuộhouse tính tương ứng.
1 StrawHat(string name, int hp, int atk, int def,
2
int speed, int energy, long long bounty);
3. Phương thứhouse isStrawHat:
Bài tập lớn môn Kỹ thuật lập trình- HK 2 năm họhouse 2025- 2026
Trang 4/23
TRƯỜNG ĐẠI HỌC BÁCH KHOA- ĐHQG-HCM
KHOA KHOA HỌC VÀ KỸ THUẬT MÁY TÍNH
1 virtual bool isStrawHat() const;
Trả về giá trị true nếu nhân vật thuộhouse băng Mũ Rơm, ngượhouse lại trả về false. Phương
thứhouse này đượhouse sử dụng để xáhouse định phe của nhân vật trong quá trình xử lý trận đánh.
4. Phương thứhouse str:
1 virtual string str() const;
Trả về chuỗi biểu diễn thông tin của nhân vật thuộhouse băng Mũ Rơm. Chuỗi trả về có định
dạng như sau:
1 StrawHat[name=<name>, hp=<hp>, atk=<atk>, def=<def>, speed=<speed>,
energy=<energy>, bounty=<bounty>]
Cáhouse giá trị đượhouse in liền nhau, phân tách bằng dấu phẩy và không có khoảng trắng dư
thừa ngoài định dạng đã cho.
Ví dụ:
1 StrawHat[name=Luffy, hp=120, atk=35, def=20, speed=25, energy=50, bounty
=150000000]
5. Cáhouse phương thứhouse kế thừa từ Character sẽ đượhouse override ở cáhouse lớp con cụ thể.
6. Khi một thành viên của băng hạ gụhouse một đặhouse vụ CP9, nếu mô tả của nhân vật không
nói gì về morale thì mặhouse định giá trị morale trong BattleContext sẽ tăng thêm 5.*/
StrawHat::StrawHat() : Character() {
    this->bounty = 0;
}

StrawHat::StrawHat(string name, int hp, int atk, int def,
                   int speed, int energy, long long bounty) {
    this->name=name;
    this->hp=(hp<0) ? 0 : hp;
    this->maxHp=hp;
    this->atk=atk;
    this->def=def;
    this->speed=speed;
    this->energy=(energy<0) ? 0 : energy;
    this->alive=(hp>0) ? true : false;
    this->bounty=bounty;

}

bool StrawHat::isStrawHat() const {
    return true;
    // TODO: implement
}

string StrawHat::str() const {
    // TODO: implement
    return "StrawHat[name=" + name + ", hp=" + to_string(hp) + ", atk=" + to_string(atk) + ", def=" + to_string(def) + ", speed=" + to_string(speed) + ", energy=" + to_string(energy) + ", bounty=" + to_string(bounty) + "]";
}

/*
 * Luffy
 */
Luffy::Luffy(string name, int hp, int atk, int def,int speed, int energy, long long bounty) : StrawHat(name, hp, atk, def, speed, energy, bounty) {
    // TODO: implement
}

int Luffy::attack(Character* target, BattleContext& context) {
    // TODO: implement
    this->attacked=true;
    double percent=this->hp*1.0/maxHp;
    double dam=0;
    if(percent>=0.5) dam=atk;
    elif(percent>=0.3) dam=ceil(this->atk*1.15);
    else dam=ceil(atk*1.3);
    target->receiveDamage((int)ceil(dam));
    int actualDam= (int)ceil(dam) - target->getDef();
    if(!target->isAlive()) {
        this->defeated=true;
        context.morale+=5;
    }
    return max(0,(int)actualDam);
}

int Luffy::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    this->attacked=true;
    if(this->energy<20 || this->hp*1.0<this->maxHp*0.15) return 0;
    this->energy-=20;
    int dam=ceil(this->atk*2);
    this->speed+=15;
    this->atk+=15;
    int actualDam= (int)ceil(dam) - target->getDef();
    this->hp-= (int)ceil(0.08*maxHp);
    target->receiveDamage((int)ceil(dam));
        if(!target->isAlive()) {
        defeated=true;
    }
    context.alarmLevel+=10;
    return max(0,actualDam);
}

int Luffy::attack(Building* target, BattleContext& context) {
    double percent=this->hp*1.0/this->maxHp;
    double dam=0;
    if(percent>=0.5) dam=atk;
    elif(percent>=0.3) dam=ceil(atk*1.15);
    else dam=ceil(atk*1.3);
    target->receiveDamage((int)ceil(dam));
    return max(0,(int)ceil(dam));
}

int Luffy::specialSkill(Building* target, BattleContext& context) {
    this->attacked=true;
    if(this->energy<20 || this->hp*1.0<this->maxHp*0.15) return 0;
    this->energy-=20;
    int dam=ceil(this->atk*2);
    this->speed+=15;
    this->atk+=15;
    this->hp=this->hp - (int)ceil(0.08*maxHp);
    context.alarmLevel+=10;
    target->receiveDamage((int)ceil(dam));
    return max(0,(int)(ceil(dam)));
}

void Luffy::endTurn(BattleContext& context) {
    // TODO: implement
    if((this->hp*1.0/this->maxHp)<=0.3) context.morale+=3;
    if(this->defeated) this->energy+=5;
    this->defeated=false;
}

/*
 * Zoro
 */
Zoro::Zoro(string name, int hp, int atk, int def, int speed, int energy, long long bounty) : StrawHat(name, hp, atk, def, speed, energy, bounty) {
    // TODO: implement
}

int Zoro::attack(Character* target, BattleContext& context) {
    double dam=this->atk+ceil(0.2*this->def);
    if(target->getHP()<0.4*this->maxHp) dam=ceil(dam*1.15);
    target->receiveDamage((int)ceil(dam));
    int actualDam= (int)ceil(dam) - target->getDef();
    if(!target->isAlive()) {
        this->defeated=true;
    }
    // TODO: implement
    return max(0,actualDam);
}

int Zoro::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    if(this->energy<15) return 0;
    this->energy-=15;
    double dam=ceil(2.2*atk);
    if(target->getHP()<0.5*maxHp) dam=ceil(dam*1.5);
    target->receiveDamage((int)ceil(dam));
    int actualDam= (int)ceil(dam) - target->getDef();
    if(!target->isAlive()) {
        this->defeated=true;
        context.morale+=4;
        this->energy+=8;
    }
    return max(0,actualDam);
}

int Zoro::attack(Building* target, BattleContext& context) {
    // TODO: implement
    double dam=this->atk+ceil(0.2*this->def);
    if(target->getHp()<0.4*target->getMaxHP()) dam=ceil(dam*1.15);
    target->receiveDamage((int)ceil(dam));
    return max(0,(int)ceil(dam));
}

int Zoro::specialSkill(Building* target, BattleContext& context) {
    // TODO: implement
        if(this->energy<15) return 0;
    this->energy-=15;
    double dam = ceil(2.2*this->atk);
    if(target->getHp()<0.5*target->getMaxHP()) dam=ceil(dam*1.5);
    target->receiveDamage((int)ceil(dam));
    return max(0,(int)ceil(dam));
}

void Zoro::endTurn(BattleContext& context) {
    // TODO: implement
    if(this->defeated){
        context.morale+=6;
        this->atk*=1.05;
    }
    this->defeated=false;
}

/*
 * Sanji
 */
Sanji::Sanji(string name, int hp, int atk, int def,
             int speed, int energy, long long bounty) : StrawHat(name, hp, atk, def, speed, energy, bounty) {
    // TODO: implement
}

int Sanji::attack(Character* target, BattleContext& context) {
    // TODO: implement
    double dam=this->atk+ceil(0.5*this->speed);
    if(this->def > target->getDef()) dam=ceil(dam*1.1);
    target->receiveDamage((int)ceil(dam));
    int actualDam= (int)ceil(dam) - target->getDef();
    if(!target->isAlive()) {
        this->defeated=true;
    }
    return max(0,actualDam);
}

int Sanji::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    if(this->energy<18) return 0;
    this->energy-=18;
    double dam=ceil(2.1*atk);
    target->receiveDamage((int)ceil(dam));
    int actualDam= (int)ceil(dam) - target->getDef();
    if(target->getName()=="Jabra") target->setDef(target->getDef()-12);
    else target->setDef(target->getDef()-8);
    if(!target->isAlive()) {
        defeated=true;
    }
    return max(0,actualDam);
}

int Sanji::attack(Building* target, BattleContext& context) {
    // TODO: implement
    double dam=atk+ceil(0.5*speed);
    target->receiveDamage((int)ceil(dam));
    return max(0,(int)ceil(dam));
}

int Sanji::specialSkill(Building* target, BattleContext& context) {
    if(energy<18) return 0;
    energy-=18;
    double dam=ceil(2.1*atk);
    target->receiveDamage((int)ceil(dam));
    return max(0,(int)ceil(dam));
}

void Sanji::endTurn(BattleContext& context) {
    // TODO: implement
    if(this->defeated) {
        context.morale+=8;
        this->atk=ceil(this->atk*1.1);
        this->defeated=false;
    }

}

/*
 * Nami
 */
Nami::Nami(string name, int hp, int atk, int def,
           int speed, int energy, long long bounty) : StrawHat(name, hp, atk, def, speed, energy, bounty) {
    // TODO: implement
}

int Nami::attack(Character* target, BattleContext& context) {
    // TODO: implement
    double dam=this->atk+floor(0.3*target->getDef());
    target->receiveDamage((int)ceil(dam));
    int actualDam= (int)ceil(dam) - target->getDef();
    if(!target->isAlive()) {
        this->defeated=true;
    }
    return max(0,actualDam);
}

int Nami::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    if(this->energy<20) return 0;
    this->energy-=20;
    double dam=this->atk+40;
    target->receiveDamage((int)ceil(dam));
    target->setSpeed(target->getSpeed()-10);
    int actualDam= (int)ceil(dam) - target->getDef();
    if(!target->isAlive()) {
        this->defeated=true;
    }
    context.busterCallTimer++;
    context.alarmLevel-=5;
    return max(0,actualDam);
}

int Nami::attack(Building* target, BattleContext& context) {
    // TODO: implement
    double dam=ceil(atk*0.5);
    target->receiveDamage((int)ceil(dam));
    return max(0,(int)ceil(dam));
}

int Nami::specialSkill(Building* target, BattleContext& context) {
    // TODO: implement
    if(this->energy<20) return 0;
    this->energy-=20;
    double dam=(this->atk+40)+ceil((this->atk+40)*0.5);
    target->receiveDamage((int)ceil(dam));
    context.busterCallTimer++;
    context.alarmLevel-=5;
    return max(0,(int)ceil(dam));
}

void Nami::endTurn(BattleContext& context) {
    // TODO: implement
    if(this->defeated) {
        this->energy+=6;
            context.morale+=5;
    }
            this->defeated=false;
}

/*
 * Chopper
 */
Chopper::Chopper(string name, int hp, int atk, int def,
                 int speed, int energy, long long bounty) : StrawHat(name, hp, atk, def, speed, energy, bounty) {
    // TODO: implement
}

int Chopper::attack(Character* target, BattleContext& context) {
    // TODO: implement
        double dam=atk;
        target->receiveDamage((int)ceil(dam));
        int actualDam= (int)ceil(dam) - target->getDef();
        if(!target->isAlive()) {
            defeated=true;
        }
    return max(0,actualDam);
}

int Chopper::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    if(energy<15) return 0;
    energy-=15;
    double heal=35+ceil(0.5*atk);
    double healHp=(target->getHP() + heal > target->getMaxHP()) ? target->getMaxHP() - target->getHP() : heal;
    target->setHP(target->getHP() + (int)ceil(healHp));
    if(target->getName()=="Luffy") {
        context.morale+=5;
    }
    return 0;
}

int Chopper::attack(Building* target, BattleContext& context) {
    // TODO: implement
            double dam=atk ;
            target->receiveDamage((int)ceil(dam));
    return max(0,(int)ceil(dam));
}

void Chopper::endTurn(BattleContext& context) {
    // TODO: implement
    if(defeated) {
        context.morale+=5;
    }
    defeated=false;
}

/*
 * Usopp
 */
Usopp::Usopp(string name, int hp, int atk, int def,
             int speed, int energy, long long bounty) : StrawHat(name, hp, atk, def, speed, energy, bounty) {
    // TODO: implement
}

int Usopp::attack(Character* target, BattleContext& context) {
    // TODO: implement
        double dam=atk;
        if(target->getSpeed()<50) dam=ceil(dam*1.2);
        target->receiveDamage((int)ceil(dam));
        if(!target->isAlive()) {
            defeated=true;
        }
    int actualDam= (int)ceil(dam) - target->getDef();
    return max(0,actualDam);
}

int Usopp::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    if(energy<16) return 0;
    energy-=16;
    double dam=ceil(atk*0.8);
    target->setSpeed((int)(target->getSpeed()-12));
    target->receiveDamage((int)ceil(dam));
    if(!target->isAlive()) {
        defeated=true;
    }
    context.escapeProgress+=8;
    int actualDam= (int)ceil(dam) - target->getDef();
    return max(0,actualDam);
}

int Usopp::attack(Building* target, BattleContext& context) {
    // TODO: implement
            double dam=atk*0.5;
        target->receiveDamage((int)ceil(dam));
    return max(0,(int)ceil(dam));
}

int Usopp::specialSkill(Building* target, BattleContext& context) {
    if(energy<16) return 0;
    energy-=16;
    double dam=ceil(atk*0.8);
    target->receiveDamage((int)ceil(dam));
    context.escapeProgress+=8;
    return max(0,(int)ceil(dam));
}

void Usopp::endTurn(BattleContext& context) {
    // TODO: implement
    context.morale+=10;
    defeated=false;
}

/*
 * Franky
 */
Franky::Franky(string name, int hp, int atk, int def,
               int speed, int energy, long long bounty) : StrawHat(name, hp, atk, def, speed, energy, bounty) {
    // TODO: implement
}

int Franky::attack(Character* target, BattleContext& context) {
    // TODO: implement
        double dam=this->atk+ceil(0.3*this->def);
        if(target->isCP9()) dam=dam+ceil(dam*0.1);
            target->receiveDamage((int)ceil(dam));
            if(!target->isAlive()) {
                this->defeated=true;
            }
    int actualDam= (int)ceil(dam) - target->getDef();
    return max(0,actualDam);
}

int Franky::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    if(energy<20) return 0;
    if(energy<30){
    energy-=20;
    double dam=atk+ceil(0.8*atk);
    if(target->getName()=="Lucci") dam=dam+ceil(dam*0.2);
    target->receiveDamage((int)ceil(dam));
    if(!target->isAlive()) {
        defeated=true;
    }
    target->setSpeed((int)(target->getSpeed()-8));
    return max(0,(int)(ceil(dam))-target->getDef());
    }
    energy-=30;
    double dam=atk+ceil(0.2*atk);
    target->receiveDamage((int)ceil(dam));
    if(!target->isAlive()) {
        defeated=true;
    }
    return max(0,(int)(ceil(dam))-target->getDef());
}

int Franky::attack(Building* target, BattleContext& context) {
            double dam=atk+ceil(0.3*def);
            target->receiveDamage((int)ceil(dam));
            return max(0,(int)(ceil(dam)));
}

int Franky::specialSkill(Building* target, BattleContext& context) {
    if(energy<20) return 0;
    if(energy<30){
    energy-=20;
    double dam=atk+ceil(0.8*atk);
    target->receiveDamage((int)ceil(dam));
    return max(0,(int)(ceil(dam)));
    }
    energy-=30;
    target->receiveDamage(target->getHp());
    return max(0,target->getHp());
}

void Franky::endTurn(BattleContext& context) {
    // TODO: implement
    if(defeated) {
        context.morale+=5;
    }
    if(hp>0.7*maxHp) def+=5;
    elif(hp<0.3*maxHp) atk=atk+ceil(atk*0.1);
    defeated=false;
}

/*
 * CP9Agent
 */
CP9Agent::CP9Agent() : Character() {
    doriki = 0;
}

CP9Agent::CP9Agent(string name, int hp, int atk, int def,
                   int speed, int energy, int doriki ) : Character(name, hp, atk, def, speed, energy), doriki(doriki) {
    // TODO: implement
}

bool CP9Agent::isCP9() const {
    // TODO: implement
    return true;
}

string CP9Agent::str() const {
    // TODO: implement
    return "CP9[name=" + name + ", hp=" + to_string(hp) + ", atk=" + to_string(atk) + ", def=" + to_string(def) + ", speed=" + to_string(speed) + ", energy=" + to_string(energy) + ", doriki=" + to_string(doriki) + "]";
}

/*
 * Lucci
 */
Lucci::Lucci(string name, int hp, int atk, int def,
             int speed, int energy, int doriki) : CP9Agent(name, hp, atk, def, speed, energy, doriki) {
    // TODO: implement
}

int Lucci::attack(Character* target, BattleContext& context) {
    // TODO: implement
    double dam=atk+ceil(doriki/20.0);
    if(target->getHP() < 0.5 * target->getMaxHP()) dam=dam+ceil(dam*0.2);
    target->receiveDamage((int)ceil(dam));
    if(!target->isAlive()) {
        defeated=true;
    }
    return max(0, (int)(ceil(dam)) - target->getDef());
}

int Lucci::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    if(this->energy<25) return 0;
        this->energy-=25;
        double dam=atk*2+ceil(0.8*atk)+floor(target->getDef()*0.5);
        target->receiveDamage((int)ceil(dam));
        if(!target->isAlive()) {
            defeated=true;
            context.morale-=10;
        }
    return max(0, (int)(ceil(dam)) - target->getDef());
}

void Lucci::endTurn(BattleContext& context) {
    // TODO: implement
    if(this->hp<0.4*this->maxHp) {
        this->atk=atk+ceil(this->atk*0.05);
    }
    defeated=false;
}

/*
 * Kaku
 */
Kaku::Kaku(string name, int hp, int atk, int def,
           int speed, int energy, int doriki) : CP9Agent(name, hp, atk, def, speed, energy, doriki) {
    // TODO: implement
}

int Kaku::attack(Character* target, BattleContext& context) {
    // TODO: implement
    double dam=atk;
    target->receiveDamage((int)ceil(dam));
    if(!target->isAlive()) {
        defeated=true;
    }
    return max(0, (int)(ceil(dam)) - target->getDef());
}

int Kaku::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    if(energy<20) return 0;
    energy-=20;
    double totalDam=0;
    double damPer[]={1.2,1.0,0.8};
    int count=0;
    for(int i=0;i<3;i++) {
        count++;
        double dam=ceil(damPer[i]*atk);
        totalDam+=dam;
        target->receiveDamage((int)ceil(dam));
        if(!target->isAlive()) {
            defeated=true;
            break;
        }
    }
    return max(0, (int)(ceil(totalDam)) - count*target->getDef());
}

void Kaku::endTurn(BattleContext& context) {
    // TODO: implement
    if(defeated) {
        context.morale-=5;
    }
    defeated=false;
}

/*
 * Jabra
 */
Jabra::Jabra(string name, int hp, int atk, int def,
             int speed, int energy, int doriki) : CP9Agent(name, hp, atk, def, speed, energy, doriki) {
    // TODO: implement
}

int Jabra::attack(Character* target, BattleContext& context) {
    // TODO: implement
    target->receiveDamage((int)ceil(atk));
    if(!target->isAlive()) {
        this->defeated=true;
    }
    return max(0, (int)(ceil(atk)) - target->getDef());
}

int Jabra::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    if(energy<18) return 0;
    energy-=18;
    double dam=atk+ceil(atk*0.5);
    if(hp<0.35*maxHp) dam=dam+ceil(dam*0.25);
    target->receiveDamage((int)ceil(dam));
    if(!target->isAlive()) {
        defeated=true;
        context.morale-=5;
    }
    return max(0, (int)(ceil(dam)) - target->getDef());
}

void Jabra::endTurn(BattleContext& context) {
    // TODO: implement
    this->defeated=false;
}

/*
 * Blueno
 */
Blueno::Blueno(string name, int hp, int atk, int def,
               int speed, int energy, int doriki) : CP9Agent(name, hp, atk, def, speed, energy, doriki) {
    // TODO: implement 
}

int Blueno::attack(Character* target, BattleContext& context) {
    // TODO: implement
    target->receiveDamage((int)ceil(this->atk));
    if(!target->isAlive()) {
        this->defeated=true;
    }
    return max(0, (int)(ceil(atk)) - target->getDef());
}

int Blueno::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    if(energy<15) return 0;
    energy-=15;
    double dam=atk+ceil(atk*0.3);
    if(hp>0.5*maxHp) dam=dam+20;
    else dam=dam+40;
    target->receiveDamage((int)ceil(dam));
    if(!target->isAlive()) {
        defeated=true;
    }
    return max(0, (int)(ceil(dam)) - target->getDef());
}

void Blueno::endTurn(BattleContext& context) {
    // TODO: implement
    if(defeated) {
        context.morale-=5;
    }
    defeated=false;
}

/*
 * Kalifa
 */
Kalifa::Kalifa(string name, int hp, int atk, int def,
               int speed, int energy, int doriki) : CP9Agent(name, hp, atk, def, speed, energy, doriki) {
    // TODO: implement
}

int Kalifa::attack(Character* target, BattleContext& context) {
    // TODO: implement
    target->receiveDamage((int)ceil(atk));
    if(!target->isAlive()) {
        defeated=true;
        context.morale-=5;
    }
    return max(0, (int)(ceil(atk)) - target->getDef());
}

int Kalifa::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    if(energy<18) return 0;
    energy-=18;
    double dam=atk+ceil(atk*0.4);
    target->receiveDamage((int)ceil(dam));
    if(!target->isAlive()) {
        defeated=true;
    }
    if(target->getName()=="Nami"){
        context.morale-=12;
        target->setSpeed(target->getSpeed()-6);
    }
    else{
        context.morale-=8;
        target->setSpeed(target->getSpeed()-6);
    }
    return max(0, (int)(ceil(dam)) - target->getDef());
}

void Kalifa::endTurn(BattleContext& context) {
    // TODO: implement
    defeated=false;
}

/*
 * Kumadori
 */
Kumadori::Kumadori(string name, int hp, int atk, int def,
                   int speed, int energy, int doriki) : CP9Agent(name, hp, atk, def, speed, energy, doriki) {
    // TODO: implement
}

int Kumadori::attack(Character* target, BattleContext& context) {
    target->receiveDamage((int)ceil(atk));
    if(!target->isAlive()) {
        defeated=true;
    }
    return max(0, (int)(ceil(atk)) - target->getDef());
}

int Kumadori::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    if(energy<16) return 0;
    energy-=16;
    double dam=30+ceil(0.1*doriki);
    if(hp<0.4*maxHp) dam=dam+25;
    target->receiveDamage((int)ceil(dam));
    if(!target->isAlive()) {
        defeated=true;
    }
    return max(0, (int)(ceil(dam)) - target->getDef());
}

void Kumadori::endTurn(BattleContext& context) {
    // TODO: implement
    if(defeated) {
        context.morale-=5;
    }
    defeated=false;
}

/*
 * Fukurou
 */
Fukurou::Fukurou(string name, int hp, int atk, int def,
                 int speed, int energy, int doriki) : CP9Agent(name, hp, atk, def, speed, energy, doriki) {
    // TODO: implement
}

int Fukurou::attack(Character* target, BattleContext& context) {
    // TODO: implement
    target->receiveDamage((int)ceil(atk));
    if(!target->isAlive()) {
        defeated=true;
    }
    return max(0, (int)(ceil(atk)) - target->getDef());
}

int Fukurou::specialSkill(Character* target, BattleContext& context) {
    // TODO: implement
    if(energy<14) return 0;
    energy-=14;
    double dam=atk+ceil(atk*0.3);
    if(target->getHP()==context.low) dam=dam+20;
    target->receiveDamage((int)ceil(dam));
    if(!target->isAlive()) {
        defeated=true;
        context.morale-=6;
    }
    return max(0, (int)(ceil(dam)) - target->getDef());
}

void Fukurou::endTurn(BattleContext& context) {
    // TODO: implement
    defeated=false;
}

/*
 * Building
 */
Building::Building(string name, int hp) {
    // TODO: implement
    this->name=name;
    this->hp=(hp<0) ? 0 : hp;
    this->maxHP=hp;
    this->destroyed=(hp<=0)&&true;
}

Building::~Building() {
    // TODO: implement if needed
}

void Building::receiveDamage(int damage) {
    // TODO: implement
    hp=(hp - damage > 0) ? hp - damage : 0;
    if(hp==0) destroyed=true;
}

bool Building::isDestroyed() const {
    // TODO: implement
    return destroyed;
}

void Building::onDestroyed(BattleContext& context) {
    return ;
}

string Building::str () const {
    // TODO: implement
    return "Building[name=" + name + ", hp=" + to_string(hp) + ", maxHP=" + to_string(maxHP) + ", destroyed=" + to_string(destroyed) + "]";
}

/*
 * MainGate
 */
MainGate::MainGate(string name, int hp) : Building(name, hp) {}

void MainGate::applyEffect(BattleContext& context) {
    // TODO: implement
}

void MainGate::onDestroyed(BattleContext& context) {
    // TODO: implement
    context.mainGateDestroyed=true;
    context.morale+=5;
    context.rescueProgress+=20;
}

/*
 * Courthouse
 */
Courthouse::Courthouse(string name, int hp) : Building(name, hp) {}

void Courthouse::applyEffect(BattleContext& context) {
    // TODO: implement
    if(destroyed) return;
    context.alarmLevel+=5;
}

void Courthouse::onDestroyed(BattleContext& context) {
    // TODO: implement
    context.alarmLevel-=20;
    if(context.alarmLevel<0) context.alarmLevel=0;
}

/*
 * TowerOfJustice
 */
TowerOfJustice::TowerOfJustice(string name, int hp) : Building(name, hp) {}

void TowerOfJustice::applyEffect(BattleContext& context) {
    // TODO: implement
    if(context.mainGateDestroyed && !context.robinRescued){
        context.rescueProgress += 5;
        if (context.rescueProgress >= 100) {
            context.rescueProgress = 100;
            context.robinRescued = true;
            context.morale += 10;
        }
    }
}

/*
 * BridgeOfHesitation
 */
BridgeOfHesitation::BridgeOfHesitation(string name, int hp) : Building(name, hp) {}

void BridgeOfHesitation::applyEffect(BattleContext& context) {
    // TODO: implement
    if(destroyed) return;
    if (context.robinRescued) {
        context.bridgeOpened = true;
        context.escapeProgress += 5;
        if (context.escapeProgress>=100) {
            context.escapeProgress=100;
            context.battleEnded=true;
            context.resultCode="STRAW_HAT_WIN";
        }
    }
}

/*
 * BusterCallShip
 */
BusterCallShip::BusterCallShip(string name, int hp) : Building(name, hp) {}

void BusterCallShip::applyEffect(BattleContext& context) {
    // TODO: implement
    if (!destroyed) {
        context.busterCallTimer-=1;
        if (context.busterCallTimer<=0){
            context.busterCallTimer=0;
            context.battleEnded=true;
            context.resultCode="BUSTER_CALL";
        }
    }
}

void BusterCallShip::onDestroyed(BattleContext& context) {
    // TODO: implement
    context.busterCallTimer+=3;
}

/*
 * EniesLobbyBattle
 */
/*Lớp EniesLobbyBattle là lớp quản lý toàn bộ trận đánh tại Enies Lobby. Lớp này chịu trách
nhiệm đọhouse dữ liệu khởi tạo, quản lý danh sách nhân vật, quản lý cáhouse công trình, điều phối lượt
đánh, cập nhật trạng thái trong BattleContext và xáhouse định kết quả cuối cùng của trận chiến.
Yêu cầu: Hiện thựhouse class EniesLobbyBattle với cáhouse mô tả sau:
1. Cáhouse thuộhouse tính private:
• strawHats: kiểu Character**, mảng động lưu cáhouse nhân vật thuộhouse băng Mũ Rơm.
Số lượng nhân vật của băng Mũ Rơm không quá 7.
• strawHatCount: kiểu int, số lượng nhân vật băng Mũ Rơm hiện có.
• cp9Agents: kiểu Character**, mảng động lưu cáhouse đặhouse vụ CP9. Số lượng đặhouse vụ
không quá 7.
• cp9Count: kiểu int, số lượng đặhouse vụ CP9 hiện có.
• buildings: kiểu Building**, mảng động lưu cáhouse công trình trên chiến trường. Số
lượng công trình không quá 5.
• buildingCount: kiểu int, số lượng công trình hiện có.
• turnOrder: danh sách liên kết đơn quản lý thứ tự hành động của cáhouse nhân vật.
• context: kiểu BattleContext, trạng thái hiện tại của trận đánh.
• maxTurns: kiểu int, số lượt tối đa của trận đánh.*/
EniesLobbyBattle::EniesLobbyBattle(const string& filename) {
    // TODO: implement
    strawHats= new Character*[7];
    cp9Agents= new Character*[7];
    buildings= new Building*[5];
    strawHatCount=0;
    cp9Count=0;
    buildingCount=0;
    loadFromFile(filename);
    tailNode=nullptr;
}

EniesLobbyBattle::~EniesLobbyBattle() {
    // TODO: implement
    if (turnOrder) {
        TurnNode* curr=turnOrder;
        TurnNode* nextNode;
        do {
            nextNode=curr->next;
            delete curr;
            curr=nextNode;
        } while (curr!=turnOrder);
    }
    for(int i=0; i < strawHatCount;i++) delete strawHats[i];
    for(int i=0;i < cp9Count;i++) delete cp9Agents[i];
    for(int i=0;i < buildingCount;i++) delete buildings[i];
    delete[] strawHats;
    delete[] cp9Agents;
    delete[] buildings;

}

void EniesLobbyBattle::loadFromFile(const string& filename) {
    // TODO: implement
    ifstream f(filename);
    if (!f.is_open()) return;
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        stringstream s(line);
        string str;
        s>>str;
        if(str=="CONTEXT"){
            s>>context.morale>>context.alarmLevel>>context.rescueProgress>>context.escapeProgress>>context.busterCallTimer>>maxTurns;
        }
        elif(str=="STRAW_HAT"){
            string name;
            int hp,atk,def,speed,energy;
            long long bounty;
            s>>name>>hp>>atk>>def>>speed>>energy>>bounty;
            if(name=="Luffy") allocSH(new Luffy(name, hp, atk, def, speed, energy, bounty));
            elif(name=="Zoro") allocSH(new Zoro(name, hp, atk, def, speed, energy, bounty));
            elif(name=="Nami") allocSH(new Nami(name, hp, atk, def, speed, energy, bounty));
            elif(name=="Usopp") allocSH(new Usopp(name, hp, atk, def, speed, energy, bounty));
            elif(name=="Sanji") allocSH(new Sanji(name, hp, atk, def, speed, energy, bounty));
            elif(name=="Chopper") allocSH(new Chopper(name, hp, atk, def, speed, energy, bounty));
            elif(name=="Franky") allocSH(new Franky(name, hp, atk, def, speed, energy, bounty));
        }
        elif(str=="CP9"){
            string name;
            int hp, atk, def, speed, energy, doriki;
            s>>name>>hp>>atk>>def>>speed>>energy>>doriki;
            if(name=="Lucci") addCP9Agent(new Lucci(name, hp, atk, def, speed, energy, doriki));
            elif(name=="Kaku") addCP9Agent(new Kaku(name, hp, atk, def, speed, energy, doriki));
            elif(name=="Jabra") addCP9Agent(new Jabra(name, hp, atk, def, speed, energy, doriki));
            elif(name=="Blueno") addCP9Agent(new Blueno(name, hp, atk, def, speed, energy, doriki));
            elif(name=="Kalifa") addCP9Agent(new Kalifa(name, hp, atk, def, speed, energy, doriki));
            elif(name=="Kumadori") addCP9Agent(new Kumadori(name, hp, atk, def, speed, energy, doriki));
            elif(name=="Fukurou") addCP9Agent(new Fukurou(name, hp, atk, def, speed, energy, doriki));
        }
        elif(str=="BUILDING"){
            string name;
            int hp;
            s>>name>>hp;
            if(name=="MainGate") addBuilding(new MainGate(name, hp));
            elif(name=="Courthouse") addBuilding(new Courthouse(name, hp));
            elif(name=="TowerOfJustice") addBuilding(new TowerOfJustice(name, hp));
            elif(name=="BridgeOfHesitation") addBuilding(new BridgeOfHesitation(name, hp));
            elif(name=="BusterCallShip") addBuilding(new BusterCallShip(name, hp));
        }
    }
    f.close();
    buildTurnOrder();
}

void EniesLobbyBattle::allocSH(Character* character) {
    // TODO: implement
    if(strawHatCount<7) strawHats[strawHatCount++]=character;
}

void EniesLobbyBattle::addCP9Agent(Character* character) {
    // TODO: implement
    if(cp9Count<7) cp9Agents[cp9Count++]=character;
}

void EniesLobbyBattle::addBuilding(Building* building) {
    // TODO: implement
    if(buildingCount<5) buildings[buildingCount++]=building;
}

void EniesLobbyBattle::buildTurnOrder() {
    // TODO: implement
    TurnNode* lastNode=nullptr;
    turnOrder= new TurnNode(strawHats[0]);
    lastNode=turnOrder;
    for(int i=1;i<strawHatCount;i++){
        lastNode->next=new TurnNode(strawHats[i]);
        lastNode=lastNode->next;
    }
    for(int i=0;i<cp9Count;i++){
        lastNode->next=new TurnNode(cp9Agents[i]);
        lastNode=lastNode->next;
    }
    if(lastNode) lastNode->next=turnOrder;
}

void EniesLobbyBattle::runBattle() {
    // TODO: implement
    if(!turnOrder) return;
    while (!context.battleEnded && context.turnCount<maxTurns){
        if(turnOrder->data->isAlive()) {       
        processTurn(turnOrder->data);
        }
        turnOrder=turnOrder->next;
        processBuildings();
        context.turnCount++;
        checkEndCondition();
    }
}

void EniesLobbyBattle::processTurn(Character* character) {
    // TODO: implement
    if(!character->isAlive()) return;
    Character* tarChar = nullptr;
    Building* targetBuilding = nullptr;
    if (character->isStrawHat()) {
        bool chopperSpecial = false;
        if (character->getName() == "Chopper" && character->getEnergy() >= 15) {
            int minHp = INT_MAX;
            for (int i = 0; i < strawHatCount; ++i) {
                if (strawHats[i]->isAlive() && strawHats[i]->getHP() < minHp) {
                    minHp = strawHats[i]->getHP();
                    tarChar = strawHats[i];
                }
            }
            if (tarChar) chopperSpecial = true;
        }
        if(!chopperSpecial){
            Building *gate=nullptr,*house=nullptr,*ship=nullptr,*bridge=nullptr;
            for(int i=0;i<buildingCount;i++){
                if(buildings[i]->getName()=="MainGate") gate=buildings[i];
                elif(buildings[i]->getName()=="Courthouse") house=buildings[i];
                elif(buildings[i]->getName()=="BusterCallShip") ship=buildings[i];
                elif(buildings[i]->getName()=="BridgeOfHesitation") bridge=buildings[i];
            }
            if(gate && !gate->isDestroyed() && !context.mainGateDestroyed) targetBuilding=gate;
            elif(house && !house->isDestroyed() && context.alarmLevel>=50) targetBuilding=house;
            elif(ship && !ship->isDestroyed() && context.busterCallTimer<=5) targetBuilding=ship;
            elif(!context.robinRescued){
                for(int i=0;i<cp9Count;i++){
                    if(cp9Agents[i]->isAlive()) {
                        tarChar=cp9Agents[i];
                        break;
                    }
                }
            }
            else{
                if(bridge && !bridge->isDestroyed() && context.robinRescued) targetBuilding=bridge;
                else{
                    for(int i=0;i<cp9Count;i++){
                        if(cp9Agents[i]->isAlive()) {
                            tarChar=cp9Agents[i];
                            break;
                        }
                    }
                }
        }
    }

    }
    else {
        for(int i=0;i<strawHatCount;i++){
            if(strawHats[i]->isAlive()) {
                tarChar=strawHats[i];
                break;
            }
        }
        int minHp = INT_MAX;
        for (int i = 0; i < strawHatCount; ++i) {
            if (strawHats[i]->isAlive() && strawHats[i]->getHP() < minHp) {
                minHp = strawHats[i]->getHP();
            }
        }
        context.low = minHp;
    }
    if (!tarChar && !targetBuilding) {
        character->endTurn(context);
        clamp();
        return;
    }
    bool special=false;
    string n=character->getName();
    int energy=character->getEnergy();
    int hp=character->getHP();
    int maxHp=character->getMaxHP();

    if (n=="Luffy" && energy>=20 && hp>=ceil(maxHp*0.15)) special=true;
    elif (n=="Zoro" && energy>=15) special=true;
    elif (n=="Sanji" && energy>=18) special=true;
    elif (n=="Nami" && energy>=20) special=true;
    elif (n=="Chopper" && energy>=15) special=true;
    elif (n=="Usopp" && energy>=16) special=true;
    elif (n=="Franky" && energy>=20) special=true;
    elif (n=="Lucci" && energy>=25) special=true;
    elif (n=="Kaku" && energy>=20) special=true;
    elif (n=="Jabra" && energy>=18) special=true;
    elif (n=="Blueno" && energy>=15) special=true;
    elif (n=="Kalifa" && energy>=18) special=true;
    elif (n=="Kumadori" && energy>=16) special=true;
    elif (n=="Fukurou" && energy>=14) special=true;

    if (tarChar) {
        if (special) character->specialSkill(tarChar, context);
        else character->attack(tarChar, context);
    } elif (targetBuilding) {
        bool wasDest = targetBuilding->isDestroyed();
        if (special) character->specialSkill(targetBuilding, context);
        else character->attack(targetBuilding, context);
        
        if (!wasDest && targetBuilding->isDestroyed()) {
            targetBuilding->onDestroyed(context);
            
        }
    }
    character->endTurn(context);
    clamp();
}
void EniesLobbyBattle::processBuildings() {
    // TODO: implement
    for(int i=0;i < buildingCount;i++){
        buildings[i]->applyEffect(context);
    }
}

void EniesLobbyBattle::checkEndCondition() {
    // TODO: implement
    if (context.robinRescued && context.escapeProgress>=100) {
        context.battleEnded=true;
        context.resultCode="STRAW_HAT_WIN";
        return;
    }
    if (context.busterCallTimer<= 0) {
        context.battleEnded=true;
        context.resultCode="BUSTER_CALL";
        return;
    }
    
    bool strawHatsAlive=false;
    for (int i=0; i<strawHatCount; i++) {
        if (strawHats[i]->isAlive()) {
            strawHatsAlive=true;
            break;
        }
    }
    if (!strawHatsAlive) {
        context.battleEnded=true;
        context.resultCode="CP9_WIN";
        return;
    }
    
    bool cp9Alive = false;
    for (int i=0; i<cp9Count; i++) {
        if (cp9Agents[i]->isAlive()) {
            cp9Alive=true;
            break;
        }
    }
    if (!cp9Alive) {
        context.battleEnded=true;
        context.resultCode="STRAW_HAT_WIN_BY_DEFEAT_CP9";
        return;
    }
    if (context.turnCount>=maxTurns) {
        context.battleEnded=true;
        context.resultCode="TIME_OUT";
        return;
    }
}
string EniesLobbyBattle::getResult() const {
    // TODO: implement
    return context.resultCode + " " + to_string(context.turnCount) + " " + to_string(context.morale) + " " + to_string(context.alarmLevel) + " " +to_string(context.rescueProgress) + " " + to_string(context.escapeProgress) + " " + to_string(context.busterCallTimer);
}

