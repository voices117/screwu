#include "packager.h"
#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include "utils.h"

/**
 * @brief Construct a new Packager object.
 *
 * @param config The packager configuration.
 */
Screw::Packager::Packager(const ScrewPackager::Config& config)
    : config(config) {
    /* creates an empty array for each screw type */
    for (auto& screw_conf : this->config) {
        this->screws.emplace(screw_conf.first, std::vector<screw_width_t>{});
    }
}

/**
 * @brief Destroy the Packager object
 */
Screw::Packager::~Packager() {
}

/**
 * @brief Adds a batch and (if possible) builds a package.
 *
 * @param batch The batch of screws to add.
 */
void Screw::Packager::add_batch(Screw::Batch batch) {
    std::lock_guard<std::mutex> lock{this->mutex};

    try {
        int p_size = this->config.get_package_size(batch.type);
        int pending = batch.count;
        while (pending > 0) {
            /* calculates the free space in the package */
            int free_space = p_size - this->screws[batch.type].size();
            for (int i = 0; i < std::min(free_space, pending); i++) {
                this->screws[batch.type].push_back(batch.width);
            }

            /* updates the pending screws */
            pending -= free_space;

            /* checks if the package was completed */
            if (this->screws[batch.type].size() >=
                static_cast<std::size_t>(p_size)) {
                int median = Utils::median(this->screws[batch.type]);

                /* builds the package */
                const std::string& type_name =
                    this->config.get_screw_type_name(batch.type);
                Screw::Package p{type_name, p_size, median};

                /* notifies the observers */
                for (auto observer : this->observers) {
                    observer->handle_package(p);
                }

                this->screws[batch.type].clear();
            }
        }
    } catch (const std::out_of_range& e) {
        /* notifies the observers an invalid screw type was found */
        for (auto observer : this->observers) {
            observer->handle_invalid_type(batch.type);
        }
    }
}

/**
 * @brief Returns the remaining unpacked screws of a given type.
 *
 * @return The remaining screws for the specified type.
 */
const std::map<screw_type_t, std::vector<screw_width_t>>&
Screw::Packager::get_remainders(void) const {
    return this->screws;
}

/**
 * @brief Adds an observer to the packager to handle the events.
 *
 * @param observer.
 */
void Screw::Packager::add_observer(PackagerObserver& observer) {
    this->observers.push_back(&observer);
}

/**
 * @brief Returns the name of the given screw type ID.
 *
 * @param t The screw type (ID).
 * @return Screw type name.
 */
const std::string& Screw::Packager::get_screw_type_name(screw_type_t t) const {
    return this->config.get_screw_type_name(t);
}
