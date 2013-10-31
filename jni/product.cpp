#include "product.h"

Product::Product(long id, std::string name, std::string logo_path) {
    this->id = id;
    this->name = name;
    this->logo_path = logo_path;
}
