#pragma once
#include <nlohmann/json.hpp>

using json = nlohmann::json;


/// <summary>
/// Gender enum.
/// </summary>
enum class Gender {
    female,
    male
};


/// <summary>
/// Handedness enum.
/// </summary>
enum class Handedness {
    left,
    right
};


/// <summary>
/// Point struct.
/// </summary>
struct Point {
    int x;
    int y;

    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}

    friend void to_json(json& j, const Point& p) {
        j = json{ {"x", p.x}, {"y", p.y} };
    }

    friend void from_json(const json& j, Point& p) {
        j.at("x").get_to(p.x);
        j.at("y").get_to(p.y);
    }

    Point operator+(const int d) {
        return Point(this->x + d, this->y + d);
    }

    Point operator*(const int f) {
        return Point(this->x * f, this->y * f);
    }
};


/// <summary>
/// Handwritten character struct.
/// </summary>
struct HandwrittenCharacter {
    wchar_t character;
    std::vector<Point> points;

    HandwrittenCharacter() : character(-1) {}
    HandwrittenCharacter(const wchar_t c, const std::vector<Point>& p) : character(c), points(p) {}

    friend void to_json(json& j, const HandwrittenCharacter& c) {
        j = json{ {"character", c.character}, {"points", c.points} };
    }

    friend void from_json(const json& j, HandwrittenCharacter& c) {
        j.at("character").get_to(c.character);
        j.at("points").get_to(c.points);
    }
};


/// <summary>
/// User data struct.
/// </summary>
struct UserData {
    int id;
    std::wstring username;
    int age;
    Gender gender;
    Handedness handedness;
    std::vector<HandwrittenCharacter> characters;

    UserData()
        : id(-1), username(std::wstring()), age(-1), gender(Gender::male), handedness(Handedness::left)
    {}

    UserData(const std::wstring& n, const int a, const Gender& g, const Handedness& h, const std::vector<HandwrittenCharacter>& c)
        : id(-1), username(n), age(a), gender(g), handedness(h), characters(c) {}

    friend void to_json(json& j, const UserData& userData) {
        j = json{ { "name", userData.username }, { "age", userData.age }, { "gender", userData.gender }, { "hand", userData.handedness }, { "characters", userData.characters } };
    }

    friend void from_json(const json& j, UserData& userData) {
        j.at("name").get_to(userData.username);
        j.at("age").get_to(userData.age);
        j.at("gender").get_to(userData.gender);
        j.at("hand").get_to(userData.handedness);
        j.at("characters").get_to(userData.characters);
    }
};