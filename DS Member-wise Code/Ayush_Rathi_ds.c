#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Workout {
    char exercise[50];
    float weight;
    int sets;
    int reps;
};

struct Workout workoutArray[100];
int count = 0;

void addWorkout(char e[], float w, int s, int r) {
    strcpy(workoutArray[count].exercise, e);
    workoutArray[count].weight = w;
    workoutArray[count].sets = s;
    workoutArray[count].reps = r;
    count++;
}

void printAll() {
    int i;
    for (i = 0; i < count; i++) {
        printf("%s %f %d %d\n", workoutArray[i].exercise, workoutArray[i].weight, workoutArray[i].sets, workoutArray[i].reps);
    }
}

int addWorkoutSafe(char e[], float w, int s, int r) {
    if (w <= 0 || s <= 0 || r <= 0) {
        printf("Invalid input for %s\n", e);
        return 0;
    }
    if (count >= 100) {
        printf("Array full\n");
        return 0;
    }
    addWorkout(e, w, s, r);
    return 1;
}

float calculateVolumeAt(int index) {
    return workoutArray[index].weight * workoutArray[index].sets * workoutArray[index].reps;
}

void printVolumes() {
    int i;
    for (i = 0; i < count; i++) {
        printf("%s Volume: %f\n", workoutArray[i].exercise, calculateVolumeAt(i));
    }
}

float totalVolumeAll() {
    int i;
    float total = 0;
    for (i = 0; i < count; i++) {
        total = total + calculateVolumeAt(i);
    }
    return total;
}

int totalSets() {
    int i;
    int total = 0;
    for (i = 0; i < count; i++) {
        total = total + workoutArray[i].sets;
    }
    return total;
}

int totalReps() {
    int i;
    int total = 0;
    for (i = 0; i < count; i++) {
        total = total + workoutArray[i].sets * workoutArray[i].reps;
    }
    return total;
}

int searchWorkout(char e[]) {
    int i;
    for (i = 0; i < count; i++) {
        if (strcmp(workoutArray[i].exercise, e) == 0) {
            return i;
        }
    }
    return -1;
}

int findHeaviest() {
    int i;
    int index = 0;
    for (i = 1; i < count; i++) {
        if (workoutArray[i].weight > workoutArray[index].weight) {
            index = i;
        }
    }
    return index;
}

void updateWeight(int index, float w) {
    if (index < 0 || index >= count) {
        printf("Invalid index\n");
        return;
    }
    if (w <= 0) {
        printf("Invalid weight\n");
        return;
    }
    workoutArray[index].weight = w;
}

void deleteWorkout(int index) {
    int i;
    if (index < 0 || index >= count) {
        printf("Invalid index\n");
        return;
    }
    for (i = index; i < count - 1; i++) {
        strcpy(workoutArray[i].exercise, workoutArray[i + 1].exercise);
        workoutArray[i].weight = workoutArray[i + 1].weight;
        workoutArray[i].sets = workoutArray[i + 1].sets;
        workoutArray[i].reps = workoutArray[i + 1].reps;
    }
    count--;
}

int main() {
    int index;

    addWorkout("Bench Press", 60, 3, 10);
    addWorkout("Squat", 80, 3, 8);
    printAll();

    addWorkoutSafe("Bench Press", 65, 3, 8);
    addWorkoutSafe("Curl", 12, 3, 12);
    addWorkoutSafe("Deadlift", -20, 0, -5);
    addWorkoutSafe("Lat Pulldown", 50, 3, 10);
    printAll();
    printVolumes();
    printf("Total volume: %f\n", totalVolumeAll());
    printf("Total sets: %d\n", totalSets());
    printf("Total reps: %d\n", totalReps());

    index = searchWorkout("Curl");
    if (index != -1) {
        printf("Curl found at position %d\n", index);
    }
    index = findHeaviest();
    printf("Heaviest workout: %s\n", workoutArray[index].exercise);
    updateWeight(0, 62.5);
    updateWeight(0, -10);
    deleteWorkout(searchWorkout("Curl"));
    printAll();

    return 0;
}
