#ifndef PACKAGER_H_
#define PACKAGER_H_

#include <map>
#include <mutex>
#include <set>
#include <string>
#include <vector>
#include "batch.h"
#include "config.h"
#include "package.h"

namespace Screw {
/**
 * @brief This class should be implemented by any object that wishes to be
 * notified whenever a Packager completes a Package or receives an invalid screw
 * type.
 */
class PackagerObserver {
   public:
    /** This method is invoked when a package is completed. */
    virtual void handle_package(const Package& p) = 0;
    /** This method is invoked when an invalid screw type is received. */
    virtual void handle_invalid_type(screw_type_t invalid) = 0;
};

class Packager {
   public:
    explicit Packager(const ScrewPackager::Config& config);
    Packager(Packager& other) = delete;
    Packager operator=(Packager& other) = delete;
    ~Packager();

    /* actions */
    void add_batch(Screw::Batch batch);
    void add_observer(PackagerObserver& observer);

    /* queries */
    const std::map<screw_type_t, int>& get_remainders(void) const;
    const std::string& get_screw_type_name(screw_type_t t) const;

   private:
    std::map<screw_type_t, int> screws_count;
    std::map<screw_type_t, std::set<screw_width_t>> screws_width;
    const ScrewPackager::Config& config;
    std::mutex mutex;
    std::vector<PackagerObserver*> observers;
};
}  // namespace Screw

#endif
