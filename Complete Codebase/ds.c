#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORKOUTS 1000
#define MAX_SESSIONS 200
#define SAVE_FILE "gym_data.txt"
#define TEMP_FILE "gym_data.tmp"
#define BAD_FILE "gym_data.bak"

struct WorkoutRecord {
    char exercise[50];
    float weight;
    int sets;
    int reps;
    int day;
    int session;
};

struct HistoryNode {
    char exercise[50];
    float weight;
    int sets;
    int reps;
    int day;
    int session;
    struct HistoryNode* next;
};

int dsAddWorkout(const char e[], float w, int s, int r, int d);
int dsUndoWorkout(char out[]);
int dsWorkoutCount(void);
struct WorkoutRecord* dsWorkoutAt(int index);
struct HistoryNode* dsHistoryHead(void);
void dsClearWorkouts(void);

int dsSessionEnd(int day, int seconds);
int dsSessionCount(void);
int dsSessionDay(int index);
int dsSessionSeconds(int index);
int dsActiveSession(void);

int dsPlanAdd(const char e[]);
int dsPlanDone(char out[]);
int dsPlanCount(void);
const char* dsPlanAt(int position);

void dsSortDescending(float values[], int order[], int n);
int dsFindName(char names[][30], int n, const char name[]);

struct SavedState {
    char name[32];
    int age;
    float height;
    float weight;
    int period;
    int wState;
    int wDay;
    int wElapsed;
    char wExercise[50];
    int wSets;
    int wTotalSets;
    float wLastWeight;
};

int dsSaveAll(const struct SavedState* st);
int dsLoadAll(struct SavedState* st);
void dsResetAll(void);
void dsDeleteSave(void);


struct WorkoutRecord workoutArray[MAX_WORKOUTS];
int workoutCount = 0;

int sessionDay[MAX_SESSIONS];
int sessionSeconds[MAX_SESSIONS];
int sessionCount = 0;

struct HistoryNode* historyHead = NULL;

char undoStack[MAX_WORKOUTS][50];
int undoTop = -1;

char planQueue[50][50];
int planFront = 0;
int planRear = -1;
int planCount = 0;

void addHistoryNode(const char e[], float w, int s, int r, int d, int session) {
    struct HistoryNode* newNode = (struct HistoryNode*)malloc(sizeof(struct HistoryNode));
    struct HistoryNode* temp;
    strncpy(newNode->exercise, e, 49);
    newNode->exercise[49] = 0;
    newNode->weight = w;
    newNode->sets = s;
    newNode->reps = r;
    newNode->day = d;
    newNode->session = session;
    newNode->next = NULL;
    if (historyHead == NULL) {
        historyHead = newNode;
        return;
    }
    temp = historyHead;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}

void removeLastHistoryNode() {
    struct HistoryNode* temp;
    if (historyHead == NULL) {
        return;
    }
    if (historyHead->next == NULL) {
        free(historyHead);
        historyHead = NULL;
        return;
    }
    temp = historyHead;
    while (temp->next->next != NULL) {
        temp = temp->next;
    }
    free(temp->next);
    temp->next = NULL;
}

void clearHistoryList() {
    struct HistoryNode* temp;
    while (historyHead != NULL) {
        temp = historyHead;
        historyHead = historyHead->next;
        free(temp);
    }
}

int addRecord(const char e[], float w, int s, int r, int d, int session) {
    if (workoutCount >= MAX_WORKOUTS) {
        return 0;
    }
    strncpy(workoutArray[workoutCount].exercise, e, 49);
    workoutArray[workoutCount].exercise[49] = 0;
    workoutArray[workoutCount].weight = w;
    workoutArray[workoutCount].sets = s;
    workoutArray[workoutCount].reps = r;
    workoutArray[workoutCount].day = d;
    workoutArray[workoutCount].session = session;
    workoutCount++;
    addHistoryNode(e, w, s, r, d, session);
    undoTop++;
    strncpy(undoStack[undoTop], e, 49);
    undoStack[undoTop][49] = 0;
    return 1;
}

/* every set is stored as one record that belongs to the workout being done now */
int dsAddWorkout(const char e[], float w, int s, int r, int d) {
    return addRecord(e, w, s, r, d, sessionCount);
}

int dsUndoWorkout(char out[]) {
    int i;
    int session;
    int stillUsed = 0;
    if (undoTop == -1 || workoutCount == 0) {
        return 0;
    }
    strcpy(out, undoStack[undoTop]);
    session = workoutArray[workoutCount - 1].session;
    undoTop--;
    workoutCount--;
    removeLastHistoryNode();
    /* if that was the last set of a finished workout, the workout disappears too */
    if (session < sessionCount) {
        for (i = 0; i < workoutCount; i++) {
            if (workoutArray[i].session == session) {
                stillUsed = 1;
            }
        }
        if (stillUsed == 0 && session == sessionCount - 1) {
            sessionCount--;
        }
    }
    return 1;
}

int dsWorkoutCount(void) {
    return workoutCount;
}

struct WorkoutRecord* dsWorkoutAt(int index) {
    return &workoutArray[index];
}

struct HistoryNode* dsHistoryHead(void) {
    return historyHead;
}

void dsClearWorkouts(void) {
    workoutCount = 0;
    undoTop = -1;
    sessionCount = 0;
    clearHistoryList();
}

int dsSessionEnd(int day, int seconds) {
    if (sessionCount >= MAX_SESSIONS) {
        return -1;
    }
    sessionDay[sessionCount] = day;
    sessionSeconds[sessionCount] = seconds;
    sessionCount++;
    return sessionCount - 1;
}

int dsSessionCount(void) {
    return sessionCount;
}

int dsSessionDay(int index) {
    return sessionDay[index];
}

int dsSessionSeconds(int index) {
    return sessionSeconds[index];
}


int dsActiveSession(void) {
    return sessionCount;
}

int dsPlanAdd(const char e[]) {
    if (planRear == 49) {
        return 0;
    }
    planRear++;
    strncpy(planQueue[planRear], e, 49);
    planQueue[planRear][49] = 0;
    planCount++;
    return 1;
}

int dsPlanDone(char out[]) {
    if (planCount == 0) {
        return 0;
    }
    strcpy(out, planQueue[planFront]);
    planFront++;
    planCount--;
    if (planCount == 0) {
        planFront = 0;
        planRear = -1;
    }
    return 1;
}

int dsPlanCount(void) {
    return planCount;
}

const char* dsPlanAt(int position) {
    return planQueue[planFront + position];
}

void dsSortDescending(float values[], int order[], int n) {
    int i;
    int j;
    int temp;
    for (i = 0; i < n; i++) {
        order[i] = i;
    }
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (values[order[j]] < values[order[j + 1]]) {
                temp = order[j];
                order[j] = order[j + 1];
                order[j + 1] = temp;
            }
        }
    }
}

int dsFindName(char names[][30], int n, const char name[]) {
    int i;
    for (i = 0; i < n; i++) {
        if (strcmp(names[i], name) == 0) {
            return i;
        }
    }
    return -1;
}


static struct WorkoutRecord stageRec[MAX_WORKOUTS];
static int stageSessDay[MAX_SESSIONS];
static int stageSessSec[MAX_SESSIONS];
static char stagePlan[50][50];

void dsResetAll(void) {
    dsClearWorkouts();
    planFront = 0;
    planRear = -1;
    planCount = 0;
}

void dsDeleteSave(void) {
    remove(SAVE_FILE);
    remove(TEMP_FILE);
}

static void cleanText(char out[], const char in[], int size) {
    int i;
    strncpy(out, in, size - 1);
    out[size - 1] = 0;
    for (i = 0; out[i] != 0; i++) {
        if (out[i] == '\r' || out[i] == '\n') {
            out[i] = ' ';
        }
    }
}

static int readLine(FILE* file, char buffer[], int size) {
    if (fgets(buffer, size, file) == NULL) {
        return 0;
    }
    buffer[strcspn(buffer, "\r\n")] = 0;
    return 1;
}


int dsSaveAll(const struct SavedState* st) {
    int i;
    char text[64];
    FILE* file = fopen(TEMP_FILE, "w");
    if (file == NULL) {
        return 0;
    }
    fprintf(file, "GYMSAVE 2\n");
    cleanText(text, st->name, 32);
    fprintf(file, "%s\n", text);
    fprintf(file, "%d %g %g %d\n", st->age, st->height, st->weight, st->period);
    fprintf(file, "%d %d %d %d %d %g\n", st->wState, st->wDay, st->wElapsed, st->wSets, st->wTotalSets, st->wLastWeight);
    cleanText(text, st->wExercise, 50);
    fprintf(file, "%s\n", text);
    fprintf(file, "%d\n", sessionCount);
    for (i = 0; i < sessionCount; i++) {
        fprintf(file, "%d %d\n", sessionDay[i], sessionSeconds[i]);
    }
    fprintf(file, "%d\n", workoutCount);
    for (i = 0; i < workoutCount; i++) {
        cleanText(text, workoutArray[i].exercise, 50);
        fprintf(file, "%s\n%d %g %d %d %d\n", text, workoutArray[i].day, workoutArray[i].weight, workoutArray[i].sets, workoutArray[i].reps, workoutArray[i].session);
    }
    fprintf(file, "%d\n", planCount);
    for (i = 0; i < planCount; i++) {
        cleanText(text, planQueue[planFront + i], 50);
        fprintf(file, "%s\n", text);
    }
    fprintf(file, "END\n");
    if (ferror(file)) {
        fclose(file);
        remove(TEMP_FILE);
        return 0;
    }
    if (fclose(file) != 0) {
        remove(TEMP_FILE);
        return 0;
    }
    remove(SAVE_FILE);
    if (rename(TEMP_FILE, SAVE_FILE) != 0) {
        return 0;
    }
    return 1;
}


static int parseSave(FILE* file, struct SavedState* st, int* nSess, int* nRec, int* nPlan) {
    char line[128];
    char name[128];
    int i;
    int d;
    int s;
    int r;
    int session;
    float w;
    if (!readLine(file, line, 128) || strcmp(line, "GYMSAVE 2") != 0) {
        return 0;
    }
    if (!readLine(file, line, 128)) {
        return 0;
    }
    strncpy(st->name, line, 31);
    st->name[31] = 0;
    if (!readLine(file, line, 128) || sscanf(line, "%d %f %f %d", &st->age, &st->height, &st->weight, &st->period) != 4) {
        return 0;
    }
    if (st->age <= 0 || st->height <= 0 || st->weight <= 0 || st->period < 1 || st->period > 31 || st->name[0] == 0) {
        return 0;
    }
    if (!readLine(file, line, 128) || sscanf(line, "%d %d %d %d %d %f", &st->wState, &st->wDay, &st->wElapsed, &st->wSets, &st->wTotalSets, &st->wLastWeight) != 6) {
        return 0;
    }
    if (st->wState < 0 || st->wState > 3 || st->wElapsed < 0 || st->wSets < 0 || st->wTotalSets < 0) {
        return 0;
    }
    if (st->wState != 0 && (st->wDay < 1 || st->wDay > st->period)) {
        return 0;
    }
    if (!readLine(file, line, 128)) {
        return 0;
    }
    strncpy(st->wExercise, line, 49);
    st->wExercise[49] = 0;
    if (!readLine(file, line, 128) || sscanf(line, "%d", nSess) != 1 || *nSess < 0 || *nSess > MAX_SESSIONS) {
        return 0;
    }
    for (i = 0; i < *nSess; i++) {
        if (!readLine(file, line, 128) || sscanf(line, "%d %d", &d, &s) != 2 || d < 1 || d > 31 || s < 0) {
            return 0;
        }
        stageSessDay[i] = d;
        stageSessSec[i] = s;
    }
    if (!readLine(file, line, 128) || sscanf(line, "%d", nRec) != 1 || *nRec < 0 || *nRec > MAX_WORKOUTS) {
        return 0;
    }
    for (i = 0; i < *nRec; i++) {
        if (!readLine(file, name, 128) || name[0] == 0) {
            return 0;
        }
        if (!readLine(file, line, 128) || sscanf(line, "%d %f %d %d %d", &d, &w, &s, &r, &session) != 5) {
            return 0;
        }
        if (d < 1 || d > 31 || w <= 0 || s <= 0 || r <= 0 || session < 0 || session > *nSess) {
            return 0;
        }
        if (session == *nSess && st->wState == 0) {
            return 0;
        }
        strncpy(stageRec[i].exercise, name, 49);
        stageRec[i].exercise[49] = 0;
        stageRec[i].weight = w;
        stageRec[i].sets = s;
        stageRec[i].reps = r;
        stageRec[i].day = d;
        stageRec[i].session = session;
    }
    if (!readLine(file, line, 128) || sscanf(line, "%d", nPlan) != 1 || *nPlan < 0 || *nPlan > 50) {
        return 0;
    }
    for (i = 0; i < *nPlan; i++) {
        if (!readLine(file, line, 128)) {
            return 0;
        }
        strncpy(stagePlan[i], line, 49);
        stagePlan[i][49] = 0;
    }
    if (!readLine(file, line, 128) || strcmp(line, "END") != 0) {
        return 0;
    }
    return 1;
}


int dsLoadAll(struct SavedState* st) {
    int i;
    int nSess = 0;
    int nRec = 0;
    int nPlan = 0;
    int ok;
    struct SavedState tmp;
    FILE* file = fopen(SAVE_FILE, "r");
    if (file == NULL) {
        return -1;
    }
    memset(&tmp, 0, sizeof(tmp));
    ok = parseSave(file, &tmp, &nSess, &nRec, &nPlan);
    fclose(file);
    if (!ok) {
        remove(BAD_FILE);
        rename(SAVE_FILE, BAD_FILE);
        return -2;
    }
    dsResetAll();
    for (i = 0; i < nSess; i++) {
        dsSessionEnd(stageSessDay[i], stageSessSec[i]);
    }
    for (i = 0; i < nRec; i++) {
        addRecord(stageRec[i].exercise, stageRec[i].weight, stageRec[i].sets, stageRec[i].reps, stageRec[i].day, stageRec[i].session);
    }
    for (i = 0; i < nPlan; i++) {
        dsPlanAdd(stagePlan[i]);
    }
    *st = tmp;
    return sessionCount;
}