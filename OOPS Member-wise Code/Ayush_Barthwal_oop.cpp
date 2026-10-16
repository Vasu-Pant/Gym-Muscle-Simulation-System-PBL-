#include <iostream>
#include <string>
using namespace std;

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

int main() {
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

    Muscle muscles[6];
    muscles[0].setMuscle("Chest");
    muscles[1].setMuscle("Back");
    muscles[2].setMuscle("Shoulders");
    muscles[3].setMuscle("Biceps");
    muscles[4].setMuscle("Triceps");
    muscles[5].setMuscle("Abs");

    muscles[0].addVolume(1260);
    muscles[0].addVolume(1275);
    muscles[0].addVolume(1092);
    muscles[0].addFrequency();
    muscles[0].addFrequency();

    muscles[1].addVolume(1200);
    muscles[1].addFrequency();

    muscles[2].addVolume(180);
    muscles[2].addVolume(156);
    muscles[2].addFrequency();
    muscles[2].addFrequency();

    muscles[3].addVolume(300);
    muscles[3].addVolume(388.8);
    muscles[3].addFrequency();
    muscles[3].addFrequency();

    muscles[4].addVolume(360);
    muscles[4].addVolume(312);
    muscles[4].addFrequency();
    muscles[4].addFrequency();

    int i;
    for (i = 0; i < 6; i++) {
        muscles[i].calculateFinalScore();
    }

    sim.showFullBody(muscles, 6);

    for (i = 0; i < 6; i++) {
        cout << muscles[i].getName() << " Frequency: " << muscles[i].getFrequency() << endl;
    }

    return 0;
}
