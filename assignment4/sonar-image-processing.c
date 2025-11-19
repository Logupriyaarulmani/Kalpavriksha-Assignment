#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

const int minimumIntensity = 0;
const int maximumIntensity = 255;

void generateSonarMatrix(int noOfRows, int *sonarMatrix){
    srand(time(NULL));
    for (int i = 0; i < noOfRows; i++){
        for (int j = 0; j < noOfRows; j++){
            sonarMatrix[i * noOfRows + j] = rand() % (maximumIntensity - minimumIntensity + 1) + minimumIntensity;
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

int calculateAverageForCell (int *sonarMatrix, int noOfRows, int row, int column) {
    int sum = 0;
    int count = 0;
    for (int rowOffset = -1; rowOffset <= 1; rowOffset++) {
        int neighborRow = row + rowOffset;
        if (neighborRow < 0 || neighborRow >= noOfRows) continue;

        for (int colOffset = -1; colOffset <= 1; colOffset++) {
            int neighborCol = column + colOffset;
            if (neighborCol < 0 || neighborCol >= noOfRows) continue;
    
            sum += *(sonarMatrix + neighborRow * noOfRows + neighborCol);
            count++;
        }
    }
    return sum / count;
}

void apply3x3SmoothingFilter(int noOfRows, int *sonarMatrix) {
    int *prevRow = NULL;

    for (int i = 0; i < noOfRows; i++) {
        int *tempRow = (int*) malloc(noOfRows * sizeof(int));
        if (!tempRow) {
            printf("Memory allocation failed\n");
            free(prevRow);
            return;
        }
        
        for (int j = 0; j < noOfRows; j++) {
            tempRow[j] = calculateAverageForCell(sonarMatrix, noOfRows, i, j);
        }

        if (i > 0) {
            for (int j = 0; j < noOfRows; j++)
                *(sonarMatrix + (i - 1) * noOfRows + j) = prevRow[j];
            free(prevRow);
        }

        prevRow = tempRow;
    }

    for (int j = 0; j < noOfRows; j++)
        *(sonarMatrix + (noOfRows - 1) * noOfRows + j) = prevRow[j];
    free(prevRow);
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

    int *sonarMatrix = (int*) malloc(noOfRows * noOfRows * sizeof(int));
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