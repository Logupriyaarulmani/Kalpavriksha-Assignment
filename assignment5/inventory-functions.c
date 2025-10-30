#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "inventory-functions.h"
#include "user-operations.h"

void clearInputBuffer() {
    while (getchar() != '\n');
}

void viewAllProducts(Product *products, int count) {
    if (count == 0) {
        printf("No products available.\n");
        return;
    }
    printf("\n========= PRODUCT LIST =========\n");
    for (int i = 0; i < count; i++) {
        printProduct(products[i]);
    }
}

bool isDuplicateId(Product *products, int count, int id) {
    for (int i = 0; i < count; i++) {
        if (products[i].productId == id)
            return true;
    }
    return false;
}

void addNewProduct(Product **products, int *count) {
    *products = realloc(*products, (*count + 1) * sizeof(Product));
    if (*products == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    Product *newProduct = &(*products)[*count];

    do {
        printf("Product ID: ");
        if (scanf("%d", &newProduct->productId) != 1) {
            printf("Invalid input. Enter a number.\n");
            clearInputBuffer();
            continue;
        }
        if (!isValidProductId(newProduct->productId))
            printf("Invalid Product ID. Must be between 1 and 10000.\n");
        else if (isDuplicateId(*products, *count, newProduct->productId))
            printf("Product ID already exists! Enter a unique ID.\n");
    } while (!isValidProductId(newProduct->productId) || isDuplicateId(*products, *count, newProduct->productId));

    printf("Product Name: ");
    scanf("%49s", newProduct->productName);

    do {
        printf("Product Price: ");
        if (scanf("%f", &newProduct->price) != 1) {
            printf("Invalid input. Enter a valid price.\n");
            clearInputBuffer();
            continue;
        }
        if (!isValidPrice(newProduct->price))
            printf("Invalid Price. Must be between 0 and 100000.\n");
    } while (!isValidPrice(newProduct->price));

    do {
        printf("Product Quantity: ");
        if (scanf("%d", &newProduct->quantity) != 1) {
            printf("Invalid input. Enter a valid quantity.\n");
            clearInputBuffer();
            continue;
        }
        if (!isValidQuantity(newProduct->quantity))
            printf("Invalid Quantity. Must be between 0 and 1000000.\n");
    } while (!isValidQuantity(newProduct->quantity));

    (*count)++;
    printf("Product added successfully!\n");
}

void updateQuantity(Product *products, int count) {
    int id, newQty;
    bool found = false;
    printf("Enter Product ID to update quantity: ");
    while (scanf("%d", &id) != 1) {
        printf("Invalid input. Enter a valid Product ID: ");
        clearInputBuffer();
    }

    for (int i = 0; i < count; i++) {
        if (products[i].productId == id) {
            do {
                printf("Enter new Quantity: ");
                if (scanf("%d", &newQty) != 1) {
                    printf("Invalid input. Enter a valid number.\n");
                    clearInputBuffer();
                    continue;
                }
                if (!isValidQuantity(newQty))
                    printf("Invalid Quantity. Must be between 0 and 1000000.\n");
            } while (!isValidQuantity(newQty));
            products[i].quantity = newQty;
            printf("Quantity updated successfully!\n");
            found = true;
            break;
        }
    }
    if (!found)
        printf("Product not found.\n");
}

void searchById(Product *products, int count) {
    int id;
    bool found = false;
    printf("Enter Product ID to search: ");
    while (scanf("%d", &id) != 1) {
        printf("Invalid input. Enter a valid Product ID: ");
        clearInputBuffer();
    }

    for (int i = 0; i < count; i++) {
        if (products[i].productId == id) {
            printProduct(products[i]);
            found = true;
            break;
        }
    }
    if (!found)
        printf("Product not found.\n");
}

void searchByName(Product *products, int count) {
    char name[50];
    bool found = false;
    printf("Enter name to search (partial allowed): ");
    scanf("%49s", name);
    printf("Products Found:\n");
    for (int i = 0; i < count; i++) {
        if (strstr(products[i].productName, name)) {
            printProduct(products[i]);
            found = true;
        }
    }
    if (!found)
        printf("No matching products found.\n");
}

void searchByPriceRange(Product *products, int count) {
    float min, max;
    bool found = false;

    printf("Enter minimum price: ");
    while (scanf("%f", &min) != 1) {
        printf("Invalid input. Enter a valid minimum price: ");
        clearInputBuffer();
    }

    printf("Enter maximum price: ");
    while (scanf("%f", &max) != 1) {
        printf("Invalid input. Enter a valid maximum price: ");
        clearInputBuffer();
    }

    printf("Products in price range:\n");
    for (int i = 0; i < count; i++) {
        if (products[i].price >= min && products[i].price <= max) {
            printProduct(products[i]);
            found = true;
        }
    }
    if (!found)
        printf("No products found in this price range.\n");
}

void deleteProduct(Product **products, int *count) {
    int id, index = -1;
    bool found = false;
    printf("Enter Product ID to delete: ");
    while (scanf("%d", &id) != 1) {
        printf("Invalid input. Enter a valid Product ID: ");
        clearInputBuffer();
    }

    for (int i = 0; i < *count; i++) {
        if ((*products)[i].productId == id) {
            index = i;
            found = true;
            break;
        }
    }

    if (found) {
        for (int i = index; i < *count - 1; i++) {
            (*products)[i] = (*products)[i + 1];
        }
        (*count)--;
        if (*count > 0)
            *products = realloc(*products, (*count) * sizeof(Product));
        else {
            free(*products);
            *products = NULL;
        }
        printf("Product deleted successfully!\n");
    } else {
        printf("Product not found.\n");
    }
}

void printProduct(Product p) {
    printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
           p.productId, p.productName, p.price, p.quantity);
}