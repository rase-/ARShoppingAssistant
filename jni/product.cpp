#include "product.h"

Product::Product(long id, std::string name, std::string logo_path) {
    this->id = id;
    this->name = name;
    this->logo_path = logo_path;
}

std::string Product::get_name() {
    return this->name;
}

void Product::set_name(std::string new_name) {
    this->name = new_name;
}
