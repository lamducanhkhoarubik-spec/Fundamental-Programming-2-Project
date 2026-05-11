
#include "eniesLobby.h"

using namespace std;

int main(int argc, const char * argv[]) {
    string inputFile = "input.txt";
    
    cout << "=== LOADING BATTLE FROM FILE ===" << endl;
    EniesLobbyBattle battle(inputFile);
    
    cout << "\n=== RUNNING BATTLE ===" << endl;
    battle.runBattle();
    
    cout << "\n=== FINAL RESULT ===" << endl;
    cout << battle.getResult() << endl;
    
    return 0;
}
