#pragma once
#ifndef COUNTINGWORDMWINDOW_H
#define COUNTINGWORDMWINDOW_H
#include <QMainWindow>
#include "DictionaryLoader.h"
#include "DictionaryModel.h"

namespace Ui {
    class CountingWordMWindow;
    }
class CountingWordMWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit CountingWordMWindow(QWidget *parent = 0);
    ~CountingWordMWindow();
signals:
    void stop();
protected slots:
    void resetGUI();
    void showRunButton();
    void showStopButton();
    void updateGUIData();
    void gettingResult();
private slots:
    void on_buttonChooseFile_clicked();
    void on_buttonRun_clicked();
    void on_buttonStop_clicked();
    void on_buttonSaveResult_clicked();

private:
    Ui::CountingWordMWindow *ui;
    QString fileWay;
    DictionaryLoader* dictionaryLoader;
    DictionaryModel* dictionaryModel;
    QTime time;
    const QString buttonRunCountingLabel, buttonStopCountingLabel;

    static const int maxWordsShown = 10;
};

#endif // COUNTINGWORDMWINDOW_H
