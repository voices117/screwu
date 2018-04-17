#ifndef UTILS_H_
#define UTILS_H_

#include <cmath>
#include <set>

namespace Utils {
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
