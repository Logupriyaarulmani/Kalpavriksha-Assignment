#include<stdio.h>
#include<string.h>
#define MAX 100
#define FILE_NAME "students.txt"
struct Student{
    int id;
    char name[MAX];
    int age;
};
void createStudent(){
    struct Student s;
    printf("ID: "); 
    scanf("%d",&s.id);
    printf("Name: "); 
    scanf(" %[^\n]",s.name);
    printf("Age: "); 
    scanf("%d",&s.age);
    FILE *f=fopen(FILE_NAME,"a");
    if (!f){ 
        printf("Error\n"); 
        return; 
    }
    fprintf(f,"%d %s %d\n",s.id,s.name,s.age);
    fclose(f);
    printf("Added\n");
}
void readStudent(){
    struct Student s;
    FILE *f=fopen(FILE_NAME,"r");
    if (!f){ 
        printf("No data.\n"); 
        return; 
    }
    printf("\nID\tName\tAge\n");
    while (fscanf(f,"%d %s %d",&s.id,s.name,&s.age)!=EOF){
        printf("%d\t%s\t%d\n",s.id,s.name,s.age);
    }
    fclose(f);
}
void updateStudent(){
    int id,found=0;
    struct Student s;
    FILE *f=fopen(FILE_NAME,"r");
    FILE *temp=fopen("temp.txt","w");
    if (!f){
        printf("No data\n"); 
        return; 
    }
    printf("ID to update:"); 
    scanf("%d",&id);
    while (fscanf(f,"%d %s %d",&s.id,s.name,&s.age)!=EOF){
        if (s.id==id){
            found=1;
            printf("New Name: "); 
            scanf(" %[^\n]",s.name);
            printf("New Age: "); 
            scanf("%d",&s.age);
        }
        fprintf(temp,"%d %s %d\n",s.id,s.name,s.age);
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
void deleteStudent(){
    int id,found=0;
    struct Student s;
    FILE *f=fopen(FILE_NAME,"r");
    FILE *temp=fopen("temp.txt","w");
    if (!f){ 
        printf("No data\n"); 
        return; 
    }
    printf("ID to delete: "); 
    scanf("%d",&id);
    while (fscanf(f,"%d %s %d",&s.id,s.name,&s.age)!=EOF){
        if (s.id==id){ 
            found = 1; 
            continue; 
        }
        fprintf(temp,"%d %s %d\n",s.id,s.name,s.age);
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
                createStudent(); 
                break;
            case 2: 
                readStudent(); 
                break;
            case 3: 
                updateStudent(); 
                break;
            case 4: 
                deleteStudent(); 
                break;
            case 5: 
                printf("Exiting\n"); 
                break;
            default: printf("Invalid!\n");
        }
    } while (choice!=5);
    return 0;
}