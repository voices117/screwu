#ifndef PACKAGE_H_
#define PACKAGE_H_

#include <iostream>
#include "screw.h"

namespace Screw {
class Package {
   public:
    std::string type;
    int count, width_median;

    Package(const std::string& type, int count, int width_median)
        : type(type), count(count), width_median(width_median) {
    }
    ~Package() {
    }

    friend std::ostream& operator<<(std::ostream& out, const Package& p) {
        return out << "Paquete listo: " << p.count << " tornillos de tipo "
                   << p.type << " (mediana: " << p.width_median << ")";
    }
};
}  // namespace Screw

#endif
