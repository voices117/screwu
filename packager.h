#ifndef PACKAGER_H_
#define PACKAGER_H_

#include <map>
#include <mutex>
#include <set>
#include <vector>
#include "batch.h"
#include "config.h"
#include "package.h"

namespace Screw {
class PackagerObserver {
   public:
    virtual void handle_package(const Package& p) = 0;
    virtual void handle_invalid_type(screw_type_t invalid) = 0;
};

class Packager {
   public:
    explicit Packager(const ScrewPackager::Config& config);
    Packager(Packager& other) = delete;
    Packager operator=(Packager& other) = delete;
    ~Packager();

    void add_batch(Screw::Batch batch);
    void add_observer(PackagerObserver& observer);
    const std::map<std::string, int>& get_remainders(void) const;

   private:
    std::map<std::string, int> screws_count;
    std::map<std::string, std::set<screw_width_t>> screws_width;
    const ScrewPackager::Config& config;
    std::mutex mutex;
    std::vector<PackagerObserver*> observers;
};
}  // namespace Screw

#endif
