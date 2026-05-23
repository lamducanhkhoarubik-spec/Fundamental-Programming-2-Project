#include "eniesLobby.h"
#include <filesystem>
namespace fs = std::filesystem;

int main(){
    std::string path = "./testcase"
    int file_count = 0;
    try {
        if (fs::exists(path) && fs::is_directory(path)) {
            
            for (const auto& entry : fs::directory_iterator(path)) {
                
                if (fs::is_regular_file(entry)) {
                    file_count++;
                }
            }
            std::cout << "Tong so file trong folder la: " << file_count << std::endl;
        } else {
            std::cout << "Duong dan khong ton tai hoac khong phai folder!" << std::endl;
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Loi hệ thong: " << e.what() << std::endl;
    }
    for (int i = 1 ; i <= 2; ++i){
        cout <<"======TESTCASE "<<i<<"==============";
         string inputFile = "./testcase/input_" + string(1, '0' + i) + ".txt";
    
    
    
    EniesLobbyBattle battle(inputFile);
    
   
    battle.runBattle();
    
    cout << "\n=== FINAL RESULT ===" << endl;
    cout << battle.getResult() << endl;
    cout <<"\n";
    }
    cout <<"======END========";
}