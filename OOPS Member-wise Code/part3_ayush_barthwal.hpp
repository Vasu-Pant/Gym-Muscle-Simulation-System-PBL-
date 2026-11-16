string muscleNames[11] = {"Chest", "Back", "Shoulders", "Biceps", "Triceps", "Forearms", "Abs", "Quads", "Hamstrings", "Glutes", "Calves"};

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

Muscle muscles[11];

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
