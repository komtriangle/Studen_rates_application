#ifndef INPUT_H
#define INPUT_H

#include <QMainWindow>
#include <QTcpSocket>

namespace Ui {
class Input;
}
/*!
 * \brief Отвечает за атворизацию пользователя
 */
class Input : public QMainWindow
{
    Q_OBJECT

public:
    explicit Input(QWidget *parent = nullptr);
    ~Input();
    /*!
     * \brief Запускает окно атворизации
     */
    void Display();
signals:
    /*!
     * \brief Сигнал отправляется в класс RatesWindow,
     * где в последующем будет отправлен на сервер.
     * \param UserName - имя позьзователя
     * \param UserPassword - пароль
     */
    void login_button_clicked(QString UserName,QString UserPassword);

private slots:
    /*!
     * \brief При имененении имени пользователя
     * в поле ввода, сохраняет его текущее значение
     * в переменную
     * \param arg1 - текущее имя пользоавтеля
     */
    void on_UserName_textEdited(const QString &arg1);
    /*!
     * \brief При изменении пароля в поле ввода,
     * сохраняет его текущее значение в переменную.
     * \param arg1
     */
    void on_UserPassword_textEdited(const QString &arg1);
    /*!
     * \brief Отправляет классу RatesWindow сигнал
     * с именем пользователя и паролем.
     */
    void on_inputButton_clicked();

private:
    Ui::Input *ui;
    /*!
     * \brief Имя пользователя
     */
    QString UserName=nullptr;
    /*!
     * \brief Пароль
     */
    QString UserPassword=nullptr;

};

#endif // INPUT_H
