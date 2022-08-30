#ifndef MULTIGDB_H
#define MULTIGDB_H

#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

#include <string>
#include <iostream>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MultiGDB; }
QT_END_NAMESPACE

class MultiGDB : public QMainWindow
{
    Q_OBJECT

public:
    MultiGDB(QWidget *parent = nullptr);
    ~MultiGDB();

public slots:
    void doItPushed(const bool &);

private:
    Ui::MultiGDB *ui;
};
#endif // MULTIGDB_H
