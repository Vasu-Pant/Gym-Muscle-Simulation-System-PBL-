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

ExerciseLibrary lib;

#ifdef _WIN32
typedef SOCKET SocketType;
#else
typedef int SocketType;
#define closesocket close
#endif

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
