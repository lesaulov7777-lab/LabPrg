#include "TramSystem.h"
#include <iostream>
#include <string>

int main() {
    TramSystem system;
    std::string commandStr;

    // Цикл обработки пользовательского ввода
    while (std::cout << ">>> " && std::cin >> commandStr) {
        CommandType cmd = ParseCommand(commandStr);
        system.ExecuteCommand(cmd, std::cin);
    }

    return 0;
}