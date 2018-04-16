#ifndef CONFIG_H_
#define CONFIG_H_

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <map>
#include "screw.h"

namespace ScrewPackager {
class Error : public std::exception {
   public:
    Error(const char* msg) : msg(msg) {
    }
    Error(std::string msg) : msg(msg) {
    }
    const char* what() const noexcept {
        return this->msg.c_str();
    }

   private:
    std::string msg;
};

class ScrewConfig {
   public:
    std::string name;
    int limit;

    ScrewConfig(std::string name, int limit) : name(name), limit(limit) {
    }
    ~ScrewConfig() {
    }
};

class Config {
   public:
    using const_iterator = std::map<screw_type_t, ScrewConfig>::const_iterator;

    static Config from_file(const std::string& file_name);

    Config();
    Config(Config&& other) = default;
    ~Config();

    /** queries */
    int get_package_size(screw_type_t t) const;
    const std::string& get_screw_type_name(screw_type_t t) const;
    const ScrewConfig& get_screw_configs(void) const;

    /** iterators */
    const_iterator begin() const;
    const_iterator end() const;

    /** operators */
    friend std::istream& operator>>(std::istream& in, Config& data) {
        while (in) {
            char c;
            int id, limit;
            std::string name;

            /* gets the values from the istream */
            in >> id;
            in >> c;
            std::getline(in, name, ',');
            in >> limit;
            if (in.eof()) {
                break;
            }
            if (!in || c != '=') {
                throw Error{"archivo de configuracion invalido"};
            }

            data.screws_conf.emplace(id, ScrewConfig{name, limit});
        }
        return in;
    }

   private:
    std::map<screw_type_t, ScrewConfig> screws_conf;
};
}  // namespace ScrewPackager

#endif
