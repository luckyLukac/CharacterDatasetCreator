#pragma once
#include <QDialog>

#include "ui_UserLoadingWindow.h"


class UserLoadingWindow : public QDialog {
    Q_OBJECT

private:
    Ui::UserLoadingWindowClass m_UI;  // UI of the dialog.
    int m_User = -1;                  // The provided user's index.
    bool m_AddSamples = false;        // Adding the new samples if true.
    bool m_EditSamples = false;       // Edit the current samples if true.

private slots:
    /// <summary>
    /// Handling the submit and checking if the user exists.
    /// </summary>
    void handleSubmit();

public:
    /// <summary>
    /// Window constructor.
    /// </summary>
    /// <param name="parent">Parent</param>
    UserLoadingWindow(QWidget* parent = nullptr);

    /// <summary>
    /// Retriever of the user index.
    /// </summary>
    /// <returns>User index, -1 if not existing</returns>
    int userIndex() const;

    /// <summary>
    /// Choosing whether to add or edit the samples.
    /// </summary>
    /// <returns>m_AddSamples, m_EditSamples</returns>
    std::pair<bool, bool> addAndEditSamples() const;
};
