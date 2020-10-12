#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <translation.h>
#include <QMainWindow>
#include <QString>
#include <QtDebug>
#include <QtSql>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_comboBox_choose_language_currentIndexChanged(const QString &arg1);

    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    void set_text(const QString & title_text,
                  const QString & choose_language,
                  const QString & text_to_find,
                  const QString & button1_text,
                  const QString & button2_text,
                  const QString & button3_text,
                  const QString & button4_text);

    Ui::MainWindow *ui;

    QSqlDatabase _sql;
    bool is_open;
};

#endif // MAINWINDOW_H
