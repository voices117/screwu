#ifndef SCREWU_H_
#define SCREWU_H_

#include <string>
#include <thread>
#include <vector>
#include "classifier.h"
#include "config.h"
#include "packager.h"

namespace App {
class ScrewU : public Screw::PackagerObserver {
   public:
    explicit ScrewU(const ScrewPackager::Config& config);
    ~ScrewU();

    void add_device(const std::string& filename);
    void print_packages(void);
    void print_remainders(void);

    /** observer handlers */
    void handle_package(const Screw::Package& p);
    void handle_invalid_type(screw_type_t invalid);

   private:
    Screw::Packager packager;
    std::vector<std::thread> workers;
    std::vector<Device::Classifier> devices;
};
}  // namespace App

#endif
