#include "screwu.h"
#include <thread>

using namespace App;

/**
 * @brief Gets screw batches from a device until it closed.
 *
 * @param device The device to get batches from.
 * @param packager The packager that handles the batches.
 */
static void _handle_device(Device::Classifier& device,
                           Screw::Packager& packager) {
    while (true) {
        try {
            packager.add_batch(device.get_batch());
        } catch (const Device::Stalled& e) {
            std::cerr << device.get_name() << " atascado" << std::endl;
        } catch (const Device::Finished& e) {
            break;
        }
    }
}

/**
 * @brief Construct a new ScrewU object.
 *
 * @param config The packager configuration.
 */
ScrewU::ScrewU(const ScrewPackager::Config& config) : packager(config) {
    this->packager.add_observer(*this);
}

/**
 * @brief Destroy the ScrewU object.
 */
ScrewU::~ScrewU() {
    /* in case a thread was not finished */
    for (std::thread& w : this->workers) {
        if (w.joinable()) {
            w.join();
        }
    }
}

/**
 * @brief Adds a device to get batches from.
 *
 * @param filename Device filename.
 */
void ScrewU::add_device(const std::string& filename) {
    try {
        Device::Classifier device{filename};

        std::cout << filename << ": se establece conexion con el dispositivo "
                  << device.get_name() << std::endl;

        this->devices.push_back(std::move(device));
    } catch (const Device::ConnectionError& e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * @brief Receives the batches from all the devices and prints the obtained
 * oackages.
 */
void ScrewU::print_packages(void) {
    /* collects screw batches from each device in individual worker threads */
    for (Device::Classifier& device : this->devices) {
        this->workers.push_back(std::thread(_handle_device, std::ref(device),
                                            std::ref(this->packager)));
    }

    /* waits for all the worker threads to finish */
    for (std::thread& w : this->workers) {
        if (w.joinable()) {
            w.join();
        }
    }
}

/**
 * @brief Prints the remaining screws.
 */
void ScrewU::print_remainders(void) {
    std::cout << "# Informe de remantentes" << std::endl;
    for (const auto remainder : this->packager.get_remainders()) {
        const std::string& type =
            this->packager.get_screw_type_name(remainder.first);
        std::cout << "* " << remainder.second << " tornillos de tipo " << type
                  << std::endl;
    }
}

/**
 * @brief Handles the event of a package that has been built.
 *
 * @param package Package that has been built.
 */
void ScrewU::handle_package(const Screw::Package& package) {
    std::cout << "Paquete listo: " << package << std::endl;
}

/**
 * @brief Handles the event of an invalid screw type received.
 *
 * @param invalid The invalid screw type.
 */
void ScrewU::handle_invalid_type(screw_type_t invalid) {
    std::cerr << "Tipo de tornillo invalido: " << invalid << std::endl;
}
