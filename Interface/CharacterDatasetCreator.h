#pragma once
#include <QtWidgets/QMainWindow>
#include <random>

#include "ui_CharacterDatasetCreator.h"
#include "Interface/InitialUserInfo.h"


class CharacterDatasetCreator : public QMainWindow {
    Q_OBJECT

private:
    Ui::CharacterDatasetCreatorClass m_UI;  // UI of the app.
    UserData m_UserData;                    // User data with their characters.
    std::vector<char> m_Characters;         // The training loop characters.
    int m_CurrentCharacterIndex = 0;        // Current character index.


    /// <summary>
    /// Setting the index for the current user.
    /// </summary>
    /// <returns>Index</returns>
    int userIndex();

public:
    /// <summary>
    /// Constructor of the window.
    /// </summary>
    /// <param name="parent">Window parent</param>
    CharacterDatasetCreator(const UserData& user, QWidget* parent = nullptr);

    /// <summary>
    /// Click on the canvas.
    /// </summary>
    /// <param name="point">Point of click</param>
    void canvasClicked(const QPoint& point);

    /// <summary>
    /// Confirming the input character.
    /// </summary>
    void confirmCharacter();

    /// <summary>
    /// Repeating the character.
    /// </summary>
    void repeatCharacter();
};