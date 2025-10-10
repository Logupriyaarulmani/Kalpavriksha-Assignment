#include <stdio.h>
#include <string.h>
#include <stdbool.h>
const int MAX = 100;
const char FILE_NAME[] = "users.txt";

struct User {
    int id;
    char name[100];
    int age;
};

struct User createUser();
void displayUser(struct User user);
void displayAllUsers(struct User users[], int count);
int loadUsers(struct User users[]);
void saveUsers(struct User users[], int count);
bool updateUserInMemory(struct User users[], int count, int id);
bool deleteUserInMemory(struct User users[], int count, int id);

struct User createUser() {
    struct User newUser;
    printf("Enter ID: ");
    scanf("%d", &newUser.id);
    printf("Enter Name: ");
    scanf(" %[^\n]", newUser.name);
    printf("Enter Age: ");
    scanf("%d", &newUser.age);
    return newUser;
}

void displayUser(struct User user) {
    printf("%-5d %-20s %-5d\n", user.id, user.name, user.age);
}

void displayAllUsers(struct User users[], int count) {
    if (count == 0) {
        printf("No users found.\n");
        return;
    }
    printf("\n%-5s %-20s %-5s\n", "ID", "Name", "Age");
    printf("\n");
    for (int i = 0; i < count; i++) {
        displayUser(users[i]);
    }
}

int loadUsers(struct User users[]) {
    FILE *filePointer = fopen(FILE_NAME, "r");
    if (!filePointer) return 0;
    int count = 0;
    while (fscanf(filePointer, "%d %s %d", &users[count].id,
                  users[count].name, &users[count].age) != EOF) {
        count++;
        if (count >= MAX) break;  
    }
    fclose(filePointer);
    return count;
}

void saveUsers(struct User users[], int count) {
    FILE *filePointer = fopen(FILE_NAME, "w");
    if (!filePointer) {
        printf("Error: Unable to open file.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(filePointer, "%d %s %d\n",
                users[i].id, users[i].name, users[i].age);
    }
    fclose(filePointer);
}

bool updateUserInMemory(struct User users[], int count, int id) {
    for (int i = 0; i < count; i++) {
        if (users[i].id == id) {
            printf("Enter new Name: ");
            scanf(" %[^\n]", users[i].name);
            printf("Enter new Age: ");
            scanf("%d", &users[i].age);
            return true;
        }
    }
    return false;
}

bool deleteUserInMemory(struct User users[], int count, int id) {
    for (int i = 0; i < count; i++) {
        if (users[i].id == id) {
            for (int j = i; j < count - 1; j++) {
                users[j] = users[j + 1];
            }
            return true;
        }
    }
    return false;
}

int main() {
    struct User users[100];
    int userCount = 0;
    int choice;
    while (true) {
        printf("\nUSER MANAGEMENT SYSTEM\n");
        printf("1. Add User\n");
        printf("2. View Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        userCount = loadUsers(users);
        switch (choice) {
            case 1: {
                if (userCount >= MAX) {
                    printf("Maximum user limit reached.\n");
                    break;
                }
                struct User newUser = createUser();
                users[userCount++] = newUser;
                saveUsers(users, userCount);
                printf("User added successfully.\n");
                break;
            }

            case 2:
                displayAllUsers(users, userCount);
                break;

            case 3: {
                int idToUpdate;
                printf("Enter ID to update: ");
                scanf("%d", &idToUpdate);
                if (updateUserInMemory(users, userCount, idToUpdate)) {
                    saveUsers(users, userCount);
                    printf("User updated successfully.\n");
                } else {
                    printf("User with ID %d not found.\n", idToUpdate);
                }
                break;
            }

            case 4: {
                int idToDelete;
                printf("Enter ID to delete: ");
                scanf("%d", &idToDelete);
                if (deleteUserInMemory(users, userCount, idToDelete)) {
                    userCount--;  // Reduce count after deletion
                    saveUsers(users, userCount);
                    printf("User deleted successfully.\n");
                } else {
                    printf("User with ID %d not found.\n", idToDelete);
                }
                break;
            }

            case 5:
                printf("Exiting program...\n");
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}