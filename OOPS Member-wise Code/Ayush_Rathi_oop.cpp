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

int main() {
    User u;
    u.setUser("Rahul", 20, 170.5, 65.0);
    u.showUser();

    Workout w1;
    w1.setData("Bench Press", 60, 3, 10);
    cout << w1.getExercise() << " Volume: " << w1.calculateVolume() << endl;

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

    return 0;
}
