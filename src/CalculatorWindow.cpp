#include "CalculatorWindow.h"

#include <QFont>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

CalculatorWindow::CalculatorWindow(QWidget* parent)
    : QMainWindow(parent) {
    buildUi();
    refreshDisplay();
}

void CalculatorWindow::buildUi() {
    auto* central = new QWidget(this);
    auto* mainLayout = new QVBoxLayout(central);
    auto* grid = new QGridLayout();

    displayLabel_ = new QLabel("0", central);
    displayLabel_->setObjectName("displayLabel");
    displayLabel_->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    displayLabel_->setMinimumHeight(64);

    QFont displayFont = displayLabel_->font();
    displayFont.setPointSize(26);
    displayFont.setBold(true);
    displayLabel_->setFont(displayFont);
    displayLabel_->setStyleSheet("QLabel { background: #1f2937; color: white; padding: 10px; border-radius: 8px; }");

    mainLayout->addWidget(displayLabel_);

    const QVector<QVector<QString>> labels = {
        {"C", "+/-", "%", "/"},
        {"7", "8", "9", "*"},
        {"4", "5", "6", "-"},
        {"1", "2", "3", "+"},
        {"0", ".", "="}
    };

    const QVector<QVector<QString>> names = {
        {"buttonClear", "buttonSign", "buttonPercent", "buttonDivide"},
        {"button7", "button8", "button9", "buttonMultiply"},
        {"button4", "button5", "button6", "buttonSubtract"},
        {"button1", "button2", "button3", "buttonAdd"},
        {"button0", "buttonDecimal", "buttonEqual"}
    };

    for (int row = 0; row < labels.size(); ++row) {
        for (int col = 0; col < labels[row].size(); ++col) {
            QPushButton* button = createButton(labels[row][col], names[row][col]);
            if (row == 4 && col == 0) {
                grid->addWidget(button, row, col, 1, 2);
            } else if (row == 4 && col > 0) {
                grid->addWidget(button, row, col + 1, 1, 1);
            } else {
                grid->addWidget(button, row, col, 1, 1);
            }

            const QString text = labels[row][col];
            if (text[0].isDigit()) {
                connect(button, &QPushButton::clicked, this, &CalculatorWindow::digitClicked);
            } else if (text == "+" || text == "-" || text == "*" || text == "/") {
                connect(button, &QPushButton::clicked, this, &CalculatorWindow::operationClicked);
            } else if (text == "=") {
                connect(button, &QPushButton::clicked, this, &CalculatorWindow::equalClicked);
            } else if (text == "C") {
                connect(button, &QPushButton::clicked, this, &CalculatorWindow::clearClicked);
            } else if (text == ".") {
                connect(button, &QPushButton::clicked, this, &CalculatorWindow::decimalClicked);
            } else if (text == "+/-") {
                connect(button, &QPushButton::clicked, this, &CalculatorWindow::signClicked);
            } else if (text == "%") {
                connect(button, &QPushButton::clicked, this, &CalculatorWindow::percentClicked);
            }
        }
    }

    mainLayout->addLayout(grid);
    setCentralWidget(central);
    setWindowTitle("Qt Calculator - QTest E2E Demo");
    resize(360, 460);
}

QPushButton* CalculatorWindow::createButton(const QString& text, const QString& objectName) {
    auto* button = new QPushButton(text, this);
    button->setObjectName(objectName);
    button->setMinimumSize(70, 54);

    QFont font = button->font();
    font.setPointSize(16);
    button->setFont(font);

    button->setStyleSheet("QPushButton { padding: 8px; border-radius: 8px; background: #e5e7eb; }"
                          "QPushButton:pressed { background: #d1d5db; }");
    return button;
}

void CalculatorWindow::digitClicked() {
    const auto* button = qobject_cast<QPushButton*>(sender());
    if (!button) {
        return;
    }
    engine_.inputDigit(button->text().toInt());
    refreshDisplay();
}

void CalculatorWindow::operationClicked() {
    const auto* button = qobject_cast<QPushButton*>(sender());
    if (!button) {
        return;
    }

    if (button->text() == "+") {
        engine_.setOperation(CalculatorEngine::Operation::Add);
    } else if (button->text() == "-") {
        engine_.setOperation(CalculatorEngine::Operation::Subtract);
    } else if (button->text() == "*") {
        engine_.setOperation(CalculatorEngine::Operation::Multiply);
    } else if (button->text() == "/") {
        engine_.setOperation(CalculatorEngine::Operation::Divide);
    }
    refreshDisplay();
}

void CalculatorWindow::equalClicked() {
    engine_.calculateResult();
    refreshDisplay();
}

void CalculatorWindow::clearClicked() {
    engine_.clear();
    refreshDisplay();
}

void CalculatorWindow::decimalClicked() {
    engine_.inputDecimalSeparator();
    refreshDisplay();
}

void CalculatorWindow::signClicked() {
    engine_.toggleSign();
    refreshDisplay();
}

void CalculatorWindow::percentClicked() {
    engine_.percent();
    refreshDisplay();
}

void CalculatorWindow::refreshDisplay() {
    displayLabel_->setText(engine_.display());
}
