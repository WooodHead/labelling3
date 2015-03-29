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
#include <QSqlDatabase>
#include <QToolButton>
#include <QStandardItemModel>

#include <deque>
using namespace std;

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
#include "MoliProperties.h"
#include "def.h"

#define DELETEPTR(ptr) if(ptr) { delete ptr; ptr = 0; }
#define THUMBNAILS_PER_ROW 4

typedef std::deque<QString>::iterator deque_it;


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
    ImageCompletionUI(QWidget *parent = 0, Qt::WFlags flags = 0);
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

public:
    void setupMainWindow();

    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();

    void setupWidgets();

    void createConnections();

    void setupBrush();

    void updateLog();

    void uncheckMethods();
    void uncheckStrikeOptions();
    void setStrikeOptionsEnabled(bool);

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

    QTabWidget              *_centralThumbnailTabWidget;
    QWidget                 *_thumbnailTab;
    QScrollArea             *_thumbnailScrollArea;
    QLabel*                 labels[20];

    QListWidget             *_bottomListWidget;

    QDockWidget             *_rightOperationWidget;
    QWidget                 *_dockWidgetContents;
    QStackedWidget          *_operationStackedWidget;
    QWidget                 *_objectCompletionPage;
    QWidget                 *_sceneCompletionPage;
    QWidget                 *_videoCompletionPage;
    QWidget                 *_regionCompetitionPage;
    QTextEdit				*_logWidget;
    QString                 *_logInformationString;
    QTimer                  *_timer;

    Ui::SceneCompletionWidget	  _sceneCompletionDialog;
    Ui::RegionCompetition         _regionCompetitionDialog;
    Ui::LeftWindow                _leftWindow;
    Ui::ButtomWindow              _bottomWindow;

    QDockWidget         *_cornerWindowWidget;
    QWidget				*_cornerDockWindowContents;

    QDockWidget         *_logWindowWidget;
    QWidget             *_logDocWindowContents;

    QDockWidget			*_leftWindowWidget;
    QWidget				*_leftDockWindowContents;

    QDockWidget         *_bottomWindowWidget;
    QWidget             *_bottomDockWindowContents;

    QStackedWidget      *_centralStackedWidget;


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
    QAction             *_exportDataAction;
    QAction             *_importDataAction;

    QAction             *_openBatchAction;

    QToolButton         *_strikeToolButton;
    QToolButton         *_strikeThicknessToolButton;
    QToolButton         *_lineThicknessToolButton;

    QAction               *_fgAction;
    QAction               *_bgAction;
    QAction               *_eraserAction;

    QAction               *_rectAction;
    QAction               *_polygonAction;
    QAction               *_manualAction;
    QAction               *_saveLabelResultAction;
    QAction               *_saveMaskAction;
    QAction               *_userManagementAction;

    QAction               *_strikeThickness[3];
    QAction               *_lineThickness[3];

    QAction               *_redo;
    QAction               *_undo;

    bool                  _bStartMarking;
    int					  _brushSize;
    PaintTools			  *_brushTool;

    QStandardItemModel    *_treeModel;

private slots:
    void	open();
    void	save();
    void	saveAs();

    void	close();

    void    search();
    void    addtosql();
    void    exportData();
    void    importData();

    void    exitApp();
    void    selectColor();

    // Basic operations
    void    scaling();
    void    brighting();
    void    notColor();
    void    measure();

    void    actionSliderReleased();

    void    userManagement();

private slots:
    void    updateBrushSize();
    void    updateMethod();

    void    changeLabel();
    void    updateLineThickness();

    void    methodChangeTriggered(QAction*);
    void    strikeChangeTriggered(QAction*);
    void    strikeThicknessChangeTriggered(QAction*);
    void    lineThicknessChangeTriggered(QAction*);

    void    redo();
    void    undo();

    void    cellDoubleClickedLeftWindow(int, int);

private:
    Searchdata *searchdata1;
    AdvanceSearchDlg *_advanceSearchDlg;
    UserManagement *userMangementDlg;
    classification class1;

    QString _strCurrentImagePath;
    double _imageScale;

    std::deque<QString> _dequeTodo;
    std::deque<QString> _dequeDone;

    QString     status(QString absolutePath);
    void        showThumbnail(QString status, int row);
    void        showThumbnailForLabelled(QString strFilePath);
    void        showThumbnailForUnLabelled(QString strFilePath);

    void        insertNewImage(QString status);
    void        showData();
    char*       getNewLogString();

private:
    bool copyFiles(QString fromDir,QString toDir,bool convertIfExits = false);
    bool importDB(const QString &path);
    bool exportDB(const QString &path);

    void    openImage(QString file);
    QColor  color(QString status);

    void clearBottomWindow();
    QImage loadLabelledResult(QString file);

    int rowIndex(QString image);
    void showImagesInTree();
    void inDeque(const QString& , std::deque<QString> &);
    int in(const QString& strFilePath, std::deque<QString> &);
    void showThumbnailsInCentral(QStringList list);

    bool eventFilter(QObject *, QEvent *);

private slots:
    void flushBottom();
    void flushLeft(QString path, QString label);
    void showContextMenu(QPoint);
    void editProperties();
    void syncFilePathStr(QString fName);
    void next();
    void on_dBTableWidget_8_cellDoubleClicked(int row, int column);
    void on_dBTableWidget_7_cellDoubleClicked(int row, int column);
    void on_dBTableWidget_6_cellDoubleClicked(int row, int column);
    void on_dBTableWidget_5_cellDoubleClicked(int row, int column);
    void on_dBTableWidget_4_cellDoubleClicked(int row, int column);
    void on_dBTableWidget_3_cellDoubleClicked(int row, int column);
    void on_dBTableWidget_2_cellDoubleClicked(int row, int column);
    void on_dBTableWidget_1_cellDoubleClicked(int row, int column);
};

#endif // IMAGECOMPLETIONUI_H
