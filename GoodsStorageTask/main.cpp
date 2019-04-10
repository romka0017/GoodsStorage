#include <iostream>

#include "ProductStorage.h"

int main()
{
    try {
        ProductStorage storage;
        for (int i = 0; i < 10; i++)
        {
            ProductRecord tmp;
            tmp.cost = i;
            tmp.manufacturer = "intel";
            tmp.name = "processor";

            std::string s = std::to_string(i);
            tmp.vendor_code = "vendor_code_intel_" + s;
            storage.AddProduct(std::move(tmp));
        }
        for (int i = 0; i < 5; i++)
        {
            ProductRecord tmp;
            tmp.cost = i;
            tmp.manufacturer = "amd";
            tmp.name = "processor";

            std::string s = std::to_string(i);
            tmp.vendor_code = "vendor_code_amd_" + s;
            storage.AddProduct(std::move(tmp));
        }

        std::string sByVendor = "vendor_code_intel_5";
        ProductRecord neededProductByVendorCode = storage.GetProductByVendorCode(sByVendor);

        std::string sByManufacturer = "amd";
        std::vector<ProductRecord> neededProductsByManufacturer = storage.GetProductsByManufacturer(sByManufacturer);

        std::string sByVendorForRemoving = "vendor_code_intel_6";
        storage.RemoveProductByVendorCode(sByVendorForRemoving);
    }
    catch (std::string & e) {
        std::cout << e << std::endl;
        throw std::string("ERROR: main");
    }
}