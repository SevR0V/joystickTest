#include "customlineedit.h"
#include <QDebug>

CustomLineEdit::CustomLineEdit(QWidget *parent) : QLineEdit(parent) {
    setStyleSheet("border: 1px solid black; padding: 3px;");  // Визуально выделяем
}

void CustomLineEdit::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit leftClicked(objectName());  // Передаём имя объекта
    } else if (event->button() == Qt::RightButton) {
        emit rightClicked(objectName());
    }
    QLineEdit::mousePressEvent(event);
}
