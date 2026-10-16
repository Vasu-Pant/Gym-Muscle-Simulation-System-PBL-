#include <iostream>
#include <string>
using namespace std;
class Workout {
private:
    string exercise;
    float weight;
    int sets;
    int reps;
public:
    void setData(string e, float w, int s, int r) {
        exercise = e;
        weight = w;
        sets = s;
        reps = r;
    }
    string getExercise() {
        return exercise;
    }
    float calculateVolume() {
        return weight * sets * reps;
    }
    void setWeight(float w) {
        if (w > 0) {
            weight = w;
        } else {
            cout << "Invalid weight" << endl;
        }
    }
    float getWeight() {
        return weight;
    }
    int getSets() {
        return sets;
    }
    int getReps() {
        return reps;
    }
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
    void showWorkout() {
        cout << exercise << " | " << weight << " kg | " << sets << " sets | " << reps << " reps | Volume: " << calculateVolume() << endl;
    }
};
class User {
private:
    string name;
    int age;
    float height;
    float weight;
    int workoutCount;
public:
    void setUser(string n, int a, float h, float w) {
        name = n;
        age = a;
        height = h;
        weight = w;
        workoutCount = 0;
    }
    void showUser() {
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "Height: " << height << endl;
        cout << "Weight: " << weight << endl;
    }
    string getName() {
        return name;
    }
    int getAge() {
        return age;
    }
    void updateWeight(float w) {
        if (w > 0) {
            weight = w;
            cout << "Weight updated to " << weight << endl;
        } else {
            cout << "Invalid weight" << endl;
        }
    }
    float calculateBMI() {
        float h = height / 100;
        return weight / (h * h);
    }
    void addWorkoutCount() {
        workoutCount = workoutCount + 1;
    }
    void showWorkoutCount() {
        cout << name << " has done " << workoutCount << " workouts" << endl;
    }
};
class Exercise {
protected:
    string name;
public:
    virtual ~Exercise() {
    }
    void setName(string n) {
        name = n;
    }
    string getName() {
        return name;
    }
    virtual float calculateWorkload(float weight, int sets, int reps) {
        return weight * sets * reps;
    }
    virtual string getType() {
        return "General";
    }
    void showExercise() {
        cout << name << " is a " << getType() << " exercise" << endl;
    }
};
class CompoundExercise : public Exercise {
public:
    float calculateWorkload(float weight, int sets, int reps) override {
        return weight * sets * reps * 1.1;
    }
    string getType() override {
        return "Compound";
    }
};
class IsolationExercise : public Exercise {
public:
    float calculateWorkload(float weight, int sets, int reps) override {
        return weight * sets * reps * 0.9;
    }
    string getType() override {
        return "Isolation";
    }
};
class CardioExercise : public Exercise {
public:
    float calculateWorkload(float weight, int sets, int reps) override {
        return sets * reps * 2.5;
    }
    string getType() override {
        return "Cardio";
    }
};
class Muscle {
private:
    string name;
    float totalVolume;
    int score;
    int frequency;
public:
    void setMuscle(string n) {
        name = n;
        totalVolume = 0;
        score = 0;
        frequency = 0;
    }
    void addVolume(float v) {
        totalVolume = totalVolume + v;
    }
    void calculateScore() {
        score = (totalVolume / 4000) * 100;
        if (score > 100) {
            score = 100;
        }
    }
    void showMuscle() {
        cout << name << " Volume: " << totalVolume << " Score: " << score << endl;
    }
    void addFrequency() {
        frequency = frequency + 1;
    }
    void calculateFinalScore() {
        float volumeScore = (totalVolume / 4000) * 100;
        if (volumeScore > 100) {
            volumeScore = 100;
        }
        float frequencyScore = (frequency / 3.0) * 100;
        if (frequencyScore > 100) {
            frequencyScore = 100;
        }
        score = volumeScore * 0.8 + frequencyScore * 0.2;
    }
    string getName() {
        return name;
    }
    float getVolume() {
        return totalVolume;
    }
    int getScore() {
        return score;
    }
    int getFrequency() {
        return frequency;
    }
    string getLevel() {
        if (score <= 20) {
            return "Very Low";
        }
        if (score <= 40) {
            return "Low";
        }
        if (score <= 60) {
            return "Moderate";
        }
        if (score <= 80) {
            return "High";
        }
        return "Very High";
    }
};
class BodySimulator {
public:
    void showBody(Muscle chest, Muscle back, Muscle abs) {
        cout << "BODY SIMULATION" << endl;
        chest.showMuscle();
        back.showMuscle();
        abs.showMuscle();
    }
    void showBar(Muscle m) {
        int i;
        cout << m.getName() << " ";
        for (i = 0; i < m.getScore() / 5; i++) {
            cout << "#";
        }
        cout << " " << m.getScore() << "% " << m.getLevel() << endl;
    }
    void showFullBody(Muscle m[], int n) {
        int i;
        cout << "FULL BODY MUSCLE MAP" << endl;
        for (i = 0; i < n; i++) {
            showBar(m[i]);
        }
    }
};
class WorkoutAnalyzer {
public:
    float calculateVolume(float weight, int sets, int reps) {
        return weight * sets * reps;
    }
    int calculateScore(float volume, float reference) {
        int score = (volume / reference) * 100;
        if (score > 100) {
            score = 100;
        }
        return score;
    }
    float calculateMuscleVolume(float volume, float percent) {
        return volume * percent / 100;
    }
    int findHighestIndex(int scores[], int n) {
        int i;
        int index = 0;
        for (i = 1; i < n; i++) {
            if (scores[i] > scores[index]) {
                index = i;
            }
        }
        return index;
    }
    int findLowestIndex(int scores[], int n) {
        int i;
        int index = 0;
        for (i = 1; i < n; i++) {
            if (scores[i] < scores[index]) {
                index = i;
            }
        }
        return index;
    }
    float calculateAverage(int scores[], int n) {
        int i;
        int total = 0;
        for (i = 0; i < n; i++) {
            total = total + scores[i];
        }
        return (float)total / n;
    }
};
class ReportGenerator {
public:
    void generateReport(string muscle, int score) {
        cout << muscle << " Activity Score: " << score << "%" << endl;
        if (score < 20) {
            cout << "Warning: " << muscle << " undertrained" << endl;
        }
    }
    void generateImbalanceReport(string a, int scoreA, string b, int scoreB) {
        int difference = scoreA - scoreB;
        if (difference < 0) {
            difference = -difference;
        }
        if (difference > 30) {
            cout << "Warning: " << a << " and " << b << " training imbalance detected" << endl;
        } else {
            cout << a << " and " << b << " are balanced" << endl;
        }
    }
    void generateSummary(int workouts, int sets, int reps, float totalVolume) {
        cout << "TRAINING SUMMARY" << endl;
        cout << "Total Workouts: " << workouts << endl;
        cout << "Total Sets: " << sets << endl;
        cout << "Total Reps: " << reps << endl;
        cout << "Total Weight Moved: " << totalVolume << " kg" << endl;
    }
};
int main() {
    User u;
    u.setUser("Rahul", 20, 170.5, 65.0);
    u.showUser();

    Workout w1;
    w1.setData("Bench Press", 60, 3, 10);
    cout << w1.getExercise() << " Volume: " << w1.calculateVolume() << endl;

    Exercise* e1 = new CompoundExercise();
    e1->setName("Bench Press");
    cout << "Compound Workload: " << e1->calculateWorkload(60, 3, 10) << endl;

    Exercise* e2 = new IsolationExercise();
    e2->setName("Bicep Curl");
    cout << "Isolation Workload: " << e2->calculateWorkload(12, 3, 12) << endl;

    Muscle chest;
    chest.setMuscle("Chest");
    chest.addVolume(1260);
    chest.addVolume(900);
    chest.calculateScore();

    Muscle back;
    back.setMuscle("Back");
    back.addVolume(1500);
    back.calculateScore();

    Muscle abs;
    abs.setMuscle("Abs");
    abs.calculateScore();

    BodySimulator sim;
    sim.showBody(chest, back, abs);

    WorkoutAnalyzer analyzer;
    float volume = analyzer.calculateVolume(60, 3, 10);
    int score = analyzer.calculateScore(volume, 4000);

    ReportGenerator report;
    report.generateReport("Chest", score);

    int absScore = analyzer.calculateScore(0, 4000);
    report.generateReport("Abs", absScore);

    cout << "BMI: " << u.calculateBMI() << endl;
    u.updateWeight(66.5);
    u.updateWeight(-5);
    u.showUser();

    Workout w2;
    w2.setData("Incline Press", 40, 3, 10);
    Workout w3;
    w3.setData("Lat Pulldown", 50, 3, 10);
    Workout w4;
    w4.setData("Bench Press", 65, 3, 8);
    Workout w5;
    w5.setData("Bicep Curl", 12, 3, 12);
    Workout bad;
    bad.setData("Squat", -20, 0, -5);

    w2.setWeight(42.5);
    w2.setWeight(-10);

    u.addWorkoutCount();
    u.addWorkoutCount();
    u.addWorkoutCount();
    u.addWorkoutCount();
    u.addWorkoutCount();
    u.showWorkoutCount();

    w1.showWorkout();
    w2.showWorkout();
    w3.showWorkout();
    w4.showWorkout();
    w5.showWorkout();

    if (bad.isValid() == false) {
        cout << "Invalid workout rejected: " << bad.getExercise() << endl;
    }

    Exercise* e3 = new CardioExercise();
    e3->setName("Running");
    cout << "Cardio Workload: " << e3->calculateWorkload(0, 3, 20) << endl;

    e1->showExercise();
    e2->showExercise();
    e3->showExercise();

    Muscle muscles[6];
    muscles[0].setMuscle("Chest");
    muscles[1].setMuscle("Back");
    muscles[2].setMuscle("Shoulders");
    muscles[3].setMuscle("Biceps");
    muscles[4].setMuscle("Triceps");
    muscles[5].setMuscle("Abs");

    muscles[0].addVolume(analyzer.calculateMuscleVolume(w1.calculateVolume(), 70));
    muscles[0].addVolume(analyzer.calculateMuscleVolume(w2.calculateVolume(), 100));
    muscles[0].addVolume(analyzer.calculateMuscleVolume(w4.calculateVolume(), 70));
    muscles[0].addFrequency();
    muscles[0].addFrequency();

    muscles[1].addVolume(analyzer.calculateMuscleVolume(w3.calculateVolume(), 80));
    muscles[1].addFrequency();

    muscles[2].addVolume(analyzer.calculateMuscleVolume(w1.calculateVolume(), 10));
    muscles[2].addVolume(analyzer.calculateMuscleVolume(w4.calculateVolume(), 10));
    muscles[2].addFrequency();
    muscles[2].addFrequency();

    muscles[3].addVolume(analyzer.calculateMuscleVolume(w3.calculateVolume(), 20));
    muscles[3].addVolume(analyzer.calculateMuscleVolume(w5.calculateVolume(), 90));
    muscles[3].addFrequency();
    muscles[3].addFrequency();

    muscles[4].addVolume(analyzer.calculateMuscleVolume(w1.calculateVolume(), 20));
    muscles[4].addVolume(analyzer.calculateMuscleVolume(w4.calculateVolume(), 20));
    muscles[4].addFrequency();
    muscles[4].addFrequency();

    int scores[6];
    string names[6];
    int i;
    for (i = 0; i < 6; i++) {
        muscles[i].calculateFinalScore();
        scores[i] = muscles[i].getScore();
        names[i] = muscles[i].getName();
    }

    sim.showFullBody(muscles, 6);

    for (i = 0; i < 6; i++) {
        report.generateReport(names[i], scores[i]);
    }

    int highest = analyzer.findHighestIndex(scores, 6);
    int lowest = analyzer.findLowestIndex(scores, 6);
    float averageScore = analyzer.calculateAverage(scores, 6);
    cout << "Most trained muscle: " << names[highest] << endl;
    cout << "Least trained muscle: " << names[lowest] << endl;
    cout << "Average score: " << averageScore << endl;

    report.generateImbalanceReport(names[0], scores[0], names[1], scores[1]);
    report.generateImbalanceReport(names[2], scores[2], names[3], scores[3]);

    int totalSets = w1.getSets() + w2.getSets() + w3.getSets() + w4.getSets() + w5.getSets();
    int totalReps = w1.getReps() * w1.getSets() + w2.getReps() * w2.getSets() + w3.getReps() * w3.getSets() + w4.getReps() * w4.getSets() + w5.getReps() * w5.getSets();
    float totalVolume = w1.calculateVolume() + w2.calculateVolume() + w3.calculateVolume() + w4.calculateVolume() + w5.calculateVolume();
    report.generateSummary(5, totalSets, totalReps, totalVolume);

    delete e1;
    delete e2;
    delete e3;

    return 0;
}
