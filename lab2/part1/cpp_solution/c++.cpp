#include <iostream>
#include <fstream>
#include <string>
using namespace std;

bool cycle(string S, string T){
    if (S.length() != T.length()){ //проверка на равность длин строк
        return false;
    }
    int n = S.length(); //проверка на пустую строку
    if (n == 0) return true;

    for (int sd = 0; sd < n; sd++){
        bool match = true;
        for (int i = 0; i < n; i++){
            if (S[(i + sd) % n] != T[i]){
                match = false;
                break;
            }
        }
        if (match) {
            return true;
        }  
    }
    return false;
}

int main(){
    string S1 = "waterbottle";
    string T1 = "erbottlewat";
    
    cout << "Пример 1: " << (cycle(S1, T1) ? "Yes" : "No") << endl;
    
    string S2 = "abc";
    string T2 = "acb";
    cout << "Пример 2: " << (cycle(S2, T2) ? "Yes" : "No") << endl;
    
    string S3 = "ngtu";
    string T3 = "ngtu";
    cout << "Пример 3: " << (cycle(S3, T3) ? "Yes" : "No") << endl;
    
    return 0;
}

