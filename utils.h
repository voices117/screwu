#ifndef UTILS_H_
#define UTILS_H_

#include <cmath>
#include <set>

namespace Utils {
/**
 * @brief Calculates the median in a set of numeroc elements.
 *
 * @tparam T Numeric type.
 * @param elems A set of elements where the median is looked up.
 * @return T Median.
 */
template <typename T>
T median(std::set<T>& elems) {
    auto it = elems.begin();
    std::advance(it, elems.size() / 2);

    float nth = *it;

    if (elems.size() % 2 == 0) {
        float sum = *--it + nth;
        return std::round(sum / 2);
    } else {
        return nth;
    }
}
}  // namespace Utils

#endif
