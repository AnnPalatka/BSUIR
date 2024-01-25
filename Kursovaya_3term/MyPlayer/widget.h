#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QMediaPlayer>
#include <QMediaPlaylist>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    QString msecsToString(qint64 n);// Вспомогательная функция слайдера(запись времени трека в строку)
    void setting();
    ~Widget();

private slots:
    void on_btn_add_clicked();// Слот для обработки добавления треков через диалоговое окно
    void on_Save_clicked();// Слот для сохранения сформированного плейлиста
    void on_Clear_clicked();// ССлот для очистки сформированного плейлиста
    void on_Lastplaylist_clicked();// Слот для обработки добавления треков из последнего сохраненного плейлиста
    void on_Music_clicked();// Слот для обработки добавления треков из папки Music
        void slotSetSliderPosition(qint64 ) ;// Слоты слайдера
        void slotSetMediaPosition (int );
        void slotSetDuration (qint64 ) ;

private:
    Ui::Widget *ui;
    QStandardItemModel  *m_playListModel;   // Модель данных плейлиста для отображения
    QMediaPlayer        *m_player;          // Проигрыватель треков
    QMediaPlaylist      *m_playlist;        // Плейлиста проигрывателя

};

#endif // WIDGET_H
