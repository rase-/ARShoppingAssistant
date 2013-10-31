#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

class Product {
  private:
    long id;
    std::string name;
    std::string logo_path;
  public:
    Product(long id, std::string name, std::string logo_path);
};

#endif
