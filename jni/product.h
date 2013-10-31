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
    std::string get_name();
    void set_name(std::string new_name);
};

#endif
