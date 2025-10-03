#include<stdio.h>
#include<string.h>
#define MAX 100
#define FILE_NAME "users.txt"
struct User{
    int id;
    char name[MAX];
    int age;
};
void createUser(){
    struct User u;
    printf("ID: "); 
    scanf("%d",&u.id);
    printf("Name: "); 
    scanf(" %[^\n]",u.name);
    printf("Age: "); 
    scanf("%d",&u.age);
    FILE *f=fopen(FILE_NAME,"a");
    if (!f){ 
        printf("Error\n"); 
        return; 
    }
    fprintf(f,"%d %s %d\n",u.id,u.name,u.age);
    fclose(f);
    printf("Added\n");
}
void readUser(){
    struct User u;
    FILE *f=fopen(FILE_NAME,"r");
    if (!f){ 
        printf("No data.\n"); 
        return; 
    }
    printf("\nID\tName\tAge\n");
    while (fscanf(f,"%d %s %d",&u.id,u.name,&u.age)!=EOF){
        printf("%d\t%s\t%d\n",u.id,u.name,u.age);
    }
    fclose(f);
}
void updateUser(){
    int id,found=0;
    struct User u;
    FILE *f=fopen(FILE_NAME,"r");
    FILE *temp=fopen("temp.txt","w");
    if (!f){
        printf("No data\n"); 
        return; 
    }
    printf("ID to update:"); 
    scanf("%d",&id);
    while (fscanf(f,"%d %s %d",&u.id,u.name,&u.age)!=EOF){
        if (u.id==id){
            found=1;
            printf("New Name: "); 
            scanf(" %[^\n]",u.name);
            printf("New Age: "); 
            scanf("%d",&u.age);
        }
        fprintf(temp,"%d %s %d\n",u.id,u.name,u.age);
    }
    fclose(f); 
    fclose(temp);
    if (found){ 
        remove(FILE_NAME); 
        rename("temp.txt",FILE_NAME); 
        printf("Updated.\n"); 
    }
    else{ 
        remove("temp.txt"); 
        printf("Not found.\n");
    }
}
void deleteUser(){
    int id,found=0;
    struct User u;
    FILE *f=fopen(FILE_NAME,"r");
    FILE *temp=fopen("temp.txt","w");
    if (!f){ 
        printf("No data\n"); 
        return; 
    }
    printf("ID to delete: "); 
    scanf("%d",&id);
    while (fscanf(f,"%d %s %d",&u.id,u.name,&u.age)!=EOF){
        if (u.id==id){ 
            found = 1; 
            continue; 
        }
        fprintf(temp,"%d %s %d\n",u.id,u.name,u.age);
    }
    fclose(f); 
    fclose(temp);
    if (found){ 
        remove(FILE_NAME); 
        rename("temp.txt",FILE_NAME); 
        printf("Deleted.\n"); 
    }
    else{ 
        remove("temp.txt"); 
        printf("Not found.\n"); 
    }
}
int main(){
    int choice;
    do{
        printf("\n1.Add 2.View 3.Update 4.Delete 5.Exit\nChoice:");
        scanf("%d",&choice);
        switch (choice){
            case 1: 
                createUser(); 
                break;
            case 2: 
                readUser(); 
                break;
            case 3: 
                updateUser(); 
                break;
            case 4: 
                deleteUser(); 
                break;
            case 5: 
                printf("Exiting\n"); 
                break;
            default: printf("Invalid!\n");
        }
    } while (choice!=5);
    return 0;
}