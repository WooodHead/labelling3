#ifndef IMAGECOMPLETIONUI_H
#define IMAGECOMPLETIONUI_H

#include <QtGui/QMainWindow>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QTextFormat>
#include <QWidgetAction>
#include <QDir>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlTableModel>

#include "ui_SceneCompletionWidget.h"
#include "ui_RegionCompetition.h"
#include "PaintTools.h"

#include "ui_LeftWindow.h"
#include "ui_ButtomWindow.h"
#include "ui_CornerWindow.h"
#include "searchdata.h"
#include "classification.h"
#include "buttom.h"
#include "advancesearchdlg.h"
#include "UserManagement.h"
#include "Global.h"


class QStackedWidget;
class QListWidget;
class QTabWidget;
class QComboBox;
class QScrollArea;
class QTableWidget;
class QPushButton;
class QTextEdit;
class ImageViewer;

class ImageCompletionUI : public QMainWindow
{
    Q_OBJECT

public:
    ////////////////////////////////////////////////////////////////////////////////////
    //   Default constructor
    ////////////////////////////////////////////////////////////////////////////////////
    ImageCompletionUI(QWidget *parent = 0, Qt::WFlags flags = 0);

    ////////////////////////////////////////////////////////////////////////////////////
    //   Default destructor
    ////////////////////////////////////////////////////////////////////////////////////
    ~ImageCompletionUI();

    enum EDITSTEP
    {
        MARKING,
        BOOSTING,
        TESTING,
        MARCHING,
        NONE,
        LOADFAILED
    };

protected:
    ///////////////////////////////////////////////////////////////////////
    //    Reimplementation of the event handler for close
    //    event in order to verify if all the images opened
    //    in the MDI interface are saved. Prompt the user
    //    if there are unsaved images for a decision on saving or not.
    ///////////////////////////////////////////////////////////////////////
    void	closeEvent(QCloseEvent *event);

private:

    bool maybeSave();

public:
    ////////////////////////////////////////////////////////////////////////////////////
    //     Setup Main window
    ////////////////////////////////////////////////////////////////////////////////////
    void	setupMainWindow();

    ////////////////////////////////////////////////////////////////////////////////////
    //    The function creates all the connections between the actions to slots.
    ////////////////////////////////////////////////////////////////////////////////////
    void	createActions();

    ////////////////////////////////////////////////////////////////////////////////////
    //    Create Menus
    ////////////////////////////////////////////////////////////////////////////////////
    void	createMenus();

    ////////////////////////////////////////////////////////////////////////////////////
    //    Setup ToolBar
    ////////////////////////////////////////////////////////////////////////////////////
    void createToolBars();

    ////////////////////////////////////////////////////////////////////////////////////
    //    Create all widgets and their layout
    ////////////////////////////////////////////////////////////////////////////////////
    void	setupWidgets();

    ////////////////////////////////////////////////////////////////////////////////////
    //    Setup ToolBar
    ////////////////////////////////////////////////////////////////////////////////////
    void	createStatusBar();

    void	createConnections();

    void	setupBrush();

    void	updateLog();

    void    uncheckMethods();
    void    uncheckStrikeOptions();
    void setStrikeOptionsEnabled(bool);
    ////////////////////////////////////////////////////////////////////////////////////
    //   The User Interface items automatically generated from the ui file
    ////////////////////////////////////////////////////////////////////////////////////
    //Ui::ImageCompletionUIClass	ui;

    ////////////////////////////////////////////////////////////////////////////////////
    //   Module selection combo box
    ////////////////////////////////////////////////////////////////////////////////////
    
    ////////////////////////////////////////////////////////////////////////////////////
    // Region Competition Operation
    ////////////////////////////////////////////////////////////////////////////////////

private:
    ////////////////////////////////////////////////////////////////////////////////////
    //   Some Widgets
    ////////////////////////////////////////////////////////////////////////////////////
    QMenu					*_menuFile;
    QMenu					*_menuWindow;
    QMenu					*_menuLabelling;
    QMenu					*_menuData;
    QMenu                   *_menuUser;


    QToolBar				*_editToolBar;
    QToolBar				*_moduleSelectToolBar;
    QComboBox				*_moduleSelectBox;

    QWidget                 *_centralwidget;
    QTabWidget				*_centralTabWidget;
    QWidget                 *_editTab;
    QScrollArea             *_editScrollArea;
    ImageViewer             *_editImageViewer;

    QListWidget             *_bottomListWidget;

    QDockWidget             *_rightOperationWidget;
    QWidget                 *_dockWidgetContents;
    QStackedWidget          *_operationStackedWidget;
    QWidget                 *_objectCompletionPage;
    QWidget                 *_sceneCompletionPage;
    QWidget                 *_videoCompletionPage;
    QWidget                 *_regionCompetitionPage;
    //QTableWidget		*_logWidget;
    //QListWidget			*_logWidget;
    QTextEdit				*_logWidget;
    QString                 *_logInformationString;
    QTimer                  *_timer;

    QList<QIcon> lableIcons;
    EDITSTEP _step;
    bool IsShowDetail;
    ////////////////////////////////////////////////////////////////////////////////////
    //   SceneCompletionWidget,
    //   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //   cannot be         *_sceneCompletionDialog
    //   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //	Oh My God 花了我一个晚上的时间, shit, shit, shit, shit, shit, shit,
    ////////////////////////////////////////////////////////////////////////////////////
    Ui::SceneCompletionWidget	  _sceneCompletionDialog;
    Ui::RegionCompetition         _regionCompetitionDialog;
    Ui::LeftWindow                _leftWindow;
    Ui::ButtomWindow              _bottomWindow;

    Ui::CornerWindow              _cornerWindow;
    //Ui::Searchdata                _searchWindow;

    QDockWidget         *_cornerWindowWidget;
    QWidget				*_cornerDockWindowContents;

    QDockWidget         *_logWindowWidget;
    QWidget             *_logDocWindowContents;

    QDockWidget			*_leftWindowWidget;
    QWidget				*_leftDockWindowContents;

    QDockWidget         *_bottomWindowWidget;
    QWidget             *_bottomDockWindowContents;


    ////////////////////////////////////////////////////////////////////////////////////
    //   buttons on _objectCompletionPage
    ////////////////////////////////////////////////////////////////////////////////////
    QPushButton			*_edgeDetectionButton;
    QPushButton			*_loadTemplateButton;
    QPushButton			*_shapeMatchButton;

    ////////////////////////////////////////////////////////////////////////////////////
    //   actions
    ////////////////////////////////////////////////////////////////////////////////////
    QAction				*_openAction;
    QAction				*_saveAction;
    QAction				*_saveAsAction;
    QAction				*_closeAction;
    QAction				*_exitAction;
    QAction				*_searchAction;
    QAction				*_addtosqlAction;
    QAction				*_saveresultAction;
    QAction                             *_exportDataAction;
    QAction                             *_importDataAction;

    QAction             *_openBatchAction;

    QComboBox           *_strikeCombobox;
    QComboBox           *_strikeThicknessCombobox;
    QComboBox           *_lineThicknessCombobox;

    QAction               *_fgAction;
    QAction               *_bgAction;
    QAction               *_eraserAction;

    QAction               *_rectAction;
    QAction               *_polygonAction;
    QAction               *_manualAction;
    QAction               *_saveLabelResultAction;
    QAction               *_saveMaskAction;

    //user
    QAction               *_userManagementAction;
    //
    QAction               *_strikeThickness[3];
    //
    QAction               *_lineThickness[3];

    QAction               *_redo;
    QAction               *_undo;
private:	
    ////////////////////////////////////////////////////////////////////////////////////
    //   variables for brush
    ////////////////////////////////////////////////////////////////////////////////////
    bool						_bStartMarking;
    int							_brushSize;
    PaintTools					*_brushTool;

    QColor _brush2_color;
    QColor _brush3_color;

    void showData();

protected:

    void			keyPressEvent(QKeyEvent *e);

private slots:
    ////////////////////////////////////////////////////////////////////////////////////
    //    Open file slot. This function will be executed when
    //    the open file acction is triggered. (Menu File->Open or Toolbar->Open)
    ////////////////////////////////////////////////////////////////////////////////////
    void	open();
    void    batchOpen();
    ////////////////////////////////////////////////////////////////////////////////////
    //    Save File slot. This function will be executed when
    //    the save file acction is triggered. (Menu File->Save or Toolbar->Save)
    ////////////////////////////////////////////////////////////////////////////////////
    void	save();

    ////////////////////////////////////////////////////////////////////////////////////
    //   Save As File slot. This function will be executed when
    //   the save as file acction is triggered. (Menu File->Save As)
    ////////////////////////////////////////////////////////////////////////////////////
    void	saveAs();

    ////////////////////////////////////////////////////////////////////////////////////
    //   Save As File slot. This function will be executed when
    //   the save as file acction is triggered. (Menu File->Save As)
    ////////////////////////////////////////////////////////////////////////////////////
    void	close();

    void        search();
    void        addtosql();
    void        savemarkresult();
    void        exportData();
    void        importData();

    ////////////////////////////////////////////////////////////////////////////////////
    //   In this function the active/inactive logic for the menu
    //   and toolbar items should be implemented.
    ////////////////////////////////////////////////////////////////////////////////////
    // void	updateMenus();

    void exitApp();
    void selectColor();

    // Basic Operation
    void scaling();
    void brighting();
    void notColor();

    void actionSliderReleased();

    void userManagement();

private slots:

    ////////////////////////////////////////////////////////////////////////////////////
    //   switch between different module
    ////////////////////////////////////////////////////////////////////////////////////
    void switchModule();

    //void updateMarkState();

    void _RegionupdateBrushSize();

    void _SceneupdateBrushSize();

    //void updateLabelTree();
    //void setLabelTree();
    //void InitializeIcons();
    //	void showLabelColor(QTreeWidgetItem*);
    //void showLabelColor(int i);
    //ImageViewer* getResultImage(){return _resultImageViewer;}

    void Excute();

    void loadLabelMap();

    void saveLabelMap();

    void updateMethod();

    void updateIsShowDetail();

    void UsePrior(int state);

    void changeLabel();
    void updateLineWidth();
    void strikeComboChanged(int index);
    void strikeThicknessComboboxChanged(int);
    void lineThicknessComboboxChanged(int);

    void labellingMethodChanged(QAction*);

    char* getNewLogString();
    void strikeChangeTriggered(QAction*);
    void saveLabelledResult();
    void saveUserLabels();
    void saveMask();

    void strikeThicknessChanged(QAction*);
    void lineThicknessChanged(QAction*);

    void redo();
    void undo();

    void on_tableWidget_doubleClicked(const QModelIndex &index);

    void cellDoubleClicked_(int, int);

private:
    Searchdata *searchdata1;
    AdvanceSearchDlg *advanceSearchDlg;
    UserManagement *userMangementDlg;
    classification class1;
    buttom buttom1;
    QString _imageName;
    int _cnt;

    QString labelStatus(QString absolutePath);
    void editImageProperties(QString fileName);

private:
    bool copyFiles(QString fromDir,QString toDir,bool convertIfExits = false);
    bool importDB(const QString &path);
    bool exportDB(const QString &path);

    void openImage(QString file);
    QColor getColor(QString status);
};



#endif // IMAGECOMPLETIONUI_H
