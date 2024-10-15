#include "CharacterDatasetCreator.h"
#include "EntryWindow.h"
#include "InitialUserInfo.h"


EntryWindow::EntryWindow(QWidget* parent) : QMainWindow(parent) {
    m_UI.setupUi(this);

    connect(m_UI.btnNewUser, &QPushButton::clicked, this, &EntryWindow::newUser);
}

void EntryWindow::newUser() {
    InitialUserInfo userInfoW;
    while (true) {
        if (userInfoW.exec() == QDialog::Accepted) {
            UserData userData = userInfoW.getUserData();

            std::unique_ptr<CharacterDatasetCreator> trainingW = std::make_unique<CharacterDatasetCreator>(userData);
            trainingW->show();
            this->close();

            // Release ownership from the smart pointer, leaving the window to be managed by Qt's parent-child hierarchy
            trainingW.release();

            break;
        }
    }
}
