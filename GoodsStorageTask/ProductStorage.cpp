#include <algorithm>

#include "ProductStorage.h"

void ProductStorage::AddProduct(ProductRecord &&in)
{
    std::lock_guard<std::mutex> lock(m_storageDefenderMutex);

    m_productsHashMapByVendorCode.insert({ in.vendor_code, in }); // O(1)

    auto neededManufacturerIt = m_productsMapByManufacturer.find(in.manufacturer);
    if (neededManufacturerIt == m_productsMapByManufacturer.end())
    {
        m_productsMapByManufacturer[in.manufacturer].push_back(in);
    }
    else
    {
        neededManufacturerIt->second.emplace_back(std::move(in));
        std::sort(neededManufacturerIt->second.begin(), neededManufacturerIt->second.end(), [](const ProductRecord &a, const ProductRecord &b) {return a.vendor_code < b.vendor_code; });
    }

    return;
}

// Use return copy of the value (no pointer, because none thread safety)
ProductRecord ProductStorage::GetProductByVendorCode(const std::string &vendor_code)
{
    std::lock_guard<std::mutex> lock(m_storageDefenderMutex);

    auto neededProduct = m_productsHashMapByVendorCode.find(vendor_code); // O(1)
    if (neededProduct != m_productsHashMapByVendorCode.end())
    {
        return neededProduct->second;
    }

    throw std::string("ERROR: GetProductByVendorCode can't find needed product");
}

std::vector<ProductRecord> ProductStorage::GetProductsByManufacturer(const std::string &manufacturer)
{
    std::lock_guard<std::mutex> lock(m_storageDefenderMutex);

    auto neededManufacturerIt = m_productsMapByManufacturer.find(manufacturer);// O(log(N))
    if (neededManufacturerIt != m_productsMapByManufacturer.end())
    {
        return neededManufacturerIt->second;
    }

    throw std::string("ERROR: GetProductsByManufacturer can't find needed products");
}

void ProductStorage::RemoveProductByVendorCode(const std::string &vendor_code)
{
    std::lock_guard<std::mutex> lock(m_storageDefenderMutex);

    auto neededProduct = m_productsHashMapByVendorCode.find(vendor_code); // O(1)
    if (neededProduct != m_productsHashMapByVendorCode.end())
    {
        std::string manufacturer = neededProduct->second.manufacturer;
        m_productsHashMapByVendorCode.erase(vendor_code);

        auto neededManufacturerIt = m_productsMapByManufacturer.find(manufacturer);// O(log(N))
        auto lower = std::lower_bound(neededManufacturerIt->second.begin(), neededManufacturerIt->second.end(), vendor_code, [](const ProductRecord &a, const std::string &value) {return a.vendor_code < value; });
        bool isFound = lower != neededManufacturerIt->second.end() && lower->vendor_code == vendor_code;
        if (isFound)
        {
            neededManufacturerIt->second.erase(lower);
        }

        return;
    }

    throw std::string("ERROR: RemoveProductByVendorCode can't find needed product");
}

