#include "MultiGDB.h"
#include "ui_MultiGDB.h"

MultiGDB::MultiGDB(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MultiGDB)
{
    ui->setupUi(this);
}

MultiGDB::~MultiGDB()
{
    delete ui;
}

