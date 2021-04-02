#ifndef B_H
#define B_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QStandardItem>
#include <QJsonArray>
#include "input.h"
#include <QtSql/QtSql>
#include <QTableWidget>
#include <QSizePolicy>
#include <QToolBar>
#include <QAction>
#include <QMenuBar>
#include <QGuiApplication>
#include <QScreen>

namespace Ui {
class RatesPage;
}
/*!
 * \brief Отвечает за все действия клиентского прлиожения
 */
class RatesPage : public QMainWindow
{
    Q_OBJECT

public:
    /*!
     * \brief Создает экземпляр сокета.
     * Устанавливает связь между
     * сигналом сокета ReadyRead() и  слотом sockReady() и
     * между сигналом disconnect() и слотом sockDisc().
     * Запускает окно авторизации
     * \param parent
     */
    explicit RatesPage(QWidget *parent = nullptr);
    ~RatesPage();
    /*!
     * \brief Запускает окно авторизации
     */
    void Display();
private slots:
    /*!
     * \brief Отвечает за действия выполняемые при готовности
     * чтения новых данных. То есть проверяет, что никаких ошибок при
     * получении данных не было, а затем определяет тип запроса и
     * делегаериует обработку запроса методу, ответственному за этот запрос.
     */
    void sockReady();
    /*!
     * \brief Отвечает за действия, выполняемые после
     * отключения клиента от сокета
     */
    void sockDisc();
    /*!
      * \brief Сохраняет
      * балл введенный в поле ввода
      */
     void on_SaveRateButton_clicked();
     /*!
      * \brief Метод автоматически вызывается при смене
      * значения в комбо-боксе с учебными группами. Метод
      * делает запрос на сервер для получения студентов и курсов
      * для выбранной группы
      * \param index
      */
     void on_choose_group_currentIndexChanged(int index);
     void on_choose_student_currentIndexChanged(int index);
     void on_choose_course_currentIndexChanged(int index);
     void on_choouselesson_currentIndexChanged(int index);
     /*!
      * \brief Получает даннае об имени пользователя и
      * его пароле и паредает их на сервер.
      * \param[in] UserName - имя пользователя
      * \param[in] UserPassword - пароль
      */
     void authorizeUser(QString UserName,QString UserPassword);
     /*!
      * \brief Делает
      * запрос на сервер на получение списка
      * курсов для определенной учебной группы
      * \param group_Id - первичный ключ группы,
      * для которой получаем список курсов.
      */
     void get_courses_for_add_lesson(QString group_Id);
     /*!
      * \brief Делает запрос на
      * получение всех студентов
      */
     void get_all_students();
     void getCoursesForStudent(QString StudentId);
     /*!
      * \brief Делает запрос
      * на получение списка занятий
      * \param student_Id - первичный ключ студента. Полученные занятия
      * должны относиться к той же учебной группе что и студент.
      * \param[in] course_Id - первичный ключ курса, по которому выбираются занятия.
      */
     void getLessonsForStudentAndCourse(QString student_Id, QString course_Id);
     /*!
      * \brief Устанавлявает балл студенту
      * \param rate - сам балл
      * \param student_Id - первичный ключ студента, которому
      * устанавливается балл
      * \param lesson_Id - первичный ключ занятия, за которое
      * устанавливается балл
      */
     void setRateToStudentInGenerallist(QString rate, int student_Id, int lesson_Id);
     /*!
      * \brief Переключение на страницу с добавлением
      * занятия
      */
     void on_AddLesoon_triggered();
     void on_AddLesson_clicked();
     /*!
      * \brief Переключение на главную страницу
      */
     void on_Main_Page_triggered();
     /*!
      * \brief Переключение на страницу с общим списком студентов
      */
     void on_GeneralListStudent_triggered();
     void on_ChooseStudent_AllStudent_currentIndexChanged(int index);
     void on_ChooesCourse_AllStudent_currentIndexChanged(int index);
     void on_ChooseGroupToAddLesson_currentIndexChanged(int index);

private:
    QAction* mainPage=nullptr;
    QAction* add_Lesson=nullptr;
    QAction* general_list_student=nullptr;
    Ui::RatesPage *ui;
    /*!
     * \brief Окно авторизации
     */
    Input input_window;
    /*!
     * \brief Сокет
     */
    QTcpSocket* socket=nullptr;

    /*!
    * \brief CurrentGroup
    * \return первичный ключ учебной группы,
    * которая выбрана в комбо-боксе
    */
   int CurrentGroup();
   /*!
    * \brief
    * \return первичный ключ студента,
    * которая выбран в комбо-боксе
    */
   int CurrentStudent();
   int CurrentCourse();
   int CurrentLesson();
   QString CurrentRate();
   /*!
    * \brief setData устанавливает данные комбо-бокс
    * \param comboBox - комбо-бокс, в который устанавливаются
    * значения
    * \param arr - набор данных для устновки
    */
   void setData(QComboBox* comboBox, QJsonArray arr);
   void getGroups();
   void getRates(int, int);
   void getLessons(int, int);
   /*!
    * \brief Устанавливает студентов
    * в комбо-бокс
    * \param docArr - список студентов
    */
   void setAllStudents(QJsonArray docArr);
   void setStudents(QJsonArray docAr_students,  QJsonArray  docAr_couse);
   void setRates(QString rate);
   /*!
    * \brief Устаналвивает в таблицу
    * баллы студентов
    * \param docAr - набор с данными о баллах
    * \param count_student - количество студентов
    */
   void setAllRates(QJsonArray docAr, int count_student);
   void resInput(QString status);
   void setGroupsForAddLesson(QJsonArray docArr);
   void ResultAddLesson(QString result);
   void ResultSetRate(QString result);

   QToolBar* createToolBar();
   void create_Act();

   void to_main_page();
   void to_add_lesson();
   void to_general_student_list();
};

#endif
