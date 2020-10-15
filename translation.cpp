#include "translation.h"
#include "ui_translation.h"


Translation::Translation(QString input_language, QString output_language, QString title_text,
                         QSqlDatabase & sql, QString table_name,
                         QString button_translate_text, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Translation)
{
    ui->setupUi(this);

    // set start values:
    this->input_language = input_language;
    this->output_language = output_language;
    this->title_text = title_text;
    this->_sql = sql;
    this->table_name = table_name;
    this->button_translate_text = button_translate_text;

    // show no text on bottom labels at the beginning:
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

    // buttons for English version:
    if( (input_language == "en" && output_language == "pol")
        || (input_language == "en" && output_language == "ger")
        || (input_language == "en" && output_language == "fre")
        || (input_language == "en" && output_language == "ita") )
    {
        // if typed some text:
        if(text != "")
        {
            translation_logic("processing...", "You cannot use \" @ # etc. symbols", "Translation not found");
        }
        else
        {
            ui->label_3_result->setText("You haven't entered anything yet!");
        }
    }
    // buttons for Polish version:
    else if ( (input_language == "pol" && output_language == "en")
                || (input_language == "pol" && output_language == "ger")
                || (input_language == "pol" && output_language == "fre")
                || (input_language == "pol" && output_language == "ita") )
    {
        // if typed some text:
        if(text != "")
        {
            translation_logic("przetwarzanie...", "Nie wolno wpisywać symboli \" @ # etc. ", "Nie znaleziono tłumaczenia");
        }
        else
        {
            ui->label_3_result->setText("Nie wpisano żadnego tekstu!");
        }
    }
    // buttons for French version:
    else if ( (input_language == "fre" && output_language == "pol")
                || (input_language == "fre" && output_language == "ger")
                || (input_language == "fre" && output_language == "en")
                || (input_language == "fre" && output_language == "ita") )
    {
        // if typed some text
        if(text != "")
        {
            translation_logic("traitement...", "Les symboles \" @ # etc. ne sont pas autorisés", "Traduction non trouvée");
        }
        else
        {
            ui->label_3_result->setText("Aucun test entré!");
        }
    }
    // buttons for German version:
    else if ( (input_language == "ger" && output_language == "pol")
                || (input_language == "ger" && output_language == "en")
                || (input_language == "ger" && output_language == "fre")
                || (input_language == "ger" && output_language == "ita") )
    {
        // if typed some text
        if(text != "")
        {
            translation_logic("verarbeitung...", "Symbole \" @ # etc. sind nicht erlaubt", "Übersetzung nicht gefunden");
        }
        else
        {
            ui->label_3_result->setText("Du hast nicht Text eingegeben!");
        }
    }
    // buttons for Italian version:
    else if ( (input_language == "ita" && output_language == "en")
                || (input_language == "ita" && output_language == "ger")
                || (input_language == "ita" && output_language == "fre")
                || (input_language == "ita" && output_language == "pol") )
    {
        // if typed some text
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

    QSqlQuery _query(_sql);
    _query.clear();

    int rows = -1;

    QString execute = "SELECT COUNT(*) FROM tabela WHERE (input_language='" + input_language + "') AND (output_language='" + output_language + "') AND (entered_text='" + entered_text + "');";
    _query.prepare(execute);
    _query.exec();

    if (_query.next())
    {
        rows = _query.value(0).toInt();
    }
    else
    {
        rows = 0;
    }

    // if found translation in database, pass it to to_return variable:
    if(rows == 1)
    {
        // search database to get translation:
        _query.exec("SELECT translation FROM tabela WHERE input_language='" + input_language +
                    "' AND output_language='" + output_language +
                    "' AND entered_text='" + entered_text + "';");
        _query.next();
        translated_text = _query.value(0).toString();
        _query.clear();

        // pass text to to_return variable:
        this->to_return = translated_text.toUtf8();

        return true;
    }
    else if(rows == 0)
    {
        return false;
    }
    else
    {
        return false;
    }
}

QString Translation::translte()
{
        QString _q = this->text;
        QString _langpair = this->input_language + "|" + this->output_language;

        // create URL:
        QString url = QString("https://api.mymemory.translated.net/get?q=%1&langpair=%2").arg(_q).arg(_langpair);

        // create network connection:
        QNetworkAccessManager manager;
        QNetworkRequest request(url);
        QNetworkReply *reply = manager.get(request);

        // get data from API:
        do
        {
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        }
        while(!reply->isFinished());

        // parse JSON:
        QString _response_data = reply->readAll();
        QJsonDocument document = QJsonDocument::fromJson(_response_data.toUtf8());

        QJsonObject jObject = document.object();

        // read value for responseData:
        QJsonValue value = jObject.value(QString("responseData"));

        // read data of this object (responseData):
        QJsonObject item = value.toObject();

        // get data of this sub-object:
        QJsonValue subobj = item["translatedText"];

        // assign subobj data to variable _translation:
        QString _traslation = subobj.toString();

        // remember to delete reply:
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
    _query.clear();
}

void Translation::translation_logic(QString processing_text, QString inappropriate_text, QString not_found_text)
{
    // set text informing about searching for translation:
    ui->label_3_result->setText(processing_text);

    // if entered text contains inappropriate characters:
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
        bool is_found = search_in_table();

        // if found translation in local database, show it:
        if(is_found == true)
        {
           ui->label_3_result->setText(to_return);
        }
        // if not, use API to get it:
        else
        {
            // check network connection:
            QNetworkAccessManager nam;
            QNetworkRequest req(QUrl("http://www.google.com"));
            QNetworkReply *reply = nam.get(req);
            QEventLoop loop;
            connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
            loop.exec();

            // if there's access to internet:
            if(reply->bytesAvailable())
            {
                QString translation = translte();

                // if API doesn't provide certain translation:
                if(translation == this->text)
                {
                     ui->label_3_result->setText(not_found_text);
                }
                // if translation is found:
                else
                {
                    // add translation to local database:
                    add_to_table(translation);

                    // and show it:
                    ui->label_3_result->setText(translation);
                }
            }
            // if there's no internet connection:
            else
            {
                ui->label_3_result->setText("ERROR - NO INTERNET CONNECTION");
            }
        }
    }
}
