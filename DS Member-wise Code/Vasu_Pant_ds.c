#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char muscleNames[10][20];
float muscleVolume[10];
int muscleFrequency[10];
int muscleCount = 0;

int findMuscle(char name[]) {
    int i;
    for (i = 0; i < muscleCount; i++) {
        if (strcmp(muscleNames[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

void addVolume(char name[], float volume) {
    int index = findMuscle(name);
    if (index == -1) {
        strcpy(muscleNames[muscleCount], name);
        muscleVolume[muscleCount] = volume;
        muscleCount++;
    } else {
        muscleVolume[index] = muscleVolume[index] + volume;
    }
}

void printMuscles() {
    int i;
    for (i = 0; i < muscleCount; i++) {
        printf("%s : %f\n", muscleNames[i], muscleVolume[i]);
    }
}

void addFrequency(char name[]) {
    int index = findMuscle(name);
    if (index == -1) {
        printf("%s not found\n", name);
        return;
    }
    muscleFrequency[index] = muscleFrequency[index] + 1;
}

float getMuscleVolume(char name[]) {
    int index = findMuscle(name);
    if (index == -1) {
        return 0;
    }
    return muscleVolume[index];
}

void distributeVolume(char exercise[], float volume) {
    if (strcmp(exercise, "Bench Press") == 0) {
        addVolume("Chest", volume * 0.70);
        addVolume("Triceps", volume * 0.20);
        addVolume("Shoulders", volume * 0.10);
    } else if (strcmp(exercise, "Squat") == 0) {
        addVolume("Quads", volume * 0.50);
        addVolume("Glutes", volume * 0.30);
        addVolume("Hamstrings", volume * 0.20);
    } else if (strcmp(exercise, "Deadlift") == 0) {
        addVolume("Back", volume * 0.60);
        addVolume("Hamstrings", volume * 0.25);
        addVolume("Glutes", volume * 0.15);
    } else if (strcmp(exercise, "Lat Pulldown") == 0) {
        addVolume("Back", volume * 0.80);
        addVolume("Biceps", volume * 0.20);
    } else if (strcmp(exercise, "Curl") == 0) {
        addVolume("Biceps", volume * 0.90);
        addVolume("Forearms", volume * 0.10);
    } else {
        printf("Unknown exercise: %s\n", exercise);
    }
}

void sortMuscles() {
    int i;
    int j;
    char tempName[20];
    float tempVolume;
    int tempFrequency;
    for (i = 0; i < muscleCount - 1; i++) {
        for (j = 0; j < muscleCount - 1 - i; j++) {
            if (muscleVolume[j] < muscleVolume[j + 1]) {
                strcpy(tempName, muscleNames[j]);
                strcpy(muscleNames[j], muscleNames[j + 1]);
                strcpy(muscleNames[j + 1], tempName);

                tempVolume = muscleVolume[j];
                muscleVolume[j] = muscleVolume[j + 1];
                muscleVolume[j + 1] = tempVolume;

                tempFrequency = muscleFrequency[j];
                muscleFrequency[j] = muscleFrequency[j + 1];
                muscleFrequency[j + 1] = tempFrequency;
            }
        }
    }
}

void printScores() {
    int i;
    int j;
    int score;
    for (i = 0; i < muscleCount; i++) {
        score = (muscleVolume[i] / 4000) * 100;
        if (score > 100) {
            score = 100;
        }
        printf("%s ", muscleNames[i]);
        for (j = 0; j < score / 5; j++) {
            printf("#");
        }
        printf(" %d%%\n", score);
    }
}

int findHighestMuscle() {
    int i;
    int index = 0;
    for (i = 1; i < muscleCount; i++) {
        if (muscleVolume[i] > muscleVolume[index]) {
            index = i;
        }
    }
    return index;
}

float totalMuscleVolume() {
    int i;
    float total = 0;
    for (i = 0; i < muscleCount; i++) {
        total = total + muscleVolume[i];
    }
    return total;
}

void checkBalance(char a[], char b[]) {
    float volumeA = getMuscleVolume(a);
    float volumeB = getMuscleVolume(b);
    float difference = volumeA - volumeB;
    if (difference < 0) {
        difference = -difference;
    }
    if (difference > 1000) {
        printf("Warning: %s and %s imbalance detected\n", a, b);
    } else {
        printf("%s and %s are balanced\n", a, b);
    }
}

int main() {
    int index;

    addVolume("Chest", 1260);
    addVolume("Chest", 900);
    addVolume("Back", 1500);
    printMuscles();

    distributeVolume("Bench Press", 1800);
    distributeVolume("Squat", 1920);
    distributeVolume("Lat Pulldown", 1500);
    distributeVolume("Curl", 432);
    distributeVolume("Swimming", 500);
    addFrequency("Chest");
    addFrequency("Chest");
    addFrequency("Back");
    addFrequency("Abs");
    printMuscles();

    printf("Abs volume: %f\n", getMuscleVolume("Abs"));
    printf("Total muscle volume: %f\n", totalMuscleVolume());
    index = findHighestMuscle();
    printf("Most trained muscle: %s\n", muscleNames[index]);

    sortMuscles();
    printMuscles();
    printScores();

    checkBalance("Chest", "Back");
    checkBalance("Quads", "Biceps");

    return 0;
}
