#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QFile>
#include <QClipboard>

#include "DialogChooseTheme.hpp"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
/// --------------------------------------
///         "File" menu tab slots
/// --------------------------------------
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionClose_triggered();
    void on_actionSave_triggered();
    void on_actionSave_as_triggered();
    void on_actionExit_triggered();

/// --------------------------------------
///         "Edit" menu tab slots
/// --------------------------------------
    void on_actionCopy_triggered();
    void on_actionCut_triggered();
    void on_actionPaste_triggered();

/// --------------------------------------
///         "View" menu tab slots
/// --------------------------------------
    void on_actionFull_screen_triggered(bool checked);
    void on_actionColorTheme_triggered();
    void on_actionFont_triggered();

/// --------------------------------------
///         "Tools" menu tab slots
/// --------------------------------------
    void on_actionShow_cursor_position_in_status_bar_triggered(bool checked);
    void cursorPositionShow();

/// --------------------------------------
///         "Debug" menu tab slots
/// --------------------------------------
    void on_actionCurrent_file_triggered();
    void on_actionWindow_size_triggered();

/// --------------------------------------
///         "Help" menu tab slots
/// --------------------------------------
    void on_actionAbout_Qt_triggered();

private:
    Ui::MainWindow* ui;
    QFile current_file;
    QString current_filename;
    QClipboard* system_clipboard;

    DialogChooseTheme dialog_choose_theme;

    bool loadFile();
    bool saveFile();
};

#endif // MAINWINDOW_HPP
