#ifndef UTILS_H_
#define UTILS_H_

#include <set>

namespace Utils {
template <typename T>
T median(std::set<T>& elems) {
    auto it = elems.begin();
    std::advance(it, elems.size() / 2);

    T nth = *it;

    if (elems.size() % 2 == 0) {
        T nth2 = *--it;
        return (nth + nth2) / 2;
    } else {
        return nth;
    }
}
}  // namespace Utils

#endif
