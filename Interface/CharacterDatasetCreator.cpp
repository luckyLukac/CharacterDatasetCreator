#include <filesystem>
#include <fstream>
#include <iostream>
#include <locale>
#include <QMessageBox>
#include <regex>
#include <sstream>

#include "CharacterDatasetCreator.h"
#include "EntryWindow.h"

using json = nlohmann::json;


int CharacterDatasetCreator::userIndex() {
    std::string folder_path = "./Results"; // Change this to your folder path
    std::filesystem::path path(folder_path);

    // Variables to track the largest number and corresponding file name
    int largestNumber = -1;
    std::string largestFileName;

    // Regular expression to match a number in the filename
    std::regex number_regex(R"(\d+)");

    try {
        // Check if the directory exists
        if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
            std::cerr << "The specified path is not a directory or does not exist." << std::endl;
            return 1;
        }

        // Iterate through files in the directory
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            if (entry.is_regular_file()) {
                std::string file_name = entry.path().filename().string();

                // Find all numbers in the filename
                std::smatch match;
                if (std::regex_search(file_name, match, number_regex)) {
                    // Convert the first match to a number
                    long long number = std::stoll(match.str());

                    // Check if this number is the largest found
                    if (number > largestNumber) {
                        largestNumber = number;
                        largestFileName = file_name;
                    }
                }
            }
        }

    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }

    return largestNumber + 1;
}

CharacterDatasetCreator::CharacterDatasetCreator(const UserData& user, const int index, QWidget* parent) : QMainWindow(parent), m_UserData(user) {
    m_UI.setupUi(this);

    m_UserData.id = index;
    m_CurrentCharacterIndex = static_cast<int>(m_UserData.characters.size());

    const std::string progressNumber = std::to_string(m_CurrentCharacterIndex + 1) + " / 350";
    const std::string progressPercent = std::to_string(100.0 * (m_CurrentCharacterIndex) / 350.0).substr(0, 4) + "%";
    m_UI.lblProgressNumber->setText(QString::fromStdString(progressNumber));
    m_UI.lblProgressPercent->setText(QString::fromStdString(progressPercent));

    // Training loop generation (10 samples of each character). Shuffled like a fine margarita.
    std::vector<char> characters;
    for (char c = 'A'; c <= 'Z'; ++c) {
        for (int i = 0; i < 10; i++) {
            characters.push_back(c);
        }
    }
    for (char c = '0'; c <= '9'; ++c) {
        for (int i = 0; i < 10; i++) {
            characters.push_back(c);
        }
    }
    std::mt19937 eng(0); // Seed the generator
    std::shuffle(characters.begin(), characters.end(), eng); // Shuffle the vector
    m_Characters = characters;

    // Generation of the initial character.
    const int character = m_Characters.at(m_CurrentCharacterIndex);
    m_UI.lblCurrentCharacter->setText(QString::fromStdString(std::string(1, character)));

    if (index == -1) {
        // Adding the user to the config file.
        std::wofstream outputFile("./Results/users.txt", std::ios::app);
        outputFile.imbue(std::locale("en_US.UTF-8"));
        outputFile << user.username << std::endl;
        outputFile.close();

        // Dump JSON to file.
        const int index = userIndex();
        m_UserData.id = index;
        std::wstringstream ss;
        ss << "./Results/" << m_UserData.id << ".json";
        std::ofstream of(ss.str());
        of << json(m_UserData).dump(4);
        of.close();
    }

    connect(m_UI.wgtCanvas, &DrawingCanvas::canvasClicked, this, &CharacterDatasetCreator::canvasClicked);
    connect(m_UI.btnConfirm, &QPushButton::clicked, this, &CharacterDatasetCreator::confirmCharacter);
    connect(m_UI.btnRepeat, &QPushButton::clicked, this, &CharacterDatasetCreator::repeatCharacter);
}

void CharacterDatasetCreator::canvasClicked(const QPoint& point) {
    m_UI.btnConfirm->setEnabled(true);
    m_UI.btnRepeat->setEnabled(true);
}

void CharacterDatasetCreator::confirmCharacter() {
    const int character = m_Characters.at(m_CurrentCharacterIndex);
    const std::vector<Point> characterPoints = m_UI.wgtCanvas->confirmCharacter();
    m_UserData.characters.push_back(HandwrittenCharacter(character, characterPoints));

    // Dump JSON to file.
    std::wstringstream ss;
    ss << "./Results/" << m_UserData.id << ".json";
    std::ofstream of(ss.str());
    of << json(m_UserData).dump(4);
    of.close();

    m_CurrentCharacterIndex++;
    const std::string progressNumber = std::to_string(m_CurrentCharacterIndex + 1) + " / 350";
    const std::string progressPercent = std::to_string(100.0 * (m_CurrentCharacterIndex) / 350.0).substr(0, 4) + "%";
    m_UI.lblProgressNumber->setText(QString::fromStdString(progressNumber));
    m_UI.lblProgressPercent->setText(QString::fromStdString(progressPercent));

    if (m_CurrentCharacterIndex == 350) {
        QMessageBox::information(nullptr, "Challenge completed!", "You are now the master of character as you have completed the challenge and vastly contributed to the Character Dataset creation. Your Karma points will soar massively! Thank you again and farewell!", QMessageBox::Ok);
        this->close();
        return;
    }

    // Generation of the new character.
    const int newCharacter = m_Characters.at(m_CurrentCharacterIndex);
    m_UI.lblCurrentCharacter->setText(QString::fromStdString(std::string(1, newCharacter)));
}

void CharacterDatasetCreator::repeatCharacter() {
    m_UI.wgtCanvas->clearCanvas();
    m_UI.btnConfirm->setDisabled(true);
    m_UI.btnRepeat->setDisabled(true);
}

void CharacterDatasetCreator::closeEvent(QCloseEvent* event) {
    std::unique_ptr<EntryWindow> entry = std::make_unique<EntryWindow>();
    entry->show();
    this->close();

    // Release ownership from the smart pointer, leaving the window to be managed by Qt's parent-child hierarchy.
    entry.release();
}