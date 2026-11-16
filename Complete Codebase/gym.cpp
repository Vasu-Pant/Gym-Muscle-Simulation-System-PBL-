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

string muscleNames[11] = {"Chest", "Back", "Shoulders", "Biceps", "Triceps", "Forearms", "Abs", "Quads", "Hamstrings", "Glutes", "Calves"};

float muscleTargets[11] = {4000, 4500, 3000, 2500, 2500, 1500, 2000, 5000, 3500, 3500, 2500};

struct AnatomyPart {
    int muscle;
    const char* points;
};

AnatomyPart frontParts[16] = {
    {-1, "101,5 91,5 85,14 85,32 91,42 101,44"},
    {-1, "101,46 94,46 92,56 101,58"},
    {1, "91,57 73,62 69,67 91,67"},
    {0, "99,70 70,70 64,81 65,92 78,102 99,99"},
    {2, "73,62 60,65 49,75 46,90 54,97 62,92 63,80 69,69"},
    {3, "46,99 58,103 53,138 42,136 41,114"},
    {5, "42,141 53,141 49,176 41,196 33,190 38,162"},
    {-1, "33,192 41,197 43,208 38,220 29,210"},
    {6, "99,103 81,105 82,118 99,118"},
    {6, "99,121 82,121 83,134 99,134"},
    {6, "99,137 83,137 85,151 99,151"},
    {6, "64,98 77,106 79,120 81,136 82,150 72,148 66,126"},
    {7, "70,153 83,151 88,190 85,229 77,232 68,215 66,180"},
    {7, "99,157 90,153 91,190 88,228 94,230 99,205"},
    {10, "75,238 93,236 91,272 86,300 79,296 73,265"},
    {-1, "79,303 88,303 91,314 75,316"}
};

AnatomyPart backParts[15] = {
    {-1, "101,5 91,5 85,14 85,32 91,42 101,44"},
    {-1, "101,46 94,46 92,56 101,58"},
    {1, "99,58 91,57 73,62 68,70 78,82 99,102"},
    {2, "73,62 60,65 49,75 46,90 54,97 62,92 63,80 67,70"},
    {1, "70,74 80,86 99,106 99,130 87,142 77,130 71,110 68,92"},
    {1, "99,134 88,145 83,153 99,156"},
    {4, "46,99 58,103 53,138 42,136 41,114"},
    {5, "42,141 53,141 49,176 41,196 33,190 38,162"},
    {-1, "33,192 41,197 43,208 38,220 29,210"},
    {9, "99,162 82,157 70,161 66,178 74,197 92,199 99,193"},
    {8, "69,203 84,201 87,230 78,235 68,222"},
    {8, "99,200 89,201 91,229 97,231 99,215"},
    {10, "72,241 84,239 86,263 84,292 78,297 74,268"},
    {10, "89,240 95,239 94,272 89,294 87,265"},
    {-1, "79,300 88,300 90,314 75,316"}
};

class Workout {
private:
    string exercise;
    float weight;
    int sets;
    int reps;
    int day;
public:
    void setData(string e, float w, int s, int r) {
        exercise = e;
        weight = w;
        sets = s;
        reps = r;
        day = 1;
    }
    void setDay(int d) { day = d; }
    int getDay() { return day; }
    string getExercise() { return exercise; }
    float getWeight() { return weight; }
    int getSets() { return sets; }
    int getReps() { return reps; }
    bool isValid() {
        if (weight <= 0) {
            return false;
        }
        if (sets <= 0) {
            return false;
        }
        if (reps <= 0) {
            return false;
        }
        return true;
    }
};

class User {
private:
    string name;
    int age;
    float height;
    float weight;
public:
    void setUser(string n, int a, float h, float w) {
        name = n;
        age = a;
        height = h;
        weight = w;
    }
    string getName() { return name; }
    int getAge() { return age; }
    float getHeight() { return height; }
    float getWeight() { return weight; }
    float calculateBMI() {
        float h = height / 100;
        return weight / (h * h);
    }
};

class Exercise {
protected:
    string name;
    int group;
    int muscleIndex[3];
    int percent[3];
public:
    virtual ~Exercise() {
    }
    void setName(string n) { name = n; }
    string getName() { return name; }
    void setGroup(int g) { group = g; }
    int getGroup() { return group; }
    void setMuscles(int a, int pa, int b, int pb, int c, int pc) {
        muscleIndex[0] = a;
        percent[0] = pa;
        muscleIndex[1] = b;
        percent[1] = pb;
        muscleIndex[2] = c;
        percent[2] = pc;
    }
    int getMuscle(int i) { return muscleIndex[i]; }
    int getPercent(int i) { return percent[i]; }
    virtual float calculateWorkload(float weight, int sets, int reps) {
        return weight * sets * reps;
    }
    virtual string getType() {
        return "General";
    }
};

class CompoundExercise : public Exercise {
public:
    float calculateWorkload(float weight, int sets, int reps) { return weight * sets * reps * 1.1; }
    string getType() { return "Compound"; }
};

class IsolationExercise : public Exercise {
public:
    float calculateWorkload(float weight, int sets, int reps) { return weight * sets * reps * 0.9; }
    string getType() { return "Isolation"; }
};

class ExerciseLibrary {
private:
    Exercise* list[30];
    char nameTable[30][30];
    int count;

    void add(char type, string name, int group, int m1, int p1, int m2, int p2, int m3, int p3) {
        Exercise* e;
        if (type == 'C') {
            e = new CompoundExercise();
        } else {
            e = new IsolationExercise();
        }
        e->setName(name);
        e->setGroup(group);
        e->setMuscles(m1, p1, m2, p2, m3, p3);
        strcpy(nameTable[count], name.c_str());
        list[count] = e;
        count++;
    }
public:
    ExerciseLibrary() {
        count = 0;
    }
    ~ExerciseLibrary() {
        int i;
        for (i = 0; i < count; i++) {
            delete list[i];
        }
    }
    void loadAll() {
        add('C', "Bench Press", 0, 0, 70, 4, 20, 2, 10);
        add('C', "Incline Bench Press", 0, 0, 60, 2, 25, 4, 15);
        add('I', "Chest Fly", 0, 0, 100, 0, 0, 0, 0);
        add('C', "Deadlift", 1, 1, 60, 8, 25, 9, 15);
        add('C', "Lat Pulldown", 1, 1, 80, 3, 20, 0, 0);
        add('C', "Barbell Row", 1, 1, 70, 3, 20, 5, 10);
        add('C', "Shoulder Press", 2, 2, 70, 4, 30, 2, 0);
        add('I', "Lateral Raise", 2, 2, 100, 0, 0, 0, 0);
        add('I', "Bicep Curl", 3, 3, 90, 5, 10, 0, 0);
        add('I', "Hammer Curl", 3, 3, 70, 5, 30, 0, 0);
        add('I', "Tricep Pushdown", 4, 4, 100, 0, 0, 0, 0);
        add('I', "Skull Crusher", 4, 4, 100, 0, 0, 0, 0);
        add('I', "Wrist Curl", 5, 5, 100, 0, 0, 0, 0);
        add('I', "Crunch", 6, 6, 100, 0, 0, 0, 0);
        add('I', "Leg Raise", 6, 6, 100, 0, 0, 0, 0);
        add('C', "Squat", 7, 7, 50, 9, 30, 8, 20);
        add('C', "Leg Press", 7, 7, 60, 9, 25, 8, 15);
        add('I', "Leg Extension", 7, 7, 100, 0, 0, 0, 0);
        add('I', "Leg Curl", 8, 8, 100, 0, 0, 0, 0);
        add('C', "Romanian Deadlift", 8, 8, 60, 9, 30, 1, 10);
        add('C', "Hip Thrust", 9, 9, 80, 8, 20, 0, 0);
        add('I', "Calf Raise", 10, 10, 100, 0, 0, 0, 0);
        add('I', "Seated Calf Raise", 10, 10, 100, 0, 0, 0, 0);
    }
    int getCount() { return count; }
    Exercise* getExercise(int i) { return list[i]; }
    int findExercise(string name) { return dsFindName(nameTable, count, name.c_str()); }
};

class Muscle {
private:
    string name;
    float totalVolume;
    float target;
    float dailyVolume[32];
public:
    void setMuscle(string n) {
        int i;
        name = n;
        totalVolume = 0;
        target = 4000;
        for (i = 0; i < 32; i++) {
            dailyVolume[i] = 0;
        }
    }
    void setTarget(float t) { target = t; }
    void addVolumeOnDay(int day, float v) {
        dailyVolume[day] = dailyVolume[day] + v;
        totalVolume = totalVolume + v;
    }
    float getVolume() { return totalVolume; }
    int countTrainedDays(int period) {
        int d;
        int days = 0;
        for (d = 1; d <= period; d++) {
            if (dailyVolume[d] > 0) {
                days = days + 1;
            }
        }
        return days;
    }
    float calculateShade(int day, int period) {
        float load = 0;
        int d;
        int k;
        for (d = 1; d <= day; d++) {
            float v = dailyVolume[d];
            for (k = d; k < day; k++) {
                v = v * 0.85;
            }
            load = load + v;
        }
        float shade = (load / (target * period / 7)) * 100;
        if (shade > 100) {
            shade = 100;
        }
        return shade;
    }
    float calculateAverageShade(int period) {
        float total = 0;
        int d;
        for (d = 1; d <= period; d++) {
            total = total + calculateShade(d, period);
        }
        return total / period;
    }
};

class BodySimulator {
public:
    string getColorName(float shade) {
        if (shade <= 0) {
            return "Grey";
        }
        if (shade <= 20) {
            return "Very Light Blue";
        }
        if (shade <= 40) {
            return "Light Blue";
        }
        if (shade <= 60) {
            return "Blue";
        }
        if (shade <= 80) {
            return "Dark Blue";
        }
        if (shade < 100) {
            return "Navy";
        }
        return "Black";
    }

    string getColor(float shade) {
        if (shade <= 0) {
            return "rgb(184,184,184)";
        }
        float t = shade / 100;
        float f;
        int r;
        int g;
        int b;
        if (t <= 0.5) {
            f = t / 0.5;
            r = 190 + (31 - 190) * f;
            g = 214 + (95 - 214) * f;
            b = 255 + (224 - 255) * f;
        } else {
            f = (t - 0.5) / 0.5;
            r = 31 + (0 - 31) * f;
            g = 95 + (0 - 95) * f;
            b = 224 + (0 - 224) * f;
        }
        return "rgb(" + to_string(r) + "," + to_string(g) + "," + to_string(b) + ")";
    }

    string getAnatomyColor(float shade) {
        float t = shade / 100;
        if (t > 1) {
            t = 1;
        }
        int r = 58 + (91 - 58) * t;
        int g = 74 + (141 - 74) * t;
        int b = 159 + (255 - 159) * t;
        return "rgb(" + to_string(r) + "," + to_string(g) + "," + to_string(b) + ")";
    }

    string drawAnatomyView(float shades[], AnatomyPart parts[], int amount, string title) {
        int i;
        string half = "";
        for (i = 0; i < amount; i++) {
            int m = parts[i].muscle;
            string points = parts[i].points;
            if (m < 0) {
                half = half + "<polygon class=\"n\" points=\"" + points + "\"/>";
            } else {
                string style = "";
                if (shades[m] > 0) {
                    style = " style=\"fill:" + getAnatomyColor(shades[m]) + "\"";
                }
                half = half + "<polygon class=\"m\" points=\"" + points + "\"" + style + "><title>" + muscleNames[m] + ": " + to_string((int)shades[m]) + "%</title></polygon>";
            }
        }
        string svg = "<svg viewBox=\"0 0 200 330\"><g>" + half + "</g><g transform=\"translate(200,0) scale(-1,1)\">" + half + "</g></svg>";
        return "<figure>" + svg + "<figcaption>" + title + "</figcaption></figure>";
    }

    string drawAnatomy(float shades[]) { return drawAnatomyView(shades, frontParts, 16, "Front") + drawAnatomyView(shades, backParts, 15, "Back"); }
};

class WorkoutAnalyzer {
public:
    int calculateScore(float volume, float reference) {
        int score = (volume / reference) * 100;
        if (score > 100) {
            score = 100;
        }
        return score;
    }
    float calculateMuscleVolume(float volume, float percent) { return volume * percent / 100; }
    void processWorkouts(ExerciseLibrary& lib, Muscle muscles[]) {
        int i;
        int j;
        for (i = 0; i < 11; i++) {
            muscles[i].setMuscle(muscleNames[i]);
            muscles[i].setTarget(muscleTargets[i]);
        }
        for (i = 0; i < dsWorkoutCount(); i++) {
            WorkoutRecord* rec = dsWorkoutAt(i);
            Workout w;
            w.setData(rec->exercise, rec->weight, rec->sets, rec->reps);
            w.setDay(rec->day);
            int index = lib.findExercise(w.getExercise());
            if (index != -1) {
                Exercise* e = lib.getExercise(index);
                float workload = e->calculateWorkload(w.getWeight(), w.getSets(), w.getReps());
                for (j = 0; j < 3; j++) {
                    float part = calculateMuscleVolume(workload, e->getPercent(j));
                    if (part > 0) {
                        muscles[e->getMuscle(j)].addVolumeOnDay(w.getDay(), part);
                    }
                }
            }
        }
    }
};

string numberText(float value) {
    char buffer[32];
    snprintf(buffer, 32, "%g", value);
    return string(buffer);
}

string oneDecimal(float value) {
    char buffer[32];
    snprintf(buffer, 32, "%.1f", value);
    return string(buffer);
}

class ReportGenerator {
public:
    float totalVolume() {
        int i;
        float total = 0;
        for (i = 0; i < dsWorkoutCount(); i++) {
            WorkoutRecord* rec = dsWorkoutAt(i);
            total = total + rec->weight * rec->sets * rec->reps;
        }
        return total;
    }

    int totalMinutes() {
        int i;
        int seconds = 0;
        for (i = 0; i < dsSessionCount(); i++) {
            seconds = seconds + dsSessionSeconds(i);
        }
        int minutes = (seconds + 30) / 60;
        if (minutes == 0 && seconds > 0) {
            minutes = 1;
        }
        return minutes;
    }

    string formatVolume(float volume) {
        char buffer[32];
        if (volume >= 1000) {
            snprintf(buffer, 32, "%.2fK", volume / 1000);
        } else {
            snprintf(buffer, 32, "%.0f", volume);
        }
        return string(buffer);
    }

    string generateWeeklyChart(int period) {
        int counts[5] = {0, 0, 0, 0, 0};
        int i;
        int weeks = (period + 6) / 7;

        for (i = 0; i < dsSessionCount(); i++) {
            int w = (dsSessionDay(i) - 1) / 7;
            if (w < 5) {
                counts[w] = counts[w] + 1;
                if (w + 1 > weeks) {
                    weeks = w + 1;
                }
            }
        }

        int maxCount = 0;
        for (i = 0; i < weeks; i++) {
            if (counts[i] > maxCount) {
                maxCount = counts[i];
            }
        }
        int step = 1;
        if (maxCount > 4) {
            step = (maxCount + 3) / 4;
        }
        int top = ((maxCount + step - 1) / step) * step;
        if (top < 2) {
            top = 2;
        }

        string html = "<div class=\"chartbox\"><div class=\"wk\"><div class=\"wk-grid\">";
        for (i = 0; i <= top; i = i + step) {
            html = html + "<div class=\"wk-line\" style=\"bottom:" + to_string(i * 100 / top) + "%\"><span>" + to_string(i) + "</span></div>";
        }

        html = html + "</div><div class=\"wk-bars\">";
        for (i = 0; i < weeks; i++) {
            string value = "";
            if (counts[i] > 0) {
                value = "<em>" + to_string(counts[i]) + "</em>";
            }
            html = html + "<div class=\"wk-col\"><div class=\"wk-bar\" style=\"height:" + to_string(counts[i] * 100 / top) + "%\">" + value + "</div></div>";
        }

        html = html + "</div></div><div class=\"wk-labels\">";
        for (i = 0; i < weeks; i++) {
            int last = (i + 1) * 7;
            if (last > period) {
                last = period;
            }
            html = html + "<div><b>Week " + to_string(i + 1) + "</b><small>Day " + to_string(i * 7 + 1) + "-" + to_string(last) + "</small></div>";
        }
        html = html + "</div></div>";
        return html;
    }

    string generateTable(Muscle m[], float avg[], int scores[], int period, BodySimulator& sim) {
        int i;
        float volumes[11];
        int order[11];
        string rows = "";
        for (i = 0; i < 11; i++) {
            volumes[i] = m[i].getVolume();
        }
        dsSortDescending(volumes, order, 11);
        for (i = 0; i < 11; i++) {
            int k = order[i];
            string status = "<span class=\"ok\">OK</span>";
            if (scores[k] < 20) {
                status = "<span class=\"warn\">Undertrained</span>";
            }
            rows = rows + "<tr>";
            rows = rows + "<td>" + to_string(i + 1) + "</td>";
            rows = rows + "<td>" + muscleNames[k] + "</td>";
            rows = rows + "<td>" + to_string((int)volumes[k]) + "</td>";
            rows = rows + "<td>" + to_string(m[k].countTrainedDays(period)) + "</td>";
            rows = rows + "<td><span class=\"swatch\" style=\"background:" + sim.getColor(avg[k]) + "\"></span>" + to_string((int)avg[k]) + "% " + sim.getColorName(avg[k]) + "</td>";
            rows = rows + "<td>" + to_string(scores[k]) + "%</td>";
            rows = rows + "<td>" + status + "</td>";
            rows = rows + "</tr>";
        }
        return rows;
    }

    string generateBalance(int scores[]) {
        int pairA[3] = {0, 7, 3};
        int pairB[3] = {1, 8, 4};
        int i;
        string out = "";
        for (i = 0; i < 3; i++) {
            int difference = scores[pairA[i]] - scores[pairB[i]];
            if (difference < 0) {
                difference = -difference;
            }
            if (difference > 30) {
                out = out + "<p class=\"warn\">Warning: " + muscleNames[pairA[i]] + " and " + muscleNames[pairB[i]] + " training imbalance detected</p>";
            } else {
                out = out + "<p class=\"ok\">" + muscleNames[pairA[i]] + " and " + muscleNames[pairB[i]] + " are balanced</p>";
            }
        }
        return out;
    }
};

#ifdef _WIN32
typedef SOCKET SocketType;
#else
typedef int SocketType;
#define closesocket close
#endif

User user;
ExerciseLibrary lib;
Muscle muscles[11];
BodySimulator sim;
WorkoutAnalyzer analyzer;
ReportGenerator report;
int period = 7;
string flash = "";
string refreshTag = "";

int wState = 0;
int wDay = 0;
time_t wStart = 0;
string wExercise = "";
int wSets = 0;
int wTotalSets = 0;
float wLastWeight = 20;

string urlDecode(string s) {
    string out = "";
    int i;
    for (i = 0; i < s.size(); i++) {
        if (s[i] == '+') {
            out = out + ' ';
        } else if (s[i] == '%' && i + 2 < s.size()) {
            int value = strtol(s.substr(i + 1, 2).c_str(), NULL, 16);
            out = out + (char)value;
            i = i + 2;
        } else {
            out = out + s[i];
        }
    }
    return out;
}

string getParam(string data, string key) {
    data = "&" + data;
    int start = data.find("&" + key + "=");
    if (start == -1) {
        return "";
    }
    start = start + key.size() + 2;
    int end = data.find("&", start);
    if (end == -1) {
        end = data.size();
    }
    return urlDecode(data.substr(start, end - start));
}

string htmlEscape(string s) {
    string out = "";
    int i;
    for (i = 0; i < s.size(); i++) {
        if (s[i] == '&') {
            out = out + "&amp;";
        } else if (s[i] == '<') {
            out = out + "&lt;";
        } else if (s[i] == '>') {
            out = out + "&gt;";
        } else if (s[i] == '"') {
            out = out + "&quot;";
        } else {
            out = out + s[i];
        }
    }
    return out;
}

string plural(int n, string word) {
    if (n == 1) {
        return "1 " + word;
    }
    return to_string(n) + " " + word + "s";
}

string formatTime(int seconds) {
    char buffer[32];
    snprintf(buffer, 32, "%d:%02d", seconds / 60, seconds % 60);
    return string(buffer);
}

string hiddenUnless(bool show) {
    if (show) {
        return "";
    }
    return "hidden";
}

string readFile(string path) {
    string content = "";
    string line;
    ifstream file(path.c_str());
    while (getline(file, line)) {
        content = content + line + "\n";
    }
    return content;
}

string readPage(string name) {
    string all = readFile("web/index.html");
    if (name == "layout") {
        return all.substr(0, all.find("<!--PAGE:"));
    }
    string start = "<!--PAGE:" + name + "-->";
    int a = all.find(start);
    if (a == -1) {
        return "";
    }
    a = a + start.size();
    int b = all.find("<!--/PAGE-->", a);
    return all.substr(a, b - a);
}

string fill(string text, string key, string value) {
    string tag = "{{" + key + "}}";
    int pos = text.find(tag);
    while (pos != -1) {
        text.replace(pos, tag.size(), value);
        pos = text.find(tag, pos + value.size());
    }
    return text;
}

string takeFlash() {
    if (flash == "") {
        return "";
    }
    string html = "<p class=\"flash\">" + htmlEscape(flash) + "</p>";
    flash = "";
    return html;
}

string renderLayout(string page, string content) {
    string html = readPage("layout");
    html = fill(html, "PAGE", page);
    html = fill(html, "REFRESH", refreshTag);
    html = fill(html, "FLASH", takeFlash());
    html = fill(html, "CONTENT", content);
    return html;
}

int elapsedSeconds() {
    if (wState < 2) {
        return 0;
    }
    return time(NULL) - wStart;
}

float sessionVolume(int session) {
    int i;
    float total = 0;
    for (i = 0; i < dsWorkoutCount(); i++) {
        WorkoutRecord* rec = dsWorkoutAt(i);
        if (rec->session == session) {
            total = total + rec->weight * rec->sets * rec->reps;
        }
    }
    return total;
}

bool dayHasWorkout(int day) {
    int i;
    for (i = 0; i < dsSessionCount(); i++) {
        if (dsSessionDay(i) == day) {
            return true;
        }
    }
    return false;
}

string exerciseOptions() {
    int g;
    int i;
    string options = "";
    for (g = 0; g < 11; g++) {
        options = options + "<optgroup label=\"" + muscleNames[g] + "\">";
        for (i = 0; i < lib.getCount(); i++) {
            Exercise* e = lib.getExercise(i);
            if (e->getGroup() == g) {
                options = options + "<option value=\"" + e->getName() + "\">" + e->getName() + " (" + e->getType() + ")</option>";
            }
        }
        options = options + "</optgroup>";
    }
    return options;
}

string exerciseGroups(int session, bool showSets) {
    string names[30];
    float volumes[30];
    int counts[30];
    int n = 0;
    int i;
    int k;
    HistoryNode* temp = dsHistoryHead();

    while (temp != NULL) {
        if (temp->session == session) {
            k = -1;
            for (i = 0; i < n; i++) {
                if (names[i] == temp->exercise) {
                    k = i;
                }
            }
            if (k == -1 && n < 30) {
                names[n] = temp->exercise;
                volumes[n] = 0;
                counts[n] = 0;
                k = n;
                n++;
            }
            if (k != -1) {
                volumes[k] = volumes[k] + temp->weight * temp->sets * temp->reps;
                counts[k] = counts[k] + 1;
            }
        }
        temp = temp->next;
    }

    string html = "";
    for (i = 0; i < n; i++) {
        string word = " sets";
        if (counts[i] == 1) {
            word = " set";
        }
        html = html + "<li><b>" + htmlEscape(names[i]) + "</b> <span class=\"vol\">" + to_string(counts[i]) + word + ", volume " + numberText(volumes[i]) + " kg</span>";
        if (showSets) {
            html = html + "<ol class=\"sets\">";
            temp = dsHistoryHead();
            while (temp != NULL) {
                if (temp->session == session && names[i] == temp->exercise) {
                    html = html + "<li>" + numberText(temp->weight) + " kg x " + to_string(temp->reps) + " reps</li>";
                }
                temp = temp->next;
            }
            html = html + "</ol>";
        }
        html = html + "</li>";
    }
    return html;
}

string saveSet(string weightText, string repsText) {
    float weight = atof(weightText.c_str());
    int reps = atoi(repsText.c_str());
    Workout x;
    x.setData(wExercise, weight, 1, reps);
    x.setDay(wDay);
    if (x.isValid() == false) {
        return "Weight and reps must be positive";
    }
    if (weight > 1000 || reps > 100) {
        return "Weight up to 1000 kg and reps up to 100 please";
    }
    if (dsAddWorkout(wExercise.c_str(), x.getWeight(), 1, x.getReps(), wDay) == 0) {
        return "Workout list is full";
    }
    wSets++;
    wTotalSets++;
    wLastWeight = weight;
    return "";
}

bool saveState() {
    SavedState st;
    memset(&st, 0, sizeof(st));
    strncpy(st.name, user.getName().c_str(), 31);
    st.age = user.getAge();
    st.height = user.getHeight();
    st.weight = user.getWeight();
    st.period = period;
    st.wState = wState;
    st.wDay = wDay;
    st.wElapsed = elapsedSeconds();
    strncpy(st.wExercise, wExercise.c_str(), 49);
    st.wSets = wSets;
    st.wTotalSets = wTotalSets;
    st.wLastWeight = wLastWeight;
    if (dsSaveAll(&st) == 1) {
        return true;
    }
    return false;
}

void applyState(SavedState& st) {
    int i;
    int total = 0;
    int active = dsActiveSession();
    user.setUser(st.name, st.age, st.height, st.weight);
    period = st.period;
    wState = st.wState;
    wDay = st.wDay;
    wExercise = st.wExercise;
    wSets = st.wSets;
    wTotalSets = st.wTotalSets;
    wLastWeight = st.wLastWeight;

    wStart = time(NULL) - st.wElapsed;

    for (i = 0; i < dsWorkoutCount(); i++) {
        if (dsWorkoutAt(i)->session == active) {
            total++;
        }
    }

    if (wState == 0) {
        wDay = 0;
        wSets = 0;
        wTotalSets = 0;
        return;
    }

    wTotalSets = total;
    if (wSets > wTotalSets) {
        wSets = wTotalSets;
    }
    if (wState == 2 && lib.findExercise(wExercise) == -1) {
        wState = 3;
    }
    if (wState == 3 && wTotalSets == 0) {
        wState = 1;
    }
    if (wState == 1 && wTotalSets > 0) {
        wState = 3;
    }
    if (wState != 2) {
        wSets = 0;
    }
}

int loadState() {
    SavedState st;
    memset(&st, 0, sizeof(st));
    int result = dsLoadAll(&st);
    if (result >= 0) {
        applyState(st);
    }
    return result;
}

string pageSetup() {
    string html = readPage("setup");
    html = fill(html, "NAME", htmlEscape(user.getName()));
    html = fill(html, "AGE", to_string(user.getAge()));
    html = fill(html, "HEIGHT", numberText(user.getHeight()));
    html = fill(html, "WEIGHT", numberText(user.getWeight()));
    html = fill(html, "PERIOD", to_string(period));
    html = fill(html, "COUNT", to_string(dsSessionCount()));
    html = fill(html, "BMI", oneDecimal(user.calculateBMI()));
    return html;
}

string addDayStep() {
    int d;
    string buttons = "";
    for (d = 1; d <= period; d++) {
        string cls = "day";
        if (dayHasWorkout(d)) {
            cls = cls + " has";
        }
        buttons = buttons + "<button class=\"" + cls + "\" type=\"submit\" name=\"day\" value=\"" + to_string(d) + "\">" + to_string(d) + "</button>";
    }
    string step = fill(readPage("add_day"), "DAYBUTTONS", buttons);
    step = fill(step, "PERIOD", to_string(period));
    return step;
}

string addSetStep() {
    int i;
    string sets = "";
    for (i = 0; i < wSets; i++) {
        WorkoutRecord* rec = dsWorkoutAt(dsWorkoutCount() - wSets + i);
        sets = sets + "<li>" + numberText(rec->weight) + " kg x " + to_string(rec->reps) + " reps</li>";
    }
    if (sets == "") {
        sets = "<li class=\"rest\">No sets yet</li>";
    }
    string html = readPage("add_set");
    html = fill(html, "DAY", to_string(wDay));
    html = fill(html, "ELAPSED", to_string(elapsedSeconds()));
    html = fill(html, "EXERCISE", htmlEscape(wExercise));
    html = fill(html, "TYPE", lib.getExercise(lib.findExercise(wExercise))->getType());
    html = fill(html, "SETS", sets);
    html = fill(html, "SETNO", to_string(wSets + 1));
    html = fill(html, "WEIGHT", numberText(wLastWeight));
    html = fill(html, "SHOW_CANCELEX", hiddenUnless(wSets == 0));
    html = fill(html, "SHOW_UNDO", hiddenUnless(wTotalSets > 0));
    return html;
}

string addPickStep() {
    string html = readPage("add_pick");
    html = fill(html, "DAY", to_string(wDay));
    html = fill(html, "ELAPSED", to_string(elapsedSeconds()));
    html = fill(html, "OPTIONS", exerciseOptions());
    html = fill(html, "DONE", exerciseGroups(dsActiveSession(), false));
    html = fill(html, "SHOW_TIMER", hiddenUnless(wState == 3));
    html = fill(html, "SHOW_HINT", hiddenUnless(wState == 1));
    html = fill(html, "SHOW_DONE", hiddenUnless(wState == 3));
    html = fill(html, "SHOW_END", hiddenUnless(wState == 3));
    html = fill(html, "SHOW_CANCEL", hiddenUnless(wState == 1));
    html = fill(html, "SHOW_UNDO", hiddenUnless(wState == 3 && wTotalSets > 0));
    return html;
}

string pageAdd() {
    int i;
    string step = "";
    string recent = "";

    if (wState == 0) {
        step = addDayStep();
    } else if (wState == 2) {
        step = addSetStep();
    } else {
        step = addPickStep();
    }

    int shown = 0;
    for (i = dsWorkoutCount() - 1; i >= 0 && shown < 5; i--) {
        WorkoutRecord* rec = dsWorkoutAt(i);
        recent = recent + "<li>Day " + to_string(rec->day) + ": <b>" + htmlEscape(rec->exercise) + "</b> " + numberText(rec->weight) + " kg x " + to_string(rec->reps) + " reps</li>";
        shown++;
    }
    if (recent == "") {
        recent = "<li class=\"rest\">No sets yet</li>";
    }

    string html = readPage("add");
    html = fill(html, "STEP", step);
    html = fill(html, "RECENT", recent);
    return html;
}

string pageHistory() {
    int d;
    int i;
    string days = "";
    for (d = 1; d <= period; d++) {
        string items = "";
        int number = 0;
        for (i = 0; i < dsSessionCount(); i++) {
            if (dsSessionDay(i) == d) {
                number++;
                items = items + "<div class=\"session\"><div class=\"sess-head\">";
                items = items + "<b>Workout " + to_string(number) + "</b>";
                items = items + "<span>" + formatTime(dsSessionSeconds(i)) + " total time</span>";
                items = items + "<span>" + numberText(sessionVolume(i)) + " kg total volume</span>";
                items = items + "</div><ul>" + exerciseGroups(i, true) + "</ul></div>";
            }
        }
        if (items == "") {
            items = "<ul><li class=\"rest\">Rest day</li></ul>";
        }
        days = days + "<details class=\"daycard\" open><summary>Day " + to_string(d) + "</summary>" + items + "</details>";
    }
    string html = readPage("history");
    html = fill(html, "SESSIONS", plural(dsSessionCount(), "workout"));
    html = fill(html, "COUNT", plural(dsWorkoutCount(), "set"));
    html = fill(html, "PERIOD", to_string(period));
    html = fill(html, "DAYS", days);
    return html;
}

string pageProgress(int day, bool play) {
    int i;
    int d;
    float shades[11];
    string rows = "";
    string links = "";
    string status = "Click a day to see the muscles on that day.";
    if (day < 1 || day > period) {
        day = 1;
    }
    analyzer.processWorkouts(lib, muscles);

    for (i = 0; i < 11; i++) {
        shades[i] = muscles[i].calculateShade(day, period);
        rows = rows + "<tr><td>" + muscleNames[i] + "</td>";
        rows = rows + "<td><div class=\"meter\"><span style=\"width:" + to_string((int)shades[i]) + "%;background:" + sim.getColor(shades[i]) + "\"></span></div></td>";
        rows = rows + "<td>" + to_string((int)shades[i]) + "%</td>";
        rows = rows + "<td>" + sim.getColorName(shades[i]) + "</td></tr>";
    }

    for (d = 1; d <= period; d++) {
        string active = "";
        if (d == day) {
            active = " active";
        }
        links = links + "<a class=\"day" + active + "\" href=\"/progress?day=" + to_string(d) + "\">" + to_string(d) + "</a>";
    }

    if (play == true && day < period) {
        refreshTag = "<meta http-equiv=\"refresh\" content=\"1;url=/progress?day=" + to_string(day + 1) + "&amp;play=1\">";
        status = "Playing... day " + to_string(day) + " of " + to_string(period);
    } else if (play == true) {
        status = "Finished. The period is over.";
    }

    string html = readPage("progress");
    html = fill(html, "DAY", to_string(day));
    html = fill(html, "PERIOD", to_string(period));
    html = fill(html, "DAYLINKS", links);
    html = fill(html, "STATUS", status);
    html = fill(html, "BODIES", sim.drawAnatomy(shades));
    html = fill(html, "ROWS", rows);
    return html;
}

string pageReport() {
    int i;
    float avg[11];
    int scores[11];
    analyzer.processWorkouts(lib, muscles);
    for (i = 0; i < 11; i++) {
        avg[i] = muscles[i].calculateAverageShade(period);
        scores[i] = analyzer.calculateScore(muscles[i].getVolume(), muscleTargets[i] * period / 7);
    }
    string html = readPage("report");
    html = fill(html, "PERIOD", to_string(period));
    html = fill(html, "WORKOUTS", to_string(dsSessionCount()));
    html = fill(html, "MINUTES", to_string(report.totalMinutes()));
    html = fill(html, "VOLUME", report.formatVolume(report.totalVolume()));
    html = fill(html, "WEEKCHART", report.generateWeeklyChart(period));
    html = fill(html, "BODIES", sim.drawAnatomy(avg));
    html = fill(html, "TABLE", report.generateTable(muscles, avg, scores, period, sim));
    html = fill(html, "BALANCE", report.generateBalance(scores));
    return html;
}

string pagePlan() {
    int i;
    string plans = "";
    for (i = 0; i < dsPlanCount(); i++) {
        plans = plans + "<li>" + htmlEscape(dsPlanAt(i)) + "</li>";
    }
    if (plans == "") {
        plans = "<li class=\"rest\">Nothing scheduled</li>";
    }
    string html = readPage("plan");
    html = fill(html, "PLANS", plans);
    return html;
}

string doSetup(string body) {
    int newPeriod = atoi(getParam(body, "period").c_str());
    int age = atoi(getParam(body, "age").c_str());
    float height = atof(getParam(body, "height").c_str());
    float weight = atof(getParam(body, "weight").c_str());
    string name = getParam(body, "name");
    if (newPeriod < 1 || newPeriod > 31 || age <= 0 || height <= 0 || weight <= 0 || name == "") {
        flash = "Please fill every field with valid values (period 1 to 31 days)";
        return "/";
    }
    user.setUser(name, age, height, weight);
    period = newPeriod;
    dsClearWorkouts();
    wState = 0;
    wDay = 0;
    wSets = 0;
    wTotalSets = 0;
    flash = "New period of " + to_string(period) + " days started for " + name;
    return "/add";
}

string doWorkoutDay(string body) {
    int day = atoi(getParam(body, "day").c_str());
    if (wState != 0) {
        flash = "Finish the current workout first";
    } else if (day < 1 || day > period) {
        flash = "Day must be between 1 and " + to_string(period);
    } else {
        wDay = day;
        wState = 1;
        wSets = 0;
        wTotalSets = 0;
        flash = "Day " + to_string(day) + " selected. Now choose an exercise";
    }
    return "/add";
}

string doWorkoutCancel() {
    if (wState == 1) {
        wState = 0;
        wDay = 0;
        flash = "Day cleared. Choose a day";
    } else {
        flash = "Use End Workout to finish a workout";
    }
    return "/add";
}

string doExerciseStart(string body) {
    string exerciseName = getParam(body, "exercise");
    if (wState != 1 && wState != 3) {
        flash = "Finish the current exercise first";
    } else if (lib.findExercise(exerciseName) == -1) {
        flash = "Please choose an exercise";
    } else {
        if (wState == 1) {
            wStart = time(NULL);
        }
        wExercise = exerciseName;
        wSets = 0;
        wLastWeight = 20;
        wState = 2;
        flash = "Started " + exerciseName + ". Fill in your first set";
    }
    return "/add";
}

string doSetDone(string body) {
    if (wState != 2) {
        flash = "Choose an exercise first";
    } else {
        string error = saveSet(getParam(body, "weight"), getParam(body, "reps"));
        if (error == "") {
            flash = "Set " + to_string(wSets) + " saved: " + numberText(wLastWeight) + " kg x " + to_string(atoi(getParam(body, "reps").c_str()));
        } else {
            flash = error;
        }
    }
    return "/add";
}

string doExerciseDone(string body) {
    if (wState != 2) {
        flash = "Choose an exercise first";
        return "/add";
    }
    string error = "";
    if (atoi(getParam(body, "reps").c_str()) > 0) {
        error = saveSet(getParam(body, "weight"), getParam(body, "reps"));
    }
    if (error != "") {
        flash = error;
    } else if (wSets == 0) {
        flash = "Complete at least one set, or pick a different exercise";
    } else {
        flash = "Finished " + wExercise + " (" + to_string(wSets) + " sets). Pick the next exercise or end the workout";
        wState = 3;
    }
    return "/add";
}

string doExerciseCancel() {
    if (wState == 2 && wSets == 0) {
        if (wTotalSets > 0) {
            wState = 3;
        } else {
            wState = 1;
        }
        flash = "Pick a different exercise";
    }
    return "/add";
}

string doWorkoutEnd() {
    if (wState != 3) {
        flash = "Finish the current exercise first";
        return "/add";
    }
    if (wTotalSets == 0) {
        wState = 0;
        wDay = 0;
        flash = "No sets were recorded, so the workout was discarded";
        return "/add";
    }
    int seconds = elapsedSeconds();
    if (seconds < 1) {
        seconds = 1;
    }
    float volume = sessionVolume(dsActiveSession());
    if (dsSessionEnd(wDay, seconds) < 0) {
        flash = "Workout history is full";
        return "/add";
    }
    flash = "Workout saved for day " + to_string(wDay) + ": " + formatTime(seconds) + " total time, " + numberText(volume) + " kg volume";
    wState = 0;
    wDay = 0;
    wSets = 0;
    wTotalSets = 0;
    return "/history";
}

string doUndo(string body) {
    char last[50];
    bool fromHistory = false;
    if (getParam(body, "back") == "history") {
        fromHistory = true;
    }
    if (wState != 0 && fromHistory) {
        flash = "Finish or cancel the current workout before undoing from history";
        return "/history";
    }
    if (wState != 0 && wTotalSets == 0) {
        flash = "Nothing to undo in this workout yet";
    } else if (dsUndoWorkout(last) == 0) {
        flash = "Nothing to undo";
    } else {
        flash = "Removed last set of " + string(last);
        if (wState != 0) {
            wTotalSets--;
            if (wState == 2 && wSets > 0) {
                wSets--;
            }
        }
    }
    if (fromHistory) {
        return "/history";
    }
    return "/add";
}

string doPlanAdd(string body) {
    string plan = getParam(body, "plan");
    if (plan == "") {
        flash = "Enter a plan name";
    } else if (dsPlanAdd(plan.c_str()) == 0) {
        flash = "Schedule is full";
    } else {
        flash = "Scheduled " + plan;
    }
    return "/plan";
}

string doPlanDone() {
    char done[50];
    if (dsPlanDone(done) == 0) {
        flash = "Nothing scheduled";
    } else {
        flash = "Completed " + string(done);
    }
    return "/plan";
}

string doSave() {
    if (saveState()) {
        flash = "Saved " + plural(dsSessionCount(), "workout") + " to gym_data.txt";
    } else {
        flash = "Could not save the file";
    }
    return "/";
}

string doLoad() {
    if (wState != 0) {
        flash = "End the current workout before loading";
        return "/";
    }
    int loaded = loadState();
    if (loaded == -1) {
        flash = "No saved file found";
    } else if (loaded == -2) {
        flash = "The saved file is damaged or from an older version (kept as gym_data.bak)";
    } else {
        flash = "Loaded " + plural(loaded, "workout");
    }
    return "/history";
}

string doEndPhase() {
    dsResetAll();
    dsDeleteSave();
    user.setUser("Guest", 20, 170, 65);
    period = 7;
    wState = 0;
    wDay = 0;
    wStart = 0;
    wExercise = "";
    wSets = 0;
    wTotalSets = 0;
    wLastWeight = 20;
    flash = "Phase ended. All workouts, the schedule and your details were reset";
    return "/";
}

string handlePost(string route, string body) {
    if (route == "/setup") {
        return doSetup(body);
    }
    if (route == "/workout_day") {
        return doWorkoutDay(body);
    }
    if (route == "/workout_cancel") {
        return doWorkoutCancel();
    }
    if (route == "/exercise_start") {
        return doExerciseStart(body);
    }
    if (route == "/set_done") {
        return doSetDone(body);
    }
    if (route == "/exercise_done") {
        return doExerciseDone(body);
    }
    if (route == "/exercise_cancel") {
        return doExerciseCancel();
    }
    if (route == "/workout_end") {
        return doWorkoutEnd();
    }
    if (route == "/undo") {
        return doUndo(body);
    }
    if (route == "/plan_add") {
        return doPlanAdd(body);
    }
    if (route == "/plan_done") {
        return doPlanDone();
    }
    if (route == "/save") {
        return doSave();
    }
    if (route == "/load") {
        return doLoad();
    }
    if (route == "/end_phase") {
        return doEndPhase();
    }
    return "/";
}

string readRequest(SocketType client) {
    string data = "";
    char buffer[2048];
    int contentLength = 0;
    while (true) {
        int n = recv(client, buffer, 2048, 0);
        if (n <= 0) {
            break;
        }
        data = data + string(buffer, n);
        int headerEnd = data.find("\r\n\r\n");
        if (headerEnd != -1) {
            int cl = data.find("Content-Length:");
            if (cl != -1 && cl < headerEnd) {
                contentLength = atoi(data.substr(cl + 15, 10).c_str());
            }
            if ((int)data.size() >= headerEnd + 4 + contentLength) {
                break;
            }
        }
    }
    return data;
}

void sendResponse(SocketType client, string status, string type, string body) {
    string response = "HTTP/1.1 " + status + "\r\n";
    response = response + "Content-Type: " + type + "; charset=utf-8\r\n";
    response = response + "Content-Length: " + to_string(body.size()) + "\r\n";
    response = response + "Connection: close\r\n";
    response = response + "Cache-Control: no-store\r\n\r\n";
    response = response + body;
    send(client, response.c_str(), (int)response.size(), 0);
}

void sendRedirect(SocketType client, string location) {
    string response = "HTTP/1.1 303 See Other\r\nLocation: " + location + "\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
    send(client, response.c_str(), (int)response.size(), 0);
}

void handleClient(SocketType client) {
    string request = readRequest(client);
    if (request == "") {
        return;
    }

    int s1 = request.find(' ');
    int s2 = request.find(' ', s1 + 1);
    if (s1 == -1 || s2 == -1) {
        return;
    }
    string method = request.substr(0, s1);
    string target = request.substr(s1 + 1, s2 - s1 - 1);

    string route = target;
    string query = "";
    int q = target.find('?');
    if (q != -1) {
        route = target.substr(0, q);
        query = target.substr(q + 1);
    }

    if (method == "POST") {
        string body = "";
        int headerEnd = request.find("\r\n\r\n");
        if (headerEnd != -1) {
            body = request.substr(headerEnd + 4);
        }
        string next = handlePost(route, body);
        if (route != "/end_phase") {
            saveState();
        }
        sendRedirect(client, next);
        return;
    }

    refreshTag = "";
    if (route == "/" || route == "/setup") {
        sendResponse(client, "200 OK", "text/html", renderLayout("setup", pageSetup()));
    } else if (route == "/add") {
        sendResponse(client, "200 OK", "text/html", renderLayout("add", pageAdd()));
    } else if (route == "/history") {
        sendResponse(client, "200 OK", "text/html", renderLayout("history", pageHistory()));
    } else if (route == "/progress") {
        bool play = (getParam(query, "play") == "1");
        string content = pageProgress(atoi(getParam(query, "day").c_str()), play);
        sendResponse(client, "200 OK", "text/html", renderLayout("progress", content));
    } else if (route == "/report") {
        sendResponse(client, "200 OK", "text/html", renderLayout("report", pageReport()));
    } else if (route == "/plan") {
        sendResponse(client, "200 OK", "text/html", renderLayout("plan", pagePlan()));
    } else if (route == "/style.css") {
        sendResponse(client, "200 OK", "text/css", readFile("web/style.css"));
    } else {
        sendResponse(client, "404 Not Found", "text/plain", "Page not found");
    }

    if (wState >= 2) {
        saveState();
    }
}

void openBrowser() {
#ifdef _WIN32
    system("start http://localhost:8080");
#elif __APPLE__
    system("open http://localhost:8080");
#else
    system("xdg-open http://localhost:8080");
#endif
}

int main() {
    lib.loadAll();
    user.setUser("Guest", 20, 170, 65);

    int restored = loadState();
    if (restored >= 0) {
        flash = "Welcome back. Your previous data was restored (" + plural(restored, "workout") + ")";
        cout << "Previous data restored from gym_data.txt" << endl;
    } else if (restored == -2) {
        flash = "The saved file was damaged, so a fresh start was made (old file kept as gym_data.bak)";
        cout << "gym_data.txt was damaged and was renamed to gym_data.bak" << endl;
    }

    ifstream check("web/index.html");
    if (!check) {
        cout << "Could not find web/index.html. Run this program from the gym_project folder" << endl;
        return 1;
    }
    check.close();

#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif

    SocketType server = socket(AF_INET, SOCK_STREAM, 0);
    int yes = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes));
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(8080);
    if (bind(server, (struct sockaddr*)&address, sizeof(address)) < 0) {
        cout << "Port 8080 is busy. Close the other program using it and try again" << endl;
        return 1;
    }
    listen(server, 10);

    cout << "===== GYM MUSCLE SIMULATOR =====" << endl;
    cout << "Running at http://localhost:8080" << endl;
    cout << "Keep this window open. Press Ctrl+C to stop" << endl;
    openBrowser();

    while (true) {
        SocketType client = accept(server, NULL, NULL);
        if (client == (SocketType)-1) {
            continue;
        }

#ifdef _WIN32
        DWORD timeout = 1000;
        setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
#else
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
#endif
        handleClient(client);
        closesocket(client);
    }

    return 0;
}
