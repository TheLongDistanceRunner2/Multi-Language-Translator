#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <QDialog>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtDebug>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QtSql>

namespace Ui {
class Translation;
}

class Translation : public QDialog
{
    Q_OBJECT

public:
    explicit Translation(QString input_language, QString output_language, QString title_text,
                         QSqlDatabase & _sql, QString table_name,
                         QString button_translate_text, QWidget *parent);
    ~Translation();

    void set_labels_and_button_text(QString window_title, QString title_text, QString button_text);

private slots:

    void on_pushButton_translate_clicked();
    void set_text(QString text);
    QString translte();
    bool search_in_table();
    void add_to_table(QString add);
    void translation_logic(QString processing_text, QString inappropriate_text, QString not_found_text);

private:
    Ui::Translation *ui;

    QString text;
    QString input_language;
    QString output_language;
    QString title_text;
    QString table_name;
    QSqlDatabase _sql;
    QString button_translate_text;

    QString to_return;

};

#endif // TRANSLATION_H
