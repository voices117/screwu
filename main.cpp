#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include "classifier.h"
#include "config.h"
#include "screwu.h"

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        std::cerr << "No se especificó archivo de configuración" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        ScrewPackager::Config config =
            ScrewPackager::Config::from_file(argv[1]);
        App::ScrewU app{config};

        for (int i = 2; i < argc; i++) {
            app.add_device(argv[i]);
        }

        app.print_packages();
        app.print_remainders();

        return EXIT_SUCCESS;
    } catch (const ScrewPackager::Error& e) {
        std::cerr << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return EXIT_FAILURE;
}
