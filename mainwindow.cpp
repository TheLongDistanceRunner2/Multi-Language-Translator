#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ustawiamy poczatkowe teksty:
    set_text("Translator", "Choose the language to translate from: ", "English" , "English-Polish", "English-German", "English-French", "English-Italian");

    // ustawiamy tytuł okna:
    QMainWindow::setWindowTitle("Multi-language translator! v1.0");

    // tworzymy bazę danych:
    _sql = QSqlDatabase::addDatabase("QSQLITE", "baza");
    _sql.setDatabaseName("C:\\Users\\Administrator\\Desktop\\TRANSLATOR\\Translator\\database\\baza_danych.sqlite3");
    _sql.close();
    _sql.open();

    // tworzymy tabelę, jeśli nie istnieje:
    QSqlQuery zapytanie(_sql);
    // tworzymy tabelę:
    QString execute = "CREATE TABLE IF NOT EXISTS tabela (id INTEGER UNIQUE PRIMARY KEY, input_language TEXT, output_language TEXT, entered_text TEXT, translation TEXT);";
    zapytanie.exec(execute);
    //qDebug() << "isActive: Tworzenie tabeli" << zapytanie.isActive();
    zapytanie.clear();

    is_open = _sql.open();
    //qDebug() << "1. Baza otwarta";
    //qDebug() << "Stworzenie bazy..." << zapytanie.lastError();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_text(const QString &title_text, const QString &choose_language, const QString &text_to_find, const QString &button1_text, const QString &button2_text, const QString &button3_text, const QString &button4_text)
{
    //=======================================================
    // ustawienie tekstu tytułowego:

    ui->label_title->setText(title_text);

    //======================================================
    // ustawienie tekstu zachęty do wybrania języka:

    ui->label_choose_language->setText(choose_language);

    //=======================================================
    // ustawienie tekstu w comboBox-ie:

    QString textToFind = text_to_find;
    int index = ui->comboBox_choose_language->findText(textToFind);
    ui->comboBox_choose_language->setCurrentIndex(index);

    //=======================================================
    // ustawienie tekstu na przyciskach wyboru tłumaczenia:

    ui->pushButton_1->setText(button1_text);
    ui->pushButton_2->setText(button2_text);
    ui->pushButton_3->setText(button3_text);
    ui->pushButton_4->setText(button4_text);
}



void MainWindow::on_comboBox_choose_language_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "Polski")
    {
        set_text("Tłumacz", "Wybierz język, z którego chcesz tłumaczyć: ", "Polski" , "Polsko-Angielski", "Polsko-Niemiecki", "Polsko-Francuski", "Polsko-Włoski");
    }
    else if(arg1 == "English")
    {
        set_text("Translator", "Choose the language to translate from: ", "English" , "English-Polish", "English-German", "English-French", "English-Italian");
    }
    else if(arg1 == "Deutsch")
    {
        set_text("Übersetzer", "Wählen Sie die Sprache, aus der Sie übersetzen möchten: ", "Deutsch" , "Deutsch-Polnisch", "Deutsch-Englisch", "Deutsch-Französisch", "Deutsch-Italienisch");
    }
    else if(arg1 == "Français")
    {
        set_text("Traducteur", "Veuillez choisir la langue à partir de laquelle vous souhaitez traduire: ", "Français" , "Français-Polonaises", "Français-Allemande", "Français-Anglais", "Français-Italienne");
    }
    else if(arg1 == "Italiana")
    {
        set_text("Traduttore", "Scegli la lingua da cui vuoi tradurre: ", "Italiana" , "Italiana-Inglese", "Italiana-Tedesca", "Italiana-Francese", "Italiana-Polacca");
    }

}

void MainWindow::on_pushButton_1_clicked()
{
    // gdy połączono z bazą danych:
    if(is_open == true)
    {
        if (ui->comboBox_choose_language->currentText() == "English")
        {
            Translation translation1_1("en", "pol", "English to Polish translator: ", _sql, "en-pol", "Translate!", this);
            //ustawiamy komponenty okna w zależności od języka:
            translation1_1.set_labels_and_button_text("English to Polish translator", "Enter text to translate: ", "Translate!");
            translation1_1.exec();
        }

        if(ui->comboBox_choose_language->currentText() == "Polski")
        {
            Translation translation1_2("pol", "en", "Tłumacz polsko-angielski: ", _sql, "pol-en", "Przetłumacz!", this);
            //ustawiamy komponenty okna w zależności od języka:
            translation1_2.set_labels_and_button_text("Tłumacz polsko-angielski", "Wprowadź tekst to przetłumaczenia: ", "Przetłumacz!");
            translation1_2.exec();
        }

        if (ui->comboBox_choose_language->currentText() == "Français")
        {
            Translation translation1_3("fre", "pol", "Traducteur Français-Polonaises: ", _sql, "fre-pol", "Traducteur!", this);
            //ustawiamy komponenty okna w zależności od języka:
            translation1_3.set_labels_and_button_text("Traducteur Français-Polonaises:", "Entrez le texte à traduire: ", "Traducteur!");
            translation1_3.exec();
        }

        if (ui->comboBox_choose_language->currentText() == "Deutsch")
        {
            Translation translation1_4("ger", "pol", "Deutsch Übersetzer für Polnisch: ", _sql, "ger-pol", "Ubersatz!", this);
            //ustawiamy komponenty okna w zależności od języka:
            translation1_4.set_labels_and_button_text("Deutsch Übersetzer für Polnisch:", "Geben Sie den Text zu übersetzenden ein: ", "Ubersatz!");
            translation1_4.exec();
        }

        if (ui->comboBox_choose_language->currentText() == "Italiana")
        {
            Translation translation1_5("ita", "en", "Traduttore italiano-inglese: ", _sql, "ita-en", "Traduttore!!", this);
            //ustawiamy komponenty okna w zależności od języka:
            translation1_5.set_labels_and_button_text("Traduttore italiano-inglese", "Inserisci il testo da tradurre: ", "Traduttore!!");
            translation1_5.exec();
        }
    }
    else
    {
        qDebug() << "Nie udało się połączyć z bazą... !!!";
    }



}

void MainWindow::on_pushButton_2_clicked()
{
    // gdy połączono z bazą danych:
    if(is_open == true)
    {

        if (ui->comboBox_choose_language->currentText() == "English")
        {
            Translation translation2_1("en", "ger", "English to German translator: ", _sql, "en-ger", "Translate!", this);
            //ustawiamy komponenty okna w zależności od języka:
            translation2_1.set_labels_and_button_text("English to German translator", "Enter text to translate: ", "Translate!");
            translation2_1.exec();
        }

        if(ui->comboBox_choose_language->currentText() == "Polski")
        {
            Translation translation2_2("pol", "ger", "Tłumacz polsko-niemiecki: ", _sql, "pol-ger", "Przetłumacz!", this);
            //ustawiamy komponenty okna w zależności od języka:
            translation2_2.set_labels_and_button_text("Tłumacz polsko-niemiecki", "Wprowadź tekst to przetłumaczenia: ", "Przetłumacz!");
            translation2_2.exec();
        }

        if (ui->comboBox_choose_language->currentText() == "Français")
        {
            Translation translation2_3("fre", "ger", "Traducteur Français-Allemande: ", _sql, "fre-ger", "Traducteur!", this);
            //ustawiamy komponenty okna w zależności od języka:
            translation2_3.set_labels_and_button_text("Traducteur Français-Allemande:", "Entrez le texte à traduire: ", "Traducteur!");
            translation2_3.exec();
        }

        if (ui->comboBox_choose_language->currentText() == "Deutsch")
        {
            Translation translation2_4("ger", "en", "Deutsch-Polnischer Übersetzer: ", _sql, "ger-en", "Ubersatz!", this);
            //ustawiamy komponenty okna w zależności od języka:
            translation2_4.set_labels_and_button_text("Deutsch-Polnischer Übersetzer", "Geben Sie den Text zu übersetzenden ein: ", "Ubersatz!");
            translation2_4.exec();
        }

        if (ui->comboBox_choose_language->currentText() == "Italiana")
        {
            Translation translation2_5("ita", "ger", "Traduttore italiano-tedesca: ", _sql, "ita-ger", "Traduttore!!", this);
            //ustawiamy komponenty okna w zależności od języka:
            translation2_5.set_labels_and_button_text("Traduttore italiano-tedesca", "Inserisci il testo da tradurre: ", "Traduttore!!");
            translation2_5.exec();
        }
    }
    else
    {
        qDebug() << "Nie udało się połączyć z bazą... !!!";
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    // gdy połączono z bazą danych:
    if(is_open == true)
    {
        if (ui->comboBox_choose_language->currentText() == "English")
        {
            Translation translation3_1("en", "fre", "English to French translator: ", _sql, "en-fre", "Translate!", this);
            //ustawiamy komponenty okna w zależności od języka:
            translation3_1.set_labels_and_button_text("English to French translator", "Enter text to translate: ", "Translate!");
            translation3_1.exec();
        }

        if(ui->comboBox_choose_language->currentText() == "Polski")
        {
            Translation translation3_2("pol", "fre", "Tłumacz polsko-francuski: ", _sql, "pol-fre", "Przetłumacz!", this);
            //ustawiamy komponenty okna w zależności od języka:
            translation3_2.set_labels_and_button_text("Tłumacz polsko-francuski", "Wprowadź tekst to przetłumaczenia: ", "Przetłumacz!");
            translation3_2.exec();
        }

        if (ui->comboBox_choose_language->currentText() == "Français")
        {
            Translation translation3_3("fre", "en", "Traducteur Français-Anglais: ", _sql, "fre-en", "Traducteur!", this);
            //ustawiamy komponenty okna w zależności od języka:
            translation3_3.set_labels_and_button_text("Traducteur Français-Anglais:", "Entrez le texte à traduire: ", "Traducteur!");
            translation3_3.exec();
        }

        if (ui->comboBox_choose_language->currentText() == "Deutsch")
        {
            Translation translation3_4("ger", "fre", "Deutsch-Franzosisch Übersetzer: ", _sql, "ger-fre", "Ubersatz!", this);
            //ustawiamy komponenty okna w zależności od języka:
            translation3_4.set_labels_and_button_text("Deutsch-Franzosisch Übersetzer", "Geben Sie den Text zu übersetzenden ein: ", "Ubersatz!");
            translation3_4.exec();
        }

        if (ui->comboBox_choose_language->currentText() == "Italiana")
        {
            Translation translation3_5("ita", "fre", "Traduttore italiano-francese: ", _sql, "ita-fre", "Traduttore!!", this);
            //ustawiamy komponenty okna w zależności od języka:
            translation3_5.set_labels_and_button_text("Traduttore italiano-francese", "Inserisci il testo da tradurre: ", "Traduttore!!");
            translation3_5.exec();
        }
    }
    else
    {
        qDebug() << "Nie udało się połączyć z bazą... !!!";
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    // gdy połączono z bazą danych:
    if(is_open == true)
    {
        if (ui->comboBox_choose_language->currentText() == "English")
        {
            Translation translation4_1("en", "ita", "English to Italian translator: ", _sql, "en-ita", "Translate!", this);
            //ustawiamy komponenty okna w zależności od języka:
            translation4_1.set_labels_and_button_text("English to Italian translator", "Enter text to translate: ", "Translate!");
            translation4_1.exec();
        }

        if(ui->comboBox_choose_language->currentText() == "Polski")
        {
            Translation translation4_2("pol", "ita", "Tłumacz polsko-włoski: ", _sql, "pol-ita", "Przetłumacz!", this);
            //ustawiamy komponenty okna w zależności od języka:
            translation4_2.set_labels_and_button_text("Tłumacz polsko-włoski", "Wprowadź tekst to przetłumaczenia: ", "Przetłumacz!");
            translation4_2.exec();
        }

        if (ui->comboBox_choose_language->currentText() == "Français")
        {
            Translation translation4_3("fre", "ita", "Traducteur Français-Italienne: ", _sql, "fre-ita", "Traducteur!", this);
            //ustawiamy komponenty okna w zależności od języka:
            translation4_3.set_labels_and_button_text("Traducteur Français-Italienne:", "Entrez le texte à traduire: ", "Traducteur!");
            translation4_3.exec();
        }

        if (ui->comboBox_choose_language->currentText() == "Deutsch")
        {
            Translation translation4_4("ger", "ita", "Deutsch-Italienisch Übersetzer: ", _sql, "ger-ita", "Ubersatz!", this);
            //ustawiamy komponenty okna w zależności od języka:
            translation4_4.set_labels_and_button_text("Deutsch-Italienisch Übersetzer", "Geben Sie den Text zu übersetzenden ein: ", "Ubersatz!");
            translation4_4.exec();
        }

        if (ui->comboBox_choose_language->currentText() == "Italiana")
        {
            Translation translation5_5("ita", "pol", "Traduttore italiano-polacca: ", _sql, "ita-pol", "Traduttore!!", this);
            //ustawiamy komponenty okna w zależności od języka:
            translation5_5.set_labels_and_button_text("Traduttore italiano-polacca", "Inserisci il testo da tradurre: ", "Traduttore!!");
            translation5_5.exec();
        }
    }
    else
    {
        qDebug() << "Nie udało się połączyć z bazą... !!!";
    }
}
