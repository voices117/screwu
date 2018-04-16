#include "packager.h"
#include "utils.h"

using namespace Screw;

/**
 * @brief Construct a new Packager object.
 *
 * @param config The packager configuration.
 */
Packager::Packager(const ScrewPackager::Config& config) : config(config) {
    for (auto screw_conf : this->config) {
        this->screws_count[screw_conf.second.name] = 0;
    }
}

/**
 * @brief Destroy the Packager object
 */
Packager::~Packager() {
}

/**
 * @brief Adds a batch and (if possible) builds a package.
 *
 * @param batch The batch of screws to add.
 */
void Packager::add_batch(Screw::Batch batch) {
    std::lock_guard<std::mutex> lock{this->mutex};

    try {
        const std::string& p_type =
            this->config.get_screw_type_name(batch.type);
        int p_size = this->config.get_package_size(batch.type);

        this->screws_count[p_type] += batch.count;
        this->screws_width[p_type].insert(batch.width);

        /* checks if there are enough screws to build a package */
        while (this->screws_count.at(p_type) >= p_size) {
            /* gets the median size */
            int median = Utils::median(this->screws_width.at(p_type));

            /* builds the package */
            Screw::Package p{p_type, p_size, median};
            for (auto observer : this->observers) {
                observer->handle_package(p);
            }

            /* updates the screws info */
            this->screws_count[p_type] -= p_size;
            this->screws_width[p_type].clear();
            if (this->screws_count[p_type] > 0) {
                this->screws_width[p_type].insert(batch.width);
            }
        }
    } catch (const std::out_of_range& e) {
        for (auto observer : this->observers) {
            observer->handle_invalid_type(batch.type);
        }
    }
}

/**
 * @brief Returns a map with the remaining screws.
 *
 * @return The remaining screws.
 */
const std::map<std::string, int>& Packager::get_remainders(void) const {
    return this->screws_count;
}

/**
 * @brief Adds an observer to the packager to handle the events.
 *
 * @param observer.
 */
void Packager::add_observer(PackagerObserver& observer) {
    this->observers.push_back(&observer);
}
