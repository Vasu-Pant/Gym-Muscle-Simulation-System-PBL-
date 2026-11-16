#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#endif
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cstdio>
#include <cstdlib>
using namespace std;

extern "C" {
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

int dsSaveAll(const struct SavedState* st);
int dsLoadAll(struct SavedState* st);
void dsResetAll(void);
void dsDeleteSave(void);
}

// Forward declarations: things defined in a later part but used in an earlier one
extern int wState;
extern string refreshTag;
bool saveState();
string plural(int n, string word);
string numberText(float value);
string formatTime(int seconds);
string readFile(string path);
string renderLayout(string page, string content);
string pageSetup();
string pageAdd();
string pageHistory();
string pageProgress(int day, bool play);
string pageReport();
string pagePlan();
string doSetup(string body);
string doWorkoutDay(string body);
string doWorkoutCancel();
string doExerciseStart(string body);
string doSetDone(string body);
string doExerciseDone(string body);
string doExerciseCancel();
string doWorkoutEnd();
string doUndo(string body);
string doPlanAdd(string body);
string doPlanDone();
string doSave();
string doLoad();
string doEndPhase();

#include "part2_arpit_kathait.hpp"
#include "part1_ayush_rathi.hpp"
#include "part3_ayush_barthwal.hpp"
#include "part4_vasu_pant.hpp"
