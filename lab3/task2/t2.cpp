#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

// Функция для быстрого возведения в степень по модулю: (base^exp) % mod
unsigned long power(unsigned long base, unsigned long exp, unsigned long mod) {
    unsigned long res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            res = (unsigned long)((__int128)res * base % mod);
        }
        base = (unsigned long)((__int128)base * base % mod);
        exp /= 2;
    }
    return res;
}

// Реализация процедуры тестирования Миллера (на основе теоремы Сэлфриджа)
// Возвращает: 1 — простое, 0 — составное, -1 — вероятно составное
int miller_selfridge_test(unsigned long n, const vector<unsigned long>& factors, int t) {
    if (n < 2) return 0;
    if (n == 2) return 1;

    // Шаг 1: Выбрать случайным образом t различных целых чисел a_j (1 < a_j < n)
    unsigned long max_allowed = n - 2;
    int actual_t = t;
    if ((unsigned long)actual_t > max_allowed) {
        actual_t = (int)max_allowed;
    }
    if (actual_t <= 0) return 0;

    vector<unsigned long> bases;
    while (bases.size() < (size_t)actual_t) {
        unsigned long aj = 2 + rand() % (n - 2);
        bool duplicate = false;
        for (unsigned long b : bases) {
            if (b == aj) { duplicate = true; break; }
        }
        if (!duplicate) {
            bases.push_back(aj);
        }
    }

    // Шаг 2: Для каждого a_j проверить малую теорему Ферма
    for (unsigned long aj : bases) {
        if (power(aj, n - 1, n) != 1) {
            return 0; // n – составное число
        }
    }

    // Шаг 3: Проверка для каждого q_i (делителей числа n-1)
    for (unsigned long qi : factors) {
        bool all_one = true;
        
        // Шаг 3.1: Вычисляем a_j^((n-1)/q_i) mod n
        for (unsigned long aj : bases) {
            if (power(aj, (n - 1) / qi, n) != 1) {
                all_one = false;
                break;
            }
        }
        
        // Шаг 3.3: Если для текущего q_i абсолютно все результаты равны 1
        if (all_one) {
            return -1; // n – вероятно, составное число
        }
    }

    // Шаг 4: Если все проверки успешно пройдены
    return 1; // n – простое число
}

// Функция для генерации таблицы простых чисел (Решето Эратосфена)
vector<unsigned long> generate_primes(int sieve_size) {
    vector<bool> is_prime(sieve_size, true);
    is_prime[0] = is_prime[1] = false;
    for (int p = 2; p * p < sieve_size; p++) {
        if (is_prime[p]) {
            for (int i = p * p; i < sieve_size; i += p) {
                is_prime[i] = false;
            }
        }
    }
    
    vector<unsigned long> primes;
    for (int p = 2; p < sieve_size; p++) {
        if (is_prime[p]) {
            primes.push_back(p);
        }
    }
    return primes;
}

// Функция для построения числа m с заданной разрядностью
unsigned long build_m(int target_bits, const vector<unsigned long>& primes, 
                      vector<unsigned long>& m_factors) {
    unsigned long m = 1;
    int attempts = 0;
    
    while (true) {
        int current_len = 0;
        unsigned long temp = m;
        while (temp > 0) { 
            current_len++; 
            temp >>= 1; 
        }
        if (m == 1) current_len = 1;

        if (current_len == target_bits) break;

        unsigned long q = primes[rand() % primes.size()];
        unsigned __int128 next_m = (unsigned __int128)m * q;

        // Вычисляем разрядность потенциального нового m
        int next_len = 0;
        unsigned __int128 temp_m = next_m;
        while (temp_m > 0) { 
            next_len++; 
            temp_m >>= 1; 
        }

        if (next_len <= target_bits) {
            m = (unsigned long)next_m;
            m_factors.push_back(q);
        } else {
            attempts++;
            if (attempts > 500) { // Защита от зависания, сброс сборки m
                m = 1;
                m_factors.clear();
                attempts = 0;
            }
        }
    }
    
    return m;
}

// Функция для получения уникальных простых делителей числа n-1
vector<unsigned long> get_unique_factors(const vector<unsigned long>& m_factors) {
    vector<unsigned long> factors;
    factors.push_back(2); // n-1 = 2 * m, поэтому 2 всегда делитель
    
    for (unsigned long q : m_factors) {
        bool found = false;
        for (unsigned long f : factors) {
            if (f == q) {
                found = true;
                break;
            }
        }
        if (!found) {
            factors.push_back(q);
        }
    }
    
    return factors;
}

// Основная функция генерации простого числа
bool generate_prime(int bits, int t, unsigned long& n, unsigned long& m, 
                   vector<unsigned long>& n_minus_1_factors, int& rejected) {
    const int SIEVE_SIZE = 2000;
    vector<unsigned long> primes = generate_primes(SIEVE_SIZE);
    
    while (true) {
        vector<unsigned long> m_factors;
        
        // Построение числа m с разрядностью ровно (bits - 1)
        m = build_m(bits - 1, primes, m_factors);
        
        // Вычисляем n = 2m + 1
        n = 2 * m + 1;
        
        // Получаем уникальные простые делители числа n-1
        vector<unsigned long> factors = get_unique_factors(m_factors);
        
        // Отправка числа n на проверку тестом Миллера
        int res = miller_selfridge_test(n, factors, t);
        
        if (res == 1) {
            // Дополнительная проверка этим же тестом при t = 2
            int res_2 = miller_selfridge_test(n, factors, 2);
            if (res_2 == 1) {
                n_minus_1_factors = factors;
                return true; // Число полностью подтверждено как простое!
            } else {
                rejected++; // Не прошло доп. тест
            }
        } else {
            // Тест вернул 0 (составное) или -1 (вероятно составное)
            rejected++;
        }
    }
}

// Функция для вывода результатов
void print_results(unsigned long n, int bits, unsigned long m, 
                  const vector<unsigned long>& n_minus_1_factors, int rejected) {
    cout << "\nУспешно найдено простое число n: " << n << endl;
    cout << "Фактическая разрядность n: " << bits << " бит" << endl;
    cout << "Компонента m: " << m << " (разрядность: " << (bits - 1) << " бит)" << endl;
    cout << "Уникальные простые делители (q_i) для n-1: ";
    for (size_t i = 0; i < n_minus_1_factors.size(); i++) {
        cout << n_minus_1_factors[i] << (i + 1 < n_minus_1_factors.size() ? ", " : "");
    }
    cout << "\nКоличество отброшенных вариантов (rejected): " << rejected << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(0));
    
    cout << "--- Задание 2: Генерация и Тест Миллера (Сэлфридж) ---" << endl;
    
    // Ввод параметров
    int bits, t;
    cout << "Введите требуемую разрядность числа (bits): ";
    cin >> bits;
    cout << "Введите параметр надежности (t): ";
    cin >> t;

    // Проверка корректности ввода
    if (bits < 3 || bits > 64) {
        cout << "Ошибка: поддерживаемая разрядность от 3 до 64 бит." << endl;
        return 1;
    }
    
    // Переменные для результатов
    unsigned long n = 0, m = 0;
    vector<unsigned long> n_minus_1_factors;
    int rejected = 0;
    
    // Генерация простого числа
    bool success = generate_prime(bits, t, n, m, n_minus_1_factors, rejected);
    
    // Вывод результатов
    if (success) {
        print_results(n, bits, m, n_minus_1_factors, rejected);
    } else {
        cout << "Не удалось сгенерировать простое число." << endl;
        return 1;
    }
    
    return 0;
}