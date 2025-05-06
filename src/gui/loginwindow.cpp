#include "loginwindow.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QFont>

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent),
    usernameEdit(new QLineEdit(this)),
    passwordEdit(new QLineEdit(this)),
    loginButton(new QPushButton("Login", this))
{
    setObjectName("loginWindow");
    passwordEdit->setEchoMode(QLineEdit::Password);
    
    QLabel *titleLabel = new QLabel("Tic Tac Toe", this);
    titleLabel->setObjectName("title");
    
    QLabel *subtitleLabel = new QLabel("Advanced Edition", this);
    subtitleLabel->setObjectName("subtitle");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    
    QFont titleFont("Arial Rounded MT Bold", 32, QFont::Bold);
    titleLabel->setFont(titleFont);
    
    QFont subtitleFont("Arial", 16, QFont::Normal);
    subtitleLabel->setFont(subtitleFont);
    
    usernameEdit->setPlaceholderText("Enter your username");
    passwordEdit->setPlaceholderText("Enter your password");
    
    loginButton->setObjectName("loginButton");
    loginButton->setCursor(Qt::PointingHandCursor);
    
    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow("Username:", usernameEdit);
    formLayout->addRow("Password:", passwordEdit);
    formLayout->setSpacing(15);
    formLayout->setContentsMargins(0, 20, 0, 20);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(subtitleLabel);
    mainLayout->addSpacing(30);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(loginButton);
    mainLayout->setAlignment(titleLabel, Qt::AlignHCenter);
    mainLayout->setAlignment(subtitleLabel, Qt::AlignHCenter);
    mainLayout->setAlignment(loginButton, Qt::AlignHCenter);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::attemptLogin);
}
void LoginWindow::clearFields()
{
    usernameEdit->clear();
    passwordEdit->clear();
}

void LoginWindow::attemptLogin()
{
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Login Failed", "Username and password cannot be empty");
        return;
    }

    // In a real application, you would verify credentials here
    emit loginSuccess(username);
}