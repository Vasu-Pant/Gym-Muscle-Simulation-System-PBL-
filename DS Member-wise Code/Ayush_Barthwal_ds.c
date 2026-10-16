#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main() {
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

    return 0;
}
