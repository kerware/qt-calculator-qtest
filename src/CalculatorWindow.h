#pragma once

#include "CalculatorEngine.h"

#include <QMainWindow>

class QLabel;
class QPushButton;

class CalculatorWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit CalculatorWindow(QWidget* parent = nullptr);

private slots:
    void digitClicked();
    void operationClicked();
    void equalClicked();
    void clearClicked();
    void decimalClicked();
    void signClicked();
    void percentClicked();

private:
    CalculatorEngine engine_;
    QLabel* displayLabel_ = nullptr;

    void buildUi();
    QPushButton* createButton(const QString& text, const QString& objectName);
    void refreshDisplay();
};
