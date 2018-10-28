#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "centerframe.h"
#include <QMainWindow>

class QLabel;
class QComboBox;
class QSpinBox;
class QToolButton;

class MainWindow : public QMainWindow
{
   Q_OBJECT
public:
   MainWindow(QWidget *parent = 0);

   ~MainWindow();

   void createToolBar();

protected slots:
   void on_ColorBtn_clicked();
public slots:
   void penStyleChangged(int index=0);

   void penColorChangged();



private:
   CenterFrame *centerFrame;
   QLabel *styleLabel;
   QComboBox *styleComboBox;
   QLabel *widthLabel;
   QSpinBox *widthSpinBox;
   QToolButton *colorBtn;
   QToolButton *clearBtn;
};
#endif // MAINWINDOW_H
