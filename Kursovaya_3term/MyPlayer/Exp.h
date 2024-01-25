#ifndef EXP_H
#define EXP_H
#include <QDialog>
#include <stdexcept>
#include <QErrorMessage>
#include <QDebug>
#include <QDir>
// Dialog
class Exp_messages
{
    QString a = "File is not open.";
    QString b = "File is not exist.";

public:

    Exp_messages();
    QString ret_a()
    {
        return a;
    }
    QString ret_b()
    {
        return b;
    }
    ~Exp_messages();

};


class Exp_file : public QErrorMessage
{
    Q_OBJECT
    int Error_code = 1;
public:
    Exp_file(QWidget * parent = 0 );
    int ret_code()
    {
        return Error_code;
    }

};
#endif // EXP_H
