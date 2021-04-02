#include "rateswindow.h"
#include "ui_rateswindow.h"

RatesPage::RatesPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RatesPage)
{
    ui->setupUi(this);
    socket=new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()),this, SLOT(sockReady()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisc()));
    connect(&input_window, SIGNAL(login_button_clicked(QString, QString)),
                this, SLOT(authorizeUser(QString, QString)));
    connect(&input_window,SIGNAL(destroyed()),
                this, SLOT(show()));
    Display();
    create_Act();
    addToolBar(Qt::TopToolBarArea, createToolBar());

    ui->stackedWidget->setCurrentIndex(0);
}


RatesPage::~RatesPage()
{
    delete ui;
}
QToolBar *RatesPage::createToolBar()
{
    QToolBar* ptb = new QToolBar("Linker ToolBar");
    ptb->addAction(mainPage);
    ptb->addAction(add_Lesson);
    ptb->addAction(general_list_student);
    return ptb;
}
void RatesPage::create_Act(){
    QToolBar* ptb = new QToolBar("Linker ToolBar");
    mainPage = ptb->addAction(tr("&Главная страница"), this, &RatesPage::to_main_page);
    add_Lesson = ptb->addAction(tr("&Добавить занятие"), this, &RatesPage::to_add_lesson);
    general_list_student = ptb->addAction(tr("&Общий список студентов"), this, &RatesPage::to_general_student_list);
        //mainPage->setShortcut(QKeySequence::Open);
}
void RatesPage::to_main_page(){
    ui->stackedWidget->setCurrentIndex(0);
     this->resize(QGuiApplication::primaryScreen()->availableSize()*(3./7));
}
void RatesPage::to_add_lesson(){
    ui->stackedWidget->setCurrentIndex(1);
    ui->AddLessonDate->setDate(QDate::currentDate());
    socket->write("{\"type\":\"select\", \"params\":\"groups_for_add_lesson\"}");
    socket->waitForBytesWritten(100);
    this->resize(QGuiApplication::primaryScreen()->availableSize()*(1./5));
    this->resize(50,50);
}
void RatesPage::to_general_student_list(){
    ui->stackedWidget->setCurrentIndex(2);
    QString str="{\"type\":\"select\", \"params\":\"allStudents\"}";
    QByteArray arr;
    std::string res= str.toStdString();
    arr.append(QByteArray::fromStdString(res));
    socket->write(arr);
    socket->waitForBytesWritten(10);
     this->resize(QGuiApplication::primaryScreen()->availableSize()*(1./5));
}
void RatesPage::sockDisc(){
    socket->deleteLater();

}
void RatesPage::sockReady(){
    if(socket->waitForConnected(10))
    {
        socket->waitForReadyRead(10);
        QByteArray  Data = socket->readAll();
        QJsonParseError docError;
         QJsonDocument doc =QJsonDocument::fromJson(Data, &docError);
        if(docError.errorString().toInt()==QJsonParseError::NoError){
            if(doc.object().value("type").toString()=="connect"){
            }
            else if (doc.object().value("type").toString()=="resultSelect_groups"){
                 QJsonArray docAr=doc.object().value("result").toArray();
                 setData(ui->choose_group, docAr);
            }
            else if(doc.object().value("type").toString()=="resultSelect_students_group"){
               QJsonArray docAr_students=doc.object().value("result").toArray();
               QJsonArray  docAr_couse=doc.object().value("result_2").toArray();
                setStudents(docAr_students,  docAr_couse);
            }
            else if(doc.object().value("type").toString()=="result_select_Lessons"){
                QJsonArray docAr=doc.object().value("result").toArray();
                setData(ui->choouselesson, docAr);
            }
            else if(doc.object().value("type").toString()=="result_select_Rates"){
                QString rate=doc.object().value("result").toString();
                setRates(rate);
            }
            else if(doc.object().value("type").toString()=="result_all_rates_on_course"){
                QJsonArray docAr=doc.object().value("result").toArray();
                QString count_std=doc.object().value("student_count").toString();
                int count_student=count_std.toInt();
                setAllRates(docAr, count_student);
            }
            else if(doc.object().value("type").toString()=="resultInput"){
                QString status=doc.object().value("status").toString();
                resInput(status);
            }
            else if(doc.object().value("type").toString()=="resultSelect_groups_for_add_lesson"){
                QJsonArray docArr=doc.object().value("result").toArray();
                setGroupsForAddLesson(docArr);
            }
            else if(doc.object().value("type").toString()=="result_getCoursesForAddLesson"){
                QJsonArray docArr=doc.object().value("result").toArray();
                setData(ui->ChooseCourseToAddLesson, docArr);
            }
            else if(doc.object().value("type").toString()=="result" && doc.object().value("params").toString()=="add_lesson"){
                QString result=doc.object().value("info").toString();
               ResultAddLesson(result);
            }
            else if(doc.object().value("type").toString()=="result" && doc.object().value("params").toString()=="set_rate"){
               QString result=doc.object().value("info").toString();
               ResultSetRate(result);
            }
            else if(doc.object().value("type").toString()=="resultAllStudents"){
                QJsonArray docArr=doc.object().value("result").toArray();
                setAllStudents(docArr);
            }
            else if(doc.object().value("type").toString()=="result_select_courses_for_student"){
                QJsonArray docArr=doc.object().value("result").toArray();
                setData(ui->ChooesCourse_AllStudent, docArr);
            }
            else if(doc.object().value("type").toString()=="result_select_Lessons_for_Student_And_Course"){
                QJsonArray docArr=doc.object().value("result").toArray();
                setData(ui->ChooseLesson_AllStudent, docArr);
        }
        }
    }
}

int RatesPage::CurrentGroup(){
    return ui->choose_group->currentData().toInt();
}
int RatesPage::CurrentStudent(){
    return  ui->choose_student->currentData().toInt();
}
int RatesPage::CurrentCourse(){
    return ui->choose_course->currentData().toInt();
}
int RatesPage::CurrentLesson(){
    return  ui->choouselesson->currentData().toInt();
}
QString RatesPage::CurrentRate(){
    return ui->rate->text();
}
void RatesPage::getGroups(){
    socket->write("{\"type\":\"select\", \"params\":\"groups\"}");
     socket->waitForBytesWritten(100);
}
void RatesPage::getRates(int Student_Id, int Lesson_Id){
    if(Lesson_Id!=0 && Student_Id!=0){
       QString query="{\"type\":\"select\", \"params\":\"rates\", "
                     "\"Student_Id\":\"%1\", \"Lesson_Id\":\"%2\"}";
       query=query
               .arg(Student_Id)
               .arg(Lesson_Id);
       QByteArray arr;
       std::string quer= query.toStdString();
       arr.append(QByteArray::fromStdString(quer));
       socket->write(arr);
       socket->waitForBytesWritten(100);
    }
}
void RatesPage::getLessons(int Group_Id, int Course_Id){
   if(Group_Id!=0 && Course_Id!=0){
       QString query= "{\"type\":\"select\", \"params\":\"Lessons\","
                      " \"par\":\"Lessons\", \"Group_Id\":\"%1\","
                       " \"Course_Id\":\"%2\"}";
       query=query
               .arg(Group_Id)
               .arg(Course_Id);
       QByteArray arr;
       std::string quer= query.toStdString();
       arr.append(QByteArray::fromStdString(quer));
       socket->write(arr);
       socket->waitForBytesWritten(100);
   }
}
void RatesPage::setStudents(QJsonArray docAr_students,  QJsonArray  docAr_couse){
    ui->choouselesson->clear();
    ui->choose_student->clear();
    setData(ui->choose_student, docAr_students);
    setData(ui->choose_course, docAr_couse);
}
void RatesPage::setData(QComboBox* box, QJsonArray docAr){
    box->clear();
    for(int i=0;i<docAr.count(); i++){
        box->addItem(docAr[i].toObject().value("Data").toString(),docAr[i].toObject().value("Id").toString());
    }
}
void RatesPage::setRates(QString rate){
    if(rate==nullptr){
                        ui->rate->clear();
    }
      else{
          ui->rate->setText(rate);
      }
    int Course_Id=CurrentCourse();
    int Group_Id=CurrentGroup();
    QString str="{\"type\":\"select\", \"params\":\"all_rates_on_course\""
                ",\"Course_Id\":\"%1\",\"Group_Id\":\"%2\"}";
    str=str.arg(Course_Id).arg(Group_Id);
    QByteArray arr;
    std::string res= str.toStdString();
    arr.append(QByteArray::fromStdString(res));
    socket->write(arr);
    socket->waitForBytesWritten(100);

}
void RatesPage::setAllRates(QJsonArray docAr, int count_student){
    int lesson_count=docAr.count()/count_student;
    QStandardItemModel* model = new QStandardItemModel(nullptr);

    QStringList horizontalHeaders;
    QStringList VerticalHeaders;
    if(lesson_count>0){
        for(int i=0;i<docAr.count();i++){
            if(i<lesson_count){
               horizontalHeaders.append(docAr[i].toObject().value("Lesson_Date").toString());
            }
            if(i%lesson_count==0){
                VerticalHeaders.append(docAr[i].toObject().value("FirstName").toString());
               }
            QStandardItem* item1=new QStandardItem(docAr[i].toObject().value("Rate").toString());
            model->setItem(i/lesson_count,i%lesson_count, item1);
    }
    }
    model->setVerticalHeaderLabels(VerticalHeaders);
    model->setHorizontalHeaderLabels(horizontalHeaders);
    ui->all_rates->setModel(model);
}
void RatesPage::resInput(QString status){
    if(status=="No"){
        QMessageBox::warning(this, "Авторизация", "Неправильное имя пользователя или пароль");
    }
    else{
        input_window.close();
        this->show();
        this->setWindowTitle("Учет успеваемости студентов");
        socket->write("{\"type\":\"select\", \"params\":\"groups\"}");
        socket->waitForBytesWritten(100);
    }
}
void RatesPage::setGroupsForAddLesson(QJsonArray docArr){
  setData(ui->ChooseGroupToAddLesson, docArr);
}
void RatesPage::Display()
{
    socket->connectToHost("127.0.0.1", 5000);
    socket->waitForConnected(100);
    input_window.Display();

}
void RatesPage::authorizeUser(QString UserName,QString UserPassword){
    QString query="{\"type\":\"input\", \"login\":\"%1\",\"password\":\"%2\"}";
    query=query
            .arg(UserName)
            .arg(UserPassword);
    QByteArray arr;
    std::string res= query.toStdString();
    arr.append(QByteArray::fromStdString(res));
    socket->write(arr);
    socket->waitForBytesWritten(100);
}
void RatesPage::on_choose_group_currentIndexChanged(int index)
{
    int Group_Id = CurrentGroup();
    if(Group_Id!=0){
        QByteArray arr;
        QString query="{\"type\":\"select_students_from_group\", \"group_Id\":\"%1\"}";
        query=query.arg(Group_Id);
        std::string res= query.toStdString();
        arr.append(QByteArray::fromStdString(res));
        socket->write(arr);
        socket->waitForBytesWritten(100);
    }
    ui->all_rates->setModel(nullptr);
}
void RatesPage::on_choose_student_currentIndexChanged(int index)
{
    int Student_Id=CurrentStudent();
    int Lesson_Id=CurrentLesson();
    getRates(Student_Id, Lesson_Id);
 }
void RatesPage::on_choose_course_currentIndexChanged(int index)
{
   int Group_Id=CurrentGroup();
   int Course_Id=CurrentCourse();
   getLessons(Group_Id, Course_Id);
}
void RatesPage::on_choouselesson_currentIndexChanged(int index)
{
    int Student_Id=CurrentStudent();
    int Lesson_Id=CurrentLesson();
     getRates(Student_Id, Lesson_Id);
}
void RatesPage::get_courses_for_add_lesson(QString group_Id){

    QString query="{\"type\":\"select\","
                  " \"params\":\"courses_for_add_lesson\", \"group_Id\":\"%1\"}";
    query=query
            .arg(group_Id);
    QByteArray arr;
    std::string res= query.toStdString();
    arr.append(QByteArray::fromStdString(res));
    socket->write(arr);
    socket->waitForBytesWritten(10);

}
void RatesPage::ResultAddLesson(QString result){
    if(result=="Успешно"){
        QMessageBox::information(this, "Добавление занятия", "Занятие успешно добавлено!");
        int Group_Id=CurrentGroup();
        int Course_Id=CurrentCourse();
        getLessons(Group_Id, Course_Id);
    }
    else{
        QMessageBox::warning(this, "Добавление занятия", "Невозможно добавить занятие, так как такое занятие уже есть!");
    }
}
void RatesPage::ResultSetRate(QString result){
    if(result=="Успешно"){
        QMessageBox::information(this, "Добавление/изменение балла за занятие", "Оценка успешно добавлена!");
    }
    else{
        QMessageBox::information(this, "Установка оценка", "При добавлении балла  произошла ошибка!");
    }
    int Student_Id=CurrentStudent();
    int Lesson_Id=CurrentLesson();
    getRates(Student_Id, Lesson_Id);
}
void RatesPage::get_all_students(){
    QString str="{\"type\":\"select\", \"params\":\"allStudents\"}";
    QByteArray arr;
    std::string res= str.toStdString();
    arr.append(QByteArray::fromStdString(res));
    socket->write(arr);
    socket->waitForBytesWritten(10);
}
void RatesPage::setAllStudents(QJsonArray docArr){
    setData(ui->ChooseStudent_AllStudent, docArr);
}
void RatesPage::getCoursesForStudent(QString StudentId){
    if(StudentId!=nullptr){
        QString query= "{\"type\":\"select\", "
                       "\"params\":\"CoursesForStudent\",\"Id\":\"%1\"}";
        query=query
                .arg(StudentId);
        QByteArray arr;
        std::string res= query.toStdString();
        arr.append(QByteArray::fromStdString(res));
        socket->write(arr);
        socket->waitForBytesWritten(10);
    }
}
void RatesPage::getLessonsForStudentAndCourse(QString student_Id, QString course_Id){
   if(student_Id!=nullptr && course_Id!=nullptr){
       QString query= "{\"type\":\"select\", "
                    "\"params\":\"LessonsForStudentAndCourse\","
                    "\"Student_Id\":\"%1\",\"Course_Id\":\"%2\"}";
       query=query
               .arg(student_Id)
               .arg(course_Id);
       QByteArray arr;
       std::string res= query.toStdString();
       arr.append(QByteArray::fromStdString(res));
       socket->write(arr);
       socket->waitForBytesWritten(10);
   }
}
void  RatesPage::setRateToStudentInGenerallist(QString rate, int student_Id, int lesson_Id){
    QString query="{\"type\":\"insert\", \"params\":\"rate\", "
                  " \"Student_Id\":\"%1\","
                 " \"Rate\":\"%2\", \"Lesson_Id\":\"%3\"}";
    query=query
            .arg(student_Id)
            .arg(rate)
            .arg(lesson_Id);
    QByteArray arr;
    std::string res= query.toStdString();
    arr.append(QByteArray::fromStdString(res));
    socket->write(arr);
    socket->waitForBytesWritten(10);
}

void RatesPage::on_AddLesoon_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->AddLessonDate->setDate(QDate::currentDate());
    socket->write("{\"type\":\"select\", \"params\":\"groups_for_add_lesson\"}");
    socket->waitForBytesWritten(100);
}

void RatesPage::on_AddLesson_clicked()
{
    QString group_Id=ui->ChooseGroupToAddLesson->currentData().toString();
    QString course_Id=ui->ChooseCourseToAddLesson->currentData().toString();
    QString lesson_date=ui->AddLessonDate->text();
    QString query="{\"type\":\"insert\", \"params\":\"Add_lesson\", "
                  "\"group_Id\":\"%1\", \"course_Id\":\"%2\", \"Date\":\"%3\"}";
    query=query
            .arg(group_Id)
            .arg(course_Id)
            .arg(lesson_date);
    QByteArray arr;
    std::string res= query.toStdString();
    arr.append(QByteArray::fromStdString(res));
    socket->write(arr);
    socket->waitForBytesWritten(10);
}

void RatesPage::on_Main_Page_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);
}
void RatesPage::on_GeneralListStudent_triggered()
{
    ui->stackedWidget->setCurrentIndex(2);
    QString str="{\"type\":\"select\", \"params\":\"allStudents\"}";
    QByteArray arr;
    std::string res= str.toStdString();
    arr.append(QByteArray::fromStdString(res));
    socket->write(arr);
    socket->waitForBytesWritten(10);
}
void RatesPage::on_ChooseStudent_AllStudent_currentIndexChanged(int index)
{
    QString student_Id=ui->ChooseStudent_AllStudent->currentData().toString();
    getCoursesForStudent(student_Id);
}
void RatesPage::on_ChooesCourse_AllStudent_currentIndexChanged(int index)
{
    QString student_Id=ui->ChooseStudent_AllStudent->currentData().toString();
    QString course_Id=ui->ChooesCourse_AllStudent->currentData().toString();
    getLessonsForStudentAndCourse(student_Id, course_Id);
}
void RatesPage::on_SaveRateButton_clicked()
{
    QString rate=CurrentRate();
    if(rate==nullptr){
        QMessageBox::warning(this,
        "Добавление/изменение балла за занятие", "Необходимо ввести балл за занятие");
    }
    else{
        int Student_Id=CurrentStudent();
        int Lesson_Id=CurrentLesson();
        if(Student_Id!=0 && Lesson_Id!=0){
            setRateToStudentInGenerallist(rate, Student_Id, Lesson_Id);
         }
    }
}
void RatesPage::on_ChooseGroupToAddLesson_currentIndexChanged(int index)
{
    QString group_Id=ui->ChooseGroupToAddLesson->currentData().toString();
    if(group_Id!=nullptr){
        get_courses_for_add_lesson(group_Id);
    }
}
