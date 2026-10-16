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

struct Node {
    char exercise[50];
    float weight;
    struct Node* next;
};

struct Node* head = NULL;

void insertEnd(char e[], float w) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    strcpy(newNode->exercise, e);
    newNode->weight = w;
    newNode->next = NULL;

    if (head == NULL) {
        head = newNode;
        return;
    }
    struct Node* temp = head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}

void printList() {
    struct Node* temp = head;
    while (temp != NULL) {
        printf("%s %f\n", temp->exercise, temp->weight);
        temp = temp->next;
    }
}

void insertFront(char e[], float w) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    strcpy(newNode->exercise, e);
    newNode->weight = w;
    newNode->next = head;
    head = newNode;
}

void deleteFirst() {
    struct Node* temp;
    if (head == NULL) {
        printf("List empty\n");
        return;
    }
    temp = head;
    head = head->next;
    free(temp);
}

void deleteByName(char e[]) {
    struct Node* temp;
    struct Node* prev;
    if (head == NULL) {
        printf("List empty\n");
        return;
    }
    if (strcmp(head->exercise, e) == 0) {
        temp = head;
        head = head->next;
        free(temp);
        return;
    }
    prev = head;
    temp = head->next;
    while (temp != NULL) {
        if (strcmp(temp->exercise, e) == 0) {
            prev->next = temp->next;
            free(temp);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("%s not found in list\n", e);
}

struct Node* searchNode(char e[]) {
    struct Node* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->exercise, e) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

int countNodes() {
    int total = 0;
    struct Node* temp = head;
    while (temp != NULL) {
        total++;
        temp = temp->next;
    }
    return total;
}

float totalListWeight() {
    float total = 0;
    struct Node* temp = head;
    while (temp != NULL) {
        total = total + temp->weight;
        temp = temp->next;
    }
    return total;
}

void freeList() {
    struct Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

char stack[10][50];
int top = -1;

void push(char e[]) {
    top++;
    strcpy(stack[top], e);
}

void pop() {
    if (top == -1) {
        printf("Stack empty\n");
        return;
    }
    printf("Undo: %s\n", stack[top]);
    top--;
}

int isStackFull() {
    if (top == 9) {
        return 1;
    }
    return 0;
}

int isStackEmpty() {
    if (top == -1) {
        return 1;
    }
    return 0;
}

void pushSafe(char e[]) {
    if (isStackFull() == 1) {
        printf("Stack full\n");
        return;
    }
    push(e);
}

void peek() {
    if (isStackEmpty() == 1) {
        printf("Stack empty\n");
        return;
    }
    printf("Last entry: %s\n", stack[top]);
}

void printStack() {
    int i;
    for (i = top; i >= 0; i--) {
        printf("%s\n", stack[i]);
    }
}

void clearStack() {
    top = -1;
}

char queue[10][50];
int front = 0;
int rear = -1;
int qcount = 0;

void enqueue(char e[]) {
    rear++;
    strcpy(queue[rear], e);
    qcount++;
}

void dequeue() {
    if (qcount == 0) {
        printf("Queue empty\n");
        return;
    }
    printf("Scheduled: %s\n", queue[front]);
    front++;
    qcount--;
}

int isQueueEmpty() {
    if (qcount == 0) {
        return 1;
    }
    return 0;
}

void enqueueSafe(char e[]) {
    if (rear == 9) {
        printf("Queue full\n");
        return;
    }
    enqueue(e);
}

void peekQueue() {
    if (isQueueEmpty() == 1) {
        printf("Queue empty\n");
        return;
    }
    printf("Next workout: %s\n", queue[front]);
}

void printQueue() {
    int i;
    for (i = front; i <= rear; i++) {
        printf("%s\n", queue[i]);
    }
}

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
    struct Node* found;

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

    insertEnd("Bench Press", 60);
    insertEnd("Squat", 80);
    printList();

    insertFront("Warmup", 20);
    printList();
    printf("Nodes: %d\n", countNodes());
    found = searchNode("Squat");
    if (found != NULL) {
        printf("Found in list: %s %f\n", found->exercise, found->weight);
    }
    deleteByName("Squat");
    deleteByName("Deadlift");
    printList();
    deleteFirst();
    printList();
    printf("Total list weight: %f\n", totalListWeight());

    push("Bench Press");
    push("Squat");
    push("Curl");
    pop();

    pushSafe("Lat Pulldown");
    printStack();
    peek();
    pop();
    clearStack();
    pop();

    enqueue("Chest Day");
    enqueue("Back Day");
    enqueue("Leg Day");
    dequeue();
    dequeue();

    enqueueSafe("Shoulder Day");
    enqueueSafe("Rest Day");
    printQueue();
    peekQueue();
    dequeue();
    peekQueue();

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

    freeList();

    return 0;
}
