#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QListWidget>
#include <QString>
#include <QVector>
#include <QTextStream>
#include <QFile>



struct Nodebst
{
    QString word;
    Nodebst*left;
    Nodebst*right;

    //constructor
    Nodebst(QString newword):word(newword),left(nullptr),right(nullptr){};
};
//Nodebst* root = nullptr;  // Define the root pointer
//QVector<QString> words;  // Define the words vector

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //functions
    Nodebst* insertword(Nodebst* root, const QString& word);
    //void inordertraversal(Nodebst* root, QTextStream& out);
    Nodebst* buildbalancedbst(QVector<QString>& words, int start, int end);
    void savewordstofile();
    void inordertraversalandwrite(Nodebst* root, QTextStream& out);
    QVector<QString> loadwordsfromfile();
    void performsearch(const QString& searchText);
    void searchtree(Nodebst*Nodebst, const QString& prefix, int numberofresults, int& count);
    void addword();
    bool wordexists(Nodebst* root, const QString& word);

    void inOrderTraversal(Nodebst* root);//uk

private slots:
    void on_searchbox_textChanged(const QString &arg1);

    void on_addwordinput_cursorPositionChanged(int arg1, int arg2);

    void on_addwordbutton_clicked();

    void on_numberofletterstostart_valueChanged(int arg1);

    void on_resultsnumber_valueChanged(int arg1);

    void on_resultslist_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
