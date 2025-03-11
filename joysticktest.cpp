#include "joysticktest.h"
#include "ui_joysticktest.h"
#include "customlineedit.h"
#include <QDebug>
#include <QLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

joystickTest::joystickTest(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::joystickTest)
{
    ui->setupUi(this);

    replaceLineEdits();
}

joystickTest::~joystickTest()
{
    delete ui;
}

void joystickTest::replaceLineEdits() {
    QList<QLineEdit*> lineEdits = findChildren<QLineEdit*>();  // Получаем все QLineEdit в UI

    for (QLineEdit *oldLineEdit : lineEdits) {
        QWidget *parentWidget = oldLineEdit->parentWidget();  // Получаем родительский виджет

        // Рекурсивно ищем layout для каждого виджета
        QLayout *parentLayout = findParentLayout(parentWidget);

        if (!parentLayout) continue;  // Если родительский layout не найден, пропускаем

        QString name = oldLineEdit->objectName();  // Сохраняем имя

        // Создаём новый CustomLineEdit
        CustomLineEdit *newLineEdit = new CustomLineEdit(parentWidget);
        newLineEdit->setObjectName(name);
        newLineEdit->setText(oldLineEdit->text());
        newLineEdit->setPlaceholderText(oldLineEdit->placeholderText());
        newLineEdit->setEchoMode(oldLineEdit->echoMode());
        newLineEdit->setAlignment(oldLineEdit->alignment());
        newLineEdit->setReadOnly(oldLineEdit->isReadOnly());
        newLineEdit->setFrame(oldLineEdit->hasFrame());
        newLineEdit->setFont(oldLineEdit->font());
        newLineEdit->setStyleSheet(oldLineEdit->styleSheet());
        newLineEdit->setMinimumSize(oldLineEdit->minimumSize());
        newLineEdit->setMaximumSize(oldLineEdit->maximumSize());
        newLineEdit->setSizePolicy(oldLineEdit->sizePolicy());

        // Подключаем сигналы кликов
        connect(newLineEdit, &CustomLineEdit::leftClicked, this, &joystickTest::onLineEditLeftClicked);
        connect(newLineEdit, &CustomLineEdit::rightClicked, this, &joystickTest::onLineEditRightClicked);

        // Удаляем старый QLineEdit и вставляем новый в родительский layout
        if (QHBoxLayout *hboxLayout = qobject_cast<QHBoxLayout*>(parentLayout)) {
            int index = hboxLayout->indexOf(oldLineEdit);
            if (index != -1) {
                hboxLayout->removeWidget(oldLineEdit);
                delete oldLineEdit;
                hboxLayout->insertWidget(index, newLineEdit);
            }
        }
        else if (QVBoxLayout *vboxLayout = qobject_cast<QVBoxLayout*>(parentLayout)) {
            int index = vboxLayout->indexOf(oldLineEdit);
            if (index != -1) {
                vboxLayout->removeWidget(oldLineEdit);
                delete oldLineEdit;
                vboxLayout->insertWidget(index, newLineEdit);
            }
        }
    }
}

QLayout* joystickTest::findParentLayout(QWidget *widget) {
    // Если родитель - это QScrollArea, ищем layout внутри его виджета
    if (QScrollArea *scrollArea = qobject_cast<QScrollArea*>(widget)) {
        return findParentLayout(scrollArea->widget());
    }

    // Если у виджета есть layout, возвращаем его
    if (QLayout *layout = widget->layout()) {
        return layout;
    }

    // Если layout нет, возвращаем nullptr
    return nullptr;
}




void joystickTest::onLineEditLeftClicked(QString name) {
    qDebug() << "Левая кнопка нажата на LineEdit с именем:" << name;
}

void joystickTest::onLineEditRightClicked(QString name) {
    qDebug() << "Правая кнопка нажата на LineEdit с именем:" << name;
}
