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

BodySimulator sim;
WorkoutAnalyzer analyzer;
ReportGenerator report;

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
