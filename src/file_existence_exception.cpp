#include "file_existence_exception.h"

FileExistenceException::FileExistenceException(const char *file_name) : file_name(file_name) {
}