#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include "raylib.h"
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

template<typename QuestionContainer>
class ProgressBar {
private:
    Rectangle bounds;
    Color baseColor;
    Color fillColor;
    const QuestionContainer& questions;
    int currentIndex;
    string label;

public:
    ProgressBar(float x, float y, float w, float h, Color base, Color fill,
        const QuestionContainer& qs, string lbl = "") :
        bounds{ x, y, w, h }, baseColor(base), fillColor(fill),
        questions(qs), currentIndex(0), label(lbl) {
    }

    float GetX() const { return bounds.x; }
    float GetY() const { return bounds.y; }
    float GetWidth() const { return bounds.width; }
    float GetHeight() const { return bounds.height; }


    void Draw() const {
        DrawRectangleRec(bounds, baseColor);

        if (questions.size() > 0) {
            float progressWidth = bounds.width * (static_cast<float>(currentIndex) / questions.size());
            DrawRectangle(bounds.x, bounds.y, progressWidth, bounds.height, fillColor);
        }

        DrawRectangleLinesEx(bounds, 2.0f, BLACK);

        if (!label.empty()) {
            int labelWidth = MeasureText(label.c_str(), 16);
            DrawText(label.c_str(),
                bounds.x + 10,
                bounds.y + 4,
                16, BLACK);
        }
    }

    ProgressBar& operator++() {
        if (currentIndex < questions.size() - 1) currentIndex++;
        return *this;
    }

    ProgressBar& operator--() {
        if (currentIndex > 0) currentIndex--;
        return *this;
    }

    ProgressBar& operator+=(int amount) {
        currentIndex = min(currentIndex + amount, static_cast<int>(questions.size()) - 1);
        return *this;
    }

    ProgressBar& operator-=(int amount) {
        currentIndex = max(currentIndex - amount, 0);
        return *this;
    }

    int GetCurrentIndex() const {
        return currentIndex;
    }

    int GetTotalQuestions() const {
        return static_cast<int>(questions.size());
    }
};

#endif