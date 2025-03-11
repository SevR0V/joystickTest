#ifndef JOYSTICKTEST_H
#define JOYSTICKTEST_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class joystickTest;
}
QT_END_NAMESPACE

class joystickTest : public QMainWindow
{
    Q_OBJECT

public:
    joystickTest(QWidget *parent = nullptr);
    ~joystickTest();

private slots:
    void onLineEditLeftClicked(QString name);
    void onLineEditRightClicked(QString name);

private:
    Ui::joystickTest *ui;
    void replaceLineEdits();
    QLayout* findParentLayout(QWidget* widget);
};
#endif // JOYSTICKTEST_H
