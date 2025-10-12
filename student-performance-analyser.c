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
    char gradeLetter;
} Student;

typedef enum Grade {
    F, D, C, B, A
} grade;

int calculateTotalMarks(Student student) {
    int total = 0;
    for (int i = 0; i < 3; i++) {
        total += student.marks[i];
    }
    return total;
}

float calculateAverageMarks(int totalMarks, int numberOfSubjects) {
    return totalMarks / (float) numberOfSubjects;
}

grade calculateGrade(float averageMarks) {
    if (averageMarks >= 85) {
        return A;
    } else if (averageMarks >= 70) {
        return B;
    } else if (averageMarks >= 50) {
        return C;
    } else if (averageMarks >= 35) {
        return D;
    } else {
        return F;
    }
}

char getGradeLetter(grade currentGrade) {
    switch (currentGrade) {
        case A: return 'A';
        case B: return 'B';
        case C: return 'C';
        case D: return 'D';
        case F: return 'F';
        default: return 'N';
    }
}

void analysePerformance(grade gradeValue) {
    switch (gradeValue) {
        case A: printf("Performance: *****\n"); break;
        case B: printf("Performance: ****\n"); break;
        case C: printf("Performance: ***\n"); break;
        case D: printf("Performance: **\n"); break;
        case F: printf("Performance: *\n"); break;
    }
}

void displayStudentDetails(Student student) {
    printf("Roll: %d\n", student.rollNo);
    printf("Name: %s\n", student.name);
    printf("Total: %d\n", student.totalMarks);
    printf("Average: %.2f\n", student.averageMarks);
    printf("Grade: %c\n", student.gradeLetter);
    if (student.gradeLetter != 'F') {
        analysePerformance(calculateGrade(student.averageMarks));
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
        for (int j = 0; j < 3; j++) {
            if (!isValidMark(student.marks[j])) {
                printf("Invalid mark %d for student %d\n", student.marks[j], i + 1);
                validMarks = false;
                break;
            }
        }

        if (!validMarks) continue;

        student.totalMarks = calculateTotalMarks(student);
        student.averageMarks = calculateAverageMarks(student.totalMarks, 3);
        grade g = calculateGrade(student.averageMarks);
        student.gradeLetter = getGradeLetter(g);

        displayStudentDetails(student);

        rollNumbers[count++] = student.rollNo;
    }

    printf("\nList of Roll Numbers: ");
    displayRollNumbersRecursive(rollNumbers, 0, count);
    return 0;
}
