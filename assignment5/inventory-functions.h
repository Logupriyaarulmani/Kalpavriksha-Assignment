#ifndef INVENTORY_FUNCTIONS_H
#define INVENTORY_FUNCTIONS_H

typedef struct {
    int productId;
    char productName[50];
    float price;
    int quantity;
} Product;

void viewAllProducts(Product *products, int count);
void addNewProduct(Product **products, int *count);
void updateQuantity(Product *products, int count);
void searchById(Product *products, int count);
void searchByName(Product *products, int count);
void searchByPriceRange(Product *products, int count);
void deleteProduct(Product **products, int *count);
void printProduct(Product p);

#endif