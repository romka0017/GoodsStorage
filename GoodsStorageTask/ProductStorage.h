#pragma once

#include <string>
#include <map>
#include <mutex>
#include <unordered_map>
#include <vector>

typedef struct {
    std::string  vendor_code;
    std::string  manufacturer;
    std::string  name;
    unsigned int cost = 0;
} ProductRecord;

class ProductStorage
{
    std::mutex m_storageDefenderMutex;
    std::unordered_map<std::string, ProductRecord> m_productsHashMapByVendorCode;
    std::map<std::string, std::vector<ProductRecord>> m_productsMapByManufacturer;

public:
    void AddProduct(ProductRecord &&in);
    ProductRecord GetProductByVendorCode(const std::string &vendor_code);
    std::vector<ProductRecord> GetProductsByManufacturer(const std::string &manufacturer);
    void RemoveProductByVendorCode(const std::string &vendor_code);
};
