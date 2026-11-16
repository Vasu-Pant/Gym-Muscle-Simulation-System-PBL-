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

User user;
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
