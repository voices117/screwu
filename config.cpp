#include "config.h"
#include <string>

/**
 * @brief Creates a Config object from a file.
 *
 * @param file_name Name of the file to read the configuration from.
 * @return Config The new config object.
 */
ScrewPackager::Config ScrewPackager::Config::from_file(
    const std::string& file_name) {
    std::fstream fs{file_name, std::fstream::in};
    if (!fs) {
        throw Error{file_name +
                    ": no se pudo abrir el archivo de configuracion"};
    }

    Config p;
    fs >> p;
    return p;
}

ScrewPackager::Config::Config() {
}

ScrewPackager::Config::~Config() {
}

/**
 * @brief Returns the package size (number of screws required) for the given
 * screw type.
 *
 * @param t Screw type.
 * @return int Number of screws required to build a package.
 */
int ScrewPackager::Config::get_package_size(screw_type_t t) const {
    return this->screws_conf.at(t).limit;
}

/**
 * @brief Returns the name of the given screw type ID.
 *
 * @param t The screw type (ID).
 * @return Screw type name.
 */
const std::string& ScrewPackager::Config::get_screw_type_name(
    screw_type_t t) const {
    return this->screws_conf.at(t).name;
}

/**
 * @brief Returns an iterator to the screw types configuration.
 *
 * @return Iterator to the different screw type configurations.
 */
ScrewPackager::Config::const_iterator ScrewPackager::Config::begin() const {
    return this->screws_conf.begin();
}

/**
 * @brief Last iterator.
 *
 * @return Last iterator.
 */
ScrewPackager::Config::const_iterator ScrewPackager::Config::end() const {
    return this->screws_conf.end();
}
