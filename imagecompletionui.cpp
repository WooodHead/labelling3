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
    _strNextImage        = "";
    _canBack             = false;
    _brushSize           = 3;
    _imageScale          = 1.0;
    _editImageViewer     = 0;
    _formLayout          = 0;
    _strCurrentImagePath = QString();
    _strMoliId           = QString();
    _thumbnailWidget = 0;

    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    // Initialize UI
    createActions();
    setupMainWindow();
    createMenus();
    createToolBars();
    setupWidgets();
    createStatusBar();
    createConnections();
    updateBrushSize();
    setStrikeOptionsEnabled(false);

    // Initialize Data
    showData();
    showImagesInTree();
    loadAllImagesAndShowInLeftWindow();
    changeMeasureButtonState(true);

    _regionCompetitionDialog._spinBox_measure->setRange(1, 10000);
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
    _menuWindow->addAction(_aboutAction);
}

void ImageCompletionUI::createActions()
{
    _openAction = new QAction( Global::Awesome->icon(folderopeno), tr("&打开"), this );
    _openAction->setObjectName(tr("_openAction"));
    connect(_openAction, SIGNAL(triggered()), this, SLOT(open()));

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

    _aboutAction = new QAction(tr("关于"), this);
    connect(_aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    _docAction = new QAction(tr("帮助文档"), this);
    connect(_docAction, SIGNAL(triggered()), this, SLOT(showDoc()));
}

void	ImageCompletionUI::createToolBars()
{
    _editToolBar = addToolBar( tr("文件") );
    _editToolBar->addAction( _openAction );
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
    _centralTabWidget->setMaximumHeight(height);
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

    _thumbnailWidget = new QWidget;
    _formLayout = new QFormLayout(_thumbnailWidget);
    _thumbnailScrollArea->setWidget(_thumbnailWidget);

    //    _thumbnailScrollArea->setLayout(_formLayout);

    _thumbnailScrollArea->setWidgetResizable(true);
    _thumbnailScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    _thumbnailScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QGridLayout *tabLayout2 = new QGridLayout;
    tabLayout2->addWidget(_thumbnailScrollArea);
    _thumbnailTab->setLayout(tabLayout2);

    _thumbnailScrollArea->setAlignment( Qt::AlignCenter );
    _thumbnailScrollArea->setAutoFillBackground( true );
    _thumbnailScrollArea->setBackgroundRole(  QPalette::Dark );

    _centralThumbnailTabWidget->addTab( _thumbnailTab, Global::Awesome->icon(pictureo), QString("缩略图") );
    _centralThumbnailTabWidget->setMaximumHeight(height);
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
    _rightOperationWidget->setMaximumSize(QSize(width, height));
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

    _sceneCompletionPage = new QWidget();
    _sceneCompletionPage->setObjectName(tr("_sceneCompletionPage"));

    _regionCompetitionPage = new QWidget();
    _regionCompetitionPage->setObjectName(tr("_regionCompetitionPage"));
    QScrollArea *rightscroArea = new QScrollArea(_operationStackedWidget);
    rightscroArea->setWidget(_regionCompetitionPage);
    _operationStackedWidget->addWidget( rightscroArea );

    ////////////////////////////////////////////////////////////////////////////////////
    //		_sceneCompletionPage
    ////////////////////////////////////////////////////////////////////////////////////
    _regionCompetitionDialog.setupUi(_regionCompetitionPage);
    _regionCompetitionDialog.buttonSelectColor->setIcon(Global::Awesome->icon(eyedropper));
    _regionCompetitionDialog.radioBrightness->setIcon(Global::Awesome->icon(adjust));
    _regionCompetitionDialog.radioNotColor->setIcon(Global::Awesome->icon(circle));

    _rightOperationWidget->setWidget(_dockWidgetContents);
    _rightOperationWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    this->addDockWidget(static_cast<Qt::DockWidgetArea>(2), _rightOperationWidget);

    _operationStackedWidget->setCurrentIndex(0);

    ////////////////////////////////////////////////////////////////////////////////////
    //   _leftWindowWidget
    ////////////////////////////////////////////////////////////////////////////////////
    _leftWindowWidget = new QDockWidget(tr("图谱信息"),this );
    _leftWindowWidget->setObjectName(tr("_leftWindowWidget"));
    _leftWindowWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    _leftWindowWidget->setMaximumSize(QSize(width, height));
    _leftWindowWidget->setMaximumHeight(height);
    _leftWindowWidget->setFloating(false);
    //
    _leftDockWindowContents = new QWidget( );
    _leftDockWindowContents->setObjectName(tr("_leftDockWindowContents"));

    _leftWindow.setupUi(_leftDockWindowContents);
    _leftWindow._listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _leftWindow._listWidget->setUniformItemSizes(true);
    _leftWindow._listWidget->setViewMode(QListView::IconMode);
    _leftWindow._listWidget->setResizeMode(QListView::Adjust);
    _leftWindow._listWidget->setFlow(QListView::TopToBottom);
    _leftWindow._listWidget->setTextElideMode(Qt::ElideRight);
    _leftWindow._listWidget->setWrapping(false);
    _leftWindow._listWidget->setSpacing(12);

    _leftWindow.tabWidgetLeftWindow->setCurrentIndex(0);
    _leftWindowWidget->setWidget(_leftDockWindowContents);

    addDockWidget(Qt::LeftDockWidgetArea, _leftWindowWidget);

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

    _logWindowWidget = new QDockWidget(tr("工作日志"),this);
    _logWindowWidget->setObjectName("_logWindowWidget");
    _logWindowWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    _logWindowWidget->setWidget(_logWidget);
    _logWindowWidget->setMinimumHeight(0.1 * height);
    _logWindowWidget->setMaximumHeight(0.3 * height);
    _logWindowWidget->setWindowIcon(Global::Awesome->icon(filetexto));
    _logWindowWidget->setVisible(false);
    addDockWidget(Qt::LeftDockWidgetArea, _logWindowWidget);

    ////////////////////////////////////////////////////////////////////////////////////
    //   _bottomWindowWidget
    ////////////////////////////////////////////////////////////////////////////////////
    _bottomWindowWidget = new QDockWidget(tr("数据库信息"),this );
    _bottomWindowWidget->setObjectName(tr("_bottomWindowWidget"));
    _bottomWindowWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
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

    _bottomWindow.dBTableWidget_1->setContextMenuPolicy(Qt::CustomContextMenu);
    _bottomWindow.dBTableWidget_2->setContextMenuPolicy(Qt::CustomContextMenu);
    _bottomWindow.dBTableWidget_3->setContextMenuPolicy(Qt::CustomContextMenu);
    _bottomWindow.dBTableWidget_4->setContextMenuPolicy(Qt::CustomContextMenu);
    _bottomWindow.dBTableWidget_5->setContextMenuPolicy(Qt::CustomContextMenu);
    _bottomWindow.dBTableWidget_6->setContextMenuPolicy(Qt::CustomContextMenu);
    _bottomWindow.dBTableWidget_7->setContextMenuPolicy(Qt::CustomContextMenu);
    _bottomWindow.dBTableWidget_8->setContextMenuPolicy(Qt::CustomContextMenu);
    _bottomWindow.dBTableWidget_9->setContextMenuPolicy(Qt::CustomContextMenu);
    _bottomWindowWidget->setWidget(_bottomDockWindowContents);
    addDockWidget(Qt::BottomDockWidgetArea, _bottomWindowWidget);

    setCorner(Qt::BottomLeftCorner,Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner,Qt::RightDockWidgetArea);

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
    connect(_regionCompetitionDialog.radioBrightness, SIGNAL(clicked()), this, SLOT(brighting()));
    connect(_regionCompetitionDialog.radioNotColor, SIGNAL(clicked()), this, SLOT(notColor()));

    connect(_regionCompetitionDialog.sliderBasicOp, SIGNAL(sliderReleased()), this, SLOT(actionSliderReleased()));

    connect(_regionCompetitionDialog.radioLineWidthThin, SIGNAL(clicked()), this, SLOT(updateLineThickness()));
    connect(_regionCompetitionDialog.radioLineWidthMedium, SIGNAL(clicked()), this, SLOT(updateLineThickness()));
    connect(_regionCompetitionDialog.radioLineWidthThick, SIGNAL(clicked()), this, SLOT(updateLineThickness()));
    connect(_regionCompetitionDialog._comboBoxLarger, SIGNAL(currentIndexChanged(QString)), this, SLOT(on__comboBoxLarger_currentIndexChanged(QString)));
    connect(_regionCompetitionDialog._horizontalSliderScale, SIGNAL(valueChanged(int)), this, SLOT(on__horizontalSliderScale_valueChanged(int)));

    connect(_leftWindow._listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(listItemDoubleClicked(QListWidgetItem*)));
    connect(_bottomWindow.dBTableWidget_8, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(on_dBTableWidget_8_cellDoubleClicked(int, int)));
    connect(_bottomWindow.dBTableWidget_7, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(on_dBTableWidget_7_cellDoubleClicked(int, int)));
    connect(_bottomWindow.dBTableWidget_6, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(on_dBTableWidget_6_cellDoubleClicked(int, int)));
    connect(_bottomWindow.dBTableWidget_5, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(on_dBTableWidget_5_cellDoubleClicked(int, int)));
    connect(_bottomWindow.dBTableWidget_4, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(on_dBTableWidget_4_cellDoubleClicked(int, int)));
    connect(_bottomWindow.dBTableWidget_3, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(on_dBTableWidget_3_cellDoubleClicked(int, int)));
    connect(_bottomWindow.dBTableWidget_2, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(on_dBTableWidget_2_cellDoubleClicked(int, int)));
    connect(_bottomWindow.dBTableWidget_1, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(on_dBTableWidget_1_cellDoubleClicked(int, int)));

    if(Global::Authority == "1")
    {
        connect(_bottomWindow.dBTableWidget_9, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(bottomWindowContextMenuEvent(const QPoint &)));
        connect(_bottomWindow.dBTableWidget_8, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(bottomWindowContextMenuEvent(const QPoint &)));
        connect(_bottomWindow.dBTableWidget_7, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(bottomWindowContextMenuEvent(const QPoint &)));
        connect(_bottomWindow.dBTableWidget_6, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(bottomWindowContextMenuEvent(const QPoint &)));
        connect(_bottomWindow.dBTableWidget_5, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(bottomWindowContextMenuEvent(const QPoint &)));
        connect(_bottomWindow.dBTableWidget_4, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(bottomWindowContextMenuEvent(const QPoint &)));
        connect(_bottomWindow.dBTableWidget_3, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(bottomWindowContextMenuEvent(const QPoint &)));
        connect(_bottomWindow.dBTableWidget_2, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(bottomWindowContextMenuEvent(const QPoint &)));
        connect(_bottomWindow.dBTableWidget_1, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(bottomWindowContextMenuEvent(const QPoint &)));
    }
    connect(_leftWindow._treeViewImages, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(OnDoubleClickTreeView(QModelIndex)));
    connect(_regionCompetitionDialog._spinBox_measure, SIGNAL(valueChanged(int)), this, SLOT(on__spinBox_measure_valueChanged(int)));
}

void ImageCompletionUI::showContextMenu(QPoint pos)
{
    // Menu 1
    QAction* editMenu = new QAction(tr("编辑磨粒属性"), this);
    connect(editMenu, SIGNAL(triggered()), this, SLOT(editProperties()));

    QAction* checkMenu = new QAction(tr("查看磨粒属性"), this);
    connect(checkMenu, SIGNAL(triggered()), this, SLOT(checkProperties()));

    // Menu 2
    QAction *appendMenu = new QAction(tr("标注新磨粒"), this);
    connect(appendMenu, SIGNAL(triggered()), this, SLOT(append()));

    // Menu 4
    QAction* showAllMenu = new QAction(tr("显示所有磨粒"), this);
    connect(showAllMenu, SIGNAL(triggered()), this, SLOT(showAll()));

    QSqlDatabase db;
    if(!_strCurrentImagePath.isEmpty() && Global::createConnection(db))
    {
        QString sql = QString("select count(*) from abrasivemarkinfo where abrasivepicpath = '%1'").arg(_strCurrentImagePath);
        QSqlQuery query;
        query.prepare(sql);
        if(query.exec() && query.next())
        {
            if(query.value(0).toInt() > 0)
            {
                checkMenu->setEnabled(true);
            }
            else
            {
                checkMenu->setEnabled(false);
            }
        }
        else
        {
            checkMenu->setEnabled(false);
        }
    }

    if(_strCurrentImagePath.isEmpty())
    {
        editMenu->setEnabled(false);
        appendMenu->setEnabled(false);
        showAllMenu->setEnabled(false);
        checkMenu->setEnabled(false);
    }

    //    if(!_editImageViewer->getMask() || !_editImageViewer->getResultSave())
    //    {
    //        editMenu->setEnabled(false);
    //        checkMenu->setEnabled(false);
    //    }

    // Menu 3
    QAction* backMenu = new QAction(tr("回退"), this);
    connect(backMenu, SIGNAL(triggered()), this, SLOT(back()));
    if(_canBack == false)
    {
        backMenu->setEnabled(false);
    }

    QAction* nextMenu = new QAction(tr("下一幅图像"), this);
    connect(nextMenu, SIGNAL(triggered()), this, SLOT(nextImage()));
    if(!_editImageViewer->getOriginalImage())
    {
        nextMenu->setEnabled(false);
    }

    QMenu menu;
    menu.addAction(editMenu);
    menu.addAction(checkMenu);
    menu.addSeparator();
    menu.addAction(appendMenu);
    menu.addAction(showAllMenu);
    menu.addSeparator();
    menu.addAction(backMenu);
    menu.addSeparator();
    menu.addAction(nextMenu);
    menu.exec(_centralTabWidget->mapToGlobal(pos));
}

void ImageCompletionUI::editProperties()
{
    if(_strCurrentImagePath.isEmpty()) return;

    QImage* result  = _editImageViewer->getResultDisplay();
    QImage* mask    = _editImageViewer->getMask();
    QImage* result2 = _editImageViewer->getResultSave();

    _imageScale = _regionCompetitionDialog._spinBox_measure->value() * 1.0 / _regionCompetitionDialog._spinBox_pixel->value();
    _imageScale *= _editImageViewer->orgWidth() * _editImageViewer->orgHeight() * 1.0 / _editImageViewer->width() / _editImageViewer->height();
    (new MoliProperties(this))->showDlg(_strCurrentImagePath, result, result2, mask, _imageScale );
}

void ImageCompletionUI::checkProperties()
{
    if(_strCurrentImagePath.isEmpty()) return;

    QImage* result  = _editImageViewer->getResultDisplay();
    QImage* mask    = _editImageViewer->getMask();
    QImage* result2 = _editImageViewer->getResultSave();

    _imageScale = _regionCompetitionDialog._spinBox_measure->value() * 1.0 / _regionCompetitionDialog._spinBox_pixel->value();
    _imageScale *= _editImageViewer->orgWidth() * _editImageViewer->orgHeight() * 1.0 / _editImageViewer->width() / _editImageViewer->height();
    (new CheckProperties(this))->showDlg(_strCurrentImagePath, result, result2, mask, _imageScale );
}

void ImageCompletionUI::syncFilePathStr(QString strFilePath)
{
    if(strFilePath.isEmpty() || !QFile::exists(strFilePath)) return;

    int pos = in(strFilePath, _dequeTodo);
    if( -1 == pos )
    {
        enDeque(strFilePath, _dequeTodo);
        pos = in(strFilePath, _dequeTodo);
        showThumbnail(strFilePath, MOLI_UNLABELLED_STATUS_CHAR, pos);

        if(pos != -1 && pos < _dequeTodo.size()-1)
        {
            _strNextImage = _dequeTodo[pos+1];
        }
        else if(pos == _dequeTodo.size()-1 && !_dequeDone.empty())
        {
            _strNextImage = _dequeDone[0];
        }
        else
        {
            _strNextImage = "";
        }
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
    Global::NewName = QString();
    _centralStackedWidget->setCurrentIndex(0);

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
    else
    {
        _regionCompetitionDialog._lineEdit_width->setText(QString::number(_editImageViewer->width()));
        _regionCompetitionDialog._lineEdit_height->setText(QString::number(_editImageViewer->height()));
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

        int index = in(strFilePath, _dequeDone);
        if(index != -1 && index < _dequeDone.size() - 1)
        {
            _strNextImage = _dequeDone[index+1];
        }
        else
        {
            _strNextImage = "";
        }

        setEnlargementFactor(strFilePath);
        this->loadLabelledResult( strFilePath );
    }
    else if(MOLI_UNLABELLED_STATUS_CHAR == status)
    {
        if(-1 == in(strFilePath, _dequeTodo))
        {
            this->enDeque(strFilePath, _dequeTodo);
            this->showThumbnailForUnLabelled( strFilePath );
        }

        int index = in(strFilePath, _dequeTodo);
        if(index != -1 && index < _dequeTodo.size() - 1)
        {
            _strNextImage = _dequeTodo[index+1];
        }
        else if(index == _dequeTodo.size()-1 && !_dequeDone.empty())
        {
            _strNextImage = _dequeDone[0];
        }
        else
        {
            _strNextImage = "";
        }
        setEnlargementFactor(strFilePath);
    }


}

void ImageCompletionUI::OnDoubleClickTreeView(QModelIndex m)
{
    QString selectedItemTitle = m.data().toString();
    if(selectedItemTitle.endsWith(tr("jpg")) || selectedItemTitle.endsWith(tr("png")) || selectedItemTitle.endsWith(tr("bmp")) || selectedItemTitle.endsWith("jpeg"))
    {
        QString strFilePath = Global::PathImage + selectedItemTitle;
        _centralStackedWidget->setCurrentIndex(0);
        openImage(strFilePath);
    }
    else
    {
        QModelIndex parent = m.parent().isValid()? m.parent() : m;
        QAbstractItemModel* model = (QAbstractItemModel*)m.model();

        QStringList list;

        if(model->hasChildren())
        {
            for(int i = 0; i < model->rowCount(parent); i++)
            {
                QModelIndex subModel = parent.child(i, 0);
                if(m.parent().isValid() && subModel.data().toString() != selectedItemTitle)
                    continue;
                else
                    selectedItemTitle = subModel.data().toString();

                if(selectedItemTitle.endsWith(tr("jpg")) || selectedItemTitle.endsWith(tr("png")) || selectedItemTitle.endsWith(tr("bmp")) || selectedItemTitle.endsWith("jpeg"))
                {
                    list.push_back(Global::PathImage+selectedItemTitle);
                }
                else
                {
                    QAbstractItemModel* model2 = (QAbstractItemModel*)subModel.model();
                    if(model2->hasChildren())
                    {
                        for(int j = 0; j < model2->rowCount(subModel); ++j)
                        {
                            QModelIndex subsubModel = subModel.child(j, 0);
                            selectedItemTitle = subsubModel.data().toString();

                            if(selectedItemTitle.endsWith(tr("jpg")) || selectedItemTitle.endsWith(tr("png")) || selectedItemTitle.endsWith(tr("bmp")) || selectedItemTitle.endsWith("jpeg"))
                            {
                                list.push_back(Global::PathImage+selectedItemTitle);
                            }
                            else
                            {
                                QAbstractItemModel* model3 = (QAbstractItemModel*)subsubModel.model();
                                if(model3->hasChildren())
                                {
                                    for(int j = 0; j < model3->rowCount(subsubModel); ++j)
                                    {
                                        QModelIndex subsubsubModel = subsubModel.child(j, 0);
                                        selectedItemTitle = subsubsubModel.data().toString();

                                        if(selectedItemTitle.endsWith(tr("jpg")) || selectedItemTitle.endsWith(tr("png")) || selectedItemTitle.endsWith(tr("bmp")) || selectedItemTitle.endsWith("jpeg"))
                                        {
                                            list.push_back(Global::PathImage+selectedItemTitle);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        showThumbnailsInCentral(list);
    }
}
void ImageCompletionUI::flushLeftTree()
{
    _treeModel.updateData();
}

void ImageCompletionUI::showImagesInTree()
{
    _leftWindow._treeViewImages->setAlternatingRowColors(true);
    _leftWindow._treeViewImages->setExpandsOnDoubleClick(false);

    _leftWindow._treeViewImages->setModel(&_treeModel);
    _leftWindow._treeViewImages->setHeaderHidden(true);
    _leftWindow._treeViewImages->setAlternatingRowColors(true);
    _leftWindow._treeViewImages->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _leftWindow._treeViewImages->setSelectionMode( QAbstractItemView::SingleSelection );
    _leftWindow._treeViewImages->show();
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
    std::deque<QString>::iterator ind = std::find(d.begin(), d.end(), strFilePath);

    if(ind == d.end())
    {
        return -1;
    }
    else
    {
        return (int)(ind - d.begin());
    }
}

void ImageCompletionUI::showThumbnail(QString file, QString status, int row)
{
    QPixmap pixmap = QPixmap(file).scaled(120, 80);
    drawEnclosingRectangle(pixmap, color(status));
    QListWidgetItem *item = new QListWidgetItem(QIcon(pixmap), file);
    item->setSizeHint(QSize(120, 80));

    // small trick here
    QFont f;
    f.setPointSize(1);
    item->setFont(f);

    _leftWindow._listWidget->setIconSize(QSize(120,  80));
    _leftWindow._listWidget->insertItem(row, item);

    QPixmapCache::clear();
}

//FIXME
void ImageCompletionUI::showThumbnailForLabelled(QString strFilePath)
{
    if(strFilePath.isEmpty() || !QFile::exists(strFilePath) ) return;

    int pos = in(strFilePath, _dequeDone);
    if(pos >= 0)
    {
        showThumbnail(strFilePath, MOLI_LABELLED_STATUS_CHAR, pos + _dequeTodo.size());
    }
}

void ImageCompletionUI::showThumbnailForUnLabelled(QString strFilePath)
{
    if(strFilePath.isEmpty() || !QFile::exists(strFilePath) ) return;

    int pos = in(strFilePath, _dequeTodo);
    if(pos >= 0)
    {
        showThumbnail(strFilePath, MOLI_UNLABELLED_STATUS_CHAR, pos);
    }
}

void	ImageCompletionUI::save()
{
    if ( Global::NewName.isEmpty() ) return;

    bool ret1, ret2;

    if(Global::PathResult.isEmpty() || !QDir(Global::PathResult).exists())
    {
        QMessageBox::warning(this,
                             tr("保存"),
                             QString("请指定标注图像保存路径:%1").arg(QFileInfo(QApplication::instance()->applicationFilePath()).baseName() + ".ini"));
        return;
    }
    if(Global::PathMask.isEmpty() || !QDir(Global::PathMask).exists())
    {
        QMessageBox::warning(this,
                             tr("保存"),
                             QString("请指定掩码图像保存路径:%1").arg(QFileInfo(QApplication::instance()->applicationFilePath()).baseName() + ".ini"));
        return;
    }

    // Save
    QString pathResult, pathMask;
    if(Global::MoliId.isEmpty())
    {
        pathResult = Global::PathResult + Global::NewName;
        pathMask = Global::PathMask + Global::NewName;
    }
    else
    {
        pathResult = Global::PathResult + Global::MoliId;
        pathMask = Global::PathMask + Global::MoliId;
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
    if(Global::NewName.isEmpty()) return;

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

void  ImageCompletionUI::addtosql()
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

void ImageCompletionUI::updateStatusBar()
{
    char* log_str = getNewLogString();
    if ( log_str )
    {
        (*_logInformationString) = log_str;

        statusBar()->showMessage(*_logInformationString, 10000);
    }

    if(log_str)
    {
        delete[] log_str;
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
                    QTableWidgetItem* item = new QTableWidgetItem(query.value(j).toString());
                    switch(k)
                    {
                    case 1:
                    {
                        if( j == 0 ) _bottomWindow.dBTableWidget_1->insertRow(_bottomWindow.dBTableWidget_1->rowCount());
                        _bottomWindow.dBTableWidget_1->setItem(i, j, item);
                        break;
                    }
                    case 2:
                    {
                        if( j == 0 ) _bottomWindow.dBTableWidget_2->insertRow(_bottomWindow.dBTableWidget_2->rowCount());
                        _bottomWindow.dBTableWidget_2->setItem(i, j, item); break;
                    }
                    case 3:
                    {
                        if( j == 0 ) _bottomWindow.dBTableWidget_3->insertRow(_bottomWindow.dBTableWidget_3->rowCount());
                        _bottomWindow.dBTableWidget_3->setItem(i, j, item); break;
                    }
                    case 4:
                    {
                        if( j == 0 ) _bottomWindow.dBTableWidget_4->insertRow(_bottomWindow.dBTableWidget_4->rowCount());
                        _bottomWindow.dBTableWidget_4->setItem(i, j, item); break;
                    }
                    case 5:
                    {
                        if( j == 0 ) _bottomWindow.dBTableWidget_5->insertRow(_bottomWindow.dBTableWidget_5->rowCount());
                        _bottomWindow.dBTableWidget_5->setItem(i, j, item); break;
                    }
                    case 6:
                    {
                        if( j == 0 ) _bottomWindow.dBTableWidget_6->insertRow(_bottomWindow.dBTableWidget_6->rowCount());
                        _bottomWindow.dBTableWidget_6->setItem(i, j, item); break;
                    }
                    case 7:
                    {
                        if( j == 0 ) _bottomWindow.dBTableWidget_7->insertRow(_bottomWindow.dBTableWidget_7->rowCount());
                        _bottomWindow.dBTableWidget_7->setItem(i, j, item); break;
                    }
                    case 8:
                    {
                        if( j == 0 ) _bottomWindow.dBTableWidget_8->insertRow(_bottomWindow.dBTableWidget_8->rowCount());
                        _bottomWindow.dBTableWidget_8->setItem(i, j, item); break;
                    }
                    case 9:
                    {
                        if( j == 0 ) _bottomWindow.dBTableWidget_9->insertRow(_bottomWindow.dBTableWidget_9->rowCount());
                        _bottomWindow.dBTableWidget_9->setItem(i, j, item); break;
                    }
                    }
                }
                i++;
            }
        }
    }
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
    if(!_editImageViewer->getOCVImage()) return;

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
    QMessageBox::StandardButton reply = QMessageBox::information(this, tr("退出"), tr("是否要退出标注程序?"), QMessageBox::Ok | QMessageBox::Cancel);
    if(reply == QMessageBox::Ok)
        qApp->exit();
}

void ImageCompletionUI::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton reply = QMessageBox::information(this, tr("退出"), tr("是否要退出标注程序?"), QMessageBox::Ok | QMessageBox::Cancel);
    if(reply == QMessageBox::Ok)
        event->accept();
    else
        event->ignore();
}

void ImageCompletionUI::setEnlargementFactor(QString strFilePath)
{
    QString factor;
    QSqlDatabase db;
    if(Global::createConnection(db))
    {
        QSqlQuery query;
        QString sql = QString("select magnification from ferrographypicinfo where ferrographypicpath = '%1' limit 1").arg(strFilePath);

        if(query.exec(sql) && query.next() )
        {
            factor = query.value(0).toString();
        }
        else
        {
            factor = "1";
        }

        factor = factor + "x";

        if(factor == "100x")
        {
            _regionCompetitionDialog._comboBoxLarger->setCurrentIndex(0);
        }
        else if(factor == "200x")
        {
            _regionCompetitionDialog._comboBoxLarger->setCurrentIndex(1);
        }
        else if(factor == "500x")
        {
            _regionCompetitionDialog._comboBoxLarger->setCurrentIndex(2);
        }
        else
        {
            _regionCompetitionDialog._comboBoxLarger->setEditText(factor);
        }
    }
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

    changeMeasureButtonState(true);
}

void ImageCompletionUI::brighting()
{
    _regionCompetitionDialog.sliderBasicOp->setMaximum(100);
    _regionCompetitionDialog.sliderBasicOp->setMinimum(-100);
    _regionCompetitionDialog.sliderBasicOp->setValue(0);

    _regionCompetitionDialog.labelSliderLeft->setText(tr("暗"));
    _regionCompetitionDialog.labelSliderRight->setText(tr("亮"));

    changeMeasureButtonState(true);
}

void ImageCompletionUI::notColor()
{
    _regionCompetitionDialog.labelSliderLeft->setText(tr(""));
    _regionCompetitionDialog.labelSliderRight->setText(tr(""));

    _editImageViewer->imageNotColor( 0.0 );

    changeMeasureButtonState(true);
}

void ImageCompletionUI::measure()
{
    changeMeasureButtonState(false);
    _imageScale = 1.0;
}

void ImageCompletionUI::actionSliderReleased()
{
    double scaleFactor;

    //    if(_regionCompetitionDialog.radioScale->isChecked())
    //    {
    //        scaleFactor = _regionCompetitionDialog.sliderBasicOp->value() * 1.0 / 10;
    //        _editImageViewer->imageScaling( scaleFactor );
    //    }
    if(_regionCompetitionDialog.radioBrightness->isChecked())
    {
        scaleFactor = _regionCompetitionDialog.sliderBasicOp->value();
        _editImageViewer->imageBrighting( scaleFactor );
    }
    else if(_regionCompetitionDialog.radioNotColor->isChecked())
    {
        //	Do nothing (Fix it in the future)

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
    if(!_editImageViewer->getOCVImage()) return;

    a->setChecked(true);

    _editImageViewer->setPaintable(true);
    setStrikeOptionsEnabled(false);

    if(a == _rectAction)
    {
        _editImageViewer->setMethod(1);
        _editImageViewer->setLabelType(1);
        setStrikeOptionsEnabled(false);
    }
    else if(a == _polygonAction)
    {
        _editImageViewer->setMethod(1);
        _editImageViewer->setLabelType(2);
        setStrikeOptionsEnabled(false);
    }
    else if(a == _manualAction)
    {
        _editImageViewer->setMethod(2);
        _editImageViewer->setLabelType(2);
        setStrikeOptionsEnabled(false);
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
    if(!_editImageViewer->getOCVImage()) return;

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
                {
                    prefix+=fieldName;
                    suffix+=QString("'%1'").arg(query.value(i).toString());
                    break;
                }
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
                    break;
                }
                default:
                {
                    prefix+=fieldName;
                    suffix+=query.value(i).toString();
                }
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
    //qDebug() << "path: " << path;
    if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        QMessageBox::warning(this,tr("提示"),tr("数据保存文件打开失败"),QMessageBox::Ok);
        return false;
    }

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
    _bottomWindow.dBTableWidget_1->clearContents();
    _bottomWindow.dBTableWidget_2->clearContents();
    _bottomWindow.dBTableWidget_3->clearContents();
    _bottomWindow.dBTableWidget_4->clearContents();
    _bottomWindow.dBTableWidget_5->clearContents();
    _bottomWindow.dBTableWidget_6->clearContents();
    _bottomWindow.dBTableWidget_7->clearContents();
    _bottomWindow.dBTableWidget_8->clearContents();
    _bottomWindow.dBTableWidget_9->clearContents();

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

void ImageCompletionUI::flush()
{
    flushLeftTree();
    showData();
}

void ImageCompletionUI::flushLeft(QString strFilePath, QString label)
{
    std::deque<QString>::iterator iter = std::find(_dequeTodo.begin(), _dequeTodo.end(), strFilePath);
    if(iter != _dequeTodo.end())
    {
        int k = (int)(iter - _dequeTodo.begin());

        if(_leftWindow._listWidget->count() > k && _leftWindow._listWidget->item(k))
        {
            _leftWindow._listWidget->takeItem(k);
            _dequeTodo.erase(iter);
        }

        if(-1 == in(strFilePath, _dequeDone))
        {
            this->enDeque(strFilePath, _dequeDone);
            this->showThumbnailForLabelled(strFilePath);
        }
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
        if(!targetDir.mkpath("."))
            return false;
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    //    qDebug()<<fileInfoList.length();
    // 遍历所有文件信息
    //    foreach(QFileInfo fileInfo, fileInfoList)
    for(int i = 0;i<fileInfoList.length();++i)
    {
        QFileInfo fileInfo = fileInfoList.at(i);
        // 去除当前目录和父目录
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;
        // 数据库文件处理
        //        qDebug() << fileInfo.fileName();
        //        if(fileInfo.fileName().split(".")[1] == "sql");

        // 当为目录时，递归的进行copy
        if(fileInfo.isDir())
        {
            if(!copyFiles(fileInfo.filePath(),
                          targetDir.filePath(fileInfo.fileName()),
                          convertIfExits))
                return false;
        }
        else
        {
            if(convertIfExits && targetDir.exists(fileInfo.fileName()))
            {
                targetDir.remove(fileInfo.fileName());
            }
            // 进行文件copy
            if(!QFile::copy(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName()))){
                //return false; // ???
            }
        }
    }
    return true;
}

void ImageCompletionUI::importData()
{
    ImpDlg *impdlg = new ImpDlg(this);
    if(impdlg->exec() == QDialog::Accepted)
    {
        QDir dir(_impPackgePath);
        QFileInfoList infoList = dir.entryInfoList();
        QStringList fileNameList;
        foreach(QFileInfo fileInfo,infoList)
        {
            if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
                continue;
            fileNameList.append(fileInfo.fileName());
        }
        // check packge file
        if(!fileNameList.contains("原始图像文件") || !fileNameList.contains("标记结果文件") || !fileNameList.contains("掩码图像文件") &&  !fileNameList.contains("databackup.sql"))
        {
            QMessageBox::warning(this,tr("提示"),tr("打包文件受损"),QMessageBox::Close);
        }
        else
        {
            QString recoverImagePath = Global::PathImage;
            QString recoverResultPath = Global::PathResult;
            QString recoverMaskPath = Global::PathMask;

#ifdef Q_OS_WIN
            QString packgeImagePath = _impPackgePath + QString::fromUtf8("/原始图像文件");
            QString packgeResultPath = _impPackgePath + QString::fromUtf8("/标记结果文件");
            QString packgeMaskPath = _impPackgePath + QString::fromUtf8("/掩码图像文件");
            QString packgeSqlPath = _impPackgePath + "/databackup.sql";
#endif

#ifdef Q_OS_LINUX
            QString packgeImagePath = _impPackgePath + tr("/原始图像文件");
            QString packgeResultPath = _impPackgePath + tr("/标记结果文件");
            QString packgeMaskPath = _impPackgePath + tr("/掩码图像文件");
            QString packgeSqlPath = _impPackgePath + "/databackup.sql";
#endif

            if(this->copyFiles(packgeImagePath,recoverImagePath) &&
                    this->copyFiles(packgeResultPath,recoverResultPath) &&
                    this->copyFiles(packgeMaskPath, recoverMaskPath) &&
                    this->importDB(packgeSqlPath))
            {
                QMessageBox::warning(this,tr("提示"),tr("数据恢复成功"),QMessageBox::Close);
            }
            else
            {
                QMessageBox::warning(this,tr("提示"),tr("数据恢复失败"),QMessageBox::Close);
            }
        }
    }
    delete impdlg;
}

void ImageCompletionUI::exportData()
{
    ExpDlg *expDlg = new ExpDlg(this, Global::PathImage, Global::PathResult, Global::PathMask);
    if(expDlg->exec() == QDialog::Accepted)
    {
        QString sourcePath = this->_expSourcePicPath;
        QString resultPath = this->_expResultPicPath;
        QString maskPath = this->_expMaskPicPath;
        QString targetPath = this->_expPackgePath;
#ifdef Q_OS_WIN
        QString sourcetargetPath = targetPath + QString::fromUtf8("/打包文件/原始图像文件");
        QString resulttargetPath = targetPath + QString::fromUtf8("/打包文件/标记结果文件");
        QString maskTargetPath = targetPath + QString::fromUtf8("/打包文件/掩码图像文件");
        QString databackupFileName = targetPath + QString::fromUtf8("/打包文件/databackup.sql");
#endif

#ifdef Q_OS_LINUX
        QString sourcetargetPath = targetPath + tr("/打包文件/原始图像文件");
        QString resulttargetPath = targetPath + tr("/打包文件/标记结果文件");
        QString maskTargetPath = targetPath + tr("/打包文件/掩码图像文件");
        QString databackupFileName = targetPath + tr("/打包文件/databackup.sql");
#endif

        if(copyFiles(sourcePath,sourcetargetPath) && copyFiles(resultPath,resulttargetPath) && copyFiles(maskPath, maskTargetPath) && exportDB(databackupFileName))
            QMessageBox::warning(this,tr("批量数据导出提示"),tr("批量数据导出成功"),QMessageBox::Close);
        else
            QMessageBox::warning(this,tr("批量数据导出提示"),tr("批量数据导出失败"),QMessageBox::Close);
    }
    delete expDlg;
}

void ImageCompletionUI::setExpPath(QString sourcePicPath, QString resultPicPath, QString maskPicPath, QString packgePath)
{
    this->_expSourcePicPath = sourcePicPath;
    this->_expResultPicPath = resultPicPath;
    this->_expMaskPicPath = maskPicPath;
    this->_expPackgePath = packgePath;
}

void ImageCompletionUI::setImpPath(QString packgePath)
{
    this->_impPackgePath = packgePath;
}

QString ImageCompletionUI::status(QString imagePath)
{
    if(imagePath.isEmpty()) return QString();

    QSqlDatabase db;
    QSqlQuery query;
    if( Global::createConnection(db) )
    {
        if(query.exec(QString("select imagesymbol from ferrographypicinfo where ferrographypicpath = '%1' limit 1").arg(imagePath)) && query.next())
        {
            return query.value(0).toString();
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
        QString sql = QString("select abrasiveResultData from abrasivemarkinfo where abrasiveid = '%1' limit 1").arg(moliId);
        QSqlQuery query;
        query.prepare(sql);

        if(query.exec() && query.next())
        {
            QByteArray arr = query.value(0).toByteArray();
            QPixmap pixmap;
            pixmap.loadFromData(arr);
            QImage image = pixmap.toImage();

            _editImageViewer->setImage(image);
        }
    }
}

void ImageCompletionUI::loadAllImagesAndShowInLeftWindow()
{
    QString imagePath, status;
    QSqlDatabase db;
    QSqlQuery query;
    if( Global::createConnection(db) )
    {
        if(query.exec(QString("select ferrographypicpath, imagesymbol from ferrographypicinfo")))
        {
            while(query.next())
            {
                imagePath = query.value(0).toString();

                status = query.value(1).toString();
                if(status == MOLI_LABELLED_STATUS_CHAR)
                {
                    this->enDeque(imagePath, _dequeDone);
                    this->showThumbnailForLabelled(imagePath);
                }
                else
                {
                    this->enDeque(imagePath, _dequeTodo);
                    this->showThumbnailForUnLabelled(imagePath);
                }
            }
        }
    }
}

void ImageCompletionUI::wheelEvent(QWheelEvent *event)
{
    //    if(_centralTabWidget->underMouse() && _editImageViewer && !_editImageViewer->image().isNull())
    //    {
    //        int numDegrees = event->delta() / 8;
    //        int numSteps = numDegrees / 15;

    //        double factor = 10 + numSteps;
    //        factor = factor >= 1 ? factor : 1;
    //        factor = double(factor) / 10;

    //        _editImageViewer->imageScaling(factor);

    //        event->accept();
    //    }
}

void ImageCompletionUI::changeMeasureButtonState(bool state)
{
    _regionCompetitionDialog.labelSliderLeft->setHidden(!state);
    _regionCompetitionDialog.labelSliderRight->setHidden(!state);
    _regionCompetitionDialog.sliderBasicOp->setHidden(!state);
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

void ImageCompletionUI::bottomWindowContextMenuEvent(const QPoint &pos)
{
    QAction* editAction = new QAction(tr("编辑"), this);
    connect(editAction, SIGNAL(triggered()), this, SLOT(editImageProperties()));

    QAction* addAction = new QAction(tr("添加采样点"), this);
    connect(addAction, SIGNAL(triggered()), this, SLOT(addSamplePoint()));

    QAction* deleteAction = new QAction(tr("删除采样点"), this);
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteSamplePoint()));


    QMenu contextMenu;
    contextMenu.addAction(editAction);

    switch(_bottomWindow.dataBaseTabWidget->currentIndex())
    {
    case 0:

        if(_bottomWindow.dBTableWidget_1->selectionModel()->selection().indexes().isEmpty())
        {
            editAction->setEnabled(false);
        }
        contextMenu.exec(_bottomWindow.dBTableWidget_1->mapToGlobal(pos));
        break;
    case 1:

        if(_bottomWindow.dBTableWidget_2->selectionModel()->selection().indexes().isEmpty())
        {
            editAction->setEnabled(false);
        }
        contextMenu.exec(_bottomWindow.dBTableWidget_2->mapToGlobal(pos));
        break;
    case 2:

        if(_bottomWindow.dBTableWidget_3->selectionModel()->selection().indexes().isEmpty())
        {
            editAction->setEnabled(false);
        }
        contextMenu.exec(_bottomWindow.dBTableWidget_3->mapToGlobal(pos));
        break;
    case 3:

        if(_bottomWindow.dBTableWidget_4->selectionModel()->selection().indexes().isEmpty())
        {
            editAction->setEnabled(false);
        }
        contextMenu.exec(_bottomWindow.dBTableWidget_4->mapToGlobal(pos));
        break;
    case 4:
        if(_bottomWindow.dBTableWidget_5->selectionModel()->selection().indexes().isEmpty())
        {
            editAction->setEnabled(false);
        }
        contextMenu.exec(_bottomWindow.dBTableWidget_5->mapToGlobal(pos));
        break;
    case 5:
        if(_bottomWindow.dBTableWidget_6->selectionModel()->selection().indexes().isEmpty())
        {
            editAction->setEnabled(false);
        }
        contextMenu.exec(_bottomWindow.dBTableWidget_6->mapToGlobal(pos));
        break;
    case 6:
        if(_bottomWindow.dBTableWidget_7->selectionModel()->selection().indexes().isEmpty())
        {
            editAction->setEnabled(false);
        }
        contextMenu.exec(_bottomWindow.dBTableWidget_7->mapToGlobal(pos));
        break;
    case 7:
        if(_bottomWindow.dBTableWidget_8->selectionModel()->selection().indexes().isEmpty())
        {
            editAction->setEnabled(false);
        }
        contextMenu.exec(_bottomWindow.dBTableWidget_8->mapToGlobal(pos));
        break;
    case 8:
        contextMenu.addAction(addAction);
        contextMenu.addAction(deleteAction);

        if(_bottomWindow.dBTableWidget_9->selectionModel()->selection().indexes().isEmpty())
        {
            editAction->setEnabled(false);
        }
        contextMenu.exec(_bottomWindow.dBTableWidget_9->mapToGlobal(pos));
        break;
    }
}

void ImageCompletionUI::editImageProperties()
{
    int index = _bottomWindow.dataBaseTabWidget->currentIndex();
    QString primaryKeyValue;

    switch(index)
    {
    case 0: primaryKeyValue = _bottomWindow.dBTableWidget_1->item(_bottomWindow.dBTableWidget_1->currentRow(), 0)->text(); break;
    case 1: primaryKeyValue = _bottomWindow.dBTableWidget_2->item(_bottomWindow.dBTableWidget_2->currentRow(), 0)->text(); break;
    case 2: primaryKeyValue = _bottomWindow.dBTableWidget_3->item(_bottomWindow.dBTableWidget_3->currentRow(), 0)->text(); break;
    case 3: primaryKeyValue = _bottomWindow.dBTableWidget_4->item(_bottomWindow.dBTableWidget_4->currentRow(), 0)->text(); break;
    case 4: primaryKeyValue = _bottomWindow.dBTableWidget_5->item(_bottomWindow.dBTableWidget_5->currentRow(), 0)->text(); break;
    case 5: primaryKeyValue = _bottomWindow.dBTableWidget_6->item(_bottomWindow.dBTableWidget_6->currentRow(), 0)->text(); break;
    case 6: primaryKeyValue = _bottomWindow.dBTableWidget_7->item(_bottomWindow.dBTableWidget_7->currentRow(), 0)->text(); break;
    case 7: primaryKeyValue = _bottomWindow.dBTableWidget_8->item(_bottomWindow.dBTableWidget_8->currentRow(), 0)->text(); break;
    case 8: primaryKeyValue = _bottomWindow.dBTableWidget_9->item(_bottomWindow.dBTableWidget_9->currentRow(), 0)->text(); break;
    }

    ImagePropertiesEditor* d = new ImagePropertiesEditor(this, index, primaryKeyValue);
    d->showDlg();
}

void ImageCompletionUI::addSamplePoint()
{
    int index = _bottomWindow.dataBaseTabWidget->currentIndex();
    QString primaryKeyValue = "NEW"; // label

    ImagePropertiesEditor* d = new ImagePropertiesEditor(this, index, primaryKeyValue);
    d->showDlg();
}

void ImageCompletionUI::deleteSamplePoint()
{
    QString primaryKeyValue = _bottomWindow.dBTableWidget_9->item(_bottomWindow.dBTableWidget_9->currentRow(), 0)->text();

    QSqlDatabase db;
    if(Global::createConnection(db))
    {
        QString sql = QString("delete from sampleSummaryInfo where sampleId = '%1' limit 1").arg(primaryKeyValue);

        QSqlQuery query;
        query.prepare(sql);
        if(query.exec())
        {
            _bottomWindow.dBTableWidget_9->removeRow(_bottomWindow.dBTableWidget_9->currentRow());
        }
    }
}

void ImageCompletionUI::nextImage()
{
    if(!_strNextImage.isEmpty())
    {
        openImage(_strNextImage);
    }
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
        QMessageBox::warning(this, tr("提示"), tr("无法打开图像"), QMessageBox::Close);
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

void ImageCompletionUI::clearAll()
{
    // bottomWindow
    clearBottomWindow();

    // leftWindow
    _leftWindow._listWidget->clear();
    _dequeTodo.clear();
    _dequeDone.clear();

    // leftTree
    _treeModel.release();
}

void ImageCompletionUI::removeImageSlot(QString path)
{
    int index;
    std::deque<QString>::iterator iter = std::find(_dequeTodo.begin(), _dequeTodo.end(), path);
    if(iter == _dequeTodo.end())
    {
        iter = std::find(_dequeDone.begin(), _dequeDone.end(), path);
        if(iter != _dequeDone.end())
        {
            index = (int)(iter-_dequeDone.begin()) + _dequeTodo.size();
            _dequeDone.erase(iter);
            if(_leftWindow._listWidget->item(index))
            {
                _leftWindow._listWidget->takeItem(index);
            }
        }
    }
    else
    {
        index = (int)(iter-_dequeTodo.begin());
        _dequeTodo.erase(iter);
        if(_leftWindow._listWidget->item(index))
        {
            _leftWindow._listWidget->takeItem(index);
        }
    }
}

void ImageCompletionUI::showThumbnailsInCentral(QStringList list)
{
    close();
    clearLayout(_formLayout);

    if(list.empty())
    {
        return;
    }

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
                    image = image.scaled(QSize(150, 75));
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

    _thumbnailScrollArea->widget()->setLayout(_formLayout);
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
    QMouseEvent * mouseEvent = static_cast <QMouseEvent *> (event);
    if(event->type() == QEvent::MouseButtonDblClick)
    {
        QString strFilePath = target->objectName();
        if( QFile::exists(strFilePath) )
        {
            _centralStackedWidget->setCurrentIndex(0);
            openImage(strFilePath);
            _canBack = true;
        }
        return true;
    }

    return false;
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
    painter.setPen(QPen(color, 10, Qt::SolidLine));
    painter.drawRect(0, 0, pixmap.width(), pixmap.height());
}

void ImageCompletionUI::on__spinBox_measure_valueChanged(int arg1)
{
    //    _imageScale = arg1 * 1.0 / _regionCompetitionDialog._spinBox_pixel->value();
    //    _imageScale *= _editImageViewer->orgWidth() * _editImageViewer->orgHeight() * 1.0 / _editImageViewer->width() / _editImageViewer->height();
}

void ImageCompletionUI::on__spinBox_pixel_valueChanged(int arg1)
{
    //    _imageScale = _regionCompetitionDialog._spinBox_measure * 1.0 / arg1;
    //    _imageScale *= _editImageViewer->orgWidth() * _editImageViewer->orgHeight() * 1.0 / _editImageViewer->width() / _editImageViewer->height();
}

void ImageCompletionUI::about()
{

    (new About)->show();
}

void ImageCompletionUI::showDoc(){}

void ImageCompletionUI::on__comboBoxLarger_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "100x")
    {
        _regionCompetitionDialog._spinBox_pixel->setValue(284);
        _regionCompetitionDialog._spinBox_measure->setValue(200);
    }
    else if(arg1 == "200x")
    {
        _regionCompetitionDialog._spinBox_pixel->setValue(579);
        _regionCompetitionDialog._spinBox_measure->setValue(200);
    }
    else if(arg1 == "500x")
    {
        _regionCompetitionDialog._spinBox_pixel->setValue(1431);
        _regionCompetitionDialog._spinBox_measure->setValue(200);
    }
}

void ImageCompletionUI::on__horizontalSliderScale_valueChanged(int value)
{
    double scaleFactor = value * 1.0 / 10;
    _editImageViewer->imageScaling( scaleFactor );

    _regionCompetitionDialog._lineEdit_width->setText( QString::number(int(_editImageViewer->width() * scaleFactor)));
    _regionCompetitionDialog._lineEdit_height->setText(QString::number(int(_editImageViewer->height() * scaleFactor)));
}

void ImageCompletionUI::listItemDoubleClicked(QListWidgetItem *item)
{
    if(item == NULL) return;

    QString strFilePath = item->text();

    if(!strFilePath.isEmpty())
    {
        if(_centralStackedWidget->currentIndex() != 0)
        {
            _centralStackedWidget->setCurrentIndex(0);
        }
        openImage(strFilePath);
    }
}
