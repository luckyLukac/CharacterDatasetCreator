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
    double x;
    double y;

    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}

    friend void to_json(json& j, const Point& p) {
        j = json{ {"x", p.x}, {"y", p.y} };
    }

    friend void from_json(const json& j, Point& p) {
        j.at("x").get_to(p.x);
        j.at("y").get_to(p.y);
    }
};


/// <summary>
/// Handwritten character struct.
/// </summary>
struct HandwrittenCharacter {
    char character;
    std::vector<Point> points;

    HandwrittenCharacter() : character(-1) {}
    HandwrittenCharacter(const int c, const std::vector<Point>& p) : character(c), points(p) {}

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
    std::wstring name;
    Gender gender;
    Handedness handedness;
    std::vector<HandwrittenCharacter> characters;

    UserData() {}

    UserData(const std::wstring& n, const Gender& g, const Handedness& h, const std::vector<HandwrittenCharacter>& c)
        : id(-1), name(n), gender(g), handedness(h), characters(c) {}

    friend void to_json(json& j, const UserData& userData) {
        j = json{ { "name", userData.name }, { "gender", userData.gender }, { "hand", userData.handedness }, { "characters", userData.characters } };
    }

    friend void from_json(const json& j, UserData& userData) {
        j.at("name").get_to(userData.name);
        j.at("gender").get_to(userData.gender);
        j.at("hand").get_to(userData.handedness);
        j.at("characters").get_to(userData.characters);
    }
};