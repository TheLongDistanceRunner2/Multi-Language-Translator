#include "translation.h"
#include "ui_translation.h"


Translation::Translation(QString input_language, QString output_language, QString title_text,
                         QSqlDatabase & sql, QString table_name,
                         QString button_translate_text, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Translation)
{
    ui->setupUi(this);

    // ustawiamy początkowe wartości:
    this->input_language = input_language;
    this->output_language = output_language;
    this->title_text = title_text;
    this->_sql = sql;
    this->table_name = table_name;
    this->button_translate_text = button_translate_text;

    // dwa dolne pola tekstowe ustawiamy początkowo na puste:
    ui->label_3_result->setText("");
    ui->label_2_added_to_database->setText("");

    this->to_return = "";
}

Translation::~Translation()
{
    delete ui;
}

void Translation::set_labels_and_button_text(QString window_title, QString title_text, QString button_text)
{
    QDialog::setWindowTitle(window_title);
    ui->label_title->setText(title_text);
    ui->pushButton_translate->setText(button_text);
}

void Translation::on_pushButton_translate_clicked()
{
    set_text(ui->lineEdit_enter_text->text());
    QString translation = "";

    //===========================================================================================
    //===========================================================================================
    // przyciski dla wersji English:

    if( (input_language == "en" && output_language == "pol")
        || (input_language == "en" && output_language == "ger")
        || (input_language == "en" && output_language == "fre")
        || (input_language == "en" && output_language == "ita") )
    {
        // gdy wpisano cokolwiek:
        if(text != "")
        {
            translation_logic("processing...", "You cannot use \" @ # etc. symbols", "Translation not found");
        }
        else
        {
            ui->label_3_result->setText("You haven't entered anything yet!");
        }
    }

    //=======================================================================================================
    //=======================================================================================================
    // przyciski dla wersji Polski:

    else if ( (input_language == "pol" && output_language == "en")
                || (input_language == "pol" && output_language == "ger")
                || (input_language == "pol" && output_language == "fre")
                || (input_language == "pol" && output_language == "ita") )
    {
        // gdy wpisano cokolwiek:
        if(text != "")
        {
            translation_logic("przetwarzanie...", "Nie wolno wpisywać symboli \" @ # etc. ", "Nie znaleziono tłumaczenia");
        }
        else
        {
            ui->label_3_result->setText("Nie wpisano żadnego tekstu!");
        }
    }

    //=======================================================================================================
    //=======================================================================================================
    // przyciski dla wersji Francais:

    else if ( (input_language == "fre" && output_language == "pol")
                || (input_language == "fre" && output_language == "ger")
                || (input_language == "fre" && output_language == "en")
                || (input_language == "fre" && output_language == "ita") )
    {
        // gdy wpisano cokolwiek:
        if(text != "")
        {
            translation_logic("traitement...", "Les symboles \" @ # etc. ne sont pas autorisés", "Traduction non trouvée");
        }
        else
        {
            ui->label_3_result->setText("Aucun test entré!");
        }
    }

    //=======================================================================================================
    //=======================================================================================================
    // przyciski dla wersji Deutsch:

    else if ( (input_language == "ger" && output_language == "pol")
                || (input_language == "ger" && output_language == "en")
                || (input_language == "ger" && output_language == "fre")
                || (input_language == "ger" && output_language == "ita") )
    {
        // gdy wpisano cokolwiek:
        if(text != "")
        {
            translation_logic("verarbeitung...", "Symbole \" @ # etc. sind nicht erlaubt", "Übersetzung nicht gefunden");
        }
        else
        {
            ui->label_3_result->setText("Du hast nicht Text eingegeben!");
        }
    }

    //=======================================================================================================
    //=======================================================================================================
    // przyciski dla wersji Italiana:

    else if ( (input_language == "ita" && output_language == "en")
                || (input_language == "ita" && output_language == "ger")
                || (input_language == "ita" && output_language == "fre")
                || (input_language == "ita" && output_language == "pol") )
    {
        if(text != "")
        {
            translation_logic("lavorazione...", "I simboli \" @ # etc. non sono ammessi", "Nessun testo inserito!");
        }
        else
        {
            ui->label_3_result->setText("Nessun testo inserito!");
        }
    }
    else
    {
        ui->label_3_result->setText("");
    }

}

void Translation::set_text(QString text)
{
    this->text = text;
}

bool Translation::search_in_table()
{
    this->to_return = "";
    QString translated_text = "";
    QString entered_text = text;
    //qDebug() << "text: " << text;

    QSqlQuery _query(_sql);
    _query.clear();

    int rows = -1;

    QString execute = "SELECT COUNT(*) FROM tabela WHERE (input_language='" + input_language + "') AND (output_language='" + output_language + "') AND (entered_text='" + entered_text + "');";
    //qDebug() << "String po zbindowaniu: " << execute;
    _query.prepare(execute);
    _query.exec();

    if (_query.next())
    {
        rows = _query.value(0).toInt();
    }
    else
    {
        qDebug() << _query.lastError();
        rows = 0;
    }

    // gdy znaleziono tłumaczenie w baze, przekaż je do zmiennej to_return:
    if(rows == 1)
    {
        // przeszukujemy baze by wyłluskać to tłumaczenie:
        _query.exec("SELECT translation FROM tabela WHERE input_language='" + input_language +
                    "' AND output_language='" + output_language +
                    "' AND entered_text='" + entered_text + "';");
         //qDebug() << "isActive: przeszukanie bazy by znaleźć tłumaczenie" << _query.isActive();
        _query.next();
        translated_text = _query.value(0).toString();
        _query.clear();

        // przekazujemy tekst do zmiennej to_return:
        this->to_return = translated_text.toUtf8();

        return true;
    }
    else if(rows == 0)
    {
        //qDebug() << " -- >> NIE ZNALEZIONO TŁUMACZENIA W BAZIE...";
        return false;
    }
    else
    {
        //qDebug() << "NIC SIE NIE WYKONALO";
        return false;
    }
}

QString Translation::translte()
{
        QString _q = this->text;
        QString _langpair = this->input_language + "|" + this->output_language;

        // tworzymy URL:
        QString url = QString("https://api.mymemory.translated.net/get?q=%1&langpair=%2").arg(_q).arg(_langpair);

        // tworzymy połączenie:
        QNetworkAccessManager manager;
        QNetworkRequest request(url);
        QNetworkReply *reply = manager.get(request);

        // odbieramy dane od API:
        do
        {
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        }
        while(!reply->isFinished());

        //=============================================================
        // parsowanie odebranego JSONa:

        QString _response_data = reply->readAll();
        QJsonDocument document = QJsonDocument::fromJson(_response_data.toUtf8());

        // tworzymy obiekt:
        QJsonObject jObject = document.object();

        // sczytujemy wartość dla responseData:
        QJsonValue value = jObject.value(QString("responseData"));
        //qWarning() << value;

        // sczytaj dane tego obiektu (responseData):
        QJsonObject item = value.toObject();
        //qWarning() << tr("QJsonObject of translatedText: ") << item["translatedText"];

        // wyciągamy dane z tego sub-obiektu:
        //qWarning() << tr("QJsonObject[responseData] of translatedText: ") << item["translatedText"];
        QJsonValue subobj = item["translatedText"];
        //qWarning() << subobj.toString();

        // przypisujemy wartosc subobj do zmiennej _translation:
        QString _traslation = subobj.toString();

        // czyścimy zapytanie:
        delete reply;

        return _traslation;
}

void Translation::add_to_table(QString add)
{
    QString entered_text = text.toUtf8();

    QSqlQuery _query(_sql);
    QString execute = "INSERT INTO tabela (input_language, output_language, entered_text, translation) VALUES ('" + input_language + "', '" + output_language + "', '" + entered_text  + "', '" + add + "');";
    _query.prepare(execute);
    _query.exec();
    _query.next();
    //====================================================================================  <<---
    // DEBUGOWANIE:
    //qDebug() << "isActive: Dodawanie do tabeli " << _query.isActive();
    //qDebug() << "Ostatnie poprawne zapytanie SQL: " << _query.executedQuery();
    _query.clear();

    //====================================================================================  <<---
    // DEBUGOWANIE:
    //qDebug() << " -- >> DODANO DO TABELI";
}

void Translation::translation_logic(QString processing_text, QString inappropriate_text, QString not_found_text)
{
    // ustawiamy wiadomość o wyszukiwaniu tłumaczenia:
    ui->label_3_result->setText(processing_text);

    // gdy wpisany tekst zawiera nieodpowiednie znaki:
    if((text.contains("\"", Qt::CaseInsensitive) == true) ||
       (text.contains(":", Qt::CaseInsensitive) == true)  ||
       (text.contains("/", Qt::CaseInsensitive) == true)  ||
       (text.contains("<", Qt::CaseInsensitive) == true)  ||
       (text.contains(">", Qt::CaseInsensitive) == true)  ||
       (text.contains("/", Qt::CaseInsensitive) == true)  ||
       (text.contains("'", Qt::CaseInsensitive) == true)  ||
       (text.contains(";", Qt::CaseInsensitive) == true)  ||
       (text.contains("{", Qt::CaseInsensitive) == true)  ||
       (text.contains("}", Qt::CaseInsensitive) == true)  ||
       (text.contains("[", Qt::CaseInsensitive) == true)  ||
       (text.contains("]", Qt::CaseInsensitive) == true)  ||
       (text.contains("|", Qt::CaseInsensitive) == true)  ||
       (text.contains("+", Qt::CaseInsensitive) == true)  ||
       (text.contains("=", Qt::CaseInsensitive) == true)  ||
       (text.contains("@", Qt::CaseInsensitive) == true)  ||
       (text.contains("#", Qt::CaseInsensitive) == true)  ||
       (text.contains("$", Qt::CaseInsensitive) == true)  ||
       (text.contains("%", Qt::CaseInsensitive) == true)  ||
       (text.contains("^", Qt::CaseInsensitive) == true)  ||
       (text.contains("&", Qt::CaseInsensitive) == true)  ||
       (text.contains("*", Qt::CaseInsensitive) == true)  ||
       (text.contains("(", Qt::CaseInsensitive) == true)  ||
       (text.contains(")", Qt::CaseInsensitive) == true)  ||
       (text.contains("`", Qt::CaseInsensitive) == true)  ||
       (text.contains("~", Qt::CaseInsensitive) == true)
         )
    {
        ui->label_3_result->setText(inappropriate_text);
    }
    else
    {
        // ===================================================================
        // TŁUMACZENIE TEKSTU:

        bool is_found = search_in_table();

        // gdy znaleziono tłumaczenie w lokalnej bazie, wyświetl je:
        if(is_found == true)
        {
           ui->label_3_result->setText(to_return);
        }
        // gdy nie znaleziono, połącz się z API i przetłumacz:
        else
        {
            // sprawdzamy czy jest dostep do internetu:
            QNetworkAccessManager nam;
            QNetworkRequest req(QUrl("http://www.google.com"));
            QNetworkReply *reply = nam.get(req);
            QEventLoop loop;
            connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
            loop.exec();

            // gdy jest połączenie:
            if(reply->bytesAvailable())
            {
                QString translation = translte();

                // gdy w API nie ma tłumaczenia:
                if(translation == this->text)
                {
                     ui->label_3_result->setText(not_found_text);
                }
                // gdy znaleziono tłumaczenie:
                else
                {
                    // dodajemy tłumaczenie do lokalnej bazy danych:
                    add_to_table(translation);

                    // wyswietlamy:
                    ui->label_3_result->setText(translation);
                }
            }
            // gdy nie ma połączenia:
            else
            {
                ui->label_3_result->setText("ERROR - NO INTERNET CONNECTION");
            }

        }
    }
}

