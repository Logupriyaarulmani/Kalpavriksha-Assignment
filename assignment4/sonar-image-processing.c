#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

void generateSonarMatrix(int noOfRows, int *sonarMatrix){
    srand(time(NULL));
    int minimumValue = 0;
    int maximumValue = 255;
    for (int i = 0; i < noOfRows; i++){
        for (int j = 0; j < noOfRows; j++){
            sonarMatrix[i * noOfRows + j] = rand() % (maximumValue - minimumValue + 1) + minimumValue;
        }
    }
}

void displaySonarMatrix (int noOfRows, int *sonarMatrix){
    for (int i = 0; i < noOfRows; i++){
        for (int j = 0; j < noOfRows; j++){
            printf("%d ", sonarMatrix[i * noOfRows + j]);
        }
        printf("\n");
    }
}

void rotateMatrix90Clockwise(int noOfRows, int *matrix){
    for (int layer = 0; layer < noOfRows / 2; layer++){
        int firstIndex = layer;
        int lastIndex = noOfRows - 1 - layer;
        for (int i = firstIndex; i < lastIndex; i++){
            int offset = i - firstIndex;

            int *top = matrix + firstIndex * noOfRows + i;
            int *right = matrix + i * noOfRows + lastIndex;
            int *bottom = matrix + lastIndex * noOfRows + (lastIndex - offset);
            int *left = matrix + (lastIndex - offset) * noOfRows + firstIndex;

            int temp = *top;
            *top = *left;
            *left = *bottom;
            *bottom = *right;
            *right = temp;
        }
    }
}

void apply3x3SmoothingFilter(int noOfRows, int *sonarMatrix) {
    int *tempMatrix = (int*) malloc(noOfRows * noOfRows * sizeof(int));
    if (!tempMatrix) {
        printf("Memory allocation failed\n");
        return;
    }

    for (int row = 0; row < noOfRows; row++) {
        for (int col = 0; col < noOfRows; col++) {
            int sum = 0, neighborCount = 0;

            for (int rowOffset = -1; rowOffset <= 1; rowOffset++) {
                int neighborRow = row + rowOffset;
                if (neighborRow < 0 || neighborRow >= noOfRows) continue;

                for (int colOffset = -1; colOffset <= 1; colOffset++) {
                    int neighborCol = col + colOffset;
                    if (neighborCol < 0 || neighborCol >= noOfRows) continue;

                    sum += *(sonarMatrix + neighborRow * noOfRows + neighborCol);
                    neighborCount++;
                }
            }

            *(tempMatrix + row * noOfRows + col) = sum / neighborCount;
        }
    }

    for (int i = 0; i < noOfRows * noOfRows; i++) {
        *(sonarMatrix + i) = *(tempMatrix + i);
    }

    free(tempMatrix);
}

bool isValidNoOfRows (int noOfRows){
    if (noOfRows >=2 && noOfRows <=10){
        return true;
    }
    else{
        return false;
    }
}

int main() {
    int noOfRows;
    printf("Enter matrix size (2-10): ");
    scanf("%d" , &noOfRows);

    if (!isValidNoOfRows(noOfRows)){
        printf("Invalid number of rows\n");
        return 0;
    }

    int noOfColumns = noOfRows;
    int *sonarMatrix = (int*) malloc(noOfRows * noOfColumns * sizeof(int));
    if (sonarMatrix == NULL){
        printf("Memory allocation failed\n");
        return 0;
    }

    printf("Original Randomly Generated Matrix:\n");
    generateSonarMatrix(noOfRows, sonarMatrix);
    displaySonarMatrix(noOfRows, sonarMatrix);

    printf("Matrix after 90° Clockwise Rotation:\n");
    rotateMatrix90Clockwise(noOfRows, sonarMatrix);
    displaySonarMatrix(noOfRows, sonarMatrix);

    printf("Matrix after Applying 3x3 Smoothing Filter:\n");
    apply3x3SmoothingFilter(noOfRows, sonarMatrix);
    displaySonarMatrix(noOfRows, sonarMatrix);

    free(sonarMatrix);
    return 0;
}