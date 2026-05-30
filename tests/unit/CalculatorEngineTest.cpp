#include "CalculatorEngine.h"

#include <QtTest/QtTest>

class CalculatorEngineTest : public QObject {
    Q_OBJECT

private slots:
    void startsWithZero();
    void addsTwoNumbers();
    void subtractsTwoNumbers();
    void multipliesTwoNumbers();
    void dividesTwoNumbers();
    void rejectsDivisionByZero();
    void supportsDecimals();
    void supportsSignToggle();
    void supportsPercent();
};

void CalculatorEngineTest::startsWithZero() {
    CalculatorEngine engine;
    QCOMPARE(engine.display(), QString("0"));
}

void CalculatorEngineTest::addsTwoNumbers() {
    CalculatorEngine engine;
    engine.inputDigit(1);
    engine.inputDigit(2);
    engine.setOperation(CalculatorEngine::Operation::Add);
    engine.inputDigit(3);
    engine.calculateResult();
    QCOMPARE(engine.display(), QString("15"));
}

void CalculatorEngineTest::subtractsTwoNumbers() {
    CalculatorEngine engine;
    engine.inputDigit(9);
    engine.setOperation(CalculatorEngine::Operation::Subtract);
    engine.inputDigit(4);
    engine.calculateResult();
    QCOMPARE(engine.display(), QString("5"));
}

void CalculatorEngineTest::multipliesTwoNumbers() {
    CalculatorEngine engine;
    engine.inputDigit(6);
    engine.setOperation(CalculatorEngine::Operation::Multiply);
    engine.inputDigit(7);
    engine.calculateResult();
    QCOMPARE(engine.display(), QString("42"));
}

void CalculatorEngineTest::dividesTwoNumbers() {
    CalculatorEngine engine;
    engine.inputDigit(8);
    engine.setOperation(CalculatorEngine::Operation::Divide);
    engine.inputDigit(2);
    engine.calculateResult();
    QCOMPARE(engine.display(), QString("4"));
}

void CalculatorEngineTest::rejectsDivisionByZero() {
    CalculatorEngine engine;
    engine.inputDigit(8);
    engine.setOperation(CalculatorEngine::Operation::Divide);
    engine.inputDigit(0);
    engine.calculateResult();
    QCOMPARE(engine.display(), QString("Erreur"));
}

void CalculatorEngineTest::supportsDecimals() {
    CalculatorEngine engine;
    engine.inputDigit(1);
    engine.inputDecimalSeparator();
    engine.inputDigit(5);
    engine.setOperation(CalculatorEngine::Operation::Add);
    engine.inputDigit(2);
    engine.inputDecimalSeparator();
    engine.inputDigit(5);
    engine.calculateResult();
    QCOMPARE(engine.display(), QString("4"));
}

void CalculatorEngineTest::supportsSignToggle() {
    CalculatorEngine engine;
    engine.inputDigit(5);
    engine.toggleSign();
    QCOMPARE(engine.display(), QString("-5"));
}

void CalculatorEngineTest::supportsPercent() {
    CalculatorEngine engine;
    engine.inputDigit(5);
    engine.inputDigit(0);
    engine.percent();
    QCOMPARE(engine.display(), QString("0.5"));
}

QTEST_MAIN(CalculatorEngineTest)
#include "CalculatorEngineTest.moc"
