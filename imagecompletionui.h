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

#define DELETEPTR(ptr) if(ptr) { delete ptr; ptr = 0; }


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


private:

    bool maybeSave();

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

    QList<QIcon> lableIcons;
    EDITSTEP _step;
    bool IsShowDetail;

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
    void	open();
    void    batchOpen();
    void	save();
    void	saveAs();

    void	close();

    void    search();
    void    addtosql();
    void    exportData();
    void    importData();

    void exitApp();
    void selectColor();

    // Basic Operation
    void scaling();
    void brighting();
    void notColor();
    void measure();

    void actionSliderReleased();

    void userManagement();

private slots:

    ////////////////////////////////////////////////////////////////////////////////////
    //   switch between different module
    ////////////////////////////////////////////////////////////////////////////////////
    void switchModule();

    void _RegionupdateBrushSize();

    void _SceneupdateBrushSize();

    void updateMethod();

    void changeLabel();
    void updateLineWidth();
    void strikeComboChanged(int index);
    void strikeThicknessComboboxChanged(int);
    void lineThicknessComboboxChanged(int);

    void labellingMethodChanged(QAction*);

    char* getNewLogString();
    void strikeChangeTriggered(QAction*);

    void strikeThicknessChanged(QAction*);
    void lineThicknessChanged(QAction*);

    void redo();
    void undo();

    void cellDoubleClicked_(int, int);

    void removeImage(QString filename);

private:
    Searchdata *searchdata1;
    AdvanceSearchDlg *_advanceSearchDlg;
    UserManagement *userMangementDlg;
    classification class1;
    buttom buttom1;
    QString _imageName;
    QString _imagePath;
    int _cnt;

    QString labelStatus(QString absolutePath);
    void showThumbnail(QString status, int row);

    std::vector<QString> _fNames;
    double _imageScale;

private:
    bool copyFiles(QString fromDir,QString toDir,bool convertIfExits = false);
    bool importDB(const QString &path);
    bool exportDB(const QString &path);

    bool openImage(QString file);
    QColor getColor(QString status);

    void clearBottomWindow();
    QImage loadLabelledResult(QString file);
    void setBackgroundColor(QString path, QColor color);
    void setImageState(QString path, QString state);

    int rowIndex(QString image);
    void showImagesInTree();

    QStandardItemModel *_treeModel;

private slots:
    void flushBottom();
    bool copyOrgImage(QString name);
    void flushLeft(QString path, QString label);
    void showContextMenu(QPoint);
    void editProperties();
    void on__treeViewImages_doubleClicked(const QModelIndex &index);
};

#endif // IMAGECOMPLETIONUI_H
