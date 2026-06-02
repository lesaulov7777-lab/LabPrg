#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <iomanip>
#include <algorithm>

// Константы алгоритма AES-128
const int Nb = 4;  // Количество столбцов в State (для 128 бит = 4)
const int Nk = 4;  // Длина ключа в 32-битных словах (для 128 бит = 4)
const int Nr = 10; // Количество раундов для AES-128

typedef std::vector<uint8_t> ByteArray;
typedef uint8_t StateMatrix[4][4];

// Таблица нелинейной замены S-Box
const uint8_t sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

// Константы Rcon для процедуры расширения ключа
const uint32_t rcon[11] = {
    0x00000000, 0x01000000, 0x02000000, 0x04000000, 0x08000000,
    0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x1B000000, 0x36000000
};

// Вспомогательная функция для форматированного вывода матриц State в консоль
void printState(const std::string& label, const StateMatrix state) {
    std::cout << "  " << label << ":\n";
    for (int i = 0; i < 4; ++i) {
        std::cout << "    ";
        for (int j = 0; j < 4; ++j) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)state[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << std::dec;
}

// Вспомогательная функция вывода одномерных массивов байт
void printHex(const std::string& label, const ByteArray& bytes) {
    std::cout << label << ": ";
    for (auto b : bytes) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)b << " ";
    }
    std::cout << std::dec << "\n";
}

// Умножение в поле Галуа GF(2^8)
uint8_t galoisMultiply(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    for (int counter = 0; counter < 8; counter++) {
        if (b & 1) p ^= a;
        bool hi_bit_set = (a & 0x80);
        a <<= 1;
        if (hi_bit_set) a ^= 0x1B; // Многочлен Икс ^ 8 + Икс ^ 4 + Икс ^ 3 + Икс + 1
        b >>= 1;
    }
    return p;
}

// ЭТАП 1: Преобразование SubBytes
void subBytes(StateMatrix state) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] = sbox[state[i][j]];
        }
    }
}

// ЭТАП 2: Преобразование ShiftRows
void shiftRows(StateMatrix state) {
    uint8_t temp;
    // Строка 1 - сдвиг влево на 1
    temp = state[1][0];
    state[1][0] = state[1][1]; state[1][1] = state[1][2]; state[1][2] = state[1][3]; state[1][3] = temp;
    // Строка 2 - сдвиг влево на 2
    temp = state[2][0]; state[2][0] = state[2][2]; state[2][2] = temp;
    temp = state[2][1]; state[2][1] = state[2][3]; state[2][3] = temp;
    // Строка 3 - сдвиг влево на 3 (вправо на 1)
    temp = state[3][3];
    state[3][3] = state[3][2]; state[3][2] = state[3][1]; state[3][1] = state[3][0]; state[3][0] = temp;
}

// ЭТАП 3: Преобразование MixColumns
void mixColumns(StateMatrix state) {
    for (int j = 0; j < 4; j++) {
        uint8_t s0 = state[0][j], s1 = state[1][j], s2 = state[2][j], s3 = state[3][j];
        state[0][j] = galoisMultiply(s0, 0x02) ^ galoisMultiply(s1, 0x03) ^ s2 ^ s3;
        state[1][j] = s0 ^ galoisMultiply(s1, 0x02) ^ galoisMultiply(s2, 0x03) ^ s3;
        state[2][j] = s0 ^ s1 ^ galoisMultiply(s2, 0x02) ^ galoisMultiply(s3, 0x03);
        state[3][j] = galoisMultiply(s0, 0x03) ^ s1 ^ s2 ^ galoisMultiply(s3, 0x02);
    }
}

// ЭТАП 4: Преобразование AddRoundKey
void addRoundKey(StateMatrix state, const std::vector<uint32_t>& RoundKey, int round) {
    for (int j = 0; j < 4; j++) {
        uint32_t word = RoundKey[round * Nb + j];
        state[0][j] ^= (word >> 24) & 0xFF;
        state[1][j] ^= (word >> 16) & 0xFF;
        state[2][j] ^= (word >> 8) & 0xFF;
        state[3][j] ^= word & 0xFF;
    }
}

// Функция циклического сдвига слова на 1 байт влево (Key Expansion)
uint32_t rotWord(uint32_t word) {
    return (word << 8) | (word >> 24);
}

// Функция замены байт в слове через S-Box (Key Expansion)
uint32_t subWord(uint32_t word) {
    uint32_t result = 0;
    result |= sbox[(word >> 24) & 0xFF] << 24;
    result |= sbox[(word >> 16) & 0xFF] << 16;
    result |= sbox[(word >> 8) & 0xFF] << 8;
    result |= sbox[word & 0xFF];
    return result;
}

// Процедура расширения ключа (Key Expansion)
std::vector<uint32_t> keyExpansion(const ByteArray& key) {
    std::vector<uint32_t> w(Nb * (Nr + 1));
    uint32_t temp;

    for (int i = 0; i < Nk; i++) {
        w[i] = (key[4 * i] << 24) | (key[4 * i + 1] << 16) | (key[4 * i + 2] << 8) | key[4 * i + 3];
    }

    for (int i = Nk; i < w.size(); i++) {
        temp = w[i - 1];
        if (i % Nk == 0) {
            temp = subWord(rotWord(temp)) ^ rcon[i / Nk];
        }
        w[i] = w[i - Nk] ^ temp;
    }
    return w;
}

// Базовое шифрование одного 16-байтового блока алгоритмом AES-128
void aesEncryptBlock(const ByteArray& inputBlock, ByteArray& outputBlock, const std::vector<uint32_t>& roundKeys) {
    StateMatrix state;
    // Заполнение матрицы State (BytesToMatrix)
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] = inputBlock[i + 4 * j];
        }
    }

    std::cout << "\n--- Промежуточные результаты работы AES блока ---\n";
    printState("Исходное состояние State (Round 0)", state);

    addRoundKey(state, roundKeys, 0);

    for (int round = 1; round < Nr; round++) {
        subBytes(state);
        shiftRows(state);
        mixColumns(state);
        addRoundKey(state, roundKeys, round);
        
        std::string label = "После раунда " + std::to_string(round);
        printState(label, state);
    }

    subBytes(state);
    shiftRows(state);
    addRoundKey(state, roundKeys, Nr);
    printState("Финальное состояние State (Round 10)", state);

    // Извлечение данных из State в выходной блок
    outputBlock.resize(16);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            outputBlock[i + 4 * j] = state[i][j];
        }
    }
}

// Дополнение данных по стандарту PKCS7 Padding
void applyPKCS7Padding(ByteArray& data) {
    size_t paddingLength = 16 - (data.size() % 16);
    for (size_t i = 0; i < paddingLength; ++i) {
        data.push_back(static_cast<uint8_t>(paddingLength));
    }
}

// Удаление дополнения PKCS7 Padding
void removePKCS7Padding(ByteArray& data) {
    if (data.empty()) return;
    size_t paddingLength = data.back();
    if (paddingLength > 0 && paddingLength <= 16) {
        data.resize(data.size() - paddingLength);
    }
}

// РЕЖИМ CFB: Функция Шифрования всего текста
ByteArray encryptCFB(const ByteArray& plaintext, const ByteArray& key, const ByteArray& iv) {
    std::vector<uint32_t> roundKeys = keyExpansion(key);
    ByteArray ciphertext;
    ciphertext.resize(plaintext.size());

    ByteArray feedback = iv; // Инициализируем вектор обратной связи
    ByteArray encryptedFeedback(16);

    std::cout << "\n================ НАЧАЛО ШИФРОВАНИЯ В РЕЖИМЕ CFB ================\n";

    for (size_t i = 0; i < plaintext.size(); i += 16) {
        std::cout << "\nОбработка блока №" << (i / 16) + 1 << "\n";
        
        // В режиме CFB шифруется именно блок обратной связи (на первом шаге это IV)
        aesEncryptBlock(feedback, encryptedFeedback, roundKeys);

        // Результат шифрования XOR-ится с открытым текстом для получения шифротекста
        for (size_t j = 0; j < 16 && (i + j) < plaintext.size(); ++j) {
            ciphertext[i + j] = plaintext[i + j] ^ encryptedFeedback[j];
            // Новый шифротекст становится вектором обратной связи для следующего блока
            feedback[j] = ciphertext[i + j];
        }
    }
    return ciphertext;
}

// РЕЖИМ CFB: Функция Расшифрования всего текста
ByteArray decryptCFB(const ByteArray& ciphertext, const ByteArray& key, const ByteArray& iv) {
    std::vector<uint32_t> roundKeys = keyExpansion(key);
    ByteArray decryptedText;
    decryptedText.resize(ciphertext.size());

    ByteArray feedback = iv;
    ByteArray encryptedFeedback(16);

    std::cout << "\n================ НАЧАЛО РАСШИФРОВАНИЯ В РЕЖИМЕ CFB ================\n";

    for (size_t i = 0; i < ciphertext.size(); i += 16) {
        std::cout << "\nОбработка блока №" << (i / 16) + 1 << "\n";
        
        // ВАЖНО: В режиме CFB при расшифровании тоже используется базовая функция шифрования AES!
        aesEncryptBlock(feedback, encryptedFeedback, roundKeys);

        for (size_t j = 0; j < 16 && (i + j) < ciphertext.size(); ++j) {
            decryptedText[i + j] = ciphertext[i + j] ^ encryptedFeedback[j];
            // Предыдущий блок шифротекста становится вектором обратной связи
            feedback[j] = ciphertext[i + j];
        }
    }
    return decryptedText;
}

// Функция генерации случайной последовательности байт
ByteArray generateRandomBytes(size_t size) {
    ByteArray bytes(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    for (size_t i = 0; i < size; ++i) {
        bytes[i] = static_cast<uint8_t>(dis(gen));
    }
    return bytes;
}

int main() {
    // Включение корректного отображения UTF-8 в консоли
    std::setlocale(LC_ALL, "Russian");

    std::cout << "Лабораторная работа №6. Задание 2. Вариант 2.\n";
    std::cout << "Реализация AES-128 в режиме CFB (Cipher Feedback)\n\n";

    // 1. Генерация случайного ключа и вектора инициализации
    ByteArray key = generateRandomBytes(16);  // Ключ 128 бит (16 байт)
    ByteArray iv = generateRandomBytes(16);   // IV 128 бит (16 байт)

    printHex("Сгенерированный случайный Ключ (Key)", key);
    printHex("Сгенерированный случайный Вектор инициализации (IV)", iv);

    // 2. Запись ключа в файл
    std::ofstream keyFile("aes_key.bin", std::ios::binary);
    if (keyFile.is_open()) {
        keyFile.write(reinterpret_cast<const char*>(key.data()), key.size());
        keyFile.close();
        std::cout << "[Успех] Ключ успешно сохранен в файл 'aes_key.bin'\n";
    }

    // 3. Ввод исходного сообщения (поддерживает кириллицу и латиницу)
    std::string inputStr;
    std::cout << "\nВведите исходный текст для шифрования: ";
    std::getline(std::cin, inputStr);

    // Преобразуем строку в массив байт
    ByteArray plaintext(inputStr.begin(), inputStr.end());

    // Применяем дополнение PKCS7 Padding
    applyPKCS7Padding(plaintext);
    std::cout << "\nПосле применения PKCS7 Padding длина данных: " << plaintext.size() << " байт.\n";

    // 4. Процесс шифрования
    ByteArray ciphertext = encryptCFB(plaintext, key, iv);
    std::cout << "\n================ РЕЗУЛЬТАТ ШИФРОВАНИЯ ================\n";
    printHex("Зашифрованный текст (Hex)", ciphertext);

    // 5. Процесс расшифрования
    ByteArray decryptedBytes = decryptCFB(ciphertext, key, iv);
    
    // Удаляем дополнение PKCS7 Padding
    removePKCS7Padding(decryptedBytes);

    std::string decryptedStr(decryptedBytes.begin(), decryptedBytes.end());
    std::cout << "\n================ РЕЗУЛЬТАТ РАСШИФРОВАНИЯ ================\n";
    std::cout << "Расшифрованный текст: " << decryptedStr << "\n";

    return 0;
}