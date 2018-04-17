#include "classifier.h"
#include <arpa/inet.h>
#include <iostream>

/**
 * @brief Construct a new Classifier.
 *
 * @param filename Name of the file of the classifier device.
 * This function throws an exception in case the device file was no found.
 */
Device::Classifier::Classifier(const std::string& filename) {
    this->input = new std::ifstream();
    this->input->open(filename, std::ios::in | std::ios::binary);

    if (this->input->fail()) {
        delete this->input;
        throw ConnectionError{filename +
                              ": no se pudo conectar con el dispositivo"};
    }

    std::getline(*this->input, this->name, '\0');
}

/**
 * @brief Copy constructor.
 *
 * @param other Ohter instance.
 */
Device::Classifier::Classifier(Classifier&& other) {
    std::swap(this->input, other.input);
    std::swap(this->name, other.name);
}

Device::Classifier::~Classifier() {
    delete this->input;
}

/**
 * @brief Gets a batch of screws from the classifier device.
 *
 * @return Batch of screws.
 *
 * This function throws an exception in case there are no more pending batches
 * or a device has stalled/jammed.
 */
Screw::Batch Device::Classifier::get_batch(void) {
    uint32_t data;

    /* reads a 32 bit integer from the input stream */
    this->input->read(reinterpret_cast<char*>(&data), sizeof(data));
    if (!(*this->input)) {
        throw Finished{};
    }

    /* big-endian -> host endianness */
    data = ntohl(data);

    /* checks if it stalled */
    if (data == 0xFFFFFFFF) {
        throw Stalled{};
    }

    int type = (data >> 27) & 0x1F;
    int quantity = (data >> 5) & 0x3FFFFF;
    int width = data & 0x1F;

    return Screw::Batch{type, quantity, width};
}

/**
 * @brief Gets the device name.
 *
 * @return Device name.
 */
const std::string& Device::Classifier::get_name(void) const {
    return this->name;
}
