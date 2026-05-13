import java.util.*;

public class Main {

    static class Route {
        String from;
        String to;

        Route(String from, String to) {
            this.from = from;
            this.to = to;
        }
    }

    public static String findLongestChainEnd(List<Route> routes) {
        if (routes == null || routes.isEmpty()) return "Ошибка";

        Map<String, String> next = new HashMap<>();      // from -> to
        Map<String, Integer> indegree = new HashMap<>(); // входящая степень
        Set<String> allNodes = new HashSet<>();

        // 1) Валидация + построение графа
        for (Route r : routes) {
            String from = r.from;
            String to = r.to;

            // A -> A запрещено
            if (from.equals(to)) return "Ошибка";

            // Из одного пункта не должно быть двух разных выходов
            if (next.containsKey(from) && !next.get(from).equals(to)) {
                return "Ошибка";
            }

            next.put(from, to);
            indegree.put(to, indegree.getOrDefault(to, 0) + 1);

            allNodes.add(from);
            allNodes.add(to);
        }

        // 2) Ищем старты цепочек (вершины без входящих ребер)
        List<String> starts = new ArrayList<>();
        for (String node : allNodes) {
            if (!indegree.containsKey(node) && next.containsKey(node)) {
                starts.add(node);
            }
        }

        // Нет стартов => цикл
        if (starts.isEmpty()) return "Ошибка";

        // 3) Идем по каждой цепочке, выбираем самую длинную
        int bestLen = -1;
        String bestEnd = "Ошибка";
        Set<String> usedEdgeStarts = new HashSet<>();

        for (String start : starts) {
            Set<String> localVisited = new HashSet<>();
            String cur = start;
            int len = 0;

            while (next.containsKey(cur)) {
                // цикл в этой компоненте
                if (localVisited.contains(cur)) return "Ошибка";
                localVisited.add(cur);

                usedEdgeStarts.add(cur);
                cur = next.get(cur);
                len++;
            }

            if (len > bestLen) {
                bestLen = len;
                bestEnd = cur;
            }
        }

        // Если часть ребер не покрылась обходом со стартов => есть цикл/некорректные данные
        if (usedEdgeStarts.size() != next.size()) return "Ошибка";

        return bestEnd;
    }

    public static void main(String[] args) {
        List<Route> ex1 = Arrays.asList(
                new Route("Лондон", "Стамбул"),
                new Route("Новосибирск", "Лондон"),
                new Route("Стамбул", "Пекин")
        );

        List<Route> ex2 = Arrays.asList(
                new Route("Барнаул", "Новосибирск")
        );

        List<Route> ex3 = Arrays.asList(
                new Route("Искитим", "Искитим")
        );

        List<Route> ex4 = Arrays.asList(
                new Route("Москва", "Норильск"),
                new Route("Норильск", "Саратов"),
                new Route("Саратов", "Москва")
        );

        System.out.println(findLongestChainEnd(ex1)); // Пекин
        System.out.println(findLongestChainEnd(ex2)); // Новосибирск
        System.out.println(findLongestChainEnd(ex3)); // Ошибка
        System.out.println(findLongestChainEnd(ex4)); // Ошибка
    }
}