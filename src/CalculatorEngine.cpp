#include "CalculatorEngine.h"

#include <QtGlobal>
#include <cmath>
#include <limits>

QString CalculatorEngine::display() const {
    if (error_) {
        return "Erreur";
    }
    return currentText_;
}

void CalculatorEngine::inputDigit(int digit) {
    if (digit < 0 || digit > 9) {
        return;
    }

    if (error_) {
        clear();
    }

    if (waitingForNewOperand_) {
        currentText_ = QString::number(digit);
        waitingForNewOperand_ = false;
    } else if (currentText_ == "0") {
        currentText_ = QString::number(digit);
    } else {
        currentText_.append(QString::number(digit));
    }

    currentValue_ = currentText_.toDouble();
}

void CalculatorEngine::inputDecimalSeparator() {
    if (error_) {
        clear();
    }

    if (waitingForNewOperand_) {
        currentText_ = "0.";
        waitingForNewOperand_ = false;
        currentValue_ = 0.0;
        return;
    }

    if (!currentText_.contains('.')) {
        currentText_.append('.');
    }
}

void CalculatorEngine::setOperation(Operation operation) {
    if (error_) {
        return;
    }

    const double operand = currentText_.toDouble();

    if (pendingOperation_ != Operation::None && !waitingForNewOperand_) {
        applyPendingOperation(operand);
        currentText_ = display();
    } else {
        accumulator_ = operand;
    }

    pendingOperation_ = operation;
    waitingForNewOperand_ = true;
}

void CalculatorEngine::calculateResult() {
    if (error_) {
        return;
    }

    if (pendingOperation_ == Operation::None) {
        return;
    }

    const double operand = currentText_.toDouble();
    applyPendingOperation(operand);
    pendingOperation_ = Operation::None;
    waitingForNewOperand_ = true;

    if (!error_) {
        currentText_ = formatNumber(accumulator_);
        currentValue_ = accumulator_;
    }
}

void CalculatorEngine::clear() {
    accumulator_ = 0.0;
    currentValue_ = 0.0;
    pendingOperation_ = Operation::None;
    currentText_ = "0";
    waitingForNewOperand_ = true;
    error_ = false;
}

void CalculatorEngine::toggleSign() {
    if (error_) {
        clear();
    }

    if (currentText_ == "0") {
        return;
    }

    if (currentText_.startsWith('-')) {
        currentText_.remove(0, 1);
    } else {
        currentText_.prepend('-');
    }
    currentValue_ = currentText_.toDouble();
}

void CalculatorEngine::percent() {
    if (error_) {
        clear();
    }

    setCurrentValue(currentText_.toDouble() / 100.0);
}

void CalculatorEngine::applyPendingOperation(double rightOperand) {
    switch (pendingOperation_) {
    case Operation::Add:
        accumulator_ += rightOperand;
        break;
    case Operation::Subtract:
        accumulator_ -= rightOperand;
        break;
    case Operation::Multiply:
        accumulator_ *= rightOperand;
        break;
    case Operation::Divide:
        if (qFuzzyIsNull(rightOperand)) {
            error_ = true;
            currentText_ = "Erreur";
            return;
        }
        accumulator_ /= rightOperand;
        break;
    case Operation::None:
        accumulator_ = rightOperand;
        break;
    }
}

void CalculatorEngine::setCurrentValue(double value) {
    currentValue_ = value;
    currentText_ = formatNumber(value);
}

QString CalculatorEngine::formatNumber(double value) const {
    if (!std::isfinite(value)) {
        return "Erreur";
    }

    QString result = QString::number(value, 'g', 12);
    if (result == "-0") {
        return "0";
    }
    return result;
}
