#include <fstream>

#include "CharacterDatasetCreator.h"
#include "EntryWindow.h"
#include "InitialUserInfo.h"
#include "UserLoadingWindow.h"


EntryWindow::EntryWindow(QWidget* parent) : QMainWindow(parent) {
    m_UI.setupUi(this);

    connect(m_UI.btnNewUser, &QPushButton::clicked, this, &EntryWindow::newUser);
    connect(m_UI.btnLoadUser, &QPushButton::clicked, this, &EntryWindow::loadUser);
}

void EntryWindow::newUser() {
    InitialUserInfo userInfoW;

    // The dialog is repeated until the user data is valid.
    while (true) {
        // If a user clicks OK, we check if everything is OK.
        if (userInfoW.exec() == QDialog::Accepted) {
            // If the data is not valid, you'll stick your head back out and shout "We'll have another round".
            if (!userInfoW.validData()) {
                continue;
            }

            // Retrieving the valid user data.
            UserData userData = userInfoW.getUserData();

            // Opening the training window.
            std::unique_ptr<CharacterDatasetCreator> trainingW = std::make_unique<CharacterDatasetCreator>(userData, -1);
            trainingW->show();
            this->close();

            // Release ownership from the smart pointer, leaving the window to be managed by Qt's parent-child hierarchy.
            trainingW.release();

            break;
        }
        // If the user clicks Cancel or closes the dialog, we do nothing.
        else {
            break;
        }
    }
}

void EntryWindow::loadUser() {
    UserLoadingWindow loadUserW;

    // The dialog is repeated until the user data is valid.
    while (true) {
        // If a user clicks OK, we check if everything is OK.
        if (loadUserW.exec() == QDialog::Accepted) {
            const int index = loadUserW.userIndex();

            // If the data is not valid, you'll stick your head back out and shout "We'll have another round".
            if (index == -1) {
                continue;
            }

            std::stringstream ss;
            ss << "./Results/" << index << ".json";
            std::ifstream file(ss.str());
            std::string json_string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            file.close();
            UserData data = json::parse(json_string);

            // Opening the training window.
            std::unique_ptr<CharacterDatasetCreator> trainingW = std::make_unique<CharacterDatasetCreator>(data, index);
            trainingW->show();
            this->close();

            // Release ownership from the smart pointer, leaving the window to be managed by Qt's parent-child hierarchy.
            trainingW.release();

            break;
        }
        // If the user clicks Cancel or closes the dialog, we do nothing.
        else {
            break;
        }
    }
}
