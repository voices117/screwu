#ifndef BATCH_H_
#define BATCH_H_

#include "screw.h"

namespace Screw {
class Batch {
   public:
    screw_type_t type;
    int count;
    int width;

    Batch(screw_type_t type, int count, screw_width_t width)
        : type(type), count(count), width(width) {
    }
    ~Batch() {
    }
};
}  // namespace Screw

#endif
