#include "CPT.h"

bool isSquare(int num) {
    if (num < 0) return false;
    int n = static_cast<int>(std::sqrt(num));
    return n * n == num;
}

std::pair<int, int> closest_pair_tonum(int upper_limit) {
    if (upper_limit < 2) {
        std::cout << "Number " << upper_limit << "is too small(choose num >= 2)" << std::endl;
        return std::make_pair(0, 0);
    }
    
    for (int m = upper_limit - 1; m > 1; --m) {
        for (int n = m - 1; n > 0; --n) {
            int sum = m + n;
            int diff = m - n;
            if (isSquare(sum) && isSquare(diff)) {
                return std::make_pair(m, n);
            }
        }
    }
    return std::make_pair(0, 0);
}