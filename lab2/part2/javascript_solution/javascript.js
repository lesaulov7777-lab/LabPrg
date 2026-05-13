function findLongestChainEnd(routes) {
  if (!routes || routes.length === 0) return "Ошибка";

  const nextPoint = new Map(); // from -> to
  const indegree = new Map();  // сколько раз в пункт приходят
  const allNodes = new Set();

  // 1) Валидация и построение графа
  for (const [from, to] of routes) {
    // Пример 3: Искитим -> Искитим => ошибка
    if (from === to) return "Ошибка";

    // Из одного пункта не должно быть двух разных переходов
    if (nextPoint.has(from) && nextPoint.get(from) !== to) return "Ошибка";
    nextPoint.set(from, to);

    indegree.set(to, (indegree.get(to) || 0) + 1);
    allNodes.add(from);
    allNodes.add(to);
  }

  // 2) Ищем начала цепочек (вершины без входящих ребер)
  const starts = [];
  for (const node of allNodes) {
    if (!indegree.has(node) && nextPoint.has(node)) {
      starts.push(node);
    }
  }

  // Если стартов нет, а ребра есть => цикл
  if (starts.length === 0) return "Ошибка";

  // 3) Проходим каждую цепочку, ищем самую длинную
  let bestLen = -1;
  let bestEnd = "Ошибка";
  const usedEdgeStarts = new Set();

  for (const s of starts) {
    const localVisited = new Set();
    let cur = s;
    let len = 0; // число переходов

    while (nextPoint.has(cur)) {
      // цикл в текущем проходе
      if (localVisited.has(cur)) return "Ошибка";
      localVisited.add(cur);

      usedEdgeStarts.add(cur);
      cur = nextPoint.get(cur);
      len++;
    }

    // cur — конечный пункт этой цепочки
    if (len > bestLen) {
      bestLen = len;
      bestEnd = cur;
    }
  }

  // 4) Если есть ребра, не достижимые из стартов => цикл/некорректный компонент
  if (usedEdgeStarts.size !== nextPoint.size) return "Ошибка";

  return bestEnd;
}

// Примеры:
const ex1 = [
  ["Лондон", "Стамбул"],
  ["Новосибирск", "Лондон"],
  ["Стамбул", "Пекин"],
];
const ex2 = [["Барнаул", "Новосибирск"]];
const ex3 = [["Искитим", "Искитим"]];
const ex4 = [
  ["Москва", "Норильск"],
  ["Норильск", "Саратов"],
  ["Саратов", "Москва"],
];

console.log(findLongestChainEnd(ex1)); // Пекин
console.log(findLongestChainEnd(ex2)); // Новосибирск
console.log(findLongestChainEnd(ex3)); // Ошибка
console.log(findLongestChainEnd(ex4)); // Ошибка