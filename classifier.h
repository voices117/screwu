#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#include <fstream>
#include <string>
#include "batch.h"

namespace Device {

enum class Status { working, stalled, finished };

class DeviceException : public std::exception {
   public:
    DeviceException(const std::string& message) : message(message) {
    }
    const char* what() const noexcept {
        return this->message.c_str();
    }

   protected:
    std::string message;
};

class Finished : public DeviceException {
   public:
    Finished() : DeviceException("") {
    }
};

class Stalled : public DeviceException {
   public:
    Stalled() : DeviceException("") {
    }
};

class ConnectionError : public DeviceException {
   public:
    ConnectionError(const std::string& message) : DeviceException(message) {
    }
};

class Classifier {
   public:
    explicit Classifier(const std::string& filename);
    Classifier(Classifier&& other);
    Classifier(Classifier& other) = delete;
    Classifier operator=(Classifier& other) = delete;
    ~Classifier();

    Screw::Batch get_batch(void);
    const std::string& get_name(void) const;

   private:
    std::ifstream* input{nullptr};
    std::string name;
};
}  // namespace Device

#endif
