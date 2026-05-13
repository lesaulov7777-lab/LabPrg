def cycle(S: str, T: str):
    
    # Если длины строк не совпадают, циклический сдвиг невозможен
    if len(S) != len(T):
        return "No"
    
    # Если строки пустые, они равны
    if len(S) == 0:
        return "Yes" if S == T else "No"
    
    # Проверяем, содержится ли T в S+S
    if T in (S + S):
        return "Yes"
    else:
        return "No"

# Пример 1
S1, T1 = "waterbottle", "erbottlewat"
print(f"S = '{S1}', T = '{T1}', Результат: {cycle(S1, T1)}")

# Пример 2
S2, T2 = "abc", "acb"
print(f"S = '{S2}', T = '{T2}', Результат: {cycle(S2, T2)}")

# Пример 3
S3, T3 = "ngtu", "ngtu"
print(f"S = '{S3}', T = '{T3}', Результат: {cycle(S3, T3)}")