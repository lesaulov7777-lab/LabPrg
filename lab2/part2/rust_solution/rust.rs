use std::collections::{HashMap, HashSet};

fn find_longest_chain_end(routes: &[(String, String)]) -> String {
    if routes.is_empty() {
        return "Ошибка".to_string();
    }

    let mut next_point: HashMap<String, String> = HashMap::new(); // from -> to
    let mut indegree: HashMap<String, usize> = HashMap::new();    // сколько раз в пункт приходят
    let mut all_nodes: HashSet<String> = HashSet::new();

    // 1) Валидация и построение графа
    for (from, to) in routes {
        // Пример 3: Искитим -> Искитим => ошибка
        if from == to {
            return "Ошибка".to_string();
        }

        // Из одного пункта не должно быть двух разных переходов
        if let Some(existing_to) = next_point.get(from) {
            if existing_to != to {
                return "Ошибка".to_string();
            }
        }
        next_point.insert(from.clone(), to.clone());

        *indegree.entry(to.clone()).or_insert(0) += 1;
        all_nodes.insert(from.clone());
        all_nodes.insert(to.clone());
    }

    // 2) Ищем начала цепочек (вершины без входящих ребер)
    let mut starts: Vec<String> = Vec::new();
    for node in &all_nodes {
        if !indegree.contains_key(node) && next_point.contains_key(node) {
            starts.push(node.clone());
        }
    }

    // Если стартов нет, а ребра есть => цикл
    if starts.is_empty() {
        return "Ошибка".to_string();
    }

    // 3) Проходим каждую цепочку, ищем самую длинную
    let mut best_len: i32 = -1;
    let mut best_end = "Ошибка".to_string();

    let mut used_edge_starts: HashSet<String> = HashSet::new();

    for s in starts {
        let mut local_visited: HashSet<String> = HashSet::new();
        let mut cur = s.clone();
        let mut len: i32 = 0; // число переходов

        while let Some(next) = next_point.get(&cur) {
            // цикл в текущем проходе
            if local_visited.contains(&cur) {
                return "Ошибка".to_string();
            }
            local_visited.insert(cur.clone());

            used_edge_starts.insert(cur.clone());
            cur = next.clone();
            len += 1;
        }

        // cur — конечный пункт этой цепочки
        if len > best_len {
            best_len = len;
            best_end = cur;
        }
    }

    // 4) Если есть ребра, не достижимые из стартов => цикл/некорректный компонент
    if used_edge_starts.len() != next_point.len() {
        return "Ошибка".to_string();
    }

    best_end
}

fn main() {
    let ex1 = vec![
        ("Лондон".to_string(), "Стамбул".to_string()),
        ("Новосибирск".to_string(), "Лондон".to_string()),
        ("Стамбул".to_string(), "Пекин".to_string()),
    ];
    let ex2 = vec![("Барнаул".to_string(), "Новосибирск".to_string())];
    let ex3 = vec![("Искитим".to_string(), "Искитим".to_string())];
    let ex4 = vec![
        ("Москва".to_string(), "Норильск".to_string()),
        ("Норильск".to_string(), "Саратов".to_string()),
        ("Саратов".to_string(), "Москва".to_string()),
    ];

    println!("{}", find_longest_chain_end(&ex1)); // Пекин
    println!("{}", find_longest_chain_end(&ex2)); // Новосибирск
    println!("{}", find_longest_chain_end(&ex3)); // Ошибка
    println!("{}", find_longest_chain_end(&ex4)); // Ошибка
}