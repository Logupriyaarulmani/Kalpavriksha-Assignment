#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct {
    int rollNo;
    char name[100];
    int marks[3];
    int totalMarks;
    float averageMarks;
    char grade;
} Student;

typedef enum {
    GRADE_A = 'A',
    GRADE_B = 'B',
    GRADE_C = 'C',
    GRADE_D = 'D',
    GRADE_F = 'F'
} Grade;

int calculateTotalMarks(Student student) {
    int total = 0;
    int subjectCount = sizeof(student.marks) / sizeof(student.marks[0]);
    for (int i = 0; i < subjectCount; i++) {
        total += student.marks[i];
    }
    return total;
}

float calculateAverageMarks(int totalMarks, int subjectCount) {
    return totalMarks / (float) subjectCount;
}

Grade calculateGrade(float averageMarks) {
    if (averageMarks >= 85) {
        return GRADE_A;
    } else if (averageMarks >= 70) {
        return GRADE_B;
    } else if (averageMarks >= 50) {
        return GRADE_C;
    } else if (averageMarks >= 35) {
        return GRADE_D;
    } else {
        return GRADE_F;
    }
}

void printStarRating(Grade gradeValue) {
    switch (gradeValue) {
        case GRADE_A: printf("Performance: *****\n"); break;
        case GRADE_B: printf("Performance: ****\n"); break;
        case GRADE_C: printf("Performance: ***\n"); break;
        case GRADE_D: printf("Performance: **\n"); break;
        default: break;
    }
}

void displayStudentDetails(Student student) {
    printf("Roll: %d\n", student.rollNo);
    printf("Name: %s\n", student.name);
    printf("Total: %d\n", student.totalMarks);
    printf("Average: %.2f\n", student.averageMarks);
    printf("Grade: %c\n", student.grade);
    if (student.grade != 'F') {
        printStarRating(calculateGrade(student.averageMarks));
    }
}

void displayRollNumbersRecursive(int arr[], int index, int count) {
    if (index == count) return;
    printf("%d ", arr[index]);
    displayRollNumbersRecursive(arr, index + 1, count);
}

bool isValidRoll(int rollNumber) {
    return rollNumber > 0;
}

bool isValidMark(int mark) {
    return mark >= 0 && mark <= 100;
}

int main() {
    int noOfStudents;
    scanf("%d", &noOfStudents);
    int rollNumbers[100];
    int count = 0;

    for (int i = 0; i < noOfStudents; i++) {
        Student student;

        scanf("%d %s %d %d %d", &student.rollNo, student.name,
              &student.marks[0], &student.marks[1], &student.marks[2]);

        if (!isValidRoll(student.rollNo)) {
            printf("Invalid roll number for student %d\n", i + 1);
            continue;
        }

        bool validMarks = true;
        int subjectCount = sizeof(student.marks) / sizeof(student.marks[0]);
        for (int j = 0; j < subjectCount; j++){
            if (!isValidMark(student.marks[j])) {
                printf("Invalid mark %d for student %d\n", student.marks[j], i + 1);
                validMarks = false;
                break;
            }
        }

        if (!validMarks) continue;

        student.totalMarks = calculateTotalMarks(student);
        student.averageMarks = calculateAverageMarks(student.totalMarks, subjectCount);
        student.grade = calculateGrade(student.averageMarks);

        displayStudentDetails(student);

        rollNumbers[count++] = student.rollNo;
    }

    printf("\nList of Roll Numbers: ");
    displayRollNumbersRecursive(rollNumbers, 0, count);
    return 0;
}
