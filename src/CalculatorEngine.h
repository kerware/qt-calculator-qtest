#pragma once

#include <QString>

class CalculatorEngine {
public:
    enum class Operation {
        None,
        Add,
        Subtract,
        Multiply,
        Divide
    };

    QString display() const;

    void inputDigit(int digit);
    void inputDecimalSeparator();
    void setOperation(Operation operation);
    void calculateResult();
    void clear();
    void toggleSign();
    void percent();

private:
    double accumulator_ = 0.0;
    double currentValue_ = 0.0;
    Operation pendingOperation_ = Operation::None;
    QString currentText_ = "0";
    bool waitingForNewOperand_ = true;
    bool error_ = false;

    void applyPendingOperation(double rightOperand);
    void setCurrentValue(double value);
    QString formatNumber(double value) const;
};
