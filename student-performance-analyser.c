#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

struct StudentDetails{
    int Roll_no;
    char Name[100];
    int Marks_subject1;
    int Marks_subject2;
    int Marks_subject3;
};


int calculateTotalMarks(struct StudentDetails student){
    return student.Marks_subject1 + student.Marks_subject2 + student.Marks_subject3;
}

float calculateAverageMarks(int totalMarks){
    return totalMarks / 3.0;
}

char gradingSystem (float averageMarks){
    if (averageMarks >= 85) {
        return 'A';
    } 
    else if (averageMarks >= 70) { 
        return 'B';
    } 
    else if (averageMarks >= 50) {
        return 'C';
    } 
    else if (averageMarks >= 35) {
        return 'D';
    } 
    else {
        return 'F';
    }
} 

void performanceAnalyser (char grade){
    switch (grade) {
        case 'A':
            printf("Performance: *****\n");
            break;
        case 'B':
            printf("Performance: ****\n");
            break;
        case 'C':
            printf("Performance: ***\n");
            break;
        case 'D':
            printf("Performance: **\n");
            break;
        case 'F':
            break;
    }  
}

void displayStudent(struct StudentDetails student, int total, float avg, char grade) {
    printf("Roll: %d\n", student.Roll_no);
    printf("Name: %s\n", student.Name);
    printf("Total: %d\n", total);
    printf("Average: %.2f\n", avg);
    printf("Grade: %c\n", grade);
    if (grade != 'F') {
        performanceAnalyser(grade);
    }
}

void insertSortedRecursive(int arr[], int size, int value) {
    if (size == 0 || arr[size - 1] <= value) {
        arr[size] = value;
        return;
    }

    int last = arr[size - 1];
    insertSortedRecursive(arr, size - 1, value);
    arr[size] = last;
}

int main() {
    int no_of_students;
    scanf("%d", &no_of_students);
    int roll_numbers[100];
    int count = 0;

    for (int i = 0; i < no_of_students; i++){
        struct StudentDetails student;
        
        scanf("%d %s %d %d %d", &student.Roll_no, student.Name, 
          &student.Marks_subject1, &student.Marks_subject2, &student.Marks_subject3);

        int totalMarks = calculateTotalMarks(student);
        float averageMarks = calculateAverageMarks(totalMarks);
        char grade = gradingSystem(averageMarks);

        displayStudent(student, totalMarks, averageMarks, grade);

        insertSortedRecursive(roll_numbers, count, student.Roll_no);
        count++;
    }
    printf("\nList of Roll Numbers: ");
    for (int j = 0; j < count; j++) {
        printf("%d ", roll_numbers[j]);
    }  
    return 0;
}