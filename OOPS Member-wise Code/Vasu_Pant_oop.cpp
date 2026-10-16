#include <iostream>
#include <string>
using namespace std;

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
    WorkoutAnalyzer analyzer;
    float volume = analyzer.calculateVolume(60, 3, 10);
    int score = analyzer.calculateScore(volume, 4000);

    ReportGenerator report;
    report.generateReport("Chest", score);

    int absScore = analyzer.calculateScore(0, 4000);
    report.generateReport("Abs", absScore);

    float benchVolume1 = analyzer.calculateVolume(60, 3, 10);
    float inclineVolume = analyzer.calculateVolume(42.5, 3, 10);
    float latVolume = analyzer.calculateVolume(50, 3, 10);
    float benchVolume2 = analyzer.calculateVolume(65, 3, 8);
    float curlVolume = analyzer.calculateVolume(12, 3, 12);

    string names[6];
    names[0] = "Chest";
    names[1] = "Back";
    names[2] = "Shoulders";
    names[3] = "Biceps";
    names[4] = "Triceps";
    names[5] = "Abs";

    float muscleVolumes[6];
    muscleVolumes[0] = analyzer.calculateMuscleVolume(benchVolume1, 70) + analyzer.calculateMuscleVolume(inclineVolume, 100) + analyzer.calculateMuscleVolume(benchVolume2, 70);
    muscleVolumes[1] = analyzer.calculateMuscleVolume(latVolume, 80);
    muscleVolumes[2] = analyzer.calculateMuscleVolume(benchVolume1, 10) + analyzer.calculateMuscleVolume(benchVolume2, 10);
    muscleVolumes[3] = analyzer.calculateMuscleVolume(latVolume, 20) + analyzer.calculateMuscleVolume(curlVolume, 90);
    muscleVolumes[4] = analyzer.calculateMuscleVolume(benchVolume1, 20) + analyzer.calculateMuscleVolume(benchVolume2, 20);
    muscleVolumes[5] = 0;

    int scores[6];
    int i;
    for (i = 0; i < 6; i++) {
        scores[i] = analyzer.calculateScore(muscleVolumes[i], 4000);
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

    float totalVolume = benchVolume1 + inclineVolume + latVolume + benchVolume2 + curlVolume;
    report.generateSummary(5, 15, 150, totalVolume);

    return 0;
}
