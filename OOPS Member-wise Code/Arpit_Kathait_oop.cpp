#include <iostream>
#include <string>
using namespace std;

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

int main() {
    Exercise* e1 = new CompoundExercise();
    e1->setName("Bench Press");
    cout << "Compound Workload: " << e1->calculateWorkload(60, 3, 10) << endl;

    Exercise* e2 = new IsolationExercise();
    e2->setName("Bicep Curl");
    cout << "Isolation Workload: " << e2->calculateWorkload(12, 3, 12) << endl;

    Exercise* e3 = new CardioExercise();
    e3->setName("Running");
    cout << "Cardio Workload: " << e3->calculateWorkload(0, 3, 20) << endl;

    e1->showExercise();
    e2->showExercise();
    e3->showExercise();

    delete e1;
    delete e2;
    delete e3;

    return 0;
}
