#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QListWidget>
#include <QString>
#include <QVector>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include <QCoreApplication>
#include <QDir>

Nodebst* root = nullptr;  // Define the root pointer
QVector<QString> words;  // Define the words vector

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QVector<QString> words = loadwordsfromfile();//directly after the execution we load our data (sortedwords.txt-->words<word>)

   // qDebug() << "Number of words loaded:" << words.size();//just to see if the words successfully are loaded in the vector

    root =MainWindow::buildbalancedbst(words, 0, words.size() - 1);//we built or bst(words<word>-->binary search tree BST)
    //again to see if its built or not
    /*if (root == nullptr) {
        qDebug() << "BST root is null.";
    } else {
        qDebug() << "BST root value:" << root->word;
    }
    //so its working here*/
    //MainWindow::inOrderTraversal(root); //and here too


    ui->resultsnumber->setValue(20);
    ui->numberofletterstostart->setValue(1);
    ui->resultsnumber->setRange(0, 100000);  // Set the rangeof the results number  (max)



    connect(ui->searchbox, &QLineEdit::textChanged, this, [this](const QString &text) {
        performsearch(text);
    });


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_searchbox_textChanged(const QString &arg1)
{

}


void MainWindow::on_addwordinput_cursorPositionChanged(int arg1, int arg2)
{

}


void MainWindow::on_addwordbutton_clicked()
{
  addword();
}


void MainWindow::on_numberofletterstostart_valueChanged(int arg1)
{

}


void MainWindow::on_resultsnumber_valueChanged(int arg1)
{

}


void MainWindow::on_resultslist_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{

}

//functions definitions
//to insert/creat a new element in our tree /recursive calling untill the correct and empty place/subtree is found
Nodebst * MainWindow::insertword(Nodebst* root, const QString& word) {
    if (root == nullptr) {
        return new Nodebst(word);
    } else if (word < root->word) {
        root->left = insertword(root->left, word);
    } else {
        root->right = insertword(root->right, word);
    }
    return root;
}
//to traverse through the tree from the smallest element to the largest,and write each in order on our outt file
//so basically we pile all thats on the left on all levels before we start writig it(like the lowest sub on the left then one before lowest sub on the left we go its right and than back up again...)
//and then almost same thing with the right subs except that only when were outof lefts we go up then right,or up up then right then left then left again...
void MainWindow::inordertraversalandwrite(Nodebst* root, QTextStream& out) {
    if (root != nullptr) {
        inordertraversalandwrite(root->left, out);
        out << root->word << "\n";
        inordertraversalandwrite(root->right, out);
    }
}
//this is to load words from our data file and puts them in a vector of strings
QVector<QString> MainWindow::loadwordsfromfile() {
    QVector<QString> words;
    //qDebug() << "Current working directory: " << QDir::currentPath();
    QFile file("C:\\Users\\client\\Documents\\acproject111\\sortedwords.txt");
    if (!file.exists()) {
        QMessageBox::warning(this, "File Not Found", "sortedwords.txt was not found.");
        return words;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) { // Open the file for writing in text mode
        QMessageBox::warning(this, "Could not open file!", "System failed to open important files"); // If the file can't be opened, show a warning
        return words;
    }

    QTextStream in(&file);// Create a QTextStream object to read to the file
    QString word;
    //QMessageBox::information(this,"dude","file was opened and next is putting stuff in that vector");
    while (!in.atEnd()) {//stop at the end of the file
        in.readLineInto(&word);//since in our file each line has one word, we read it line by line
        words.push_back(word);//since it is already sorted as wee need we push it to the end of the vector
    }
    //QMessageBox::information(this,"dude","file was opened and did we putt stuff in that vector?");
    file.close();//close the file afer reading
    return words;
}

//so basically we have lexicographically(like in dectionary) sorted wordds inside a vectore, jsut inserting them in that order in bst creates a highly imbalanced bst
//so i used this method where i take the element in the middle and then the element in the left side as left and the right side as right ,and just keep splitting like that
//untill out of wordds and that is when our starting point excceds our ending element/index so we stop recuring at that sub and then insert, and all that should creat a balanced bst
Nodebst* MainWindow::buildbalancedbst(QVector<QString>& words, int start, int end) {
    //QMessageBox::warning(this,"dude","bst was called");
    if (start > end) {
        return nullptr;
    }
    int mid = (start + end) / 2;
    Nodebst* root = new Nodebst(words[mid]);
    root->left = buildbalancedbst(words, start, mid - 1);
    root->right = buildbalancedbst(words, mid + 1, end);
    return root;
}
//to add words to our already built and balanced bst while running the program(kinda just a helper function)
void MainWindow::addword() {
    QString wordtoadd = ui->addwordinput->text();  // Get the word from the input thingy in the ui
    //isEmpy is built in method in <QString>
    if (wordtoadd.isEmpty()) {  // If the input is empty, show a warning msg
        QMessageBox::warning(this, "empty word!", "You didn't type a word");
        return;
    }

    if (wordexists(root, wordtoadd)) {  // Check if the word already exists in the BST
        QMessageBox::warning(this, "Duplicate Word!", "That word already exists in the list.");
    } else {
        root = insertword(root, wordtoadd);  // Insert the word into the BST
        savewordstofile();  // Save the updated tree to the file so we keep the database uptodate even after closing the program
        QMessageBox::information(this, "Word Added!", "The word was added successfully.");//msg
    }
}
//checks on the bst if the wordd exist or not;
bool MainWindow::wordexists(Nodebst* root, const QString& word) {
    if (root == nullptr) {  // If the tree is empty or the word isn't found, return false
        return false;
    }

    if (root->word == word) {  // If the current node's word matches the search word, return true
        return true;
    }

    // Recursively search the left and right subtrees
    return wordexists(root->left, word) || wordexists(root->right, word);
}
//this too kinda helper function to perform the search in our bst
void MainWindow::performsearch(const QString& searchText) {
    //qDebug() << "performsearch called with text:" << searchText;//uk
    /*if (!root) {
        qDebug() << "BST root is null in performsearch.";
        return;
    }*/ //this was the freaking problem cuz when the root variable was assigned to our to be bst,it was redeclared Nodebst*root=build...
    //and that made it kinda just used inside that constructor and after it is destroyed/deconsttructed/(delete ui;) it too was goon so
    //the solution to this mistake was root=build... not redeclareing just assigning to a global variable that can be used later.
    int numberofresults = ui->resultsnumber->value();  // Get the number of results from the ui thingy
    int count = 0;  // Count the number of results found

    ui->resultslist->clear();  // Clear the list widget to show new results in the ui thingy
    if (searchText.length() >= ui->numberofletterstostart->value()) {  // Start search only if the input is long enough(we also get that number from the ui thingy)
        Nodebst* current = root;  // Start searching from the root of the BST
        searchtree(current, searchText, numberofresults, count);  // Perform the actual search
    }
}
//this to actually do the search
void MainWindow::searchtree(Nodebst* Nodebst, const QString& prefix, int numberofresults, int& count) {
    if (Nodebst == nullptr || count >= numberofresults) {  // Stop if we reach an empty node or if we've found enough results
        return;
    }

    if (Nodebst->word.startsWith(prefix, Qt::CaseInsensitive)) {  // Check if the current word starts with the search prefix
        if (count < numberofresults) {  // Only add the word if we haven't reached the limit of results
            ui->resultslist->addItem(new QListWidgetItem(Nodebst->word));  // Add the word to the list widget
            count++;  // Increment the result count
        }
    }

    searchtree(Nodebst->left, prefix, numberofresults, count);  // Recursively search the left subtree
    searchtree(Nodebst->right, prefix, numberofresults, count);  // Recursively search the right subtree
}
//this to save our updated tree to the datafile
void MainWindow::savewordstofile() {
    QFile file("C:\\Users\\client\\Documents\\acproject111\\sortedwords.txt");  // Open the file
    //qDebug() << "File operation at: " << __FILE__ << ":" << __LINE__;
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {  // Open the file for writing in text mode
        QMessageBox::warning(this, "System Failed", "The file could not be opened for writing!");  // If the file can't be opened, show a warning
        return;
    }

    QTextStream out(&file);  // Create a QTextStream object to write to the file
    inordertraversalandwrite(root, out);  // Traverse the tree and write the words to the file
    file.close();  // Close the file after writing
}
//this is a thingy to check if roott is used properly or not
/*void MainWindow::inOrderTraversal(Nodebst* root) {
    if (!root) return;
    inOrderTraversal(root->left);
    qDebug() << root->word; // Print the node's data
    inOrderTraversal(root->right);
}*/


