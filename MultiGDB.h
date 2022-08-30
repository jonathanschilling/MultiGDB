#ifndef MULTIGDB_H
#define MULTIGDB_H

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

private:
    Ui::MultiGDB *ui;
};
#endif // MULTIGDB_H
