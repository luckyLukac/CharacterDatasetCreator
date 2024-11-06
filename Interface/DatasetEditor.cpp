#include <fstream>
#include <random>

#include "DatasetEditor.h"
#include "EntryWindow.h"


DatasetEditor::DatasetEditor(const UserData& user, QWidget* parent) : m_UserData(user), QMainWindow(parent) {
    m_UI.setupUi(this);
    m_UI.wgtCanvas->disable();

    m_CurrentCharacterIndex = 0;

    const std::string progressNumber = " / " + std::to_string(m_UserData.characters.size());
    m_UI.sbxCurrentCharacterIndex->setValue(m_CurrentCharacterIndex + 1);
    m_UI.lblProgressNumber->setText(QString::fromStdString(progressNumber));

    // Training loop generation (5 samples of each character). Shuffled like a fine margarita.
    std::vector<wchar_t> characters;
    for (char c = 'A'; c <= 'Z'; ++c) {
        for (int i = 0; i < 5; i++) {
            characters.push_back(c);
        }
    }
    for (int i = 0; i < 5; i++) {
        characters.push_back(L'Č');
    }
    for (int i = 0; i < 5; i++) {
        characters.push_back(L'Š');
    }
    for (int i = 0; i < 5; i++) {
        characters.push_back(L'Ž');
    }
    for (char c = '0'; c <= '9'; ++c) {
        for (int i = 0; i < 5; i++) {
            characters.push_back(c);
        }
    }

    std::mt19937 eng(0); // Seed the generator
    std::shuffle(characters.begin(), characters.end(), eng); // Shuffle the vector
    m_Characters = std::vector<wchar_t>(characters.begin(), characters.begin() + m_UserData.characters.size());

    // Generation of the initial character.
    const int character = m_Characters.at(m_CurrentCharacterIndex);
    m_UI.lblCurrentCharacter->setText(QString::fromStdWString(std::wstring(1, character)));
    renderCharacter(1);

    connect(m_UI.btnPreviousSample, &QPushButton::clicked, this, &DatasetEditor::previousSample);
    connect(m_UI.btnNextSample, &QPushButton::clicked, this, &DatasetEditor::nextSample);
    connect(m_UI.btnEditCharacter, &QPushButton::clicked, this, &DatasetEditor::editCharacter);
    connect(m_UI.btnCancelEdit, &QPushButton::clicked, this, &DatasetEditor::cancelEditing);
    connect(m_UI.btnConfirm, &QPushButton::clicked, this, &DatasetEditor::confirmEditedCharacter);
    connect(m_UI.btnRepeat, &QPushButton::clicked, this, &DatasetEditor::repeatEditedCharacter);
    connect(m_UI.sbxCurrentCharacterIndex, &QSpinBox::valueChanged, this, &DatasetEditor::renderCharacter);

}

void DatasetEditor::previousSample() {
    const int currentValue = m_UI.sbxCurrentCharacterIndex->value() - 1;
    if (currentValue > 0) {
        m_CurrentCharacterIndex--;
        m_UI.sbxCurrentCharacterIndex->setValue(m_CurrentCharacterIndex + 1);
    }
}

void DatasetEditor::nextSample() {
    const int currentValue = m_UI.sbxCurrentCharacterIndex->value() - 1;
    if (currentValue < m_UserData.characters.size() - 1) {
        m_CurrentCharacterIndex++;
        m_UI.sbxCurrentCharacterIndex->setValue(m_CurrentCharacterIndex + 1);
    }
}

void DatasetEditor::editCharacter() {
    m_UI.btnEditCharacter->setEnabled(false);
    m_UI.btnConfirm->setEnabled(true);
    m_UI.btnRepeat->setEnabled(true);
    m_UI.btnCancelEdit->setEnabled(true);

    m_UI.wgtCanvas->clearCanvas();
    m_UI.wgtCanvas->enable();
}

void DatasetEditor::cancelEditing() {
    m_UI.btnEditCharacter->setEnabled(true);
    m_UI.btnConfirm->setEnabled(false);
    m_UI.btnRepeat->setEnabled(false);
    m_UI.btnCancelEdit->setEnabled(false);

    m_UI.wgtCanvas->clearCanvas();
    m_UI.wgtCanvas->disable();

    renderCharacter(m_UI.sbxCurrentCharacterIndex->value());
}

void DatasetEditor::confirmEditedCharacter() {
    const wchar_t character = m_Characters.at(m_CurrentCharacterIndex);
    const std::vector<Point> characterPoints = m_UI.wgtCanvas->confirmCharacter();
    if (characterPoints.empty()) {
        return;
    }

    m_UserData.characters.at(m_CurrentCharacterIndex) = HandwrittenCharacter(character, characterPoints);

    // Dump JSON to file.
    std::wstringstream ss;
    ss << "./Results/" << m_UserData.id << ".json";
    std::ofstream of(ss.str());
    of << json(m_UserData).dump(4);
    of.close();

    m_UI.btnEditCharacter->setEnabled(true);
    m_UI.btnConfirm->setEnabled(false);
    m_UI.btnRepeat->setEnabled(false);
    m_UI.btnCancelEdit->setEnabled(false);

    renderCharacter(m_UI.sbxCurrentCharacterIndex->value());
    m_UI.wgtCanvas->disable();
}

void DatasetEditor::repeatEditedCharacter() {
    m_UI.wgtCanvas->clearCanvas();
}

void DatasetEditor::renderCharacter(int character) {
    const int index = character - 1;

    m_UI.lblCurrentCharacter->setText(QString::fromStdWString(std::wstring(1, m_UserData.characters.at(index).character)));
    m_UI.sbxCurrentCharacterIndex->setValue(character);
    m_CurrentCharacterIndex = index;

    m_UI.wgtCanvas->clearCanvas();
    std::vector<Point> points = m_UserData.characters.at(index).points;
    for (int i = 1; i < points.size(); i++) {
        if (points.at(i - 1).x < 0 || points.at(i - 1).y < 0 || points.at(i).x < 0 || points.at(i).y < 0) {
            continue;
        }

        m_UI.wgtCanvas->drawLine(points.at(i - 1), points.at(i));
    }
}

void DatasetEditor::closeEvent(QCloseEvent* event) {
    std::unique_ptr<EntryWindow> entry = std::make_unique<EntryWindow>();
    entry->show();
    this->close();

    // Release ownership from the smart pointer, leaving the window to be managed by Qt's parent-child hierarchy.
    entry.release();
}