#pragma once
#include <QMainWindow>

#include "Structs/Structs.h"
#include "ui_DatasetEditor.h"


class DatasetEditor : public QMainWindow {
    Q_OBJECT
private:
    Ui::DatasetEditorClass m_UI;      // UI of the window.
    UserData m_UserData;              // User data with their characters.
    std::vector<char> m_Characters;   // The training loop characters.
    int m_CurrentCharacterIndex = 0;  // Current character index.

public:
    /// <summary>
    /// Constructor of the window.
    /// </summary>
    /// <param name="user">The provided user</param>
    /// <param name="parent">Parent</param>
    DatasetEditor(const UserData& user, QWidget* parent = nullptr);

    /// <summary>
    /// Selection of the previous sample.
    /// </summary>
    void previousSample();

    /// <summary>
    /// Selection of the next sample.
    /// </summary>
    void nextSample();

    /// <summary>
    /// Editing the current character.
    /// </summary>
    void editCharacter();

    /// <summary>
    /// Cancel the editing of the current character.
    /// </summary>
    void cancelEditing();

    /// <summary>
    /// Confirming the edited character.
    /// </summary>
    void confirmEditedCharacter();

    /// <summary>
    /// Repeating the edited character.
    /// </summary>
    void repeatEditedCharacter();

    /// <summary>
    /// Rendering the current character.
    /// </summary>
    /// <param name="character">Index of the character</param>
    void renderCharacter(int character);

public slots:
    /// <summary>
    /// Closing of the window.
    /// </summary>
    void closeEvent(QCloseEvent* event);
};
