#ifndef DIALOGCHOOSETHEME_HPP
#define DIALOGCHOOSETHEME_HPP

#include <QDialog>

namespace Ui { class DialogChooseTheme; }

class DialogChooseTheme : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChooseTheme(QWidget* parent = nullptr);
    ~DialogChooseTheme();

private slots:
    void on_pushButton_BackgroundColor_clicked();
    void on_pushButton_TextColor_clicked();

signals:
    void newBackgroundColor(const QColor& bcolor);
    void newTextColor(const QColor& tcolor);

private:
    Ui::DialogChooseTheme* ui;
};

#endif // DIALOGCHOOSETHEME_HPP
