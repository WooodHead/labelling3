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
#include <QFormLayout>

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
#include "advancesearchdlg.h"
#include "UserManagement.h"
#include "Global.h"
#include "MoliProperties.h"
#include "def.h"
#include "expdlg.h"
#include "impdlg.h"
#include "ImagePropertiesEditor.h"
#include "About.h"

#define THUMBNAILS_PER_ROW 6

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
    void updateStatusBar();

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

    QAction *_aboutAction;
    QAction *_docAction;

    QAction               *_redo;
    QAction               *_undo;

    bool                  _bStartMarking;
    int					  _brushSize;
    PaintTools			  *_brushTool;

    QStandardItemModel    *_treeModel;

    QFormLayout           *_formLayout;

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
    
    void setExpPath(QString sourcePicPath,QString resultPicPath,QString packgePath);
    void setImpPath(QString packgePath);

private:
    Searchdata          *searchdata1;
    AdvanceSearchDlg    *_advanceSearchDlg;
    UserManagement      *userMangementDlg;
    classification      class1;

    QString             _strCurrentImagePath;
    double              _imageScale;

    std::deque<QString> _dequeTodo;
    std::deque<QString> _dequeDone;

    bool                _canBack;
    QString             _strMoliId;
    
    QString _expSourcePicPath;
    QString _expResultPicPath;
    QString _expPackgePath;
    
    QString _impPackgePath;

    QString _strNextImage;

private: // methods

    QString     status(QString absolutePath);
    void        showThumbnail(QString strFilePath, QString status, int row);
    void        showThumbnailForLabelled(QString strFilePath);
    void        showThumbnailForUnLabelled(QString strFilePath);

    void        insertNewImage(QString status);
    void        showData();
    char*       getNewLogString();

    bool        copyFiles(QString fromDir,QString toDir,bool convertIfExits = true);
    bool        importDB(const QString &path);
    bool        exportDB(const QString &path);


    QColor      color(QString status);

    void        clearBottomWindow();
    QImage      loadLabelledResult(QString file);

    int         rowIndex(QString image);
    void        showImagesInTree();
    void        enDeque(const QString& , std::deque<QString> &);
    int         in(const QString& strFilePath, std::deque<QString> &);
    void        showThumbnailsInCentral(QStringList list);

    bool        eventFilter(QObject *, QEvent *);
    void        clearLayout(QLayout *layout);

    void        drawEnclosingRectangle(QPixmap& pixmap, const QColor color);
    void        loadMoliImage(QString moliId);

    void        loadAllImagesAndShowInLeftWindow();
    void        wheelEvent(QWheelEvent *event);
    void        changeMeasureButtonState(bool state);
    void        closeEvent(QCloseEvent *);
private slots:
    void        openImage(QString file);
    void        flushLeftTree();
    void        flush();
    void        flushLeft(QString path, QString label);
    void        showContextMenu(QPoint);
    void        editProperties();
    void        syncFilePathStr(QString fName);
    void        next();

    void        on_dBTableWidget_8_cellDoubleClicked(int row, int column);
    void        on_dBTableWidget_7_cellDoubleClicked(int row, int column);
    void        on_dBTableWidget_6_cellDoubleClicked(int row, int column);
    void        on_dBTableWidget_5_cellDoubleClicked(int row, int column);
    void        on_dBTableWidget_4_cellDoubleClicked(int row, int column);
    void        on_dBTableWidget_3_cellDoubleClicked(int row, int column);
    void        on_dBTableWidget_2_cellDoubleClicked(int row, int column);
    void        on_dBTableWidget_1_cellDoubleClicked(int row, int column);
    void        bottomWindowContextMenuEvent(const QPoint &);
    void        editImageProperties();
    void        addSamplePoint();
    void        deleteSamplePoint();

    void        nextImage();
    void        back();
    void        append();
    void        showAll();

    void        queryThumbnails(QStringList list);
    void        OnDoubleClickTreeView(QModelIndex);
    void on__spinBox_measure_valueChanged(int arg1);

    void about();
    void showDoc();
};

#endif // IMAGECOMPLETIONUI_H
