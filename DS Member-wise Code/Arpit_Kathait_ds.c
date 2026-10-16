#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main() {
    struct Node* found;

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

    freeList();

    return 0;
}
