
#include "debug.h"

using namespace std;

int main(int argc, const char * argv[]) {

    // Mặc định là input_1.txt nếu không truyền gì vào
    string inputFile = "input_1.txt"; 
    
    // Nếu có truyền tham số từ dòng lệnh (ví dụ: ./main input_2.txt)
    if (argc > 1) {
        inputFile = argv[1];
    }
    
    cout << "=== LOADING BATTLE FROM FILE: " << inputFile << " ===" << endl;
    EniesLobbyBattle battle(inputFile);
    
    cout << "\n=== RUNNING BATTLE ===" << endl;
    battle.runBattle();
    
    cout << "\n=== FINAL RESULT ===" << endl;
    cout << battle.getResult() << endl;
    
    return 0;
}
