#include <stdio.h>
#include <stdlib.h>
#include "inventory-functions.h"
#include "user-operations.h"

int main() {
    int noOfProducts, choice;
    printf("Enter the initial number of products: ");
    while (scanf("%d", &noOfProducts) != 1) {
        printf("Invalid input. Please enter a number: ");
        while (getchar() != '\n'); 
    }

    if (noOfProducts < MIN_PRODUCTS || noOfProducts > MAX_PRODUCTS) {
        printf("Invalid number of products. Must be between %d and %d.\n", MIN_PRODUCTS, MAX_PRODUCTS);
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
            while (scanf("%d", &products[i].productId) != 1) {
                printf("Invalid input. Please enter a number: ");
                while (getchar() != '\n');
            }

        } while (!isValidProductId(products[i].productId));

        printf("Product Name: ");
        scanf("%49s", products[i].productName);

        do {
            printf("Product Price: ");
            while (scanf("%f", &products[i].price) != 1) {
                printf("Invalid input. Please enter a valid price: ");
                while (getchar() != '\n'); 
            }

        } while (!isValidPrice(products[i].price));

        do {
            printf("Product Quantity: ");
            while (scanf("%d", &products[i].quantity) != 1) {
                printf("Invalid input. Please enter a number: ");
                while (getchar() != '\n');
        }
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
        while (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number: ");
            while (getchar() != '\n');
        }


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
                products = NULL;
                printf("Exiting program...\n"); 
                break;
            default: 
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 8);

    return 0;
}