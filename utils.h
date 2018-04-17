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
float median(std::vector<T>& elems) {
    int index = elems.size() / 2;
    std::sort(elems.begin(), elems.end());

    if (elems.size() % 2 == 0) {
        float sum = elems[index - 1] + elems[index];
        return sum / 2;
    } else {
        return elems[index];
    }
}
}  // namespace Utils

#endif
