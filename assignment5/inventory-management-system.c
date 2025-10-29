#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct {
    int productId;
    char productName[50];
    float price;
    int quantity;
} Product;

bool isValidNoOfProducts(int noOfProducts) {
    return noOfProducts > 0 && noOfProducts <= 100;
}

bool isValidProductId(int productId) {
    return productId > 0 && productId <= 10000;
}

bool isValidPrice(float price) {
    return price >= 0 && price <= 100000;
}

bool isValidQuantity(int quantity) {
    return quantity >= 0 && quantity <= 1000000;
}

void viewAllProducts(Product *products, int count) {
    if (count == 0) {
        printf("No products available.\n");
        return;
    }
    printf("\n========= PRODUCT LIST =========\n");
    for (int i = 0; i < count; i++) {
        printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
               products[i].productId, products[i].productName,
               products[i].price, products[i].quantity);
    }
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
        scanf("%d", &newProduct->productId);
        if (!isValidProductId(newProduct->productId))
            printf("Invalid Product ID. Must be between 1 and 10000.\n");
    } while (!isValidProductId(newProduct->productId));

    printf("Product Name: ");
    scanf("%49s", newProduct->productName);

    do {
        printf("Product Price: ");
        scanf("%f", &newProduct->price);
        if (!isValidPrice(newProduct->price))
            printf("Invalid Price. Must be between 0 and 100000.\n");
    } while (!isValidPrice(newProduct->price));

    do {
        printf("Product Quantity: ");
        scanf("%d", &newProduct->quantity);
        if (!isValidQuantity(newProduct->quantity))
            printf("Invalid Quantity. Must be between 0 and 1000000.\n");
    } while (!isValidQuantity(newProduct->quantity));

    (*count)++;
    printf("Product added successfully!\n");
}

void updateQuantity(Product *products, int count) {
    int id, newQty, found = 0;
    printf("Enter Product ID to update quantity: ");
    scanf("%d", &id);
    for (int i = 0; i < count; i++) {
        if (products[i].productId == id) {
            do {
                printf("Enter new Quantity: ");
                scanf("%d", &newQty);
                if (!isValidQuantity(newQty))
                    printf("Invalid Quantity. Must be between 0 and 1000000.\n");
            } while (!isValidQuantity(newQty));
            products[i].quantity = newQty;
            printf("Quantity updated successfully!\n");
            found = 1;
            break;
        }
    }
    if (!found)
        printf("Product not found.\n");
}

void searchById(Product *products, int count) {
    int id, found = 0;
    printf("Enter Product ID to search: ");
    scanf("%d", &id);
    for (int i = 0; i < count; i++) {
        if (products[i].productId == id) {
            printf("Product Found: Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   products[i].productId, products[i].productName,
                   products[i].price, products[i].quantity);
            found = 1;
            break;
        }
    }
    if (!found)
        printf("Product not found.\n");
}

void searchByName(Product *products, int count) {
    char name[50];
    int found = 0;
    printf("Enter name to search (partial allowed): ");
    scanf("%49s", name);
    printf("Products Found:\n");
    for (int i = 0; i < count; i++) {
        if (strstr(products[i].productName, name)) {
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   products[i].productId, products[i].productName,
                   products[i].price, products[i].quantity);
            found = 1;
        }
    }
    if (!found)
        printf("No matching products found.\n");
}

void searchByPriceRange(Product *products, int count) {
    float min, max;
    int found = 0;
    printf("Enter minimum price: ");
    scanf("%f", &min);
    printf("Enter maximum price: ");
    scanf("%f", &max);
    printf("Products in price range:\n");
    for (int i = 0; i < count; i++) {
        if (products[i].price >= min && products[i].price <= max) {
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   products[i].productId, products[i].productName,
                   products[i].price, products[i].quantity);
            found = 1;
        }
    }
    if (!found)
        printf("No products found in this price range.\n");
}

void deleteProduct(Product **products, int *count) {
    int id, found = 0, index = -1;
    printf("Enter Product ID to delete: ");
    scanf("%d", &id);
    for (int i = 0; i < *count; i++) {
        if ((*products)[i].productId == id) {
            index = i;
            found = 1;
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

int main() {
    int noOfProducts, choice;
    printf("Enter the initial number of products: ");
    scanf("%d", &noOfProducts);

    if (!isValidNoOfProducts(noOfProducts)) {
        printf("Invalid number of products. Must be between 1 and 100.\n");
        return 0;
    }

    Product *products = (Product *)calloc(noOfProducts, sizeof(Product));
    if (products == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < noOfProducts; i++) {
        printf("\nEnter details for product %d:\n", i + 1);

        do {
            printf("Product ID: ");
            scanf("%d", &products[i].productId);
            if (!isValidProductId(products[i].productId))
                printf("Invalid Product ID. Must be between 1 and 10000.\n");
        } while (!isValidProductId(products[i].productId));

        printf("Product Name: ");
        scanf("%49s", products[i].productName);

        do {
            printf("Product Price: ");
            scanf("%f", &products[i].price);
            if (!isValidPrice(products[i].price))
                printf("Invalid Price. Must be between 0 and 100000.\n");
        } while (!isValidPrice(products[i].price));

        do {
            printf("Product Quantity: ");
            scanf("%d", &products[i].quantity);
            if (!isValidQuantity(products[i].quantity))
                printf("Invalid Quantity. Must be between 0 and 1000000.\n");
        } while (!isValidQuantity(products[i].quantity));
    }

    do {
        printf("\n========= INVENTORY MENU =========\n");
        printf("1. Add New Product\n");
        printf("2. View All Products\n");
        printf("3. Update Quantity\n");
        printf("4. Search Product by ID\n");
        printf("5. Search Product by Name\n");
        printf("6. Search Product by Price Range\n");
        printf("7. Delete Product\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addNewProduct(&products, &noOfProducts);
                break;
            case 2:
                viewAllProducts(products, noOfProducts);
                break;
            case 3:
                updateQuantity(products, noOfProducts);
                break;
            case 4:
                searchById(products, noOfProducts);
                break;
            case 5:
                searchByName(products, noOfProducts);
                break;
            case 6:
                searchByPriceRange(products, noOfProducts);
                break;
            case 7:
                deleteProduct(&products, &noOfProducts);
                break;
            case 8:
                free(products);
                printf("Memory released successfully. Exiting program...\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 8);
    return 0;
}