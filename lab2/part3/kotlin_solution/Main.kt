import java.util.StringTokenizer

private class FastScanner {
    private val br = System.`in`.bufferedReader()
    private var st: StringTokenizer? = null

    fun nextInt(): Int {
        while (st == null || !st!!.hasMoreElements()) {
            st = StringTokenizer(br.readLine())
        }
        return st!!.nextToken().toInt()
    }
}

fun main() {
    val fs = FastScanner()
    val n = fs.nextInt()

    val out = StringBuilder()

    repeat(n) {
        val x = fs.nextInt()

        // Если число состоит из одной цифры — выводим "-"
        if (x in 1..9) {
            out.append("-")
        } else {
            var num = x
            var evenCount = 0

            while (num > 0) {
                val d = num % 10
                if (d % 2 == 0) evenCount++
                num /= 10
            }

            out.append(evenCount)
        }

        if (it != n - 1) out.append(" ")
    }

    println(out.toString())
}