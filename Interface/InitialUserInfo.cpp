#include <algorithm>
#include <fstream>
#include <iostream>
#include <QMessageBox>
#include <set>
#include <string>

#include "InitialUserInfo.h"


void InitialUserInfo::handleSubmit() {
    const std::wstring username = m_UI.leUsername->text().toLower().toStdWString();
    const int age = m_UI.sbxAge->value();
    const bool validGender = m_UI.rbGenderFemale->isChecked() || m_UI.rbGenderMale->isChecked();
    const bool validHand = m_UI.rbHandednessLeft->isChecked() || m_UI.rbHandednessRight->isChecked();

    accept();

    if (username.empty()) {
        QMessageBox::warning(this, "Input Error", "Username cannot be empty. Please enter a valid name.");
    }
    else if (User::userIndex(username) != -1) {
        QMessageBox::warning(this, "Input Error", "The provided username already exists.");
    }
    else if (!validGender) {
        QMessageBox::warning(this, "Input Error", "Gender cannot be NULL. Please select a valid gender.");
    }
    else if (!validHand) {
        QMessageBox::warning(this, "Input Error", "You have at least one hand. Probably. Please select a hand.");
    }
    else {
        m_IsValid = true;
    }
}



InitialUserInfo::InitialUserInfo(QWidget* parent) : QDialog(parent) {
    m_UI.setupUi(this);
    connect(m_UI.okButton, &QPushButton::clicked, this, &InitialUserInfo::handleSubmit);
    connect(m_UI.cancelButton, &QPushButton::clicked, this, [this]() { this->close();  reject(); });
}

InitialUserInfo::~InitialUserInfo() {
    reject();
}

UserData InitialUserInfo::getUserData() const {
    const std::wstring name = m_UI.leUsername->text().toLower().toStdWString();
    const int age = m_UI.sbxAge->value();
    const Gender gender = m_UI.rbGenderFemale->isChecked() ? Gender::female : Gender::male;
    const Handedness hand = m_UI.rbHandednessLeft->isChecked() ? Handedness::left : Handedness::right;

    return UserData(name, age, gender, hand, {});
}

bool InitialUserInfo::validData() const {
    return m_IsValid;
}

int User::userIndex(const std::wstring& username) {
    // Open a wide input file stream (wifstream)
    std::wifstream inputFile("./Results/users.txt");

    // Check if the file was successfully opened
    if (!inputFile.is_open()) {
        return -1;
    }

    // Set the locale to UTF-8 (important for proper reading of wide characters like 'č')
    inputFile.imbue(std::locale("en_US.UTF-8"));

    // Define a set to store unique lines (std::wstring for wide strings)
    std::vector<std::wstring> usernames;

    // Read lines from the file and insert them into the set
    std::wstring line;
    while (std::getline(inputFile, line)) {
        usernames.push_back(line);  // std::set automatically handles unique entries
    }

    // Close the file stream
    inputFile.close();

    auto it = std::find(usernames.begin(), usernames.end(), username);

    // Check if the element was found
    if (it != usernames.end()) {
        // Calculate the index
        return std::distance(usernames.begin(), it);
    }
    else {
        return -1;
    }
}