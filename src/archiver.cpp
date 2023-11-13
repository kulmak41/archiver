#include <iostream>
#include <cstring>
#include "compression.h"
#include "decompression.h"
#include "file_existence_exception.h"

int main(int argc, char** argv) {
    const std::string try_help_message = "archiver: Try `archiver -h' for more information";

    if (argc == 1) {
        std::cout << "archiver: missing option flag" << std::endl;
        std::cout << try_help_message << std::endl;
        return 111;
    }
    if (strcmp(argv[1], "-c") == 0) {
        if (argc < 4) {
            std::cout << "archiver: wrong command usage format" << std::endl;
            std::cout << try_help_message << std::endl;
            return 111;
        }
        try {
            CompressFiles(argv[2], argc - 3, argv + 3);
        } catch (FileExistenceException& e) {
            std::cout << "archiver: file \"" << e.file_name << "\" does not exist" << std::endl;
            return 111;
        } catch (std::exception& e) {
            std::cout << "archiver: internal error" << std::endl;
            return 111;
        }
    } else if (strcmp(argv[1], "-d") == 0) {
        if (argc < 3) {
            std::cout << "archiver: archive name is not given" << std::endl;
            std::cout << try_help_message << std::endl;
            return 111;
        }
        try {
            DecompressFiles(argv[2]);
        } catch (FileExistenceException& e) {
            std::cout << "archiver: file \"" << e.file_name << "\" does not exist" << std::endl;
            return 111;
        } catch (std::exception& e) {
            std::cout << "archiver: internal error" << std::endl;
            return 111;
        }
    } else if (strcmp(argv[1], "-h") == 0) {
        std::cout << "archiver -c archive_name file1 [file2 ...] : compress file1, file2, ... into file archive_name"
                  << std::endl;
        std::cout << "archiver -d archive_name                   : decompress files from archive archive_name and put "
                     "them into current directory"
                  << std::endl;
    } else {
        std::cout << "archiver: illegal option '" << argv[1] << "'" << std::endl;
        std::cout << try_help_message << std::endl;
        return 111;
    }

    return 0;
}
