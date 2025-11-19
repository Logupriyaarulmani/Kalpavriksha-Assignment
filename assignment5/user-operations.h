#ifndef USER_OPERATIONS_H
#define USER_OPERATIONS_H

#include <stdbool.h>

#define MIN_PRODUCTS 1
#define MAX_PRODUCTS 100

#define MIN_PRODUCT_ID 1
#define MAX_PRODUCT_ID 10000

#define MIN_PRICE 0.0
#define MAX_PRICE 100000.0

#define MIN_PRODUCT_QUANTITY 0
#define MAX_PRODUCT_QUANTITY 1000000

bool isValidProductId(int id);
bool isValidPrice(float price);
bool isValidQuantity(int qty);

#endif