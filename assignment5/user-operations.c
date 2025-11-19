#include "user-operations.h"

bool isValidProductId(int id) {
    return id >= MIN_PRODUCT_ID && id <= MAX_PRODUCT_ID;
}

bool isValidPrice(float price) {
    return price >= MIN_PRICE && price <= MAX_PRICE;
}

bool isValidQuantity(int qty) {
    return qty >= MIN_PRODUCT_QUANTITY && qty <= MAX_PRODUCT_QUANTITY;
}