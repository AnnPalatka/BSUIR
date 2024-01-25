#include "widget.h"
#include "ui_widget.h"
#include "shablon.h"
#include "fileexception.h"
#include <QFileDialog>
#include <QDir>
#include <QTime>
#include <QFile>
#include <QDirIterator>
#include <QMessageBox>
List<QString> list;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    // Настройка таблицы плейлиста
    m_playListModel = new QStandardItemModel(this);
    ui->playlistView->setModel(m_playListModel);    // Устанавливаем модель данных в TableView
    // Устанавливаем заголовки таблицы


    m_playListModel->setHorizontalHeaderLabels(QStringList()  << tr("Audio Track")
                                                            << tr("File Path"));
    //ui->playlistView->hideColumn(1);    // Скрываем колонку, в которой хранится путь к файлу
    ui->playlistView->verticalHeader()->setVisible(false);                  // Скрываем нумерацию строк
    ui->playlistView->setSelectionBehavior(QAbstractItemView::SelectRows);  // Включаем выделение строк
    ui->playlistView->setSelectionMode(QAbstractItemView::SingleSelection); // Разрешаем выделять только одну строку
    ui->playlistView->setEditTriggers(QAbstractItemView::NoEditTriggers);   // Отключаем редактирование
    // Включаем подгонку размера последней видимой колонки к ширине TableView
    ui->playlistView->horizontalHeader()->setStretchLastSection(true);

    m_player = new QMediaPlayer(this);          // Инициализируем плеер
    m_playlist = new QMediaPlaylist(m_player);  // Инициализируем плейлист
    m_player->setPlaylist(m_playlist);          // Устанавливаем плейлист в плеер
    ui->pdiaVolume->setRange(0, 100);// Установка границ громкости звукка
    int nDefaultVolume = 50; // Инициализация значения звука по умолчанию
    m_player->setVolume(nDefaultVolume);                    // Устанавливаем громкость воспроизведения треков
    ui->pdiaVolume->setValue(nDefaultVolume); // Установка значения звука по умолчанию
    m_playlist->setPlaybackMode(QMediaPlaylist::Loop);  // Устанавливаем циклический режим проигрывания плейлиста

    // подключаем кнопки управления к слотам управления
    // Здесь отметим, что навигация по плейлисту осуществляется именно через плейлист
    // а запуск/пауза/остановка через сам плеер
    connect(ui->btn_previous, &QToolButton::clicked, m_playlist, &QMediaPlaylist::previous);
    connect(ui->btn_next, &QToolButton::clicked, m_playlist, &QMediaPlaylist::next);
    connect(ui->btn_play, &QToolButton::clicked, m_player, &QMediaPlayer::play);
    connect(ui->btn_pause, &QToolButton::clicked, m_player, &QMediaPlayer::pause);
    connect(ui->btn_stop, &QToolButton::clicked, m_player, &QMediaPlayer::stop);

    connect(ui->pdiaVolume, SIGNAL(valueChanged(int)), m_player, SLOT (setVolume (int)));//установка регулировки звука

        connect(ui->m_psldPosition, SIGNAL(sliderMoved(int) ), SLOT(slotSetMediaPosition(int)) ) ;//установки для слайдера
        connect(m_player, SIGNAL(positionChanged(qint64) ), SLOT(slotSetSliderPosition(qint64)) ) ;
        connect(m_player, SIGNAL(durationChanged(qint64)), SLOT(slotSetDuration(qint64)) ) ;

    // При даблклике по треку в таблице устанавливаем трек в плейлисте
    connect(ui->playlistView, &QTableView::doubleClicked, [this](const QModelIndex &index){
        m_playlist->setCurrentIndex(index.row());
    });

    // при изменении индекса текущего трека в плейлисте, устанавливаем название файла в специальном лейбле
    connect(m_playlist, &QMediaPlaylist::currentIndexChanged, [this](int index){
        ui->currentTrack->setText(m_playListModel->data(m_playListModel->index(index, 0)).toString());
    });
}

Widget::~Widget()
{
    delete ui;
    delete m_playListModel;
    delete m_playlist;
    delete m_player;
}
//Slots for slider
void Widget::slotSetMediaPosition(int n)
{
    m_player->setPosition(n);
}
QString Widget::msecsToString(qint64 n)
{
    int nHours = (n / (60 * 60 * 1000));
    int nМinutes = ( (n % (60 * 60 * 1000)) / (60 * 1000));
    int nSeconds = ( (n % (60 * 1000)) / 1000);
    return QTime(nHours, nМinutes, nSeconds).toString("hh:mm:ss");
}
void Widget::slotSetDuration(qint64 n)
{
    ui->m_psldPosition->setRange(0, n);
    ui->m_Current->setText(msecsToString(0));
    ui->m_Remain->setText(msecsToString(n));
}
void Widget::slotSetSliderPosition(qint64 n)
{
    ui->m_psldPosition->setValue(n);
    ui->m_Current->setText(msecsToString(n));
    int nDuration = ui->m_psldPosition->maximum();
    ui->m_Remain->setText(msecsToString(nDuration -n) );
}
//Slots for slider end

void Widget::on_btn_add_clicked()
{
    // С помощью диалога выбора файлов делаем множественный выбор mp3 файлов
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      tr("Open files"),
                                                      QString(),
                                                      tr("Audio Files (*.mp3)"));
    for (int i = 0; i < files.size(); ++i) {
        list.push(files.at(i));
    }
    // Далее устанавливаем данные по именам и пути к файлам
    // в плейлист и таблицу отображающую плейлист
    foreach (QString filePath, files) {
        QList<QStandardItem *> items;
        items.append(new QStandardItem(QDir(filePath).dirName()));
        items.append(new QStandardItem(filePath));
        m_playListModel->appendRow(items);
        m_playlist->addMedia(QUrl(filePath));
    }
}

void Widget::on_Save_clicked()
{
    QMessageBox msg;
    msg.setText("Вы действительно хотите Сохранить Нынешний Плейлист?");
    msg.setInformativeText("При Нажатии ОК Предыдущий Плейлист будет заменен Новым.");
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msg.setIcon(QMessageBox::Question);
    msg.setDefaultButton(QMessageBox::Ok);
    int res = msg.exec();
    if (res == QMessageBox::Ok)
    {
        QFile file;
        file.setFileName("D:/qt_projects/music.txt");
        QFileInfo inf("D:/qt_projects/music.txt");
        try
        {
        if (!inf.exists() && !inf.isFile())
            throw FileException<Widget>(Errors::fileIsNotOpen, this, "Не удалось открыть файл");
        else
        {
            if(file.open(QIODevice::WriteOnly))
                {
                    QTextStream stream(&file);
                    MyIterator<QString> iter = list.begin();
                    for (int i = 0; i < list.getsize(); ++i) {
                        stream << *iter << '\n';
                        ++iter;
                    }
                }
            file.close();
        }
        }
        catch(FileException<Widget> obj)
        {
            obj.show();
            return;
        }
    }
    else
        return;

}

void Widget::setting()
{
    m_playListModel->clear();
    m_playlist->clear();
    m_playListModel->setHorizontalHeaderLabels(QStringList()  << tr("Audio Track")
                                                            << tr("File Path"));
    ui->playlistView->hideColumn(1);    // Скрываем колонку, в которой хранится путь к файлу
    ui->playlistView->verticalHeader()->setVisible(false);                  // Скрываем нумерацию строк
    ui->playlistView->setSelectionBehavior(QAbstractItemView::SelectRows);  // Включаем выделение строк
    ui->playlistView->setSelectionMode(QAbstractItemView::SingleSelection); // Разрешаем выделять только одну строку
    ui->playlistView->setEditTriggers(QAbstractItemView::NoEditTriggers);   // Отключаем редактирование
    // Включаем подгонку размера последней видимой колонки к ширине TableView
    ui->playlistView->horizontalHeader()->setStretchLastSection(true);
}

void Widget::on_Clear_clicked()
{
    QMessageBox msg;
    msg.setText("Вы действительно хотите Очистить Нынешний Плейлист?");
    msg.setInformativeText("При Нажатии ОК Плейлист будет Очищен.");
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msg.setIcon(QMessageBox::Question);
    msg.setDefaultButton(QMessageBox::Ok);
    int res = msg.exec();
    if (res == QMessageBox::Ok)
    {
        list.DeleteAll();
        setting();
    }
    else
        return;



}

void Widget::on_Music_clicked()
{
    QStringList nameFilters;
    nameFilters<<"*mp3";
    QDir dirFrom("C:/Users/shala/Music/");
    QStringList fileList = dirFrom.entryList(nameFilters,QDir::Files);
    QList<QFileInfo> listinfo(dirFrom.entryInfoList(nameFilters,QDir::Files));

    for (int i = 0; i < listinfo.size(); ++i) {
        QString s = listinfo.at(i).absoluteFilePath();
        list.push(s);
    }
    foreach (QString filePath, fileList)
    {
        QList<QStandardItem *> items;
        items.append(new QStandardItem(QDir(filePath).dirName()));
        items.append(new QStandardItem("C:/Users/shala/Music/"+filePath));
        m_playListModel->appendRow(items);
        m_playlist->addMedia(QUrl("C:/Users/shala/Music/"+filePath));

    }
}
void Widget::on_Lastplaylist_clicked()
{

    QFile file;
    file.setFileName("D:/qt_projects/music.txt");
    QFileInfo inf("D:/qt_projects/music.txt");
        try
        {
        if (!inf.exists() && !inf.isFile())
            throw FileException<Widget>(Errors::fileIsNotOpen, this, "Не удалось открыть файл");
        else
        {
            if(file.open(QIODevice::ReadOnly))
            {
                QTextStream in(&file);
                while(!in.atEnd())
                {
                    QString direc = in.readLine();
                    QStringList nameFilters;
                    nameFilters<<"*mp3";
                    QFileInfo fileinfo(direc);
                    QString dirpath = fileinfo.absolutePath();
                    QString dirnamepath = fileinfo.absoluteFilePath();
                    QString dirname = fileinfo.fileName();
                    QDirIterator it(dirpath, QDir::Files);
                    it.next();
                    while (it.fileInfo().exists())
                    {
                        QString dirname2 = it.fileInfo().fileName();
                        if(dirname2 == dirname)
                        {
                            list.push(dirnamepath);
                            QList<QStandardItem *> items;
                            items.append(new QStandardItem(dirname));
                            items.append(new QStandardItem(dirnamepath));
                            m_playListModel->appendRow(items);
                            m_playlist->addMedia(QUrl::fromLocalFile(it.filePath()));
                        }
                        it.next();
                    }
                }
            }
            file.close();
        }
        }

        catch(FileException<Widget> obj)
        {
            obj.show();
            return;
        }


}

