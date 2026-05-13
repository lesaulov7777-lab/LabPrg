import Foundation

// Функция для подсчета четных цифр в числе
func countEvenDigits(in number: Int) -> Int {
    var num = abs(number) // Берем модуль на случай отрицательных
    var count = 0
    
    // Если число однозначное (цифра)
    if num < 10 {
        return -1 // Возвращаем -1, чтобы позже вывести "-"
    }
    
    // Подсчитываем четные цифры
    while num > 0 {
        let digit = num % 10
        if digit % 2 == 0 {
            count += 1
        }
        num /= 10
    }
    
    return count
}

// Чтение входных данных
print("Введите N:", terminator: " ")
guard let nInput = readLine(), let n = Int(nInput), n > 0 else {
    print("Ошибка: N должно быть положительным числом")
    exit(1)
}

print("Введите \(n) чисел через пробел:", terminator: " ")
guard let numbersLine = readLine() else {
    print("Ошибка: не удалось прочитать числа")
    exit(1)
}

let numbersStrings = numbersLine.split(separator: " ")
guard numbersStrings.count == n else {
    print("Ошибка: введено не \(n) чисел")
    exit(1)
}

// Обработка каждого числа
var results: [String] = []
for str in numbersStrings {
    guard let number = Int(str) else {
        results.append("?")
        continue
    }
    
    let evenCount = countEvenDigits(in: number)
    if evenCount == -1 {
        results.append("-")
    } else {
        results.append("\(evenCount)")
    }
}

// Вывод результата
print("Результат:", results.joined(separator: " "))