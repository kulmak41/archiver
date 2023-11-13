add_executable(
        archiver
        archiver.cpp
        compression.cpp
        decompression.cpp
        binary_reader.cpp
        binary_writer.cpp symbols_constants.h
        canonical_code.cpp trie.cpp file_existence_exception.cpp)
