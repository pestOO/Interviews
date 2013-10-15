#include "CountingWordMWindow.h"
#include "ui_CountingWordMWindow.h"
#include <QFileDialog>
#include <QMessageBox>

CountingWordMWindow::CountingWordMWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::CountingWordMWindow), dictionaryLoader(NULL)
    {
    ui->setupUi(this);
    dictionaryModel = new DictionaryModel(this);
    ui->tablePopularWords->setModel(dictionaryModel);
    ui->progressBar->setFormat("%v/%m KiB");
    resetGUI();
    }
CountingWordMWindow::~CountingWordMWindow()
    {
    emit stop();
    if(dictionaryLoader)
        {
        dictionaryLoader->deleteLater();
        dictionaryLoader->thread()->deleteLater();
        }
    delete ui;
    }
void CountingWordMWindow::resetGUI()
    {
    ui->progressBar->reset();
    ui->labelTimeElapsed->setText( QTime(0,0,0).toString() );
    ui->labelTimeRemain->setText( tr("unknown"));
    ui->labelWordCount->setText( QVariant(0).toString() );
    dictionaryModel->setTopDictionary(QMultiMap<qint32, QString>());
    ui->buttonStop->setEnabled(false);
    ui->buttonSaveResult->setEnabled(false);
    showRunButton();
    }
void CountingWordMWindow::showRunButton()
    {
    ui->buttonRun->show();
    ui->buttonStop->hide();
    ui->buttonRun->setEnabled(true);
    }
void CountingWordMWindow::showStopButton()
    {
    ui->buttonStop->show();
    ui->buttonRun->hide();
    ui->buttonStop->setEnabled(true);
    }
void CountingWordMWindow::updateGUIData()
    {
    Q_ASSERT(dictionaryLoader);

    const int mSecs = time.elapsed();
    const QTime elapsedTime = QTime().addMSecs(mSecs);
    ui->labelTimeElapsed->setText( elapsedTime.toString("hh:mm:ss") );

    //exponent should be 2 (worth), but optimal for user'll be 1.5
    const qreal doneWork = pow(dictionaryLoader->getCurrentFilePos(), 1.5) /
                           pow(dictionaryLoader->getFileSize(), 1.5);
    ui->labelTimeRemain->setText(QTime().addMSecs( (1.0-doneWork)/doneWork*mSecs).toString("hh:mm:ss"));
    ui->labelWordCount->setText( QVariant(dictionaryLoader->countWords()).toString() );

    dictionaryModel->setTopDictionary(dictionaryLoader->getMaxWords(CountingWordMWindow::maxWordsShown));

    if(!dictionaryLoader->isStopped() && !dictionaryLoader->thread()->isFinished())
        {
        ui->progressBar->setValue(dictionaryLoader->getCurrentFilePos()/1024);
        QTimer::singleShot(1000, this, SLOT(updateGUIData()));
        }
    else
        {
        ui->labelTimeRemain->setText(tr("unknown"));
        //don't hide size indication on cansel
        if(!dictionaryLoader->isStopped())
            ui->progressBar->setValue(ui->progressBar->maximum());
        }
    }
void CountingWordMWindow::on_buttonChooseFile_clicked()
    {
    //use getOpenFileName for windows native file dialog
    const QString newFileWay = QFileDialog::getOpenFileName( this,	tr("Select file") );
    if(!newFileWay.isNull())
        {
        if(!QFile::exists(newFileWay))
            {
            QMessageBox::warning(this, tr("Select file error"), tr("No such file %1").arg(newFileWay));
            return;
            }
        resetGUI();
        fileWay = newFileWay;
        ui->buttonSaveResult->setEnabled(false);
        statusBar()->showMessage(fileWay);
        }
    }
void CountingWordMWindow::on_buttonRun_clicked()
    {
    if(dictionaryLoader)
        delete dictionaryLoader;
    dictionaryLoader = new DictionaryLoader(fileWay);
    QThread* thread = new QThread;
    dictionaryLoader->moveToThread(thread);
    connect(thread,         SIGNAL(started()),  dictionaryLoader,SLOT(load()));
    connect(this,           SIGNAL(stop()),     dictionaryLoader,SLOT(stop()));
    connect(dictionaryLoader,SIGNAL(finished()), thread,         SLOT(quit()));
    connect(dictionaryLoader,SIGNAL(finished()), this,           SLOT(gettingResult()));

    thread->start();

    ui->progressBar->setRange(0, dictionaryLoader->getFileSize()/1024);

    time = QTime::currentTime();
    QTimer::singleShot(100, this, SLOT(updateGUIData()));

    ui->buttonChooseFile->setEnabled(false);
    showStopButton();
    ui->buttonSaveResult->setEnabled(false);
    }
void CountingWordMWindow::on_buttonStop_clicked()
    {
    Q_ASSERT(dictionaryLoader);
    emit stop();
    ui->buttonStop->setEnabled(false);
    }
void CountingWordMWindow::on_buttonSaveResult_clicked()
    {
    //use getOpenFileName for windows native file dialog
    const QString newFileWay = QFileDialog::getSaveFileName( this,	tr("Select file to save statistics") );
    if(!newFileWay.isNull())
        {
        setCursor(Qt::WaitCursor);
        dictionaryLoader->saveToFile(newFileWay);
        unsetCursor();
        }
    }
void CountingWordMWindow::gettingResult()
    {
    unsetCursor();
    updateGUIData();
    const bool isCompletlyFinished = !dictionaryLoader->isStopped();
    ui->buttonChooseFile->setEnabled(true);
    showRunButton();
    ui->buttonRun->setEnabled(!isCompletlyFinished);
    ui->buttonSaveResult->setEnabled(isCompletlyFinished);
    }
