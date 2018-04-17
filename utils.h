#ifndef UTILS_H_
#define UTILS_H_

#include <algorithm>
#include <vector>

namespace Utils {
/**
 * @brief Calculates the median in a group of numeric elements.
 *
 * @tparam T Numeric type.
 * @param elems Elements where the median is looked up.
 * @return T Median.
 */
template <typename T>
T median(std::vector<T>& elems) {
    int index = elems.size() / 2;
    std::sort(elems.begin(), elems.end());

    if (elems.size() % 2 == 0) {
        T sum = elems[index - 1] + elems[index];
        return std::round(sum / 2);
    } else {
        return elems[index];
    }
}
}  // namespace Utils

#endif
