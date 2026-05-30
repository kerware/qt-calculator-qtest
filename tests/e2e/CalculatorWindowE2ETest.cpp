#include "CalculatorWindow.h"

#include <QtTest/QtTest>
#include <QLabel>
#include <QPushButton>

class CalculatorWindowE2ETest : public QObject {
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void userCanAddTwoNumbers();
    void userCanChainOperations();
    void userCanUseDecimalValues();
    void userSeesErrorWhenDividingByZero();
    void userCanClearAfterError();

private:
    CalculatorWindow* window_ = nullptr;

    QPushButton* button(const QString& objectName) const;
    QLabel* display() const;
    void click(const QString& objectName) const;
    void expectDisplay(const QString& expected) const;
};

void CalculatorWindowE2ETest::init() {
    window_ = new CalculatorWindow();
    window_->show();
    QVERIFY(QTest::qWaitForWindowExposed(window_));
}

void CalculatorWindowE2ETest::cleanup() {
    delete window_;
    window_ = nullptr;
}

QPushButton* CalculatorWindowE2ETest::button(const QString& objectName) const {
    auto* result = window_->findChild<QPushButton*>(objectName);

    if (result == nullptr) {
        const QString message = "Bouton introuvable: " + objectName;
        QTest::qFail(qPrintable(message), __FILE__, __LINE__);
        return nullptr;
    }

    return result;
}

QLabel* CalculatorWindowE2ETest::display() const {
    auto* result = window_->findChild<QLabel*>("displayLabel");

    if (result == nullptr) {
        QTest::qFail("Display label not found", __FILE__, __LINE__);
        return nullptr;
    }

    return result;
}

void CalculatorWindowE2ETest::click(const QString& objectName) const {
    auto* targetButton = button(objectName);

    if (targetButton == nullptr) {
        return;
    }

    QTest::mouseClick(targetButton, Qt::LeftButton);
}

void CalculatorWindowE2ETest::expectDisplay(const QString& expected) const {
    auto* targetDisplay = display();

    if (targetDisplay == nullptr) {
        return;
    }

    QCOMPARE(targetDisplay->text(), expected);
}

void CalculatorWindowE2ETest::userCanAddTwoNumbers() {
    click("button1");
    click("button2");
    click("buttonAdd");
    click("button3");
    click("buttonEqual");

    expectDisplay("15");
}

void CalculatorWindowE2ETest::userCanChainOperations() {
    click("button2");
    click("buttonAdd");
    click("button3");
    click("buttonMultiply");
    click("button4");
    click("buttonEqual");

    // Cette calculatrice applique l'opération courante à chaque nouvel opérateur : (2 + 3) * 4.
    expectDisplay("20");
}

void CalculatorWindowE2ETest::userCanUseDecimalValues() {
    click("button1");
    click("buttonDecimal");
    click("button5");
    click("buttonAdd");
    click("button2");
    click("buttonDecimal");
    click("button5");
    click("buttonEqual");

    expectDisplay("4");
}

void CalculatorWindowE2ETest::userSeesErrorWhenDividingByZero() {
    click("button8");
    click("buttonDivide");
    click("button0");
    click("buttonEqual");

    expectDisplay("Erreur");
}

void CalculatorWindowE2ETest::userCanClearAfterError() {
    click("button8");
    click("buttonDivide");
    click("button0");
    click("buttonEqual");
    expectDisplay("Erreur");

    click("buttonClear");
    expectDisplay("0");

    click("button9");
    click("buttonSubtract");
    click("button4");
    click("buttonEqual");
    expectDisplay("5");
}

QTEST_MAIN(CalculatorWindowE2ETest)
#include "CalculatorWindowE2ETest.moc"
