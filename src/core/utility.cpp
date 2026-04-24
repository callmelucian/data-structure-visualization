#include "../../include/core/utility.hpp"

const int INPUT_NUMBER_LIMIT = 1e8;

float magnitude(const sf::Vector2f &v) { 
    return std::sqrt(v.x * v.x + v.y * v.y); 
}

float squaredMagnitude (const sf::Vector2f &v) {
    return v.x * v.x + v.y * v.y;
}

float distance(const sf::Vector2f &a, const sf::Vector2f &b) { 
    return magnitude(a - b); 
}

float squaredDistance (const sf::Vector2f &a, const sf::Vector2f &b) {
    return squaredMagnitude(a - b);
}

sf::Vector2f unitVector (const sf::Vector2f &a, const sf::Vector2f &b) {
    if (a == b) return sf::Vector2f({0, 0});
    return (b - a) / distance(a, b);
}

bool isPointOnSegment (sf::Vector2f a, sf::Vector2f b, sf::Vector2f p, float epsilon) {
    float ab = distance(a, b);
    float ap = distance(a, p);
    float bp = distance(b, p);
    return std::abs(ab - (ap + bp)) <= epsilon;
}

float cube(float a) { 
    return a * a * a; 
}

int convert (const std::string &s) {
    int ans = 0;
    for (char c : s) {
        if (c < '0' || '9' < c) return -1;
        ans = ans * 10 + c - '0';
    }
    return ans;
}

std::vector<int> stringToNumbers(const std::string &s) {
    std::vector<int> result;
    int curValue = 0;
    bool sgn = false, filled = false;

    for (char c : s) {
        if (c == ' ') {
            if (filled) result.push_back(sgn ? -curValue : curValue);
            curValue = 0, sgn = filled = false;
        }
        else if (c == '-') {
            if (!filled) sgn = true;
            else return {};
            filled = true;
        }
        else if ('0' <= c && c <= '9')
            curValue = curValue * 10 + (c - '0'), filled = true;
        else return {};
    }
    if (filled) result.push_back(curValue);
    return result;
}

std::ostream& operator<<(std::ostream &oup, const sf::Vector2f &v) {
    return oup << "(" << v.x << ", " << v.y << ")", oup;
}

// Global Variable Definitions
std::function<std::string(float)> floatToPercentage = [](float value) {
    int percentage = static_cast<int>(std::round(value * 100.0f));
    return std::to_string(percentage) + "%";
};

std::mt19937 rng(21);

float randFloat(float L, float R) {
    return std::uniform_real_distribution<float>(L, R)(rng);
}

int randInt (int L, int R) {
    return std::uniform_int_distribution<int>(L, R)(rng);
}

std::string openFileDialog() {
    OPENFILENAMEA ofn;       // Common dialog box structure
    char szFile[260] = { 0 }; // Buffer for file name

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Text Files\0*.txt\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameA(&ofn) == TRUE) {
        return std::string(szFile);
    }
    return "";
}

// ========== COUNTDOWN CLOCK ==========
CountDownClock::CountDownClock() : duration(0.0f), startTime(sf::Time::Zero) {
    internalClock.restart();
}

sf::Time CountDownClock::getRemainingTime() const {
    sf::Time totalElapsed = internalClock.getElapsedTime() + startTime;
    sf::Time totalLimit = sf::seconds(duration);
    if (totalElapsed >= totalLimit) return sf::Time::Zero;
    return totalLimit - totalElapsed;
}

bool CountDownClock::isFinished() const {
    return getRemainingTime() <= sf::Time::Zero;
}

void CountDownClock::restart() {
    internalClock.restart();
    startTime = sf::Time::Zero;
}

void CountDownClock::changeDuration(float newDuration) {
    float remainingSeconds = getRemainingTime().asSeconds();
    if (remainingSeconds > newDuration) remainingSeconds = newDuration;
    duration = newDuration;
    internalClock.restart();
    startTime = sf::seconds(duration - remainingSeconds);
}