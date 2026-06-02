#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
using namespace std;

class RC4 {
    vector<unsigned char> S;

public:
    // Фаза KSA: Инициализация S-блока
    RC4(const vector<unsigned char>& key) {
        S.resize(256);
        iota(S.begin(), S.end(), 0); // Заполнение значениями 0-255

        int j = 0; 
        for (int i = 0; i < 256; ++i) {
            j = (j + S[i] + key[i % key.size()]) % 256;
            swap(S[i], S[j]);
        }
    }

    // Фаза PRGA: Генерация гаммы и шифрование (XOR)
    vector<unsigned char> process(const vector<unsigned char>& data) {
        vector<unsigned char> result;
        result.reserve(data.size());
        
        int i = 0;
        int j = 0;

        // Использование range-based for для обработки входных данных
        for (const auto& byte : data) {
            i = (i + 1) % 256;
            j = (j + S[i]) % 256;
            swap(S[i], S[j]);
            
            unsigned char K = S[(S[i] + S[j]) % 256];
            result.push_back(byte ^ K);
        }
        return result;
    }
};

int main() {
    string key_str = "nstu_key_2026";
    string message = "Sample text for RC4 encryption";

    vector<unsigned char> key(key_str.begin(), key_str.end());
    vector<unsigned char> data(message.begin(), message.end());

    // Шифрование
    RC4 encryptor(key);
    vector<unsigned char> encrypted = encryptor.process(data); 
    for(auto c : encrypted) cout << hex << (int)c << " ";
    cout << endl;

    // Дешифрование (алгоритм симметричен)
    RC4 decryptor(key);
    vector<unsigned char> decrypted = decryptor.process(encrypted);
    for(auto c : decrypted) cout << hex << (int)c << " ";
    cout << endl;

    cout << "Original:  " << message << endl;
    cout << "Decrypted: ";
    for (auto c : decrypted) cout << c;
    cout << endl;

    return 0;
}