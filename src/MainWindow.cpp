#include "MainWindow.hpp"
#include "./ui_MainWindow.h"

#include <QColor>
#include <QPalette>
#include <QFont>

#include <QFileDialog>
#include <QFontDialog>

#include <QMessageBox>

static constexpr uint16_t STATUSBAR_MESSAGE_TIMEOUT{2800};



MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent}
    , ui{new Ui::MainWindow}
    , current_file{}
    , current_filename{}
    , system_clipboard{QGuiApplication::clipboard()}
    , dialog_choose_theme{this}
{
    ui->setupUi(this);
    setMinimumSize(width() - 250, height() - 180);

    connect(ui->textEdit, &QTextEdit::copyAvailable, ui->actionCopy, &QAction::setEnabled);
    connect(ui->textEdit, &QTextEdit::copyAvailable, ui->actionCut, &QAction::setEnabled);

    connect(&dialog_choose_theme, &DialogChooseTheme::newBackgroundColor, this, [this](const QColor& bcolor)
    {
        QPalette palette{ui->textEdit->palette()};
        palette.setColor(QPalette::ColorRole::Base, std::move(bcolor));
        ui->textEdit->setPalette(std::move(palette));
    });

    connect(&dialog_choose_theme, &DialogChooseTheme::newTextColor, this, [this](const QColor& tcolor)
    {
        QPalette palette{ui->textEdit->palette()};
        palette.setColor(QPalette::ColorRole::Text, std::move(tcolor));
        ui->textEdit->setPalette(std::move(palette));
    });
}



MainWindow::~MainWindow()
{
    if (current_file.isOpen()) {
        current_file.close();
    }
    delete ui;
}



void MainWindow::on_actionOpen_triggered()
{
    QFileDialog dialog_open_file{this, "Choose file..."};
    dialog_open_file.setAcceptMode(QFileDialog::AcceptMode::AcceptOpen);

    if (dialog_open_file.exec() != QDialog::DialogCode::Accepted) {
        return;
    }

    ui->textEdit->clear();
    current_filename = dialog_open_file.selectedFiles().constFirst();

    if (!loadFile()) {
        QMessageBox::critical(this, "Error", QString{"Could not load file: %1"}.arg(current_filename));
        return;
    }

    ui->statusbar->showMessage(QString{"File loaded: %1"}.arg(current_filename), STATUSBAR_MESSAGE_TIMEOUT);
}



void MainWindow::on_actionExit_triggered()
{
    close();
}



void MainWindow::on_actionNew_triggered()
{
    ui->textEdit->clear();
    on_actionSave_as_triggered();
}



void MainWindow::on_actionClose_triggered()
{
    ui->statusbar->showMessage(QString{"File closed: %1"}.arg(current_filename), STATUSBAR_MESSAGE_TIMEOUT);
    ui->textEdit->clear();
    current_filename.clear();
}



void MainWindow::on_actionSave_triggered()
{
    if (current_filename.isEmpty()) {
        on_actionSave_as_triggered();
    }
    else if (ui->textEdit->document()->isModified()) {
        saveFile();
        ui->statusbar->showMessage(QString{"File saved: %1"}.arg(current_filename), STATUSBAR_MESSAGE_TIMEOUT);
    }
}



void MainWindow::on_actionSave_as_triggered()
{
    QFileDialog dialog_save_file{this, "Save file as..."};
    dialog_save_file.setAcceptMode(QFileDialog::AcceptMode::AcceptSave);

    if (dialog_save_file.exec() != QDialog::DialogCode::Accepted) {
        return;
    }

    current_filename = dialog_save_file.selectedFiles().constFirst();

    if (!saveFile()) {
        QMessageBox::critical(this, "Error", QString{"Could not save file: %1"}.arg(current_filename));
        current_filename.clear();
        return;
    }

    ui->statusbar->showMessage(QString{"File saved as: %1"}.arg(current_filename), STATUSBAR_MESSAGE_TIMEOUT);
}



void MainWindow::on_actionFull_screen_triggered(bool checked)
{
    if (checked) {
        showFullScreen();
    }
    else {
        showNormal();
    }
}



bool MainWindow::loadFile()
{
    if (current_file.isOpen()) {
        current_file.close();
    }

    current_file.setFileName(current_filename);

    if (!current_file.open(QIODevice::ReadOnly)) {
        return false;
    }

    const auto data{current_file.readAll()};
    current_file.close();

    ui->textEdit->document()->setPlainText(data);
    ui->textEdit->document()->setModified(false);

    return true;
}



bool MainWindow::saveFile()
{
    if (current_file.isOpen()) {
        current_file.close();
    }

    current_file.setFileName(current_filename);

    if (!current_file.open(QIODevice::WriteOnly)) {
        return false;
    }

    const auto data{ui->textEdit->toPlainText().toUtf8()};

    current_file.write(data);
    current_file.close();

    ui->textEdit->document()->setModified(false);

    return true;
}



void MainWindow::on_actionCopy_triggered()
{
    const QStringView selected_text{ui->textEdit->textCursor().selectedText()};
    system_clipboard->setText(selected_text.toString());
}



void MainWindow::on_actionCut_triggered()
{
    const QStringView selected_text{ui->textEdit->textCursor().selectedText()};
    ui->textEdit->textCursor().removeSelectedText();
    system_clipboard->setText(selected_text.toString());
}



void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->insertPlainText(system_clipboard->text(QClipboard::Mode::Clipboard));
}



void MainWindow::on_actionCurrent_file_triggered()
{
    const QStringView action_text{static_cast<QAction*>(sender())->text()};
    QMessageBox::information(this, "Debug info: " + action_text.toString(), current_filename.isEmpty() ? "Empty" : current_filename);
}



void MainWindow::on_actionWindow_size_triggered()
{
    const QStringView action_text{static_cast<QAction*>(sender())->text()};
    QMessageBox::information(this, "Debug info: " + action_text.toString(), QString{"Height: %1\nWidth: %2"}.arg(height()).arg(width()));
}



void MainWindow::on_actionShow_cursor_position_in_status_bar_triggered(bool checked)
{
    if (checked) {
        connect(ui->textEdit, &QTextEdit::cursorPositionChanged, this, &MainWindow::cursorPositionShow);
    }
    else {
        disconnect(ui->textEdit, &QTextEdit::cursorPositionChanged, this, &MainWindow::cursorPositionShow);
    }
}



void MainWindow::on_actionColorTheme_triggered()
{
    dialog_choose_theme.show();
}



void MainWindow::cursorPositionShow()
{
    const QTextCursor cursor{ui->textEdit->textCursor()};
    ui->statusbar->showMessage(QString{"Global pos: [%1] | In block pos [%2]"}.arg(cursor.position()).arg(cursor.positionInBlock()));
}



void MainWindow::on_actionAbout_Qt_triggered()
{
    QApplication::aboutQt();
}



void MainWindow::on_actionFont_triggered()
{
    bool ok{};
    const QFont new_font{QFontDialog::getFont(&ok, this)};

    if (!ok) {
        return;
    }

    ui->textEdit->setFont(new_font);
}
