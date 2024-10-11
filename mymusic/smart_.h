#ifndef SMART__H
#define SMART__H

#include <QWidget>

namespace Ui {
class smart_;
}

class smart_ : public QWidget
{
    Q_OBJECT

public:
    explicit smart_(QWidget *parent = nullptr);
    ~smart_();

private:
    Ui::smart_ *ui;
};

#endif // SMART__H
