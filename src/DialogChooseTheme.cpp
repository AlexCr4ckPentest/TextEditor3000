#include "DialogChooseTheme.hpp"
#include "ui_DialogChooseTheme.h"

#include <QColor>
#include <QColorDialog>



DialogChooseTheme::DialogChooseTheme(QWidget* parent)
    : QDialog{parent}
    , ui{new Ui::DialogChooseTheme}
{
    ui->setupUi(this);
    setMinimumSize(size());
    setMaximumSize(size());
}



DialogChooseTheme::~DialogChooseTheme()
{
    delete ui;
}



void DialogChooseTheme::on_pushButton_BackgroundColor_clicked()
{
    const QColor background_color{QColorDialog::getColor(Qt::GlobalColor::white, this, "Choose background color...")};
    if (background_color.isValid()) {
        emit newBackgroundColor(background_color);
    }
}



void DialogChooseTheme::on_pushButton_TextColor_clicked()
{
    const QColor text_color{QColorDialog::getColor(Qt::GlobalColor::white, this, "Choose background color...")};
    if (text_color.isValid()) {
        emit newTextColor(text_color);
    }
}
