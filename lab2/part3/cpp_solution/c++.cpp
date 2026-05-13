#include <iostream>
using namespace std;

int main() {
    int N;
    cin >> N;

    for (int i = 0; i < N; i++) {
        int x;
        cin >> x;

        if (x >= 1 && x <= 9) {
            cout << "-";
        } else {
            int evenCount = 0;
            while (x > 0) {
                int d = x % 10;
                if (d % 2 == 0) evenCount++;
                x /= 10;
            }
            cout << evenCount;
        }

        if (i != N - 1) cout << " ";
    }

    return 0;
}