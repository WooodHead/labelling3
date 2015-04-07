#include <QtGui>
#include <QPixmap>
#include <QSqlQuery>
#include <QSqlError>

#include "imagecompletionui.h"
#include "interfaces.h"
#include "ImageViewer.h"
#include "ImageProperties.h"


ImageCompletionUI::ImageCompletionUI(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    _canBack             = false;
    _brushSize           = 3;
    _imageScale          = 1.0;
    _editImageViewer     = 0;
    _formLayout          = 0;
    _strCurrentImagePath = QString();
    _strMoliId           = QString();

    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    createActions();

    setupMainWindow();

    createMenus();

    createToolBars();

    setupWidgets();

    createStatusBar();

    createConnections();

    updateBrushSize();

    setStrikeOptionsEnabled(false);

    showData();

    showImagesInTree();
}

ImageCompletionUI::~ImageCompletionUI()
{
    DELETEPTR( _formLayout      );
    DELETEPTR( Global::Awesome  );
    DELETEPTR( _editImageViewer );
}

void ImageCompletionUI::setupMainWindow()
{
    this->resize( 1024, 800 );
    this->setMinimumSize( QSize(1024, 0) );
    this->showMaximized();
    this->setWindowTitle( MOLI_APP_TITLE );
}

void ImageCompletionUI::createMenus()
{
    _menuFile = menuBar()->addMenu( tr("&文件") );

    _menuFile->addAction( _openAction );
    _menuFile->addSeparator();
    _menuFile->addAction( _saveAction );
    _menuFile->addAction( _saveAsAction );

    _menuFile->addSeparator();
    _menuFile->addAction( _closeAction );
    _menuFile->addSeparator();
    _menuFile->addAction( _exitAction );

    _menuLabelling = menuBar()->addMenu( tr("&图像标注") );
    QMenu* submenu = _menuLabelling->addMenu( Global::Awesome->icon(pencil), tr("笔画标注") );
    submenu->addAction( _fgAction );
    submenu->addAction( _bgAction );
    submenu->addAction( _eraserAction );

    _menuLabelling->addAction( _rectAction );
    _menuLabelling->addAction( _polygonAction );
    _menuLabelling->addAction( _manualAction );
    _menuLabelling->addSeparator();

    submenu = _menuLabelling->addMenu(Global::Awesome->icon(circleo), tr("笔画粗细"));
    for(int i = 0; i < 3; i++) submenu->addAction(_strikeThickness[i]);

    submenu = _menuLabelling->addMenu( Global::Awesome->icon(minus_), tr("线条粗细") );
    for(int i = 0; i < 3; i++) submenu->addAction(_lineThickness[i]);

    _menuData=menuBar()->addMenu(tr("&数据管理"));
    _menuData->addAction(_searchAction);
    _menuData->addAction(_addtosqlAction);
    _menuData->addAction(_exportDataAction);
    _menuData->addAction(_importDataAction);

    // user management
    if(Global::Authority == "1") //admin
    {
        _menuUser = menuBar()->addMenu(tr("&用户管理"));
        _menuUser->addAction(_userManagementAction);
    }

    _menuWindow = menuBar()->addMenu( tr("&帮助") );
}

void	ImageCompletionUI::createActions()
{
    _openAction = new QAction( Global::Awesome->icon(folderopeno), tr("&打开"), this );
    _openAction->setObjectName(tr("_openAction"));
    connect(_openAction, SIGNAL(triggered()), this, SLOT(open()));

    _saveAction = new QAction( Global::Awesome->icon(floppyo), tr("&保存"), this );
    _saveAction->setObjectName(tr("_saveAction"));
    connect(_saveAction, SIGNAL(triggered()), this, SLOT(save()));

    _saveAsAction = new QAction(  Global::Awesome->icon(floppyo), tr("&另存为"), this );
    _saveAsAction->setObjectName(tr("_saveAsAction"));
    connect(_saveAsAction, SIGNAL(triggered()), this, SLOT( saveAs() ));

    _closeAction = new QAction( Global::Awesome->icon(times), tr("关闭"), this );
    connect(_closeAction, SIGNAL(triggered()), this, SLOT(close()));

    _exitAction = new QAction( Global::Awesome->icon(poweroff), tr("退出"), this );
    _exitAction->setObjectName(tr("_exitAction"));
    connect(_exitAction, SIGNAL(triggered()), this, SLOT(exitApp()));

    _searchAction = new QAction( Global::Awesome->icon(areachart), tr("查询"), this );
    _searchAction->setObjectName(tr("_searchAction"));
    connect(_searchAction, SIGNAL(triggered()), this, SLOT(search()));

    _addtosqlAction = new QAction( Global::Awesome->icon(reorder), tr("数据管理"), this );
    _addtosqlAction->setObjectName(tr("_addtosqlAction"));
    connect(_addtosqlAction, SIGNAL(triggered()), this, SLOT(addtosql()));

    _exportDataAction = new QAction( QIcon(":/new/prefix1/icons/export.png"), tr("批量导出数据"),this);
    _exportDataAction->setObjectName(tr("_exportDataAction"));
    connect(_exportDataAction, SIGNAL(triggered()), this, SLOT(exportData()));

    _importDataAction = new QAction(QIcon(":/new/prefix1/icons/import.png"), tr("批量导入数据"),this);
    _importDataAction->setObjectName(tr("_importDataAction"));
    connect(_importDataAction, SIGNAL(triggered()), this, SLOT(importData()));

    // Image Labelling
    _strikeToolButton = new QToolButton(this);
    _strikeToolButton->setText(tr("笔画标注"));
    _strikeToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    _strikeToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    _strikeToolButton->setIcon(Global::Awesome->icon(pencil));
    _strikeToolButton->setChecked(false);

    // Group 1
    _fgAction = new QAction( Global::Awesome->icon(suno), tr("前景"), this );
    _fgAction->setCheckable(true);
    _bgAction = new QAction( Global::Awesome->icon(moono), tr("背景"), this );
    _bgAction->setCheckable(true);
    _eraserAction = new QAction( Global::Awesome->icon(eraser), tr("橡皮"), this );
    _eraserAction->setCheckable(true);

    _strikeToolButton->addAction(_fgAction);
    _strikeToolButton->addAction(_bgAction);
    _strikeToolButton->addAction(_eraserAction);

    QActionGroup *group = new QActionGroup(this);
    group->addAction(_fgAction);
    group->addAction(_bgAction);
    group->addAction(_eraserAction);
    connect(group, SIGNAL(triggered(QAction*)), this, SLOT(strikeChangeTriggered(QAction*)));

    _rectAction = new QAction( Global::Awesome->icon(squareo), tr("矩形标注"), this );
    _rectAction->setCheckable(true);

    _polygonAction = new QAction( Global::Awesome->icon(staro), tr("多边形标注"), this );
    _polygonAction->setCheckable(true);

    _manualAction = new QAction( Global::Awesome->icon(pencilsquareo), tr("手工标注"), this );
    _manualAction->setCheckable(true);

    QActionGroup *group2 = new QActionGroup(this);
    group2->addAction(_rectAction);
    group2->addAction(_polygonAction);
    group2->addAction(_manualAction);
    connect(group2, SIGNAL(triggered(QAction*)), this, SLOT(methodChangeTriggered(QAction*)));

    _redo = new QAction( Global::Awesome->icon(repeat), tr("重做"), this );
    _redo->setObjectName( tr("_redo") );
    connect( _redo, SIGNAL(triggered()), this, SLOT(redo()) );

    _undo = new QAction( Global::Awesome->icon("undo"), tr("撤销"), this );
    _undo->setObjectName( tr("_undo") );
    connect( _undo, SIGNAL(triggered()), this, SLOT(undo()) );

    //
    _strikeThickness[0] = new QAction( tr("细"), this );
    _strikeThickness[1] = new QAction( tr("中"), this );
    _strikeThickness[2] = new QAction( tr("粗"), this );

    QActionGroup *group3 = new QActionGroup(this);
    for(int i = 0; i < 3; i++)
    {
        _strikeThickness[i]->setCheckable(true);
        group3->addAction(_strikeThickness[i]);
    }
    connect(group3, SIGNAL(triggered(QAction*)), this, SLOT(strikeThicknessChangeTriggered(QAction*)) );

    _lineThickness[0] = new QAction( tr("细"), this );
    _lineThickness[1] = new QAction( tr("中"), this );
    _lineThickness[2] = new QAction( tr("粗"), this );
    QActionGroup *group4 = new QActionGroup(this);
    for(int i = 0; i < 3; i++)
    {
        _lineThickness[i]->setCheckable(true);
        group4->addAction(_lineThickness[i]);
    }
    connect(group4, SIGNAL(triggered(QAction*)), this, SLOT(lineThicknessChangeTriggered(QAction*)) );

    _strikeThicknessToolButton = new QToolButton(this);
    _strikeThicknessToolButton->setText(tr("笔画粗细"));
    _strikeThicknessToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    _strikeThicknessToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    _strikeThicknessToolButton->setIcon(Global::Awesome->icon(circlethin));
    for(int i = 0; i < 3; i++)
    {
        _strikeThicknessToolButton->addAction(_strikeThickness[i]);
    }

    _lineThicknessToolButton = new QToolButton(this);
    _lineThicknessToolButton->setText(tr("线条粗细"));
    _lineThicknessToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    _lineThicknessToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    _lineThicknessToolButton->setIcon(Global::Awesome->icon(minus_));
    for(int i = 0; i < 3; i++)
    {
        _lineThicknessToolButton->addAction(_lineThickness[i]);
    }

    // User
    if(Global::Authority == "1") //admin
    {
        _userManagementAction = new QAction( Global::Awesome->icon(user), tr("用户管理"), this );
        _userManagementAction->setObjectName(tr("_userManagementAction"));
        connect( _userManagementAction, SIGNAL(triggered()), this, SLOT(userManagement()) );
    }
}

void	ImageCompletionUI::createToolBars()
{
    _editToolBar = addToolBar( tr("文件") );
    _editToolBar->addAction( _openAction );
    _editToolBar->addAction( _saveAction );
    _editToolBar->addAction( _closeAction );

    _editToolBar->addSeparator();

    _editToolBar->addWidget(_strikeToolButton);
    _editToolBar->addAction(_rectAction);
    _editToolBar->addAction(_polygonAction);
    _editToolBar->addAction(_manualAction);

    _editToolBar->addSeparator();

    _editToolBar->addAction(_redo);
    _editToolBar->addAction(_undo);

    _editToolBar->addSeparator();
    _editToolBar->addWidget(_strikeThicknessToolButton);
    _editToolBar->addWidget(_lineThicknessToolButton);

    _editToolBar->addSeparator();
    _editToolBar->addAction(_searchAction);
    _editToolBar->addAction(_addtosqlAction);
    _editToolBar->addAction(_exportDataAction);
    _editToolBar->addAction(_importDataAction);

    _editToolBar->addSeparator();

    if(Global::Authority == "1")
    {
        _editToolBar->addAction(_userManagementAction);
    }
}

void ImageCompletionUI::setupWidgets()
{

    int width = this->width();
    int height = this->height();

    _centralStackedWidget = new QStackedWidget(this);

    ////////////////////////////////////////////////////////////////////////////////////
    //   _centralTabWidget
    ////////////////////////////////////////////////////////////////////////////////////
    _centralTabWidget = new QTabWidget( _centralStackedWidget );
    _centralTabWidget->setObjectName( tr( "_centralTabWidget") );

    QVBoxLayout *centralwigetLayout = new QVBoxLayout;
    centralwigetLayout->addWidget(_centralTabWidget);
    centralwigetLayout->setAlignment(Qt::AlignTop);
    _centralStackedWidget->setLayout(centralwigetLayout);

    ////////////////////////////////////////////////////////////////////////////////////
    //   _editTab
    ////////////////////////////////////////////////////////////////////////////////////
    _editTab = new QWidget();
    _editTab->setObjectName(tr("_editTab"));

    _editScrollArea = new QScrollArea(_editTab);
    _editScrollArea->setObjectName(tr("scrollArea"));

    QGridLayout *tabLayout = new QGridLayout;
    tabLayout->addWidget(_editScrollArea);
    _editTab->setLayout(tabLayout);

    _editScrollArea->setAlignment( Qt::AlignCenter );
    _editScrollArea->setAutoFillBackground( true );
    _editScrollArea->setBackgroundRole(  QPalette::Dark );

    _editImageViewer = new ImageViewer( );
    _editScrollArea->setWidget( _editImageViewer );
    _editImageViewer->setObjectName( tr("_editImageViewer") );
    _editImageViewer->getMainWindow(this);
    _editImageViewer->setLineColor(QColor(colorTable[3], colorTable[4], colorTable[5]));

    _centralTabWidget->addTab( _editTab, Global::Awesome->icon(pictureo), QString("图像标注") );
    _centralTabWidget->setMaximumHeight(0.8 * height);
    _centralTabWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    QPalette palette;
    QPalette palette1;
    QBrush brush3(QColor(255, 255, 255, 255));
    brush3.setStyle(Qt::SolidPattern);
    QBrush brush2(QColor(236, 233, 216, 255));
    brush2.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::Base, brush2);
    palette1.setBrush(QPalette::Active, QPalette::Base, brush3);
    palette1.setBrush(QPalette::Inactive, QPalette::Base, brush3);
    palette1.setBrush(QPalette::Disabled, QPalette::Base, brush2);

    _centralTabWidget->setPalette(palette);

    // Thumbnail widget
    _centralThumbnailTabWidget = new QTabWidget(_centralStackedWidget);
    QVBoxLayout *centralThumbnailTabWidgetLayout = new QVBoxLayout;
    centralThumbnailTabWidgetLayout->addWidget(_centralThumbnailTabWidget);
    _centralStackedWidget->setLayout(centralThumbnailTabWidgetLayout);

    _thumbnailTab = new QWidget();
    _thumbnailTab->setObjectName(tr("_thumbnailTab"));

    _thumbnailScrollArea = new QScrollArea(_thumbnailTab);
    _thumbnailScrollArea->setObjectName(tr("_thumbnailScrollArea"));
    _formLayout = new QFormLayout;
    _thumbnailScrollArea->setLayout(_formLayout);

    QGridLayout *tabLayout2 = new QGridLayout;
    tabLayout2->addWidget(_thumbnailScrollArea);
    _thumbnailTab->setLayout(tabLayout2);

    _thumbnailScrollArea->setAlignment( Qt::AlignCenter );
    _thumbnailScrollArea->setAutoFillBackground( true );
    _thumbnailScrollArea->setBackgroundRole(  QPalette::Dark );

    _centralThumbnailTabWidget->addTab( _thumbnailTab, Global::Awesome->icon(pictureo), QString("缩略图") );
    _centralThumbnailTabWidget->setMaximumHeight(0.8 * height);
    _centralThumbnailTabWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    _centralThumbnailTabWidget->setPalette(palette);

    _centralStackedWidget->addWidget(_centralTabWidget);
    _centralStackedWidget->addWidget(_centralThumbnailTabWidget);
    _centralStackedWidget->setCurrentIndex(0);
    setCentralWidget(_centralStackedWidget);

    ////////////////////////////////////////////////////////////////////////////////////
    //   _rightOperationWidget
    ////////////////////////////////////////////////////////////////////////////////////
    _rightOperationWidget = new QDockWidget(tr("工具箱"),this );
    _rightOperationWidget->setObjectName(tr("_rightOperationWidget"));
    _rightOperationWidget->setWindowIcon( Global::Awesome->icon(inbox) );
    _rightOperationWidget->setMaximumSize(QSize(0.2*width, 0.8*height));
    _rightOperationWidget->setFloating(false);

    _dockWidgetContents = new QWidget( );
    _dockWidgetContents->setObjectName(tr("_dockWidgetContents"));

    ////////////////////////////////////////////////////////////////////////////////////
    //   _operationStackedWidget
    ////////////////////////////////////////////////////////////////////////////////////
    _operationStackedWidget = new QStackedWidget(_dockWidgetContents);
    _operationStackedWidget->setObjectName(tr("_operationStackedWidget"));
    _operationStackedWidget->setFrameShape(QFrame::StyledPanel);
    _operationStackedWidget->setLineWidth(1);

    QVBoxLayout *rightwidgetLayout = new QVBoxLayout;
    rightwidgetLayout->addWidget(_operationStackedWidget);
    rightwidgetLayout->setAlignment(Qt::AlignTop);
    _dockWidgetContents->setLayout(rightwidgetLayout);

    _sceneCompletionPage = new QWidget(  );
    _sceneCompletionPage->setObjectName(tr("_sceneCompletionPage"));

    _regionCompetitionPage = new QWidget(  );
    _regionCompetitionPage->setObjectName(tr("_regionCompetitionPage"));
    QScrollArea *rightscroArea = new QScrollArea(_operationStackedWidget);
    rightscroArea->setWidget(_regionCompetitionPage);
    _operationStackedWidget->addWidget( rightscroArea );

    ////////////////////////////////////////////////////////////////////////////////////
    //		_sceneCompletionPage
    ////////////////////////////////////////////////////////////////////////////////////
    _sceneCompletionDialog.setupUi( _sceneCompletionPage );
    _regionCompetitionDialog.setupUi(_regionCompetitionPage);
    _regionCompetitionDialog.buttonSelectColor->setIcon(Global::Awesome->icon(eyedropper));
    _regionCompetitionDialog.radioBrightness->setIcon(Global::Awesome->icon(adjust));
    _regionCompetitionDialog.radioScale->setIcon(QIcon(":/new/prefix1/icons/scale.png"));
    _regionCompetitionDialog.radioNotColor->setIcon(Global::Awesome->icon(circle));
    _regionCompetitionDialog.radioMesuarement->setIcon(QIcon(":/new/prefix1/icons/ruler.png"));
    _regionCompetitionDialog._line->setHidden(true);
    _regionCompetitionDialog._labelRulerText->setHidden(true);

    _rightOperationWidget->setWidget(_dockWidgetContents);
    this->addDockWidget(static_cast<Qt::DockWidgetArea>(2), _rightOperationWidget);

    _operationStackedWidget->setCurrentIndex(0);

    ////////////////////////////////////////////////////////////////////////////////////
    //   _leftWindowWidget
    ////////////////////////////////////////////////////////////////////////////////////
    _leftWindowWidget = new QDockWidget(tr("图谱信息"),this );
    _leftWindowWidget->setObjectName(tr("_leftWindowWidget"));
    _leftWindowWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    _leftWindowWidget->setMaximumSize(QSize(0.2*width, height));
    _leftWindowWidget->setMaximumHeight(height);
    _leftWindowWidget->setFloating(false);
    //
    _leftDockWindowContents = new QWidget( );
    _leftDockWindowContents->setObjectName(tr("_leftDockWindowContents"));

    _leftWindow.setupUi(_leftDockWindowContents);
    _leftWindow.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _leftWindow.tabWidgetLeftWindow->setCurrentIndex(0);
    _leftWindow.tabWidgetLeftWindow->removeTab(2);
    _leftWindowWidget->setWidget(_leftDockWindowContents);

    addDockWidget(Qt::LeftDockWidgetArea, _leftWindowWidget);

    // Hide Columns (Fix it in the future)
    _leftWindow.tableWidget->hideColumn(2);
    _leftWindow.tableWidget->hideColumn(1);

    ////////////////////////////////////////////////////////////////////////////////////
    //   _logWidget
    ////////////////////////////////////////////////////////////////////////////////////
    _logWidget = new QTextEdit;
    QPalette palette2;
    palette2.setBrush(QPalette::Active, QPalette::Base, brush3);
    palette2.setBrush(QPalette::Inactive, QPalette::Base, brush3);
    palette2.setBrush(QPalette::Disabled, QPalette::Base, brush2);
    _logWidget->setPalette(palette2);
    _logInformationString = new QString();
    _logWidget->setReadOnly(true);

    _logWindowWidget = new QDockWidget(tr("工作日志"),this)    ;
    _logWindowWidget->setObjectName("_logWindowWidget");
    _logWindowWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    _logWindowWidget->setWidget(_logWidget);
    _logWindowWidget->setMinimumHeight(0.27 * height);
    _logWindowWidget->setWindowIcon(Global::Awesome->icon(filetexto));
    addDockWidget(Qt::LeftDockWidgetArea, _logWindowWidget);

    ////////////////////////////////////////////////////////////////////////////////////
    //   _bottomWindowWidget
    ////////////////////////////////////////////////////////////////////////////////////
    _bottomWindowWidget = new QDockWidget(tr("数据库信息"),this );
    _bottomWindowWidget->setObjectName(tr("_bottomWindowWidget"));
    _bottomWindowWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
    _bottomWindowWidget->setMinimumHeight(0.35 * height);
    _bottomDockWindowContents = new QWidget( );
    _bottomDockWindowContents->setObjectName(tr("_bottomDockWindowContents"));

    _bottomWindow.setupUi(_bottomDockWindowContents);

    _bottomWindow.dBTableWidget_1->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _bottomWindow.dBTableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _bottomWindow.dBTableWidget_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _bottomWindow.dBTableWidget_4->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _bottomWindow.dBTableWidget_5->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _bottomWindow.dBTableWidget_6->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _bottomWindow.dBTableWidget_7->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _bottomWindow.dBTableWidget_8->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _bottomWindow.dBTableWidget_9->setEditTriggers(QAbstractItemView::NoEditTriggers);

    _bottomWindow.dBTableWidget_1->setSelectionBehavior(QAbstractItemView::SelectRows);
    _bottomWindow.dBTableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    _bottomWindow.dBTableWidget_3->setSelectionBehavior(QAbstractItemView::SelectRows);
    _bottomWindow.dBTableWidget_4->setSelectionBehavior(QAbstractItemView::SelectRows);
    _bottomWindow.dBTableWidget_5->setSelectionBehavior(QAbstractItemView::SelectRows);
    _bottomWindow.dBTableWidget_6->setSelectionBehavior(QAbstractItemView::SelectRows);
    _bottomWindow.dBTableWidget_7->setSelectionBehavior(QAbstractItemView::SelectRows);
    _bottomWindow.dBTableWidget_8->setSelectionBehavior(QAbstractItemView::SelectRows);
    _bottomWindow.dBTableWidget_9->setSelectionBehavior(QAbstractItemView::SelectRows);

    _bottomWindow.dBTableWidget_1->setSelectionMode ( QAbstractItemView::SingleSelection);
    _bottomWindow.dBTableWidget_2->setSelectionMode ( QAbstractItemView::SingleSelection);
    _bottomWindow.dBTableWidget_3->setSelectionMode ( QAbstractItemView::SingleSelection);
    _bottomWindow.dBTableWidget_4->setSelectionMode ( QAbstractItemView::SingleSelection);
    _bottomWindow.dBTableWidget_5->setSelectionMode ( QAbstractItemView::SingleSelection);
    _bottomWindow.dBTableWidget_6->setSelectionMode ( QAbstractItemView::SingleSelection);
    _bottomWindow.dBTableWidget_7->setSelectionMode ( QAbstractItemView::SingleSelection);
    _bottomWindow.dBTableWidget_8->setSelectionMode ( QAbstractItemView::SingleSelection);
    _bottomWindow.dBTableWidget_9->setSelectionMode ( QAbstractItemView::SingleSelection);

    _bottomWindowWidget->setWidget(_bottomDockWindowContents);
    addDockWidget(Qt::BottomDockWidgetArea, _bottomWindowWidget);

    setCorner(Qt::BottomLeftCorner,Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner,Qt::BottomDockWidgetArea);

    QPixmap pixmap(30,30);
    pixmap.fill(_editImageViewer->getLineColor());
    _regionCompetitionDialog.labelCurrentColor->setPixmap(pixmap);
}

void ImageCompletionUI::createStatusBar()
{
    statusBar()->showMessage(tr("已就绪"));
}

void ImageCompletionUI::createConnections()
{
    connect(_centralTabWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

    connect(_regionCompetitionDialog.brushSizeSmallRadio, SIGNAL(clicked()), this, SLOT(updateBrushSize()));
    connect(_regionCompetitionDialog.brushSizeMediumRadio, SIGNAL(clicked()), this, SLOT(updateBrushSize()));
    connect(_regionCompetitionDialog.brushSizeLargeRadio, SIGNAL(clicked()), this, SLOT(updateBrushSize()));

    connect(_regionCompetitionDialog.radioStrikeLabelling, SIGNAL(clicked()), this, SLOT(updateMethod()));
    connect(_regionCompetitionDialog.radioRectLabelling, SIGNAL(clicked()), this, SLOT(updateMethod()));
    connect(_regionCompetitionDialog.radioPolygonLabelling, SIGNAL(clicked()), this, SLOT(updateMethod()));
    connect(_regionCompetitionDialog.radioManualLabelling, SIGNAL(clicked()), this, SLOT(updateMethod()));

    connect(_regionCompetitionDialog.buttonSelectColor, SIGNAL(clicked()), this, SLOT(selectColor()) );

    connect(_regionCompetitionDialog.radioForeground, SIGNAL(clicked()), this, SLOT(changeLabel()));
    connect(_regionCompetitionDialog.radioBackground, SIGNAL(clicked()), this, SLOT(changeLabel()));
    connect(_regionCompetitionDialog.radioErazer, SIGNAL(clicked()), this, SLOT(changeLabel()));

    // Basic Operation
    connect(_regionCompetitionDialog.radioScale, SIGNAL(clicked()), this, SLOT(scaling()));
    connect(_regionCompetitionDialog.radioBrightness, SIGNAL(clicked()), this, SLOT(brighting()));
    connect(_regionCompetitionDialog.radioNotColor, SIGNAL(clicked()), this, SLOT(notColor()));
    connect(_regionCompetitionDialog.radioMesuarement, SIGNAL(clicked()), this, SLOT(measure()));

    connect(_regionCompetitionDialog.sliderBasicOp, SIGNAL(sliderReleased()), this, SLOT(actionSliderReleased()));

    connect(_regionCompetitionDialog.radioLineWidthThin, SIGNAL(clicked()), this, SLOT(updateLineThickness()));
    connect(_regionCompetitionDialog.radioLineWidthMedium, SIGNAL(clicked()), this, SLOT(updateLineThickness()));
    connect(_regionCompetitionDialog.radioLineWidthThick, SIGNAL(clicked()), this, SLOT(updateLineThickness()));

    connect(_leftWindow.tableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(cellDoubleClickedLeftWindow(int, int)));
    connect(_bottomWindow.dBTableWidget_8, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(on_dBTableWidget_8_cellDoubleClicked(int, int)));
    connect(_bottomWindow.dBTableWidget_7, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(on_dBTableWidget_7_cellDoubleClicked(int, int)));
    connect(_bottomWindow.dBTableWidget_6, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(on_dBTableWidget_6_cellDoubleClicked(int, int)));
    connect(_bottomWindow.dBTableWidget_5, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(on_dBTableWidget_5_cellDoubleClicked(int, int)));
    connect(_bottomWindow.dBTableWidget_4, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(on_dBTableWidget_4_cellDoubleClicked(int, int)));
    connect(_bottomWindow.dBTableWidget_3, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(on_dBTableWidget_3_cellDoubleClicked(int, int)));
    connect(_bottomWindow.dBTableWidget_2, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(on_dBTableWidget_2_cellDoubleClicked(int, int)));
    connect(_bottomWindow.dBTableWidget_1, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(on_dBTableWidget_1_cellDoubleClicked(int, int)));
}

void ImageCompletionUI::showContextMenu(QPoint pos)
{
    // Menu 1
    QAction* editMenu = new QAction(tr("编辑磨粒属性"), this);
    connect(editMenu, SIGNAL(triggered()), this, SLOT(editProperties()));

    // Menu 2
    QAction *appendMenu = new QAction(tr("标注新磨粒"), this);
    connect(appendMenu, SIGNAL(triggered()), this, SLOT(append()));

    // Menu 4
    QAction* showAllMenu = new QAction(tr("显示所有磨粒"), this);
    connect(showAllMenu, SIGNAL(triggered()), this, SLOT(showAll()));

    if(_strCurrentImagePath.isEmpty())
    {
        editMenu->setEnabled(false);
        appendMenu->setEnabled(false);
        showAllMenu->setEnabled(false);
    }

    // Menu 3
    QAction* backMenu = new QAction(tr("回退"), this);
    connect(backMenu, SIGNAL(triggered()), this, SLOT(back()));
    if(_canBack == false)
    {
        backMenu->setEnabled(false);
    }

    QMenu menu;
    menu.addAction(editMenu);
    menu.addAction(appendMenu);
    menu.addAction(showAllMenu);
    menu.addSeparator();
    menu.addAction(backMenu);
    menu.exec(_centralTabWidget->mapToGlobal(pos));
}

void ImageCompletionUI::editProperties()
{
    if(_strCurrentImagePath.isEmpty()) return;

    QImage* result  = _editImageViewer->getResultDisplay();
    QImage* mask    = _editImageViewer->getMask();
    QImage* result2 = _editImageViewer->getResultSave();

    (new MoliProperties(this))->showDlg(_strCurrentImagePath, result != NULL ? *result : QImage(), result2 != NULL ? *result2 : QImage(), mask != NULL ? *mask : QImage(), _imageScale );
}

void ImageCompletionUI::syncFilePathStr(QString strFilePath)
{
    if(strFilePath.isEmpty() || !QFile::exists(strFilePath)) return;

    int pos = in(strFilePath, _dequeTodo);
    if( -1 == pos )
    {
        enDeque(strFilePath, _dequeTodo);
        pos = in(strFilePath, _dequeTodo);
        showThumbnail(MOLI_UNLABELLED_STATUS_CHAR, pos);
    }

    _strCurrentImagePath = strFilePath;
}

void ImageCompletionUI::next()
{
    if(!_dequeTodo.empty())
    {
        this->openImage(_dequeTodo[0]);
    }
}

void ImageCompletionUI::setupBrush()
{
    _brushTool = new PaintTools;
    BrushInterface *iBrush = qobject_cast<BrushInterface *>( _brushTool );
    _editImageViewer->setBrush( iBrush, "Pencil" );
}

void ImageCompletionUI::open()
{
    close();

    QSettings settings("ImageCompletion", "ImageCompletion");
    QString strLastImportPath = settings.value("lastImportPath", QDir::homePath()).toString();

    QString strFilePath = QFileDialog::getOpenFileName( this,
                                                        tr("打开图像"),
                                                        strLastImportPath,
                                                        MOLI_IMAGE_FILTER );
    if( strFilePath.isEmpty() )
    {
        return;
    }

    if( !QFile::exists(strFilePath) )
    {
        return;
    }

    settings.setValue("lastImportPath", QVariant(strFilePath));

    this->openImage(strFilePath);
}

void ImageCompletionUI::openbyquery(QString picid, QString picpath)
{
    close();

//    QSettings settings("ImageCompletion", "ImageCompletion");
//    QString strLastImportPath = settings.value("lastImportPath", QDir::homePath()).toString();

//    QString strFilePath = QFileDialog::getOpenFileName( this,
//                                                        tr("打开图像"),
//                                                        strLastImportPath,
//                                                        MOLI_IMAGE_FILTER );
    QMessageBox::warning(this,tr("提示"),picpath,QMessageBox::Close);

    if( picpath.isEmpty() )
    {
        return;
    }

    if( !QFile::exists(picpath) )
    {
        return;
    }

//    settings.setValue("lastImportPath", QVariant(picpath));
    QMessageBox::warning(this,tr("提示"),picpath,QMessageBox::Close);

    this->openImage(picpath);
}

void ImageCompletionUI::openImage(QString strFilePath)
{
    if( strFilePath.isEmpty() ) return;

    bool ok = _editImageViewer->openImage( strFilePath );
    if( !ok )
    {
        close();
        this->setStrikeOptionsEnabled( false );
        return;
    }

    _editImageViewer->repaint();
    _strCurrentImagePath = strFilePath;

    QString status = this->status( strFilePath );
    // New image
    if(status != MOLI_LABELLED_STATUS_CHAR && status != MOLI_UNLABELLED_STATUS_CHAR)
    {
        QMessageBox::StandardButton reply = QMessageBox::information(0,
                                                                     MOLI_MESSAGEBOX_TITLE_PROMPT_STRING,
                                                                     "此图像为新图像,是否要导入数据库?",
                                                                     QMessageBox::Ok | QMessageBox::Cancel);
        if(QMessageBox::Ok == reply)
        {
            (new ImageProperties(this))->showDlg( strFilePath );
        }
        else
        {
            close();
        }
    }
    // Labelled image
    else if(MOLI_LABELLED_STATUS_CHAR == status)
    {
        if(-1 == in(strFilePath, _dequeDone))
        {
            this->enDeque(strFilePath, _dequeDone);
            this->showThumbnailForLabelled(strFilePath);
        }

        this->loadLabelledResult( strFilePath );
    }
    else if(MOLI_UNLABELLED_STATUS_CHAR == status)
    {
        if(-1 == in(strFilePath, _dequeTodo))
        {
            this->enDeque(strFilePath, _dequeTodo);
            this->showThumbnailForUnLabelled( strFilePath );
        }
    }
}

void ImageCompletionUI::showImagesInTree()
{
    _leftWindow._treeViewImages->setAlternatingRowColors(true);
    _leftWindow._treeViewImages->setExpandsOnDoubleClick(false);

    QSqlDatabase db;
    if(!Global::createConnection(db))
    {
        QMessageBox::critical(0,
                              MOLI_MESSAGEBOX_TITLE_PROMPT_STRING,
                              tr("数据库连接失败!"),
                              QMessageBox::Close);
        return;
    }

    QSqlQuery query;
    query.prepare("select * from equipmentinfo");

    std::multimap<QString, QString> departMap;

    if(query.exec())
    {
        while(query.next())
        {
            departMap.insert(std::pair<QString, QString>(query.value(2).toString(), query.value(0).toString()));
        }
    }

    int cnt = 0;
    std::vector<QString> departs;
    for(std::multimap<QString, QString>::iterator it = departMap.begin(), end = departMap.end(); it != end;
        it = departMap.upper_bound(it->first))
    {
        cnt++;
        departs.push_back(it->first);
    }

    QStandardItem* item;
    QStandardItem* items[cnt];
    _treeModel = new QStandardItemModel(cnt,1);
    for(int i = 0; i < cnt; i++)
    {
        items[i] = new QStandardItem(departs[i]);
        items[i]->setIcon(Global::Awesome->icon(group));
        _treeModel->setItem(i, 0, items[i]);

        for(std::multimap<QString, QString>::iterator it = departMap.begin(); it != departMap.end(); it++)
        {
            if((*it).first == departs[i])
            {
                item = new QStandardItem((*it).second);
                item->setIcon(Global::Awesome->icon(plane));
                items[i]->appendRow(item);
            }

            // Append Images
            QSqlQuery query;
            QString sql = QString("select ferrographypicinfo.ferrographypicid from ferrographypicinfo join ferrographyinfo join oilsampleinfo on ferrographypicinfo.ferrographysheetid = ferrographyinfo.ferrographysheetid and ferrographyinfo.oilsampleid = oilsampleinfo.oilsampleid and oilsampleinfo.sampledepartid = '%1' and oilsampleinfo.planeid = '%2'").arg(departs[i]).arg((*it).second);

            query.prepare(sql);
            if(query.exec())
            {
                while(query.next())
                {
                    QString fName = query.value(0).toString();
                    QImage image(Global::PathImage + fName + ".jpg");

                    QStandardItem *item2 = new QStandardItem(fName + ".jpg");
                    item2->setIcon(QPixmap::fromImage(image));
                    item->appendRow(item2);
                }
            }
        }
    }

    _treeModel->setHeaderData(0, Qt::Horizontal, QString());
    _leftWindow._treeViewImages->setModel(_treeModel);
    _leftWindow._treeViewImages->setHeaderHidden(true);
    _leftWindow._treeViewImages->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _leftWindow._treeViewImages->setSelectionMode( QAbstractItemView::SingleSelection );
    _leftWindow._treeViewImages->show();

    db.close();
}

void ImageCompletionUI::enDeque(const QString &elem, std::deque<QString> &d)
{
    if( in(elem, d) >= 0 ) return;

    deque_it it;
    for(it = d.begin(); it != d.end(); it++)
    {
        if(elem < *it)
        {
            d.insert(it, elem);
            break;
        }
    }

    if(it == d.end())
    {
        d.insert(it, elem);
    }
}

int ImageCompletionUI::in(const QString& strFilePath, std::deque<QString> &d)
{
    int k;
    deque_it it;
    for(it = d.begin(), k = 0; it != d.end(); it++)
    {
        if(*it == strFilePath) return k;
        k++;
    }

    return -1;
}

void ImageCompletionUI::showThumbnail(QString status, int row)
{
    _leftWindow.tableWidget->insertRow(row);
    _leftWindow.tableWidget->setItem(row, 0, new QTableWidgetItem(""));
    QImage* temp = _editImageViewer->getOriginalImage();
    _leftWindow.tableWidget->item(row, 0)->setData(Qt::DecorationRole, QPixmap::fromImage(*temp).scaled(80, 80));
    _leftWindow.tableWidget->item(row, 0)->setBackgroundColor(color(status));

    DELETEPTR(temp);
}

//FIXME
void ImageCompletionUI::showThumbnailForLabelled(QString strFilePath)
{
    if(strFilePath.isEmpty() || !QFile::exists(strFilePath) ) return;

    int pos = this->in(strFilePath, _dequeDone);
    if(pos >= 0)
    {
        this->showThumbnail(MOLI_LABELLED_STATUS_CHAR, pos + _dequeTodo.size());
    }
}

void ImageCompletionUI::showThumbnailForUnLabelled(QString strFilePath)
{
    if(strFilePath.isEmpty() || !QFile::exists(strFilePath) ) return;

    int pos = this->in(strFilePath, _dequeTodo);
    if(pos >= 0)
    {
        this->showThumbnail(MOLI_UNLABELLED_STATUS_CHAR, pos);
    }
}

void	ImageCompletionUI::save()
{
    if ( Global::NewName.isEmpty() ) return;

    bool ret1, ret2;

    if(Global::PathResult.isEmpty())
    {
        QMessageBox::warning(this,
                             tr("保存"),
                             QString("请指定标注图像保存路径:%1").arg(QFileInfo(QApplication::instance()->applicationFilePath()).baseName() + ".ini"));
        return;
    }
    if(Global::PathMask.isEmpty())
    {
        QMessageBox::warning(this,
                             tr("保存"),
                             QString("请指定掩码图像保存路径:%1").arg(QFileInfo(QApplication::instance()->applicationFilePath()).baseName() + ".ini"));
        return;
    }

    if(!QDir(Global::PathResult).exists())
    {
        QDir().mkdir(Global::PathResult);
    }
    if(!QDir(Global::PathMask).exists())
    {
        QDir().mkdir(Global::PathMask);
    }

    // Save
    QString pathResult, pathMask;
    if(Global::MoliId.isEmpty())
    {
        pathResult = Global::PathResult + Global::NewName + "." + Global::ExtResult;
        pathMask = Global::PathMask + Global::NewName + "." + Global::ExtMask;
    }
    else
    {
        pathResult = Global::PathResult + Global::MoliId + "." + Global::ExtResult;
        pathMask = Global::PathMask + Global::MoliId + "." + Global::ExtMask;
    }
    if(!(ret1 = _editImageViewer->saveLabelledResult(pathResult, Global::ExtResult)))
    {
        QMessageBox::warning(this,
                             tr("保存"),
                             tr("保存标注图像失败"),
                             QMessageBox::Close);
    }

    if(!(ret2 = _editImageViewer->saveMask(pathMask, Global::ExtMask)))
    {
        QMessageBox::warning(this,
                             tr("保存"),
                             tr("保存掩码图像失败"),
                             QMessageBox::Close);
    }
}

void ImageCompletionUI::saveAs()
{
    if(_strCurrentImagePath.isEmpty()) return;

    QString pathResult, pathMask;
    bool ret1, ret2;

    if(!(ret1 = _editImageViewer->saveAsLabelledResult(pathResult)))
    {
        QMessageBox::warning(this,
                             tr("保存"),
                             tr("保存标注图像失败"),
                             QMessageBox::Close);
    }
    else if(!(ret2 = _editImageViewer->saveAsMask(pathMask)))
    {
        QMessageBox::warning(this,
                             tr("保存"),
                             tr("保存掩码图像失败"),
                             QMessageBox::Close);
    }
}

void	ImageCompletionUI::close()
{
    _editImageViewer->deleteImage();
    QPixmap pixmap(0, 0);
    _regionCompetitionDialog.showColorLabel->setPixmap(pixmap);
    _regionCompetitionDialog.radioForeground->setEnabled(false);
    _regionCompetitionDialog.radioBackground->setEnabled(false);
    _regionCompetitionDialog.radioErazer->setEnabled(false);
    _regionCompetitionDialog.radioForeground->setChecked(false);
    _regionCompetitionDialog.radioBackground->setChecked(false);
    _regionCompetitionDialog.radioErazer->setChecked(false);

    _strCurrentImagePath = QString();

    uncheckMethods();

    _editImageViewer->setDefaultCursor();
}

void	ImageCompletionUI::search()
{
    _advanceSearchDlg = new AdvanceSearchDlg(this);
    _advanceSearchDlg->show();
}

void	ImageCompletionUI::addtosql()
{
//    class1.show();
    _advanceSearchDlg = new AdvanceSearchDlg(this,true);
//    _advanceSearchDlg->setDeleteFlag(true);
    _advanceSearchDlg->show();
}

void ImageCompletionUI::updateLog()
{
    char* log_str = getNewLogString();
    if ( log_str )
    {
        (*_logInformationString) = log_str;
        _logWidget->clear();
        QTextCursor cursor(_logWidget->textCursor());
        cursor.movePosition(QTextCursor::End);
        QTextCharFormat textFormat;
        cursor.insertText((*_logInformationString), textFormat);
        cursor.insertBlock();
    }

    // Maybe bug here
    if(log_str)
    {
        DELETEPTR(log_str);
    }
}

void ImageCompletionUI::uncheckMethods()
{
    _regionCompetitionDialog.buttonGroup->setExclusive(false);
    _regionCompetitionDialog.radioStrikeLabelling->setChecked(false);
    _regionCompetitionDialog.radioRectLabelling->setChecked(false);
    _regionCompetitionDialog.radioPolygonLabelling->setChecked(false);
    _regionCompetitionDialog.radioManualLabelling->setChecked(false);
    _regionCompetitionDialog.buttonGroup->setExclusive(true);
}

void ImageCompletionUI::uncheckStrikeOptions()
{
    _regionCompetitionDialog.buttonGroup_4->setExclusive(false);
    _regionCompetitionDialog.radioForeground->setChecked(false);
    _regionCompetitionDialog.radioBackground->setChecked(false);
    _regionCompetitionDialog.radioErazer->setChecked(false);
    _regionCompetitionDialog.buttonGroup_4->setExclusive(true);
}

void ImageCompletionUI::showData()
{
    clearBottomWindow();

    QSqlDatabase db;
    if(!Global::createConnection(db))
    {
        QMessageBox::warning(0,
                             tr("数据库"),
                             tr("数据库连接失败!"),
                             QMessageBox::Cancel);
    }

    bool ret;
    QSqlRecord rec;

    QString tableNames[9] = { "equipmentinfo", "movepartinfo", "movepartrepairinfo", "oilsampleinfo", "oilanalyzeinfo", "ferrographyinfo", "ferrographypicinfo", "abrasivemarkinfo", "sampleSummaryInfo" };

    for(int k = 1; k <= 9; k++)
    {
        QSqlQuery query;
        ret = query.exec(QString("select * from %1").arg(tableNames[k-1]));
        rec = query.record();
        if(ret)
        {
            int i = 0;
            while(query.next())
            {
                for(int j=0; j<rec.count(); j++)
                {
                    switch(k)
                    {
                    case 1:
                    {
                        if( j == 0 ) _bottomWindow.dBTableWidget_1->insertRow(_bottomWindow.dBTableWidget_1->rowCount());
                        _bottomWindow.dBTableWidget_1->setItem(i, j, new QTableWidgetItem(query.value(j).toString()));
                        break;
                    }
                    case 2:
                    {
                        if( j == 0 ) _bottomWindow.dBTableWidget_2->insertRow(_bottomWindow.dBTableWidget_2->rowCount());
                        _bottomWindow.dBTableWidget_2->setItem(i, j, new QTableWidgetItem(query.value(j).toString())); break;
                    }
                    case 3:
                    {
                        if( j == 0 ) _bottomWindow.dBTableWidget_3->insertRow(_bottomWindow.dBTableWidget_3->rowCount());
                        _bottomWindow.dBTableWidget_3->setItem(i, j, new QTableWidgetItem(query.value(j).toString())); break;
                    }
                    case 4:
                    {
                        if( j == 0 ) _bottomWindow.dBTableWidget_4->insertRow(_bottomWindow.dBTableWidget_4->rowCount());
                        _bottomWindow.dBTableWidget_4->setItem(i, j, new QTableWidgetItem(query.value(j).toString())); break;
                    }
                    case 5:
                    {
                        if( j == 0 ) _bottomWindow.dBTableWidget_5->insertRow(_bottomWindow.dBTableWidget_5->rowCount());
                        _bottomWindow.dBTableWidget_5->setItem(i, j, new QTableWidgetItem(query.value(j).toString())); break;
                    }
                    case 6:
                    {
                        if( j == 0 ) _bottomWindow.dBTableWidget_6->insertRow(_bottomWindow.dBTableWidget_6->rowCount());
                        _bottomWindow.dBTableWidget_6->setItem(i, j, new QTableWidgetItem(query.value(j).toString())); break;
                    }
                    case 7:
                    {
                        if( j == 0 ) _bottomWindow.dBTableWidget_7->insertRow(_bottomWindow.dBTableWidget_7->rowCount());
                        _bottomWindow.dBTableWidget_7->setItem(i, j, new QTableWidgetItem(query.value(j).toString())); break;
                    }
                    case 8:
                    {
                        if( j == 0 ) _bottomWindow.dBTableWidget_8->insertRow(_bottomWindow.dBTableWidget_8->rowCount());
                        _bottomWindow.dBTableWidget_8->setItem(i, j, new QTableWidgetItem(query.value(j).toString())); break;
                    }
                    case 9:
                    {
                        if( j == 0 ) _bottomWindow.dBTableWidget_9->insertRow(_bottomWindow.dBTableWidget_9->rowCount());
                        _bottomWindow.dBTableWidget_9->setItem(i, j, new QTableWidgetItem(query.value(j).toString())); break;
                    }
                    }
                }
                i++;
            }
        }
    }

    db.close();
}

void ImageCompletionUI::updateBrushSize()
{
    if ( _editImageViewer->image().isNull() ) return;

    if (_regionCompetitionDialog.brushSizeSmallRadio->isChecked())
    {
        _brushSize = 5;
    }
    if (_regionCompetitionDialog.brushSizeMediumRadio->isChecked())
    {
        _brushSize = 15;
    }
    if (_regionCompetitionDialog.brushSizeLargeRadio->isChecked())
    {
        _brushSize = 30;
    }

    _editImageViewer->setBrushWidth( _brushSize );
}

void ImageCompletionUI::setStrikeOptionsEnabled(bool b)
{
    if(!b) // uncheck all
    {
        _regionCompetitionDialog.buttonGroup_4->setExclusive(false);
        _regionCompetitionDialog.radioForeground->setChecked(false);
        _regionCompetitionDialog.radioBackground->setChecked(false);
        _regionCompetitionDialog.radioErazer->setChecked(false);
        _regionCompetitionDialog.buttonGroup_4->setExclusive(true);
    }

    _regionCompetitionDialog.radioErazer->setEnabled(b);
    _regionCompetitionDialog.radioForeground->setEnabled(b);
    _regionCompetitionDialog.radioBackground->setEnabled(b);
}

void ImageCompletionUI::updateMethod()
{
    _editImageViewer->setPaintable(true);
    if (_regionCompetitionDialog.radioStrikeLabelling->isChecked())
    {
        _editImageViewer->setMethod(0);
        _editImageViewer->setLabelType(0);
        setStrikeOptionsEnabled(true);
        setupBrush();
    }
    else if(_regionCompetitionDialog.radioRectLabelling->isChecked())
    {
        _editImageViewer->setMethod(1);
        _editImageViewer->setLabelType(1);
        setStrikeOptionsEnabled(false);
    }
    else if(_regionCompetitionDialog.radioPolygonLabelling->isChecked())
    {
        _editImageViewer->setMethod(1);
        _editImageViewer->setLabelType(2);
        setStrikeOptionsEnabled(false);
    }
    else if(_regionCompetitionDialog.radioManualLabelling->isChecked())
    {
        _editImageViewer->setMethod(2);
        _editImageViewer->setLabelType(2);
        setStrikeOptionsEnabled(false);
    }
}

void ImageCompletionUI::changeLabel()
{
    if (_regionCompetitionDialog.radioBackground->isChecked())
    {
        _editImageViewer->changeObjectLabeling(0);
        QPixmap pixmap(30,30);
        pixmap.fill(qRgb(colorTable[0], colorTable[1], colorTable[2]));
        _regionCompetitionDialog.showColorLabel->setPixmap(pixmap);
    }
    else if (_regionCompetitionDialog.radioForeground->isChecked())
    {
        _editImageViewer->changeObjectLabeling(1);
        QPixmap pixmap(30,30);
        pixmap.fill(qRgb(colorTable[3], colorTable[4], colorTable[5]));
        _regionCompetitionDialog.showColorLabel->setPixmap(pixmap);
    }
    else
    {
        _editImageViewer->setEraser();
        QPixmap pixmap(0,0);
        _regionCompetitionDialog.showColorLabel->setPixmap(pixmap);
    }
}

void ImageCompletionUI::exitApp()
{
    qApp->exit();
}

void ImageCompletionUI::selectColor()
{
    QColorDialog *colorDlg = new QColorDialog( this );
    QColor color = colorDlg->getColor( _editImageViewer->getLineColor() );
    _editImageViewer->setLineColor(color);

    QPixmap pixmap(30,30);
    pixmap.fill(_editImageViewer->getLineColor());
    _regionCompetitionDialog.labelCurrentColor->setPixmap(pixmap);
}

void ImageCompletionUI::scaling()
{
    _regionCompetitionDialog.sliderBasicOp->setMaximum(20);
    _regionCompetitionDialog.sliderBasicOp->setMinimum(1);
    _regionCompetitionDialog.sliderBasicOp->setValue(10);

    _regionCompetitionDialog.labelSliderLeft->setText(tr("小"));
    _regionCompetitionDialog.labelSliderRight->setText(tr("大"));

    _regionCompetitionDialog._line->setHidden(true);
    _regionCompetitionDialog._labelRulerText->setHidden(true);
}

void ImageCompletionUI::brighting()
{
    _regionCompetitionDialog.sliderBasicOp->setMaximum(100);
    _regionCompetitionDialog.sliderBasicOp->setMinimum(-100);
    _regionCompetitionDialog.sliderBasicOp->setValue(0);

    _regionCompetitionDialog.labelSliderLeft->setText(tr("暗"));
    _regionCompetitionDialog.labelSliderRight->setText(tr("亮"));

    _regionCompetitionDialog._line->setHidden(true);
    _regionCompetitionDialog._labelRulerText->setHidden(true);
}

void ImageCompletionUI::notColor()
{
    _regionCompetitionDialog.labelSliderLeft->setText(tr(""));
    _regionCompetitionDialog.labelSliderRight->setText(tr(""));

    _editImageViewer->imageNotColor( 0.0 );

    _regionCompetitionDialog._line->setHidden(true);
    _regionCompetitionDialog._labelRulerText->setHidden(true);
}

void ImageCompletionUI::measure()
{
    _regionCompetitionDialog.sliderBasicOp->setRange(0, 1000);
    _regionCompetitionDialog.sliderBasicOp->setTickInterval(50);
    _regionCompetitionDialog.sliderBasicOp->setTickPosition(QSlider::TicksRight);

    _regionCompetitionDialog.labelSliderLeft->setText(tr("小"));
    _regionCompetitionDialog.labelSliderRight->setText(tr("大"));

    _regionCompetitionDialog._line->show();
    _regionCompetitionDialog._labelRulerText->show();
    _regionCompetitionDialog._labelRulerText->setScaledContents(true);
    _regionCompetitionDialog._labelRulerText->setText(tr("50微米(50像素)"));

    _regionCompetitionDialog.sliderBasicOp->setValue(50);

    _imageScale = 1.0;
}

void ImageCompletionUI::actionSliderReleased()
{
    double scaleFactor;

    if(_regionCompetitionDialog.radioScale->isChecked())
    {
        scaleFactor = _regionCompetitionDialog.sliderBasicOp->value() * 1.0 / 10;
        _editImageViewer->imageScaling( scaleFactor );
    }
    else if(_regionCompetitionDialog.radioBrightness->isChecked())
    {
        scaleFactor = _regionCompetitionDialog.sliderBasicOp->value();
        _editImageViewer->imageBrighting( scaleFactor );
    }
    else if(_regionCompetitionDialog.radioNotColor->isChecked())
    {
        //	Do nothing (Fix it in the future)

    }
    else if(_regionCompetitionDialog.radioMesuarement->isChecked())
    {
        _regionCompetitionDialog._labelRulerText->setText(QString::number(_regionCompetitionDialog.sliderBasicOp->value()) +"微米(50像素)");
        _imageScale = _regionCompetitionDialog.sliderBasicOp->value() * 1.0 / 50;
    }
}

void ImageCompletionUI::userManagement()
{
    userMangementDlg = new UserManagement(this);
    userMangementDlg->setWindowFlags(userMangementDlg->windowFlags() & ~Qt::WindowMinimizeButtonHint &  ~Qt::WindowMaximizeButtonHint);
    userMangementDlg->show();
}

void ImageCompletionUI::updateLineThickness()
{
    if ( _editImageViewer->image().isNull() ) return;
    int w;
    if(_regionCompetitionDialog.radioLineWidthThick->isChecked())       w = 7;
    else if(_regionCompetitionDialog.radioLineWidthMedium->isChecked()) w = 5;
    else if(_regionCompetitionDialog.radioLineWidthThin->isChecked())   w = 3;

    _editImageViewer->setLineThickness( w );
}

void ImageCompletionUI::methodChangeTriggered(QAction *a)
{
    a->setChecked(true);

    _editImageViewer->setPaintable(true);
    setStrikeOptionsEnabled(false);

    if(a == _rectAction)
    {
        _editImageViewer->setMethod(1);
        _editImageViewer->setLabelType(1);
    }
    else if(a == _polygonAction)
    {
        _editImageViewer->setMethod(1);
        _editImageViewer->setLabelType(2);
    }
    else if(a == _manualAction)
    {
        _editImageViewer->setMethod(2);
        _editImageViewer->setLabelType(2);
    }
}

char* ImageCompletionUI::getNewLogString()
{
    if(_editImageViewer->Method() != -1)
    {
        char* temp = new char[1000];
        sprintf(temp, "标注用时: %.4fs", _editImageViewer->_seg_during);

        return temp;
    }

    return NULL;
}

void ImageCompletionUI::strikeChangeTriggered(QAction *a)
{
    a->setChecked(true);

    _editImageViewer->setPaintable(true);
    _editImageViewer->setMethod(0);
    _editImageViewer->setLabelType(0);
    setStrikeOptionsEnabled(true);

    setupBrush();
    if(a == _fgAction)
    {
        _editImageViewer->changeObjectLabeling(1);
        QPixmap pixmap(30,30);
        pixmap.fill(qRgb(colorTable[3], colorTable[4], colorTable[5]));
        _regionCompetitionDialog.showColorLabel->setPixmap(pixmap);
    }
    else if(a == _bgAction)
    {
        _editImageViewer->changeObjectLabeling(0);
        QPixmap pixmap(30,30);
        pixmap.fill(qRgb(colorTable[0], colorTable[1], colorTable[2]));
        _regionCompetitionDialog.showColorLabel->setPixmap(pixmap);
    }
    else if(a == _eraserAction)
    {
        _editImageViewer->setEraser();
        QPixmap pixmap(0,0);
        _regionCompetitionDialog.showColorLabel->setPixmap(pixmap);
    }
}

void ImageCompletionUI::strikeThicknessChangeTriggered(QAction *a)
{
    if ( _editImageViewer->image().isNull() ) return;

    a->setChecked(true);

    if(a == _strikeThickness[0])      _brushSize = 5;
    else if(a == _strikeThickness[1]) _brushSize = 15;
    else if(a == _strikeThickness[2]) _brushSize = 30;

    _editImageViewer->setBrushWidth( _brushSize );
}

void ImageCompletionUI::lineThicknessChangeTriggered(QAction *a)
{
    a->setChecked(true);

    int w;
    if     (a == _lineThickness[0]) w = 3;
    else if(a == _lineThickness[1]) w = 5;
    else if(a == _lineThickness[2]) w = 7;

    _editImageViewer->setLineThickness( w );
}

void ImageCompletionUI::redo()
{
    _regionCompetitionDialog.buttonGroup->setExclusive(false);
    _regionCompetitionDialog.radioStrikeLabelling->setChecked(false);
    _regionCompetitionDialog.radioRectLabelling->setChecked(false);
    _regionCompetitionDialog.radioPolygonLabelling->setChecked(false);
    _regionCompetitionDialog.radioManualLabelling->setChecked(false);
    _regionCompetitionDialog.buttonGroup->setExclusive(true);

    setStrikeOptionsEnabled(false);

    _regionCompetitionDialog.brushSizeSmallRadio->setChecked(true);

    _editImageViewer->redo();
}

void ImageCompletionUI::undo()
{
    redo();
}

bool ImageCompletionUI::importDB(const QString &path)
{
    /**
     *@brief 读取sql文本内容,并写入至数据库
     *@param path sql文件路径
     */
    QSqlDatabase gAuthDB;
    if(!Global::createConnection(gAuthDB))
        return false;

    QSqlQuery query(gAuthDB);
    QFile file(path);
    file.open(QFile::ReadOnly);
    int count = 0;

    QTextStream in(&file);
    while(!in.atEnd())
    {
        QString sql=in.readLine();
        // 通过分析values(E'),判断是否有二进制数据,如没有直接运行sql语句,如有则需要将16进制文本转换为blob数据
        QRegExp reg("E'([0-9a-f]{1,})'");

        if(!sql.contains(reg))
        {
            if(query.exec(sql))
                count ++;
        }else
        {
            int pos=0;
            QStringList bList;

            // 探索所有的blob字段
            while((pos=reg.indexIn(sql,pos))!=-1)
            {
                bList.append(reg.cap(0));

                QString blob=reg.cap(1);
                pos+=reg.matchedLength();
            }

            // blob字段填充占位符
            foreach(QString key,bList)
            {
                sql.replace(key,"?");
            }

            query.prepare(sql);

            // 绑定占位符数据
            for(int i=0;i<bList.size();i++)
            {
                // 去除E''
                QString hexBlob=bList[i].mid(2,bList[i].size()-1);
                // 还原16进制数据
                QByteArray ba=QByteArray::fromHex(hexBlob.toLocal8Bit());

                query.bindValue(i,ba);
            }
            query.exec();
        }
    }
    return true;
}

bool ImageCompletionUI::exportDB(const QString &path)
{
    /**
     *@brief 导出数据库数据到文件中
     *@param path 文件路径
     */

    QSqlDatabase gAuthDB;
    if(!Global::createConnection(gAuthDB))
        return false;

    QStringList vList;

    // 列出数据库所有名称
    QStringList tables=gAuthDB.tables();
    foreach(QString table,tables)
    {
        QSqlQuery query(gAuthDB);
        QString sql=QString("select * from %1").arg(table);
        query.exec(sql);

        QSqlRecord record=query.record();
        while(query.next())
        {
            QString prefix=QString("insert into %1(").arg(table); // 记录属性字段名
            QString suffix="values(";                             // 记录属性值

            // 遍历属性字段
            for(int i=0;i<record.count();i++)
            {
                QSqlField field=record.field(i);
                QString fieldName=field.name();

                switch(field.type())
                {
                case QVariant::String:
                    prefix+=fieldName;
                    suffix+=QString("'%1'").arg(query.value(i).toString());
                    break;
                case QVariant::ByteArray:
                {
                    prefix+=fieldName;
                    QByteArray data=query.value(i).toByteArray();
                    if(data.isNull())
                    {
                        suffix+="null";
                    }else
                    {
                        suffix+=QString("E'%1'").arg(data.toHex().data()); // blob数据按16进制格式导出
                    }
                }
                    break;
                default:
                    prefix+=fieldName;
                    suffix+=query.value(i).toString();
                }

                if(record.count()==1)
                {
                    prefix+=")";
                    suffix+=")";
                }else if(i!=record.count()-1)
                {
                    prefix+=",";
                    suffix+=",";
                }else if(i==record.count()-1)
                {
                    prefix+=")";
                    suffix+=")";
                }
            }
            // 组装sql语句 insert into auth_test values(0,'hello',E'003f')
            QString iSql=QString("%1 %2;").arg(prefix).arg(suffix);
            vList.append(iSql);
        }
    }

    QFile file(path);
    file.open(QIODevice::WriteOnly|QIODevice::Truncate);

    // 将sql语句写入文件
    QTextStream out(&file);
    foreach(QString line,vList)
    {
        out<<line+"\n";
    }
    gAuthDB.close();
    return true;
}

QColor ImageCompletionUI::color(QString status)
{
    return status == "Y" ? Global::LabelledColor : Global::UnLabelledColor;
}

void ImageCompletionUI::clearBottomWindow()
{
    while(_bottomWindow.dBTableWidget_1->rowCount() > 0) _bottomWindow.dBTableWidget_1->removeRow(0);
    while(_bottomWindow.dBTableWidget_2->rowCount() > 0) _bottomWindow.dBTableWidget_2->removeRow(0);
    while(_bottomWindow.dBTableWidget_3->rowCount() > 0) _bottomWindow.dBTableWidget_3->removeRow(0);
    while(_bottomWindow.dBTableWidget_4->rowCount() > 0) _bottomWindow.dBTableWidget_4->removeRow(0);
    while(_bottomWindow.dBTableWidget_5->rowCount() > 0) _bottomWindow.dBTableWidget_5->removeRow(0);
    while(_bottomWindow.dBTableWidget_6->rowCount() > 0) _bottomWindow.dBTableWidget_6->removeRow(0);
    while(_bottomWindow.dBTableWidget_7->rowCount() > 0) _bottomWindow.dBTableWidget_7->removeRow(0);
    while(_bottomWindow.dBTableWidget_8->rowCount() > 0) _bottomWindow.dBTableWidget_8->removeRow(0);
    while(_bottomWindow.dBTableWidget_9->rowCount() > 0) _bottomWindow.dBTableWidget_9->removeRow(0);

    _bottomWindow.dBTableWidget_1->setRowCount(0);
    _bottomWindow.dBTableWidget_2->setRowCount(0);
    _bottomWindow.dBTableWidget_3->setRowCount(0);
    _bottomWindow.dBTableWidget_4->setRowCount(0);
    _bottomWindow.dBTableWidget_5->setRowCount(0);
    _bottomWindow.dBTableWidget_6->setRowCount(0);
    _bottomWindow.dBTableWidget_7->setRowCount(0);
    _bottomWindow.dBTableWidget_8->setRowCount(0);
    _bottomWindow.dBTableWidget_9->setRowCount(0);
}

QImage ImageCompletionUI::loadLabelledResult(QString file)
{
    QList<QByteArray> list;
    QPixmap image;
    QSqlTableModel *model = new QSqlTableModel;

    model->setTable("abrasivemarkinfo");
    model->setFilter(QString("abrasivepicpath = '%1'").arg(file));
    if(model->select())
    {
        for(int i = 0; i < model->rowCount(); i++)
        {
            QSqlRecord record = model->record(i);

            QByteArray arr = record.value("abrasiveResultData").toByteArray();
            list.push_back(arr);

            _editImageViewer->drawAllMoli(list);
        }
    }

    model->deleteLater();

    return image.toImage();
}

void ImageCompletionUI::flushBottom()
{
    showData();
}

void ImageCompletionUI::flushLeft(QString strFilePath, QString label)
{
    int k = 0;
    deque_it it;

    for(it = _dequeTodo.begin(); it != _dequeTodo.end(); it++, k++)
    {
        if(*it == strFilePath)
        {
            _dequeTodo.erase(it);
            _leftWindow.tableWidget->removeRow(k);
            break;
        }
    }

    if(-1 == in(strFilePath, _dequeDone))
    {
        this->enDeque(strFilePath, _dequeDone);
        this->showThumbnailForLabelled(strFilePath);
    }
}

bool ImageCompletionUI::copyFiles(QString fromDir, QString toDir, bool convertIfExits)
{
    /**
     *@brief 将fromDir文件夹内的图片文件，拷贝到toDir文件夹下
     *@param fromDir 图片文件的源目录
     *@param toDir   拷贝图片文件的目标目录
     *@param convertIfExits 是否覆盖已存在文件标识，默认值是false
     */
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);

    if(!targetDir.exists())
    {
        //< 如果目标目录不存在，则进行创建
        if(!targetDir.mkdir(targetDir.absolutePath()))
            return false;
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    // 遍历所有文件信息
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        // 去除当前目录和父目录
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;
        // 数据库文件处理
        if(fileInfo.fileName().split(".")[1] == "sql")
            qDebug()<<fileInfo.fileName();

        // 当为目录时，递归的进行copy
        if(fileInfo.isDir())
        {
            if(!copyFiles(fileInfo.filePath(),
                          targetDir.filePath(fileInfo.fileName()),
                          convertIfExits))
                return false;
        }
        else
        {   //当允许覆盖操作时，将旧文件进行删除操作
            if(convertIfExits && targetDir.exists(fileInfo.fileName()))
            {
                targetDir.remove(fileInfo.fileName());
            }
            // 进行文件copy
            if(!QFile::copy(fileInfo.filePath(),
                            targetDir.filePath(fileInfo.fileName()))){
                return false;
            }
        }
    }
    return true;
}

void ImageCompletionUI::importData()
{
    QFileDialog *packgeDir = new QFileDialog(this,tr("选择打包文件"),"","");
    packgeDir->setFileMode(QFileDialog::DirectoryOnly);
    packgeDir->setViewMode(QFileDialog::Detail);

    QString packgePath;
    if(packgeDir->exec())
    {
        QStringList packgePaths = packgeDir->selectedFiles();
        packgePath = packgePaths.at(0);
    }
    else
        return;
    QDir dir(packgePath);
    QFileInfoList infoList = dir.entryInfoList();
    QStringList fileNameList;
    foreach(QFileInfo fileInfo,infoList)
    {
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;
        fileNameList.append(fileInfo.fileName());
    }
    if(!fileNameList.contains("resultFile") || !fileNameList.contains("sourceFile") || !fileNameList.contains("databackup.sql"))
        QMessageBox::warning(this,tr("提示"),tr("打包文件受损"),QMessageBox::Close);
    else
    {
        QFileDialog *targetDir = new QFileDialog(this,tr("选择打包恢复目录"),"","");
        targetDir->setFileMode(QFileDialog::DirectoryOnly);
        targetDir->setViewMode(QFileDialog::Detail);
        QString targetPath;
        if(targetDir->exec())
        {
            QStringList targetPaths = targetDir->selectedFiles();
            targetPath  = targetPaths.at(0);
        }
        else
            return;
#ifdef Q_OS_WIN
        QString sourcepackgePath = packgePath + "\\sourceFile";
        QString resultpackgePath = packgePath + "\\resultFile";
        QString databackupFileName = packgePath + "\\databackup.sql";

        QString sourcetargetPath = targetPath + "\\sourceFile";
        QString resulttargetPath = targetPath + "\\resultFile";
#endif

#ifdef Q_OS_LINUX
        QString sourcepackgePath = packgePath + "/sourceFile";
        QString resultpackgePath = packgePath + "/resultFile";
        QString databackupFileName = packgePath + "/databackup.sql";

        QString sourcetargetPath = targetPath + "/sourceFile";
        QString resulttargetPath = targetPath + "/resultFile";
#endif
        if(this->copyFiles(sourcepackgePath,sourcetargetPath) &&
                this->copyFiles(resultpackgePath,resulttargetPath) &&
                this->importDB(databackupFileName))
            QMessageBox::warning(this,tr("提示"),tr("数据恢复成功"),QMessageBox::Close);
        else
            QMessageBox::warning(this,tr("提示"),tr("数据恢复失败"),QMessageBox::Close);
    }
}

void ImageCompletionUI::exportData()
{
//    QFileDialog *sourceDir = new QFileDialog(this,tr("选择铁谱图片目录"),"","");
//    sourceDir->setFileMode(QFileDialog::DirectoryOnly);
//    sourceDir->setViewMode(QFileDialog::Detail);
//    QString sourcePath;
//    if(sourceDir->exec())
//    {
//        QStringList sourcePaths = sourceDir->selectedFiles();
//        sourcePath = sourcePaths.at(0);
//        //qDebug()<<sourcePath;
//    }
//    else
//        return;

//    QFileDialog *resultDir = new QFileDialog(this,tr("选择磨粒标注结果目录"),"","");
//    resultDir->setFileMode(QFileDialog::DirectoryOnly);
//    resultDir->setViewMode(QFileDialog::Detail);
//    QString resultPath;
//    if(resultDir->exec())
//    {
//        QStringList resultPaths = resultDir->selectedFiles();
//        resultPath = resultPaths.at(0);
//        //qDebug()<<resultPath;
//    }
//    else
//        return;

//    QFileDialog *targetDir = new QFileDialog(this,tr("选择打包存档目录"),"","");
//    targetDir->setFileMode(QFileDialog::DirectoryOnly);
//    targetDir->setViewMode(QFileDialog::Detail);
//    QString targetPath;
//    if(targetDir->exec())
//    {
//        QStringList targetPaths = targetDir->selectedFiles();
//        targetPath  = targetPaths.at(0);
//        //qDebug()<<targetPath;
//    }
//    else
//        return;

//    if(resultPath.isEmpty() | resultPath.isEmpty()| targetPath.isEmpty())
//        return;
    ExpDlg *expDlg = new ExpDlg(this,"","");
    if(expDlg->exec() == QDialog::Accepted)
    {
        QString sourcePath = this->_expSourcePicPath;
        QString resultPath = this->_expResultPicPath;
        QString targetPath = this->_expPackgePath;
    #ifdef Q_OS_WIN
        QString sourcetargetPath = targetPath + "\\sourceFile";
        QString resulttargetPath = targetPath + "\\resultFile";
        QString databackupFileName = targetPath + "\\databackup.sql";
    #endif
    
    #ifdef Q_OS_LINUX
        QString sourcetargetPath = targetPath + "/sourceFile";
        QString resulttargetPath = targetPath + "/resultFile";
        QString databackupFileName = targetPath + "/databackup.sql";
    #endif
    
        // 导出数据库信息
        /* edit code */
    
        if(this->copyFiles(sourcePath,sourcetargetPath)
                && this->copyFiles(resultPath,resulttargetPath)
                && this->exportDB(databackupFileName))
            QMessageBox::warning(this,tr("批量数据导出提示"),tr("批量数据导出成功"),QMessageBox::Close);
        else
            QMessageBox::warning(this,tr("批量数据导出提示"),tr("批量数据导出失败"),QMessageBox::Close);
    }
}

void ImageCompletionUI::setExpPath(QString sourcePicPath, QString resultPicPath, QString packgePath)
{
    this->_expSourcePicPath = sourcePicPath;
    this->_expResultPicPath = resultPicPath;
    this->_expPackgePath = packgePath;
}

void ImageCompletionUI::cellDoubleClickedLeftWindow(int row, int /* col */)
{
    QString strFilePath = QString();

    if(_leftWindow.tableWidget->item(row, 0))
    {
        if(row < _dequeTodo.size())
        {
            strFilePath = _dequeTodo[row];
        }
        else if(row < _dequeDone.size() + _dequeTodo.size())
        {
            strFilePath = _dequeDone[row - _dequeTodo.size()];
        }

        if(!strFilePath.isEmpty())
        {
            openImage(strFilePath);
        }
    }
}

QString ImageCompletionUI::status(QString imagePath)
{
    if(imagePath.isEmpty()) return QString();

    QSqlDatabase db;
    if( Global::createConnection(db) )
    {
        QSqlQuery query;
        bool ret = query.exec(QString("select * from ferrographypicinfo"));
        if(ret)
        {
            int index = query.record().indexOf("ferrographypicpath");
            while(query.next())
            {
                if(imagePath == query.value(index).toString())
                {
                    index = query.record().indexOf("imagesymbol");
                    return query.value(index).toString();
                }
            }
        }
    }
    return QString();
}

void ImageCompletionUI::on_dBTableWidget_8_cellDoubleClicked(int row, int column)
{   
    QString strFilePath;
    QTableWidgetItem* item = _bottomWindow.dBTableWidget_8->item(row, 5);
    if(item)
    {
        strFilePath = item->text();
    }

    if(!strFilePath.isEmpty() && QFile::exists(strFilePath) )
    {
        if(_centralStackedWidget->currentIndex() != 0)
        {
            _centralStackedWidget->setCurrentIndex(0);
        }
        // Open Current Moli Image
        this->openImage(strFilePath);
        this->loadMoliImage(_bottomWindow.dBTableWidget_8->item(row, 0)->text()); // Cover the source image
    }
}

void ImageCompletionUI::loadMoliImage(QString moliId)
{
    QSqlDatabase db;
    if(Global::createConnection(db))
    {
        QString sql = QString("select abrasiveResultData from abrasivemarkinfo where abrasiveid = '%1'").arg(moliId);
        QSqlQuery query;
        query.prepare(sql);

        if(query.exec() && query.record().count() == 1)
        {
            if(query.next())
            {
                QByteArray arr = query.value(0).toByteArray();
                QPixmap pixmap;
                pixmap.loadFromData(arr);
                QImage image = pixmap.toImage();

                _editImageViewer->setImage(image);
            }
        }
    }
    db.close();
}

void ImageCompletionUI::on_dBTableWidget_7_cellDoubleClicked(int row, int column)
{
    QString strFilePath;
    QTableWidgetItem* item = _bottomWindow.dBTableWidget_7->item(row, 9);
    if(item)
    {
        strFilePath = item->text();
    }

    if(!strFilePath.isEmpty() && QFile::exists(strFilePath) )
    {
        if(_centralStackedWidget->currentIndex() != 0)
        {
            _centralStackedWidget->setCurrentIndex(0);
        }
        this->openImage(strFilePath);
    }
}

void ImageCompletionUI::on_dBTableWidget_6_cellDoubleClicked(int row, int column)
{
    QStringList list;
    QTableWidgetItem* item = _bottomWindow.dBTableWidget_6->item(row, 0);
    if(item)
    {
        QString id = item->text();

        QSqlDatabase db;
        if(Global::createConnection(db))
        {
            QSqlQuery query;
            QString str = QString("select ferrographypicpath from ferrographypicinfo where ferrographysheetid='%1'").arg(id);
            query.prepare(str);

            if(query.exec())
            {
                while(query.next())
                {
                    list << query.value(0).toString();
                }
            }
        }
        db.close();
    }

    showThumbnailsInCentral(list);
}

void ImageCompletionUI::on_dBTableWidget_5_cellDoubleClicked(int row, int column)
{
    QStringList list;
    QTableWidgetItem* item = _bottomWindow.dBTableWidget_5->item(row, 0);
    if(item)
    {
        QString id = item->text();

        QSqlDatabase db;
        if(Global::createConnection(db))
        {
            QSqlQuery query;
            QString str = QString("select ferrographypicinfo.ferrographypicpath from ferrographypicinfo join ferrographyinfo on ferrographyinfo.oilsampleid='%1' and ferrographypicinfo.ferrographysheetid = ferrographyinfo.ferrographysheetid").arg(id);
            query.prepare(str);

            if(query.exec())
            {
                while(query.next())
                {
                    list << query.value(0).toString();
                }
            }
        }
        db.close();
    }
    showThumbnailsInCentral(list);
}

void ImageCompletionUI::on_dBTableWidget_4_cellDoubleClicked(int row, int column)
{
    QStringList list;
    QTableWidgetItem* item = _bottomWindow.dBTableWidget_4->item(row, 0);
    if(item)
    {
        QString id = item->text();

        QSqlDatabase db;
        if(Global::createConnection(db))
        {
            QSqlQuery query;
            QString str = QString("select ferrographypicinfo.ferrographypicpath from ferrographypicinfo join ferrographyinfo on ferrographyinfo.oilsampleid='%1' and ferrographypicinfo.ferrographysheetid = ferrographyinfo.ferrographysheetid").arg(id);
            query.prepare(str);

            if(query.exec())
            {
                while(query.next())
                {
                    list << query.value(0).toString();
                }
            }
        }
        db.close();
    }
    showThumbnailsInCentral(list);
}

void ImageCompletionUI::on_dBTableWidget_3_cellDoubleClicked(int row, int column)
{
    QStringList list;
    QTableWidgetItem* item = _bottomWindow.dBTableWidget_3->item(row, 1);
    if(item)
    {
        QString id = item->text();

        QSqlDatabase db;
        if(Global::createConnection(db))
        {
            QSqlQuery query;
            QString str = QString("select ferrographypicinfo.ferrographypicpath from ferrographypicinfo join oilsampleinfo join movepartinfo join ferrographyinfo on ferrographyinfo.oilsampleid=oilsampleinfo.oilsampleid and oilsampleinfo.planeid = movepartinfo.planeid and ferrographypicinfo.ferrographysheetid = ferrographyinfo.ferrographysheetid and movepartinfo.movepartid = '%1'").arg(id);
            query.prepare(str);
            if(query.exec())
            {
                while(query.next())
                {
                    list << query.value(0).toString();
                }
            }
        }
        db.close();
    }
    showThumbnailsInCentral(list);
}

void ImageCompletionUI::on_dBTableWidget_2_cellDoubleClicked(int row, int column)
{
    QStringList list;
    QTableWidgetItem* item = _bottomWindow.dBTableWidget_2->item(row, 5);
    if( item )
    {
        QString id = item->text();

        QSqlDatabase db;
        if(Global::createConnection(db))
        {
            QSqlQuery query;
            QString str = QString("select ferrographypicinfo.ferrographypicpath from ferrographypicinfo join oilsampleinfo join ferrographyinfo on ferrographyinfo.oilsampleid = oilsampleinfo.oilsampleid and ferrographypicinfo.ferrographysheetid = ferrographyinfo.ferrographysheetid and oilsampleinfo.planeid = '%1' ").arg(id);
            query.prepare(str);

            if(query.exec())
            {
                while(query.next())
                {
                    list << query.value(0).toString();
                }
            }
        }
        db.close();
    }
    showThumbnailsInCentral(list);
}

void ImageCompletionUI::on_dBTableWidget_1_cellDoubleClicked(int row, int column)
{
    QStringList list;
    QTableWidgetItem* item = _bottomWindow.dBTableWidget_1->item(row, 0);
    if( item )
    {
        QString id = item->text();

        QSqlDatabase db;
        if(Global::createConnection(db))
        {
            QSqlQuery query;
            QString str = QString("select ferrographypicinfo.ferrographypicpath from ferrographypicinfo join oilsampleinfo join ferrographyinfo on ferrographyinfo.oilsampleid = oilsampleinfo.oilsampleid and ferrographypicinfo.ferrographysheetid = ferrographyinfo.ferrographysheetid and oilsampleinfo.planeid = '%1'").arg(id);

            query.prepare(str);

            if(query.exec())
            {
                while(query.next())
                {
                    list << query.value(0).toString();
                }
            }
        }
        db.close();
    }
    showThumbnailsInCentral(list);
}

void ImageCompletionUI::back()
{
    if(0 == _centralStackedWidget->currentIndex())
    {
        _centralStackedWidget->setCurrentIndex(1);
    }
}

void ImageCompletionUI::append()
{
    bool ok = _editImageViewer->openImage(_strCurrentImagePath);
    if(!ok)
    {
        QMessageBox::warning(this,
                             tr("提示"),
                             tr("无法打开图像"),
                             QMessageBox::Close);
        return;
    }
}

void ImageCompletionUI::showAll()
{   
    QList<QByteArray> list;

    QSqlDatabase db;
    if(Global::createConnection(db))
    {
        QString sql = QString("select abrasiveResultData from abrasivemarkinfo where abrasivepicpath = '%1'").arg(_strCurrentImagePath);

        QSqlQuery query;
        query.prepare(sql);

        if(query.exec())
        {
            while(query.next())
            {
                list.push_back(query.value(0).toByteArray());
            }
            _editImageViewer->drawAllMoli(list);
        }
    }
    db.close();
}

void ImageCompletionUI::showThumbnailsInCentral(QStringList list)
{
    if(list.empty()) return;

    close();

    clearLayout(_formLayout);

    int nRow = list.size() / THUMBNAILS_PER_ROW + 1;

    for(int i = 0; i < nRow; i++)
    {
        QHBoxLayout* lll = new QHBoxLayout;
        for(int j = 0; j < THUMBNAILS_PER_ROW; j++)
        {
            int index = i * THUMBNAILS_PER_ROW + j;
            if( index < list.size())
            {
                QLabel* label = new QLabel(list[index]);
                label->installEventFilter(this);
                label->setObjectName(list[index]);
                label->setFixedSize(150, 75);
                label->setScaledContents(true);
                QPixmap image;
                if(image.load(list[index]))
                {
                    drawEnclosingRectangle(image, color(status(list[index])));
                    label->setPixmap(image);
                }
                lll->addWidget(label);
            }
            else
            {
                break;
            }
        }
        _formLayout->addRow(lll);
    }

    _thumbnailScrollArea->setLayout(_formLayout);
    if(_centralStackedWidget->currentIndex() == 0)
    {
        _canBack = false;
        _centralStackedWidget->setCurrentIndex(1);
    }
}


void ImageCompletionUI::queryThumbnails(QStringList list)
{
    this->showThumbnailsInCentral(list);
}

bool ImageCompletionUI::eventFilter(QObject *target, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonDblClick)
    {
        QString strFilePath = target->objectName();
        if( QFile::exists(strFilePath) )
        {
            _centralStackedWidget->setCurrentIndex(0);
            openImage(strFilePath);
            _canBack = true;
        }
    }
}

void ImageCompletionUI::clearLayout(QLayout *layout)
{
    if(layout)
    {
        while(QLayoutItem* item = layout->takeAt(0))
        {
            if(QWidget* widget = item->widget())
            {
                delete widget;
            }

            if(QLayout* childLayout = item->layout())
            {
                clearLayout(childLayout);
            }
            delete item;
        }
    }
}

void ImageCompletionUI::drawEnclosingRectangle(QPixmap& pixmap, const QColor color)
{
    QPainter painter(&pixmap);
    painter.setPen(QPen(color, 20, Qt::SolidLine));
    painter.drawRect(0, 0, pixmap.width(), pixmap.height());
}

