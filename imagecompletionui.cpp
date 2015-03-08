#include <QtGui>
#include <QPixmap>
#include <QSqlQuery>
#include "imagecompletionui.h"
#include "interfaces.h"
#include "ImageViewer.h"
#include "Connection.h"
#include "ImageProperties.h"

ImageCompletionUI::ImageCompletionUI(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    //setupUi(this);

    _brushSize = 3;
    _step = NONE;

    IsShowDetail = false;

    _editImageViewer = NULL;
    _cnt = -1;

    _imageName = "";
    _imagePath = "";

    createActions();

    setupMainWindow();

    createMenus();

    createToolBars();

    setupWidgets();

    createStatusBar();

    createConnections();

    //    setupBrush();

    _RegionupdateBrushSize();

    setStrikeOptionsEnabled(false);

    showData();
}

ImageCompletionUI::~ImageCompletionUI()
{

}

void	ImageCompletionUI::setupMainWindow()
{
    this->resize( 1024, 800 );
    this->setMinimumSize( QSize(1024, 0) );
    this->showMaximized();
    this->setWindowTitle(tr("磨粒图像标注"));
}

void	 ImageCompletionUI::createMenus()
{
    _menuFile = menuBar()->addMenu( tr("&文件") );

    _menuFile->addAction( _openAction );
    _menuFile->addAction( _openBatchAction );
    _menuFile->addSeparator();
    _menuFile->addAction( _saveAction );
    _menuFile->addAction( _saveAsAction );

    _menuFile->addSeparator();
    _menuFile->addAction( _closeAction );
    _menuFile->addSeparator();
    _menuFile->addAction( _exitAction );

    _menuLabelling = menuBar()->addMenu( tr("&图像标注") );
    QMenu* submenu = _menuLabelling->addMenu( tr("笔画标注") );
    submenu->addAction(_fgAction);
    submenu->addAction(_bgAction);
    submenu->addAction(_eraserAction);

    _menuLabelling->addAction( _rectAction );
    _menuLabelling->addAction( _polygonAction );
    _menuLabelling->addAction( _manualAction );
    _menuLabelling->addSeparator();

    submenu = _menuLabelling->addMenu( tr("笔画粗细") );
    for(int i = 0; i < 3; i++) submenu->addAction(_strikeThickness[i]);

    submenu = _menuLabelling->addMenu( tr("线条粗细") );
    for(int i = 0; i < 3; i++) submenu->addAction(_lineThickness[i]);

    //    _menuLabelling->addSeparator();
    //    _menuLabelling->addAction(_saveLabelResultAction);
    //    _menuLabelling->addAction(_saveMaskAction);

    _menuData=menuBar()->addMenu(tr("&数据管理"));
    _menuData->addAction(_searchAction);
    _menuData->addAction(_addtosqlAction);
    _menuData->addAction(_saveresultAction);
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

void	ImageCompletionUI::closeEvent( QCloseEvent */*event*/ ){}

void	ImageCompletionUI::createActions()
{
    ////////////////////////////////////////////////////////////////////////////////////
    //   _openAction
    ////////////////////////////////////////////////////////////////////////////////////
    _openAction = new QAction( tr("&打开"), this );
    _openAction->setObjectName(tr("_openAction"));
    QIcon icon1;
    icon1.addPixmap(QPixmap(tr(":/new/prefix1/icons/fileopen.png")), QIcon::Normal, QIcon::Off);
    _openAction->setIcon(icon1);
    connect(_openAction, SIGNAL(triggered()), this, SLOT(open()));

    _openBatchAction = new QAction( tr("&打开(多)"), this );
    _openBatchAction->setObjectName(tr("_openBatchAction"));
    _openBatchAction->setIcon(icon1);
    connect(_openBatchAction, SIGNAL(triggered()), this, SLOT(batchOpen()));

    ////////////////////////////////////////////////////////////////////////////////////
    //   _saveAction
    ////////////////////////////////////////////////////////////////////////////////////
    _saveAction = new QAction( tr("&保存"), this );
    _saveAction->setObjectName(tr("_saveAction"));
    QIcon icon2;
    icon2.addPixmap(QPixmap(tr(":/new/prefix1/icons/filesave.png")), QIcon::Normal, QIcon::Off);
    _saveAction->setIcon(icon2);
    connect(_saveAction, SIGNAL(triggered()), this, SLOT(save()));

    ////////////////////////////////////////////////////////////////////////////////////
    //   _saveAsAction
    ////////////////////////////////////////////////////////////////////////////////////
    _saveAsAction = new QAction(  tr("&另存为"), this );
    _saveAsAction->setObjectName(tr("_saveAsAction"));
    _saveAsAction->setIcon(icon2);
    connect(_saveAsAction, SIGNAL(triggered()), this, SLOT( saveAs() ));

    ////////////////////////////////////////////////////////////////////////////////////
    //   _closeAction
    ////////////////////////////////////////////////////////////////////////////////////
    _closeAction = new QAction( tr("关闭"), this );
    _closeAction->setIcon( QIcon(":/new/prefix1/icons/fileclose.png") );
    connect( _closeAction, SIGNAL(triggered()), this, SLOT( close() ));

    ////////////////////////////////////////////////////////////////////////////////////
    //   _exitAction
    ////////////////////////////////////////////////////////////////////////////////////
    _exitAction = new QAction( tr("退出"), this );
    _exitAction->setObjectName(tr("_exitAction"));
    connect( _exitAction, SIGNAL(triggered()), this, SLOT(exitApp()) );


    ////////////////////////////////////////////////////////////////////////////////////
    //   _searchAction
    ////////////////////////////////////////////////////////////////////////////////////
    _searchAction = new QAction( tr("查询"), this );
    _searchAction->setObjectName(tr("_searchAction"));
    _searchAction->setIcon( QIcon(":/new/prefix1/icons/histo.png") );
    connect( _searchAction, SIGNAL(triggered()), this, SLOT( search() ));

    ////////////////////////////////////////////////////////////////////////////////////
    //   _addtosqlAction
    ////////////////////////////////////////////////////////////////////////////////////
    _addtosqlAction = new QAction( tr("属性分类"), this );
    _addtosqlAction->setObjectName(tr("_addtosqlAction"));
    _addtosqlAction->setIcon( QIcon(":/new/prefix1/icons/add.png") );
    connect( _addtosqlAction, SIGNAL(triggered()), this, SLOT( addtosql() ));

    ////////////////////////////////////////////////////////////////////////////////////
    //   _saveresultAction
    ////////////////////////////////////////////////////////////////////////////////////
    _saveresultAction = new QAction( tr("保存标注结果"), this );
    _saveresultAction->setObjectName(tr("_saveresultAction"));
    _saveresultAction->setIcon( QIcon(":/new/prefix1/icons/filesave.png") );
    connect( _saveresultAction, SIGNAL(triggered()), this, SLOT( savemarkresult() ));

    ////////////////////////////////////////////////////////////////////////////////////
    //   _exportDataAction
    ////////////////////////////////////////////////////////////////////////////////////
    _exportDataAction = new QAction(tr("批量导出数据"),this);
    _exportDataAction->setObjectName(tr("_exportDataAction"));
    connect(_exportDataAction,SIGNAL(triggered()),this,SLOT(exportData()));

    ////////////////////////////////////////////////////////////////////////////////////
    //   _importDataAction
    ////////////////////////////////////////////////////////////////////////////////////
    _importDataAction = new QAction(tr("批量导入数据"),this);
    _importDataAction->setObjectName(tr("_importDataAction"));
    connect(_importDataAction,SIGNAL(triggered()),this,SLOT(importData()));

    // Image Labelling

    _strikeCombobox = new QComboBox;
    _strikeCombobox->setInsertPolicy(QComboBox::InsertAfterCurrent);
    _strikeCombobox->setObjectName( tr("_strikeCombox") );
    connect( _strikeCombobox, SIGNAL(currentIndexChanged(int)), this, SLOT(strikeComboChanged(int)));
    QStringList list = (QStringList() << "笔画标注" << "前景" << "背景" << "橡皮");
    _strikeCombobox->addItems(list);

    // Group 1
    _fgAction = new QAction( tr("前景"), this );
    _fgAction->setCheckable(true);
    _bgAction = new QAction( tr("背景"), this );
    _bgAction->setCheckable(true);
    _eraserAction = new QAction( tr("橡皮"), this );
    _eraserAction->setCheckable(true);

    QActionGroup *group = new QActionGroup(this);
    group->addAction(_fgAction);
    group->addAction(_bgAction);
    group->addAction(_eraserAction);
    connect(group, SIGNAL(triggered(QAction*)), this, SLOT(strikeChangeTriggered(QAction*)));

    _rectAction = new QAction( tr("矩形标注"), this );
    _rectAction->setCheckable(true);

    _polygonAction = new QAction( tr("多边形标注"), this );
    _polygonAction->setCheckable(true);

    _manualAction = new QAction( tr("手工标注"), this );
    _manualAction->setCheckable(true);

    QActionGroup *group2 = new QActionGroup(this);
    group2->addAction(_rectAction);
    group2->addAction(_polygonAction);
    group2->addAction(_manualAction);
    connect(group2, SIGNAL(triggered(QAction*)), this, SLOT(labellingMethodChanged(QAction*)));

    //    _saveLabelResultAction = new QAction( tr("保存标注图像"), this );
    //    _saveLabelResultAction->setObjectName( tr("_saveLabelResultAction") );
    //    connect( _saveLabelResultAction, SIGNAL(triggered()), this, SLOT(saveLabelledResult()));

    //    _saveMaskAction = new QAction( tr("保存掩码"), this );
    //    _saveMaskAction->setObjectName( tr("_saveMaskAction") );
    //    connect( _saveMaskAction, SIGNAL(triggered()), this, SLOT(saveMask()) );

    _redo = new QAction( tr("重做"), this );
    _redo->setObjectName( tr("_redo") );
    connect( _redo, SIGNAL(triggered()), this, SLOT(redo()) );

    _undo = new QAction( tr("撤销"), this );
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
    connect(group3, SIGNAL(triggered(QAction*)), this, SLOT(strikeThicknessChanged(QAction*)) );

    _lineThickness[0] = new QAction( tr("细"), this );
    _lineThickness[1] = new QAction( tr("中"), this );
    _lineThickness[2] = new QAction( tr("粗"), this );
    QActionGroup *group4 = new QActionGroup(this);
    for(int i = 0; i < 3; i++)
    {
        _lineThickness[i]->setCheckable(true);
        group4->addAction(_lineThickness[i]);
    }
    connect(group4, SIGNAL(triggered(QAction*)), this, SLOT(lineThicknessChanged(QAction*)) );

    _strikeThicknessCombobox = new QComboBox;
    _strikeThicknessCombobox->setInsertPolicy(QComboBox::InsertAfterCurrent);
    _strikeThicknessCombobox->setObjectName( tr("_strikeCombox") );
    connect( _strikeThicknessCombobox, SIGNAL(currentIndexChanged(int)), this, SLOT(strikeThicknessComboboxChanged(int)));
    list = (QStringList() << "笔画粗细" << "细" << "中" << "粗");
    _strikeThicknessCombobox->addItems(list);

    _lineThicknessCombobox = new QComboBox;
    _lineThicknessCombobox->setInsertPolicy(QComboBox::InsertAfterCurrent);
    _lineThicknessCombobox->setObjectName( tr("_strikeCombox") );
    connect( _lineThicknessCombobox, SIGNAL(currentIndexChanged(int)), this, SLOT(lineThicknessComboboxChanged(int)));
    list = (QStringList() << "线条粗细" << "细" << "中" << "粗");
    _lineThicknessCombobox->addItems(list);

    // user
    if(Global::Authority == "1") //admin
    {
        _userManagementAction = new QAction( tr("用户管理"), this );
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

    // Labeling Actions
    _editToolBar->addWidget(_strikeCombobox);
    _editToolBar->addAction(_rectAction);
    _editToolBar->addAction(_polygonAction);
    _editToolBar->addAction(_manualAction);

    _editToolBar->addSeparator();

    _editToolBar->addAction(_redo);
    _editToolBar->addAction(_undo);

    //_editToolBar->addSeparator();
    //_editToolBar->addAction(_saveLabelResultAction);
    //_editToolBar->addAction(_saveMaskAction);

    _editToolBar->addSeparator();
    _editToolBar->addWidget(_strikeThicknessCombobox);
    _editToolBar->addWidget(_lineThicknessCombobox);
}

void	ImageCompletionUI::setupWidgets()
{

    int width = this->width();
    int height = this->height();

    ////////////////////////////////////////////////////////////////////////////////////
    //   _centralwidget
    ////////////////////////////////////////////////////////////////////////////////////
    _centralwidget  = new QWidget( this );
    _centralwidget->setObjectName( tr( "_centralwidget") );

    ////////////////////////////////////////////////////////////////////////////////////
    //   _centralTabWidget
    ////////////////////////////////////////////////////////////////////////////////////
    _centralTabWidget = new QTabWidget( _centralwidget );
    _centralTabWidget->setObjectName( tr( "_centralTabWidget") );

    QVBoxLayout *centralwigetLayout = new QVBoxLayout;
    centralwigetLayout->addWidget(_centralTabWidget);
    centralwigetLayout->setAlignment(Qt::AlignCenter);
    _centralwidget->setLayout(centralwigetLayout);

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
    _editImageViewer->setObjectName(tr("_editImageViewer"));
    _editImageViewer->getMainWindow(this);
    _editImageViewer->setLineColor(QColor(colorTable[3], colorTable[4], colorTable[5]));

    _centralTabWidget->addTab( _editTab, QString("EditTab") );
    _centralTabWidget->setTabText(_centralTabWidget->indexOf(_editTab), QApplication::translate("ImageCompletionUIClass", tr("图像标注").toLocal8Bit().data(), 0));

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
    setCentralWidget(_centralwidget);
    _centralTabWidget->setCurrentIndex(0);

    ////////////////////////////////////////////////////////////////////////////////////
    //   _rightOperationWidget
    ////////////////////////////////////////////////////////////////////////////////////

    _rightOperationWidget = new QDockWidget(tr("工具箱"),this );
    _rightOperationWidget->setObjectName(tr("_rightOperationWidget"));
    _rightOperationWidget->setMaximumSize(QSize(0.2*width, height));
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
    _dockWidgetContents->setLayout(rightwidgetLayout);

    _sceneCompletionPage = new QWidget(  );
    _sceneCompletionPage->setObjectName(tr("_sceneCompletionPage"));
    _operationStackedWidget->addWidget( _sceneCompletionPage );

    _regionCompetitionPage = new QWidget(  );
    _regionCompetitionPage->setObjectName(tr("_regionCompetitionPage"));
    _operationStackedWidget->addWidget( _regionCompetitionPage );

    ////////////////////////////////////////////////////////////////////////////////////
    //		_sceneCompletionPage
    ////////////////////////////////////////////////////////////////////////////////////
    _sceneCompletionDialog.setupUi( _sceneCompletionPage );
    _regionCompetitionDialog.setupUi(_regionCompetitionPage);

    _rightOperationWidget->setWidget(_dockWidgetContents);
    this->addDockWidget(static_cast<Qt::DockWidgetArea>(2), _rightOperationWidget);

    _operationStackedWidget->setCurrentIndex(1);

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
    _logWindowWidget->setFeatures(QDockWidget::DockWidgetMovable);
    _logWindowWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    _logWindowWidget->setWidget(_logWidget);
    _logWindowWidget->setMinimumHeight(0.28 * height);
    addDockWidget(Qt::RightDockWidgetArea,_logWindowWidget);

    ////////////////////////////////////////////////////////////////////////////////////
    //   _leftWindowWidget
    ////////////////////////////////////////////////////////////////////////////////////
    _leftWindowWidget = new QDockWidget(tr(""),this );
    _leftWindowWidget->setObjectName(tr("_leftWindowWidget"));
    _leftWindowWidget->setFeatures(QDockWidget::DockWidgetMovable);
    _leftWindowWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    _leftWindowWidget->setMaximumSize(QSize(0.2*width, height));
    _leftWindowWidget->setMaximumHeight(height);
    _leftWindowWidget->setFloating(false);
    //
    _leftDockWindowContents = new QWidget( );
    _leftDockWindowContents->setObjectName(tr("_leftDockWindowContents"));

    _leftWindow.setupUi(_leftDockWindowContents);
    _leftWindow.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _leftWindow.tabWidgetLeftWindow->setTabText(0, tr("图谱信息"));
    _leftWindow.tabWidgetLeftWindow->removeTab(1); //TODO: now remove these 2 tabs
    _leftWindow.tabWidgetLeftWindow->removeTab(1);
    _leftWindowWidget->setWidget(_leftDockWindowContents);
    addDockWidget(Qt::LeftDockWidgetArea, _leftWindowWidget);

    ////////////////////////////////////////////////////////////////////////////////////
    //   _cornerWindowWidget
    ////////////////////////////////////////////////////////////////////////////////////
    _cornerWindowWidget = new QDockWidget(this );
    _cornerWindowWidget->setObjectName(tr("_cornerWindowWidget"));
    _cornerWindowWidget->setFeatures(QDockWidget::DockWidgetMovable);
    _cornerWindowWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    _cornerWindowWidget->setFloating(false);
    _cornerDockWindowContents = new QWidget( );
    _cornerDockWindowContents->setObjectName(tr("_leftDockWindowContents"));
    _cornerWindow.setupUi(_cornerDockWindowContents);
    _cornerWindowWidget->setWidget(_cornerDockWindowContents);
    addDockWidget(Qt::LeftDockWidgetArea, _cornerWindowWidget);

    ////////////////////////////////////////////////////////////////////////////////////
    //   _bottomWindowWidget
    ////////////////////////////////////////////////////////////////////////////////////

    _bottomWindowWidget = new QDockWidget(tr("数据库信息"),this );
    _bottomWindowWidget->setObjectName(tr("_bottomWindowWidget"));
    _bottomWindowWidget->setFeatures(QDockWidget::DockWidgetMovable);
    _bottomWindowWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
    _bottomDockWindowContents = new QWidget( );
    _bottomDockWindowContents->setObjectName(tr("_bottomDockWindowContents"));
    _bottomWindow.setupUi(_bottomDockWindowContents);

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
    connect( _sceneCompletionDialog._brushSizeHSlider, SIGNAL(sliderMoved(int)), this, SLOT(_SceneupdateBrushSize()) );
    connect( _regionCompetitionDialog.brushSizeSmallRadio, SIGNAL(clicked()), this, SLOT(_RegionupdateBrushSize()) );
    connect( _regionCompetitionDialog.brushSizeMediumRadio, SIGNAL(clicked()), this, SLOT(_RegionupdateBrushSize()) );
    connect( _regionCompetitionDialog.brushSizeLargeRadio, SIGNAL(clicked()), this, SLOT(_RegionupdateBrushSize()) );

    //	connect(_regionCompetitionDialog.saveLabelMapBtn, SIGNAL(clicked()), this, SLOT(saveLabelMap()));
    //	connect(_regionCompetitionDialog.loadLabelMapBtn, SIGNAL(clicked()), this, SLOT(loadLabelMap()));

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

    connect(_regionCompetitionDialog.sliderBasicOp, SIGNAL(sliderReleased()), this, SLOT(actionSliderReleased()));

    connect(_regionCompetitionDialog.radioLineWidthThin, SIGNAL(clicked()), this, SLOT(updateLineWidth()));
    connect(_regionCompetitionDialog.radioLineWidthMedium, SIGNAL(clicked()), this, SLOT(updateLineWidth()));
    connect(_regionCompetitionDialog.radioLineWidthThick, SIGNAL(clicked()), this, SLOT(updateLineWidth()));

    connect(_leftWindow.tableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(cellDoubleClicked_(int, int)));
}

void ImageCompletionUI::setupBrush()
{
    _brushTool = new PaintTools;

    BrushInterface *iBrush = qobject_cast<BrushInterface *>( _brushTool );

    QString brushName = "Pencil";

    _editImageViewer->setBrush( iBrush, brushName );
}

void	ImageCompletionUI::open()
{
    /*_bottomWindow.dBTableWidget_1->clear();
    _bottomWindow.dBTableWidget_2->clear();
    _bottomWindow.dBTableWidget_3->clear();
    _bottomWindow.dBTableWidget_4->clear();
    _bottomWindow.dBTableWidget_5->clear();
    _bottomWindow.dBTableWidget_6->clear();
    _bottomWindow.dBTableWidget_7->clear();
    _bottomWindow.dBTableWidget_8->clear();
    _bottomWindow.dBTableWidget_9->clear();

    _bottomWindow.dBTableWidget_1->setItem(0,0,new QTableWidgetItem("序号"));
    _bottomWindow.dBTableWidget_1->setItem(0,1,new QTableWidgetItem("机号"));
    _bottomWindow.dBTableWidget_1->setItem(0,2,new QTableWidgetItem("机型"));
    _bottomWindow.dBTableWidget_1->setItem(0,3,new QTableWidgetItem("单位编号"));
    _bottomWindow.dBTableWidget_1->setItem(0,4,new QTableWidgetItem("飞行小时数"));
    _bottomWindow.dBTableWidget_1->setItem(0,5,new QTableWidgetItem("运行阶段"));
    _bottomWindow.dBTableWidget_1->setItem(0,6,new QTableWidgetItem("修理次数"));

    _bottomWindow.dBTableWidget_2->setItem(0,0,new QTableWidgetItem("序号"));
    _bottomWindow.dBTableWidget_2->setItem(0,1,new QTableWidgetItem("动部件编号"));
    _bottomWindow.dBTableWidget_2->setItem(0,2,new QTableWidgetItem("动部件型号"));
    _bottomWindow.dBTableWidget_2->setItem(0,3,new QTableWidgetItem("动部件名称"));
    _bottomWindow.dBTableWidget_2->setItem(0,4,new QTableWidgetItem("运行时数"));
    _bottomWindow.dBTableWidget_2->setItem(0,5,new QTableWidgetItem("运行阶段"));
    _bottomWindow.dBTableWidget_2->setItem(0,6,new QTableWidgetItem("机号"));
    _bottomWindow.dBTableWidget_2->setItem(0,7,new QTableWidgetItem("机型"));
    _bottomWindow.dBTableWidget_2->setItem(0,8,new QTableWidgetItem("动部件起始日期"));
    _bottomWindow.dBTableWidget_2->setItem(0,9,new QTableWidgetItem("动部件终止日期"));

    _bottomWindow.dBTableWidget_3->setItem(0,0,new QTableWidgetItem("序号"));
    _bottomWindow.dBTableWidget_3->setItem(0,1,new QTableWidgetItem("动部件维修编号"));
    _bottomWindow.dBTableWidget_3->setItem(0,2,new QTableWidgetItem("动部件编号"));
    _bottomWindow.dBTableWidget_3->setItem(0,3,new QTableWidgetItem("动部件型号"));
    _bottomWindow.dBTableWidget_3->setItem(0,4,new QTableWidgetItem("维修原因"));
    _bottomWindow.dBTableWidget_3->setItem(0,5,new QTableWidgetItem("维修日期"));
    _bottomWindow.dBTableWidget_3->setItem(0,6,new QTableWidgetItem("维修次数"));
    _bottomWindow.dBTableWidget_3->setItem(0,7,new QTableWidgetItem("维修执行单位"));
    _bottomWindow.dBTableWidget_3->setItem(0,8,new QTableWidgetItem("维修内容"));
    _bottomWindow.dBTableWidget_3->setItem(0,9,new QTableWidgetItem("维修换件情况"));

    _bottomWindow.dBTableWidget_4->setItem(0,0,new QTableWidgetItem("序号"));
    _bottomWindow.dBTableWidget_4->setItem(0,1,new QTableWidgetItem("油样编号"));
    _bottomWindow.dBTableWidget_4->setItem(0,2,new QTableWidgetItem("采样单位编号"));
    _bottomWindow.dBTableWidget_4->setItem(0,3,new QTableWidgetItem("机型"));
    _bottomWindow.dBTableWidget_4->setItem(0,4,new QTableWidgetItem("机号"));
    _bottomWindow.dBTableWidget_4->setItem(0,5,new QTableWidgetItem("监控部件名称"));
    _bottomWindow.dBTableWidget_4->setItem(0,6,new QTableWidgetItem("监控部件编号"));
    _bottomWindow.dBTableWidget_4->setItem(0,7,new QTableWidgetItem("采样点编号"));
    _bottomWindow.dBTableWidget_4->setItem(0,8,new QTableWidgetItem("滑油工作时数"));
    _bottomWindow.dBTableWidget_4->setItem(0,9,new QTableWidgetItem("滑油添加量"));
    _bottomWindow.dBTableWidget_4->setItem(0,10,new QTableWidgetItem("采样原因"));
    _bottomWindow.dBTableWidget_4->setItem(0,11,new QTableWidgetItem("采样单位"));
    _bottomWindow.dBTableWidget_4->setItem(0,12,new QTableWidgetItem("采样人"));
    _bottomWindow.dBTableWidget_4->setItem(0,13,new QTableWidgetItem("采样日期"));
    _bottomWindow.dBTableWidget_4->setItem(0,14,new QTableWidgetItem("采样时间"));
    _bottomWindow.dBTableWidget_4->setItem(0,15,new QTableWidgetItem("采样时机"));
    _bottomWindow.dBTableWidget_4->setItem(0,16,new QTableWidgetItem("采样方式"));
    _bottomWindow.dBTableWidget_4->setItem(0,17,new QTableWidgetItem("采样量"));
    _bottomWindow.dBTableWidget_4->setItem(0,18,new QTableWidgetItem("采样说明"));
    _bottomWindow.dBTableWidget_4->setItem(0,19,new QTableWidgetItem("送样人"));
    _bottomWindow.dBTableWidget_4->setItem(0,20,new QTableWidgetItem("送样日期"));
    _bottomWindow.dBTableWidget_4->setItem(0,21,new QTableWidgetItem("送样时间"));
    _bottomWindow.dBTableWidget_4->setItem(0,0,new QTableWidgetItem("序号"));
    _bottomWindow.dBTableWidget_4->setItem(0,1,new QTableWidgetItem("油样编号"));
    _bottomWindow.dBTableWidget_4->setItem(0,2,new QTableWidgetItem("采样单位编号"));
    _bottomWindow.dBTableWidget_4->setItem(0,3,new QTableWidgetItem("机型"));
    _bottomWindow.dBTableWidget_4->setItem(0,4,new QTableWidgetItem("机号"));
    _bottomWindow.dBTableWidget_4->setItem(0,5,new QTableWidgetItem("监控部件名称"));
    _bottomWindow.dBTableWidget_4->setItem(0,6,new QTableWidgetItem("监控部件编号"));
    _bottomWindow.dBTableWidget_4->setItem(0,7,new QTableWidgetItem("采样点编号"));
    _bottomWindow.dBTableWidget_4->setItem(0,8,new QTableWidgetItem("滑油工作时数"));
    _bottomWindow.dBTableWidget_4->setItem(0,9,new QTableWidgetItem("滑油添加量"));
    _bottomWindow.dBTableWidget_4->setItem(0,10,new QTableWidgetItem("采样原因"));
    _bottomWindow.dBTableWidget_4->setItem(0,11,new QTableWidgetItem("采样单位"));
    _bottomWindow.dBTableWidget_4->setItem(0,12,new QTableWidgetItem("采样人"));
    _bottomWindow.dBTableWidget_4->setItem(0,13,new QTableWidgetItem("采样日期"));
    _bottomWindow.dBTableWidget_4->setItem(0,14,new QTableWidgetItem("采样时间"));
    _bottomWindow.dBTableWidget_4->setItem(0,15,new QTableWidgetItem("采样时机"));
    _bottomWindow.dBTableWidget_4->setItem(0,16,new QTableWidgetItem("采样方式"));
    _bottomWindow.dBTableWidget_4->setItem(0,17,new QTableWidgetItem("采样量"));
    _bottomWindow.dBTableWidget_4->setItem(0,18,new QTableWidgetItem("采样说明"));
    _bottomWindow.dBTableWidget_4->setItem(0,19,new QTableWidgetItem("送样人"));
    _bottomWindow.dBTableWidget_4->setItem(0,20,new QTableWidgetItem("送样日期"));
    _bottomWindow.dBTableWidget_4->setItem(0,21,new QTableWidgetItem("送样时间"));
    _bottomWindow.dBTableWidget_4->setItem(0,0,new QTableWidgetItem("序号"));
    _bottomWindow.dBTableWidget_4->setItem(0,1,new QTableWidgetItem("油样编号"));
    _bottomWindow.dBTableWidget_4->setItem(0,2,new QTableWidgetItem("采样单位编号"));
    _bottomWindow.dBTableWidget_4->setItem(0,3,new QTableWidgetItem("机型"));
    _bottomWindow.dBTableWidget_4->setItem(0,4,new QTableWidgetItem("机号"));
    _bottomWindow.dBTableWidget_4->setItem(0,5,new QTableWidgetItem("监控部件名称"));
    _bottomWindow.dBTableWidget_4->setItem(0,6,new QTableWidgetItem("监控部件编号"));
    _bottomWindow.dBTableWidget_4->setItem(0,7,new QTableWidgetItem("采样点编号"));
    _bottomWindow.dBTableWidget_4->setItem(0,8,new QTableWidgetItem("滑油工作时数"));
    _bottomWindow.dBTableWidget_4->setItem(0,9,new QTableWidgetItem("滑油添加量"));
    _bottomWindow.dBTableWidget_4->setItem(0,10,new QTableWidgetItem("采样原因"));
    _bottomWindow.dBTableWidget_4->setItem(0,11,new QTableWidgetItem("采样单位"));
    _bottomWindow.dBTableWidget_4->setItem(0,12,new QTableWidgetItem("采样人"));
    _bottomWindow.dBTableWidget_4->setItem(0,13,new QTableWidgetItem("采样日期"));
    _bottomWindow.dBTableWidget_4->setItem(0,14,new QTableWidgetItem("采样时间"));
    _bottomWindow.dBTableWidget_4->setItem(0,15,new QTableWidgetItem("采样时机"));
    _bottomWindow.dBTableWidget_4->setItem(0,16,new QTableWidgetItem("采样方式"));
    _bottomWindow.dBTableWidget_4->setItem(0,17,new QTableWidgetItem("采样量"));
    _bottomWindow.dBTableWidget_4->setItem(0,18,new QTableWidgetItem("采样说明"));
    _bottomWindow.dBTableWidget_4->setItem(0,19,new QTableWidgetItem("送样人"));
    _bottomWindow.dBTableWidget_4->setItem(0,20,new QTableWidgetItem("送样日期"));
    _bottomWindow.dBTableWidget_4->setItem(0,21,new QTableWidgetItem("送样时间"));

    _bottomWindow.dBTableWidget_5->setItem(0,0,new QTableWidgetItem("序号"));
    _bottomWindow.dBTableWidget_5->setItem(0,1,new QTableWidgetItem("油样编号"));
    _bottomWindow.dBTableWidget_5->setItem(0,2,new QTableWidgetItem("检测分析单位名称"));
    _bottomWindow.dBTableWidget_5->setItem(0,3,new QTableWidgetItem("送检单位"));
    _bottomWindow.dBTableWidget_5->setItem(0,4,new QTableWidgetItem("送检原因"));
    _bottomWindow.dBTableWidget_5->setItem(0,5,new QTableWidgetItem("送样人"));
    _bottomWindow.dBTableWidget_5->setItem(0,6,new QTableWidgetItem("收油样日期"));
    _bottomWindow.dBTableWidget_5->setItem(0,7,new QTableWidgetItem("收油样人"));
    _bottomWindow.dBTableWidget_5->setItem(0,8,new QTableWidgetItem("污染度分析方法"));
    _bottomWindow.dBTableWidget_5->setItem(0,9,new QTableWidgetItem("污染度分析人"));
    _bottomWindow.dBTableWidget_5->setItem(0,10,new QTableWidgetItem("污染度分析日期"));
    _bottomWindow.dBTableWidget_5->setItem(0,11,new QTableWidgetItem("污染度分析设备"));
    _bottomWindow.dBTableWidget_5->setItem(0,12,new QTableWidgetItem("污染度分析报告编号"));
    _bottomWindow.dBTableWidget_5->setItem(0,13,new QTableWidgetItem("光谱分析方法"));
    _bottomWindow.dBTableWidget_5->setItem(0,14,new QTableWidgetItem("光谱分析人"));
    _bottomWindow.dBTableWidget_5->setItem(0,15,new QTableWidgetItem("光谱分析日期"));
    _bottomWindow.dBTableWidget_5->setItem(0,16,new QTableWidgetItem("光谱分析设备"));
    _bottomWindow.dBTableWidget_5->setItem(0,17,new QTableWidgetItem("光谱分析报告编号"));
    _bottomWindow.dBTableWidget_5->setItem(0,18,new QTableWidgetItem("铁谱分析方法"));
    _bottomWindow.dBTableWidget_5->setItem(0,19,new QTableWidgetItem("铁谱分析人"));
    _bottomWindow.dBTableWidget_5->setItem(0,20,new QTableWidgetItem("铁谱分析日期"));
    _bottomWindow.dBTableWidget_5->setItem(0,21,new QTableWidgetItem("铁谱分析设备"));
    _bottomWindow.dBTableWidget_5->setItem(0,22,new QTableWidgetItem("铁谱分析报告编号"));
    _bottomWindow.dBTableWidget_5->setItem(0,23,new QTableWidgetItem("理化分析方法"));
    _bottomWindow.dBTableWidget_5->setItem(0,24,new QTableWidgetItem("理化分析人"));
    _bottomWindow.dBTableWidget_5->setItem(0,25,new QTableWidgetItem("理化分析日期"));
    _bottomWindow.dBTableWidget_5->setItem(0,26,new QTableWidgetItem("理化分析设备"));
    _bottomWindow.dBTableWidget_5->setItem(0,27,new QTableWidgetItem("理化分析报告编号"));

    _bottomWindow.dBTableWidget_6->setItem(0,0,new QTableWidgetItem("序号"));
    _bottomWindow.dBTableWidget_6->setItem(0,1,new QTableWidgetItem("铁谱片编号"));
    _bottomWindow.dBTableWidget_6->setItem(0,2,new QTableWidgetItem("铁谱分析报告编号"));
    _bottomWindow.dBTableWidget_6->setItem(0,3,new QTableWidgetItem("油样编号"));
    _bottomWindow.dBTableWidget_6->setItem(0,4,new QTableWidgetItem("分析铁谱仪型号"));
    _bottomWindow.dBTableWidget_6->setItem(0,5,new QTableWidgetItem("铁谱片制取油样消耗量"));
    _bottomWindow.dBTableWidget_6->setItem(0,6,new QTableWidgetItem("铁谱片制取方法"));
    _bottomWindow.dBTableWidget_6->setItem(0,7,new QTableWidgetItem("铁谱片制取人"));

    _bottomWindow.dBTableWidget_7->setItem(0,0,new QTableWidgetItem("序号"));
    _bottomWindow.dBTableWidget_7->setItem(0,1,new QTableWidgetItem("铁谱图片编号"));
    _bottomWindow.dBTableWidget_7->setItem(0,2,new QTableWidgetItem("铁谱片编号"));
    _bottomWindow.dBTableWidget_7->setItem(0,3,new QTableWidgetItem("铁谱分析报告编号"));
    _bottomWindow.dBTableWidget_7->setItem(0,4,new QTableWidgetItem("显微镜型号"));
    _bottomWindow.dBTableWidget_7->setItem(0,5,new QTableWidgetItem("图像采集器型号"));
    _bottomWindow.dBTableWidget_7->setItem(0,6,new QTableWidgetItem("光源类型"));
    _bottomWindow.dBTableWidget_7->setItem(0,7,new QTableWidgetItem("放大倍数"));
    _bottomWindow.dBTableWidget_7->setItem(0,8,new QTableWidgetItem("铁谱图片采集区域"));
    _bottomWindow.dBTableWidget_7->setItem(0,9,new QTableWidgetItem("铁谱图片采集人"));
    _bottomWindow.dBTableWidget_7->setItem(0,10,new QTableWidgetItem("铁谱图片(路径)"));
    _bottomWindow.dBTableWidget_7->setItem(0,11,new QTableWidgetItem("铁谱图片识别分析信息"));
    _bottomWindow.dBTableWidget_7->setItem(0,12,new QTableWidgetItem("铁谱图片标识符号"));

    _bottomWindow.dBTableWidget_8->setItem(0,0,new QTableWidgetItem("序号"));
    _bottomWindow.dBTableWidget_8->setItem(0,1,new QTableWidgetItem("磨粒编号"));
    _bottomWindow.dBTableWidget_8->setItem(0,2,new QTableWidgetItem("铁谱图片编号"));
    _bottomWindow.dBTableWidget_8->setItem(0,3,new QTableWidgetItem("铁谱片编号"));
    _bottomWindow.dBTableWidget_8->setItem(0,4,new QTableWidgetItem("铁谱分析报告编号"));
    _bottomWindow.dBTableWidget_8->setItem(0,5,new QTableWidgetItem("磨粒标注人"));
    _bottomWindow.dBTableWidget_8->setItem(0,6,new QTableWidgetItem("磨粒图片（路径）"));
    _bottomWindow.dBTableWidget_8->setItem(0,7,new QTableWidgetItem("磨粒材质"));
    _bottomWindow.dBTableWidget_8->setItem(0,8,new QTableWidgetItem("磨粒位置"));
    _bottomWindow.dBTableWidget_8->setItem(0,9,new QTableWidgetItem("磨粒尺寸"));
    _bottomWindow.dBTableWidget_8->setItem(0,10,new QTableWidgetItem("磨粒周长"));
    _bottomWindow.dBTableWidget_8->setItem(0,11,new QTableWidgetItem("磨粒形状"));
    _bottomWindow.dBTableWidget_8->setItem(0,12,new QTableWidgetItem("磨粒颜色"));
    _bottomWindow.dBTableWidget_8->setItem(0,13,new QTableWidgetItem("磨粒表面纹理类型"));
    _bottomWindow.dBTableWidget_8->setItem(0,14,new QTableWidgetItem("磨粒磨损类型"));
    _bottomWindow.dBTableWidget_8->setItem(0,15,new QTableWidgetItem("磨粒损伤部位"));
    _bottomWindow.dBTableWidget_8->setItem(0,16,new QTableWidgetItem("磨粒磨损机理"));
    _bottomWindow.dBTableWidget_8->setItem(0,17,new QTableWidgetItem("磨粒反映故障信息"));
    _bottomWindow.dBTableWidget_8->setItem(0,18,new QTableWidgetItem("磨粒典型性"));*/

    close();

    QSettings settings("ImageCompletion", "ImageCompletion");
    QString initialPath = settings.value("lastImportPath", QVariant(QDir::homePath())).toString();
    if(initialPath.isEmpty())
    {
        initialPath = QDir::homePath() + "/untitled";
    }

    QString fileName;
    fileName = QFileDialog::getOpenFileName( this, tr("打开图像"), initialPath, tr("图像 (*.png *.bmp *.jpg *.jpeg)") );
    if(fileName.isEmpty()) return;

    _imagePath = fileName;
    _imageName = QFileInfo(fileName).fileName();
    settings.setValue("lastImportPath", QVariant(fileName));

    if ( _editImageViewer->openImage(fileName) )
    {
        statusBar()->showMessage(tr("打开图像"), 2000);
        _editImageViewer->repaint();
        _step = NONE;
        updateLog();
    }
    else
    {
        _editImageViewer->close();
        _step = LOADFAILED;
        _regionCompetitionDialog.radioForeground->setEnabled(false);
        _regionCompetitionDialog.radioBackground->setEnabled(false);
        _regionCompetitionDialog.radioErazer->setEnabled(false);
        return;
    }

    int width = _editImageViewer->image().width();
    int height = _editImageViewer->image().height();
    this->setMinimumSize( width < 800 ? 800 : height, height < 600 ? 600 : height );


    QString status = this->labelStatus(fileName);
    if(status != "N" && status != "Y")
    {
        QMessageBox::StandardButton reply = QMessageBox::information(0, tr("提示"), "此图像为新图像,是否要导入数据库?", QMessageBox::Ok | QMessageBox::Cancel);
        if(reply == QMessageBox::Ok)
        {
            (new ImageProperties)->showDlg(fileName);
        }
    }

    /* QSqlDatabase db;
    if(!createConnection(db))
    {
        QMessageBox::critical(0, qApp->tr("打开数据库失败"),
                              qApp->tr("无法创建数据库连接"),
                              QMessageBox::Cancel);
    }

    QSqlQuery query;

    QString SQL2="select equipmentinfo.*,movepartinfo.*,movepartrepairinfo.*,oilsampleinfo.*,oilanalyzeinfo.*,ferrographyinfo.*,ferrographypicinfo.*,abrasivemarkinfo.* from equipmentinfo,movepartinfo,movepartrepairinfo,oilsampleinfo,oilanalyzeinfo,ferrographyinfo,ferrographypicinfo,abrasivemarkinfo where equipmentinfo.planeid=movepartinfo.planeid and movepartinfo.movepartid=movepartrepairinfo.movepartid and movepartinfo.movepartid=oilsampleinfo.monitorpartid and oilsampleinfo.oilsampleid=oilanalyzeinfo.oilsampleid and oilsampleinfo.oilsampleid=ferrographyinfo.oilsampleid and ferrographyinfo.ferrographysheetid=ferrographypicinfo.ferrographysheetid and ferrographypicinfo.ferrographypicid=abrasivemarkinfo.ferrographypicid and ferrographypicinfo.ferrographypicpath=?";
    query.prepare(SQL2);
    query.addBindValue(fileName);


    if(query.exec())
    {
    }

    while(query.next())
    {
        _bottomWindow.dBTableWidget_1->setItem(1,0,new QTableWidgetItem(query.value(0).toString()));
        _bottomWindow.dBTableWidget_1->setItem(1,1,new QTableWidgetItem(query.value(1).toString()));
        _bottomWindow.dBTableWidget_1->setItem(1,2,new QTableWidgetItem(query.value(2).toString()));
        _bottomWindow.dBTableWidget_1->setItem(1,3,new QTableWidgetItem(query.value(3).toString()));
        _bottomWindow.dBTableWidget_1->setItem(1,4,new QTableWidgetItem(query.value(4).toString()));
        _bottomWindow.dBTableWidget_1->setItem(1,5,new QTableWidgetItem(query.value(5).toString()));
        _bottomWindow.dBTableWidget_1->setItem(1,6,new QTableWidgetItem(query.value(6).toString()));

        _bottomWindow.dBTableWidget_2->setItem(1,0,new QTableWidgetItem(query.value(7).toString()));
        _bottomWindow.dBTableWidget_2->setItem(1,1,new QTableWidgetItem(query.value(8).toString()));
        _bottomWindow.dBTableWidget_2->setItem(1,2,new QTableWidgetItem(query.value(9).toString()));
        _bottomWindow.dBTableWidget_2->setItem(1,3,new QTableWidgetItem(query.value(10).toString()));
        _bottomWindow.dBTableWidget_2->setItem(1,4,new QTableWidgetItem(query.value(11).toString()));
        _bottomWindow.dBTableWidget_2->setItem(1,5,new QTableWidgetItem(query.value(12).toString()));
        _bottomWindow.dBTableWidget_2->setItem(1,6,new QTableWidgetItem(query.value(13).toString()));
        _bottomWindow.dBTableWidget_2->setItem(1,7,new QTableWidgetItem(query.value(14).toString()));
        _bottomWindow.dBTableWidget_2->setItem(1,8,new QTableWidgetItem(query.value(15).toString()));
        _bottomWindow.dBTableWidget_2->setItem(1,9,new QTableWidgetItem(query.value(16).toString()));

        _bottomWindow.dBTableWidget_3->setItem(1,0,new QTableWidgetItem(query.value(17).toString()));
        _bottomWindow.dBTableWidget_3->setItem(1,1,new QTableWidgetItem(query.value(18).toString()));
        _bottomWindow.dBTableWidget_3->setItem(1,2,new QTableWidgetItem(query.value(19).toString()));
        _bottomWindow.dBTableWidget_3->setItem(1,3,new QTableWidgetItem(query.value(20).toString()));
        _bottomWindow.dBTableWidget_3->setItem(1,4,new QTableWidgetItem(query.value(21).toString()));
        _bottomWindow.dBTableWidget_3->setItem(1,5,new QTableWidgetItem(query.value(22).toString()));
        _bottomWindow.dBTableWidget_3->setItem(1,6,new QTableWidgetItem(query.value(23).toString()));
        _bottomWindow.dBTableWidget_3->setItem(1,7,new QTableWidgetItem(query.value(24).toString()));
        _bottomWindow.dBTableWidget_3->setItem(1,8,new QTableWidgetItem(query.value(25).toString()));
        _bottomWindow.dBTableWidget_3->setItem(1,9,new QTableWidgetItem(query.value(26).toString()));

        _bottomWindow.dBTableWidget_4->setItem(1,0,new QTableWidgetItem(query.value(27).toString()));
        _bottomWindow.dBTableWidget_4->setItem(1,1,new QTableWidgetItem(query.value(28).toString()));
        _bottomWindow.dBTableWidget_4->setItem(1,2,new QTableWidgetItem(query.value(29).toString()));
        _bottomWindow.dBTableWidget_4->setItem(1,3,new QTableWidgetItem(query.value(30).toString()));
        _bottomWindow.dBTableWidget_4->setItem(1,4,new QTableWidgetItem(query.value(31).toString()));
        _bottomWindow.dBTableWidget_4->setItem(1,5,new QTableWidgetItem(query.value(32).toString()));
        _bottomWindow.dBTableWidget_4->setItem(1,6,new QTableWidgetItem(query.value(33).toString()));
        _bottomWindow.dBTableWidget_4->setItem(1,7,new QTableWidgetItem(query.value(34).toString()));
        _bottomWindow.dBTableWidget_4->setItem(1,8,new QTableWidgetItem(query.value(35).toString()));
        _bottomWindow.dBTableWidget_4->setItem(1,9,new QTableWidgetItem(query.value(36).toString()));
        _bottomWindow.dBTableWidget_4->setItem(1,10,new QTableWidgetItem(query.value(37).toString()));
        _bottomWindow.dBTableWidget_4->setItem(1,11,new QTableWidgetItem(query.value(38).toString()));
        _bottomWindow.dBTableWidget_4->setItem(1,12,new QTableWidgetItem(query.value(39).toString()));
        _bottomWindow.dBTableWidget_4->setItem(1,13,new QTableWidgetItem(query.value(40).toString()));
        _bottomWindow.dBTableWidget_4->setItem(1,14,new QTableWidgetItem(query.value(41).toString()));
        _bottomWindow.dBTableWidget_4->setItem(1,15,new QTableWidgetItem(query.value(42).toString()));
        _bottomWindow.dBTableWidget_4->setItem(1,16,new QTableWidgetItem(query.value(43).toString()));
        _bottomWindow.dBTableWidget_4->setItem(1,17,new QTableWidgetItem(query.value(44).toString()));
        _bottomWindow.dBTableWidget_4->setItem(1,18,new QTableWidgetItem(query.value(45).toString()));
        _bottomWindow.dBTableWidget_4->setItem(1,19,new QTableWidgetItem(query.value(46).toString()));
        _bottomWindow.dBTableWidget_4->setItem(1,20,new QTableWidgetItem(query.value(47).toString()));
        _bottomWindow.dBTableWidget_4->setItem(1,21,new QTableWidgetItem(query.value(48).toString()));

        _bottomWindow.dBTableWidget_5->setItem(1,0,new QTableWidgetItem(query.value(49).toString()));
        _bottomWindow.dBTableWidget_5->setItem(1,1,new QTableWidgetItem(query.value(50).toString()));
        _bottomWindow.dBTableWidget_5->setItem(1,2,new QTableWidgetItem(query.value(51).toString()));
        _bottomWindow.dBTableWidget_5->setItem(1,3,new QTableWidgetItem(query.value(52).toString()));
        _bottomWindow.dBTableWidget_5->setItem(1,4,new QTableWidgetItem(query.value(53).toString()));
        _bottomWindow.dBTableWidget_5->setItem(1,5,new QTableWidgetItem(query.value(54).toString()));
        _bottomWindow.dBTableWidget_5->setItem(1,6,new QTableWidgetItem(query.value(55).toString()));
        _bottomWindow.dBTableWidget_5->setItem(1,7,new QTableWidgetItem(query.value(56).toString()));
        _bottomWindow.dBTableWidget_5->setItem(1,8,new QTableWidgetItem(query.value(57).toString()));
        _bottomWindow.dBTableWidget_5->setItem(1,9,new QTableWidgetItem(query.value(58).toString()));
        _bottomWindow.dBTableWidget_5->setItem(1,10,new QTableWidgetItem(query.value(59).toString()));
        _bottomWindow.dBTableWidget_5->setItem(1,11,new QTableWidgetItem(query.value(60).toString()));
        _bottomWindow.dBTableWidget_5->setItem(1,12,new QTableWidgetItem(query.value(61).toString()));
        _bottomWindow.dBTableWidget_5->setItem(1,13,new QTableWidgetItem(query.value(62).toString()));
        _bottomWindow.dBTableWidget_5->setItem(1,14,new QTableWidgetItem(query.value(63).toString()));
        _bottomWindow.dBTableWidget_5->setItem(1,15,new QTableWidgetItem(query.value(64).toString()));
        _bottomWindow.dBTableWidget_5->setItem(1,16,new QTableWidgetItem(query.value(65).toString()));
        _bottomWindow.dBTableWidget_5->setItem(1,17,new QTableWidgetItem(query.value(66).toString()));
        _bottomWindow.dBTableWidget_5->setItem(1,18,new QTableWidgetItem(query.value(67).toString()));
        _bottomWindow.dBTableWidget_5->setItem(1,19,new QTableWidgetItem(query.value(68).toString()));
        _bottomWindow.dBTableWidget_5->setItem(1,20,new QTableWidgetItem(query.value(69).toString()));
        _bottomWindow.dBTableWidget_5->setItem(1,21,new QTableWidgetItem(query.value(70).toString()));
        _bottomWindow.dBTableWidget_5->setItem(1,22,new QTableWidgetItem(query.value(71).toString()));
        _bottomWindow.dBTableWidget_5->setItem(1,23,new QTableWidgetItem(query.value(72).toString()));
        _bottomWindow.dBTableWidget_5->setItem(1,24,new QTableWidgetItem(query.value(73).toString()));
        _bottomWindow.dBTableWidget_5->setItem(1,25,new QTableWidgetItem(query.value(74).toString()));
        _bottomWindow.dBTableWidget_5->setItem(1,26,new QTableWidgetItem(query.value(75).toString()));
        _bottomWindow.dBTableWidget_5->setItem(1,27,new QTableWidgetItem(query.value(76).toString()));

        _bottomWindow.dBTableWidget_6->setItem(1,0,new QTableWidgetItem(query.value(77).toString()));
        _bottomWindow.dBTableWidget_6->setItem(1,1,new QTableWidgetItem(query.value(78).toString()));
        _bottomWindow.dBTableWidget_6->setItem(1,2,new QTableWidgetItem(query.value(79).toString()));
        _bottomWindow.dBTableWidget_6->setItem(1,3,new QTableWidgetItem(query.value(80).toString()));
        _bottomWindow.dBTableWidget_6->setItem(1,4,new QTableWidgetItem(query.value(81).toString()));
        _bottomWindow.dBTableWidget_6->setItem(1,5,new QTableWidgetItem(query.value(82).toString()));
        _bottomWindow.dBTableWidget_6->setItem(1,6,new QTableWidgetItem(query.value(83).toString()));
        _bottomWindow.dBTableWidget_6->setItem(1,7,new QTableWidgetItem(query.value(84).toString()));

        _bottomWindow.dBTableWidget_7->setItem(1,0,new QTableWidgetItem(query.value(85).toString()));
        _bottomWindow.dBTableWidget_7->setItem(1,1,new QTableWidgetItem(query.value(86).toString()));
        _bottomWindow.dBTableWidget_7->setItem(1,2,new QTableWidgetItem(query.value(87).toString()));
        _bottomWindow.dBTableWidget_7->setItem(1,3,new QTableWidgetItem(query.value(88).toString()));
        _bottomWindow.dBTableWidget_7->setItem(1,4,new QTableWidgetItem(query.value(89).toString()));
        _bottomWindow.dBTableWidget_7->setItem(1,5,new QTableWidgetItem(query.value(90).toString()));
        _bottomWindow.dBTableWidget_7->setItem(1,6,new QTableWidgetItem(query.value(91).toString()));
        _bottomWindow.dBTableWidget_7->setItem(1,7,new QTableWidgetItem(query.value(92).toString()));
        _bottomWindow.dBTableWidget_7->setItem(1,8,new QTableWidgetItem(query.value(93).toString()));
        _bottomWindow.dBTableWidget_7->setItem(1,9,new QTableWidgetItem(query.value(94).toString()));
        _bottomWindow.dBTableWidget_7->setItem(1,10,new QTableWidgetItem(query.value(95).toString()));
        _bottomWindow.dBTableWidget_7->setItem(1,11,new QTableWidgetItem(query.value(96).toString()));
        _bottomWindow.dBTableWidget_7->setItem(1,12,new QTableWidgetItem(query.value(97).toString()));

        _bottomWindow.dBTableWidget_8->setItem(1,0,new QTableWidgetItem(query.value(98).toString()));
        _bottomWindow.dBTableWidget_8->setItem(1,1,new QTableWidgetItem(query.value(99).toString()));
        _bottomWindow.dBTableWidget_8->setItem(1,2,new QTableWidgetItem(query.value(100).toString()));
        _bottomWindow.dBTableWidget_8->setItem(1,3,new QTableWidgetItem(query.value(101).toString()));
        _bottomWindow.dBTableWidget_8->setItem(1,4,new QTableWidgetItem(query.value(102).toString()));
        _bottomWindow.dBTableWidget_8->setItem(1,5,new QTableWidgetItem(query.value(103).toString()));
        _bottomWindow.dBTableWidget_8->setItem(1,6,new QTableWidgetItem(query.value(104).toString()));
        _bottomWindow.dBTableWidget_8->setItem(1,7,new QTableWidgetItem(query.value(105).toString()));
        _bottomWindow.dBTableWidget_8->setItem(1,8,new QTableWidgetItem(query.value(106).toString()));
        _bottomWindow.dBTableWidget_8->setItem(1,9,new QTableWidgetItem(query.value(107).toString()));
        _bottomWindow.dBTableWidget_8->setItem(1,10,new QTableWidgetItem(query.value(108).toString()));
        _bottomWindow.dBTableWidget_8->setItem(1,11,new QTableWidgetItem(query.value(109).toString()));
        _bottomWindow.dBTableWidget_8->setItem(1,12,new QTableWidgetItem(query.value(110).toString()));
        _bottomWindow.dBTableWidget_8->setItem(1,13,new QTableWidgetItem(query.value(111).toString()));
        _bottomWindow.dBTableWidget_8->setItem(1,14,new QTableWidgetItem(query.value(112).toString()));
        _bottomWindow.dBTableWidget_8->setItem(1,15,new QTableWidgetItem(query.value(113).toString()));
        _bottomWindow.dBTableWidget_8->setItem(1,16,new QTableWidgetItem(query.value(114).toString()));
        _bottomWindow.dBTableWidget_8->setItem(1,17,new QTableWidgetItem(query.value(115).toString()));
        _bottomWindow.dBTableWidget_8->setItem(1,18,new QTableWidgetItem(query.value(116).toString()));


        _leftWindow.tableWidget->setItem(_cnt, 1, new QTableWidgetItem(query.value(95).toString()));
        _leftWindow.tableWidget->setItem(_cnt, 2, new QTableWidgetItem(query.value(97).toString()));
    }*/

    _cnt++;
    if(_leftWindow.tableWidget->rowCount() <= _cnt+1)
    {
        _leftWindow.tableWidget->insertRow(_leftWindow.tableWidget->rowCount()-1);
    }

    _leftWindow.tableWidget->setItem(_cnt, 0, new QTableWidgetItem(_imageName));
    _leftWindow.tableWidget->setItem(_cnt, 1, new QTableWidgetItem(fileName));
    _leftWindow.tableWidget->setItem(_cnt, 2, new QTableWidgetItem(status == "Y" ? "Y" : "N"));
    QColor color = getColor(status);
    for(int i = 0; i < _leftWindow.tableWidget->columnCount(); i++)
    {
        _leftWindow.tableWidget->item(_cnt, i)->setBackgroundColor(color);
    }
}

void ImageCompletionUI::batchOpen()
{
    QFileDialog batchFileDialog;
    batchFileDialog.setFileMode(QFileDialog::Directory);
    batchFileDialog.setOption(QFileDialog::ShowDirsOnly);
    batchFileDialog.setViewMode(QFileDialog::Detail);

    QDir dir;

    if(batchFileDialog.exec())
    {
        dir = batchFileDialog.selectedFiles()[0];
        dir.setNameFilters(QStringList() << "*.jpg" << "*.png" << "*.bmp" << "*.jpeg");
        QStringList fileList = dir.entryList(QDir::Files);
        QString absolutePath = dir.absolutePath();

        foreach(QString file, fileList)
        {
            _cnt++;

            if(_leftWindow.tableWidget->rowCount() <= _cnt+1)
            {
                _leftWindow.tableWidget->insertRow(_leftWindow.tableWidget->rowCount()-1);
            }

            QString status = this->labelStatus(absolutePath);

            _leftWindow.tableWidget->setItem(_cnt, 0, new QTableWidgetItem(file));
            _leftWindow.tableWidget->setItem(_cnt, 1, new QTableWidgetItem(absolutePath+"/"+file));
            _leftWindow.tableWidget->setItem(_cnt, 2, new QTableWidgetItem(status == "Y" ? "Y" : "N"));
            QColor color = getColor(status);
            for(int i = 0; i < _leftWindow.tableWidget->columnCount(); i++)
            {
                _leftWindow.tableWidget->item(_cnt, i)->setBackgroundColor(color);
            }
        }
    }
}

void	ImageCompletionUI::save()
{
    if ( _imageName.isEmpty() ) return;

    bool ret1, ret2;

    // Check Path
    if(!QDir(Global::PathResult).exists())
    {
        QMessageBox::warning(this, tr("保存"), QString("请指定标注图像保存路径:%1").arg(QFileInfo(QApplication::instance()->applicationFilePath()).baseName() + ".ini"));
    }
    if(!QDir(Global::PathMask).exists())
    {
        QMessageBox::warning(this, tr("保存"), QString("请指定掩码图像保存路径:%1").arg(QFileInfo(QApplication::instance()->applicationFilePath()).baseName() + ".ini"));
    }

    // Save
    QString pathResult = Global::PathResult.append(QFileInfo(_imageName).baseName());
    if(!(ret1 = _editImageViewer->saveLabelledResult(pathResult, Global::ExtResult)))
    {
        QMessageBox::warning(this, tr("保存"), tr("保存标注图像失败"), QMessageBox::Close);
    }

    QString pathMask = Global::PathMask.append(QFileInfo(_imageName).baseName());
    if(!(ret2 = _editImageViewer->saveMask(pathMask, Global::ExtMask)))
    {
        QMessageBox::warning(this, tr("保存"), tr("保存掩码图像失败"), QMessageBox::Close);
    }

    //TODO: Sync Database
    if(ret1 && ret2)
        syncLabelledImage(_imagePath, pathResult.append(".").append(Global::ExtResult),pathMask.append(".").append(Global::ExtMask));

    showData();
}

void	ImageCompletionUI::saveAs()
{
    if(_imageName.isEmpty()) return;

    QString pathResult, pathMask;
    bool ret1, ret2;

    if(!(ret1 = _editImageViewer->saveAsLabelledResult(pathResult)))
    {
        QMessageBox::warning(this, tr("保存"), tr("保存标注图像失败"), QMessageBox::Close);
    }
    else if(!(ret2 = _editImageViewer->saveAsMask(pathMask)))
    {
        QMessageBox::warning(this, tr("保存"), tr("保存掩码图像失败"), QMessageBox::Close);
    }

    //TODO: Sync Database
    if(ret1 && ret2) syncLabelledImage(_imagePath, pathResult, pathMask);

    showData();
}

void	ImageCompletionUI::close()
{
    _editImageViewer->deleteImage();
    QPixmap pixmap(0, 0);
    _regionCompetitionDialog.showColorLabel->setPixmap(pixmap);
    setupBrush();
    _regionCompetitionDialog.radioForeground->setEnabled(false);
    _regionCompetitionDialog.radioBackground->setEnabled(false);
    _regionCompetitionDialog.radioErazer->setEnabled(false);
    _regionCompetitionDialog.radioForeground->setChecked(false);
    _regionCompetitionDialog.radioBackground->setChecked(false);
    _regionCompetitionDialog.radioErazer->setChecked(false);

    _imageName = "";
    _imagePath = "";

    uncheckMethods();

    _editImageViewer->setDefaultCursor();
}

void	ImageCompletionUI::search()
{
    //   this->advanceSearchDlg = new advanceSearchDlg(this);
    //   this->advanceSearchDlg->show();
    //    this->searchdata1 = new Searchdata();
    //    searchdata1->show();
    //    searchdata1.show();
    advanceSearchDlg = new AdvanceSearchDlg(this);
    advanceSearchDlg->show();
}

void	ImageCompletionUI::addtosql()
{
    class1.show();

}

void	ImageCompletionUI::savemarkresult()
{
    //class1.show();
    QSqlDatabase db;
    createConnection(db);//连接数据库
    QSqlQuery query1;
    QSqlQuery query2;
    QSqlQuery query3;
    QSqlQuery query4;
    QSqlQuery query5;
    QSqlQuery query6;
    QSqlQuery query7;
    QSqlQuery query8;

    //update equipmentinfo
    query1.prepare("update equipmentinfo set planeid=?,planetype=?,departid=?,runhour=?,runstage=?,repairtime=? where id=?");//对数据库中Tuser中对应uid的一条记录进行修改
    query1.addBindValue(_bottomWindow.dBTableWidget_1->item(1,1)->data(Qt::DisplayRole).toString());//对其SQL语句中的每一项进行幅值操作
    query1.addBindValue(_bottomWindow.dBTableWidget_1->item(1,2)->data(Qt::DisplayRole).toString());
    query1.addBindValue(_bottomWindow.dBTableWidget_1->item(1,3)->data(Qt::DisplayRole).toString());
    query1.addBindValue(_bottomWindow.dBTableWidget_1->item(1,4)->data(Qt::DisplayRole).toString());
    query1.addBindValue(_bottomWindow.dBTableWidget_1->item(1,5)->data(Qt::DisplayRole).toString());
    query1.addBindValue(_bottomWindow.dBTableWidget_1->item(1,6)->data(Qt::DisplayRole).toString());
    query1.addBindValue(_bottomWindow.dBTableWidget_1->item(1,0)->data(Qt::DisplayRole).toString());
    if(!query1.exec())//若SQL语句没有执行，则提示错误
        QMessageBox::warning(this,tr("failed1"),tr("Modify error"),QMessageBox::Close);
    else{
        QMessageBox::warning(this,tr("success1"),tr("Modify success"),QMessageBox::Close);
    }

    //update movepartinfo
    query2.prepare("update movepartinfo set movepartid=?,moveparttype=?,movepartname=?,runhour=?,runstage=?,planeid=?,planetype=?,startdate=?,enddate=? where id=?");//对数据库中Tuser中对应uid的一条记录进行修改
    query2.addBindValue(_bottomWindow.dBTableWidget_2->item(1,1)->data(Qt::DisplayRole).toString());//对其SQL语句中的每一项进行幅值操作
    query2.addBindValue(_bottomWindow.dBTableWidget_2->item(1,2)->data(Qt::DisplayRole).toString());
    query2.addBindValue(_bottomWindow.dBTableWidget_2->item(1,3)->data(Qt::DisplayRole).toString());
    query2.addBindValue(_bottomWindow.dBTableWidget_2->item(1,4)->data(Qt::DisplayRole).toString());
    query2.addBindValue(_bottomWindow.dBTableWidget_2->item(1,5)->data(Qt::DisplayRole).toString());
    query2.addBindValue(_bottomWindow.dBTableWidget_2->item(1,6)->data(Qt::DisplayRole).toString());
    query2.addBindValue(_bottomWindow.dBTableWidget_2->item(1,7)->data(Qt::DisplayRole).toString());
    query2.addBindValue(_bottomWindow.dBTableWidget_2->item(1,8)->data(Qt::DisplayRole).toString());
    query2.addBindValue(_bottomWindow.dBTableWidget_2->item(1,9)->data(Qt::DisplayRole).toString());
    //query2.addBindValue("F:/abrasivemarksystem/abrasivepic/"+wholefilename);//可以保存到表8对应的数据库当中
    query2.addBindValue(_bottomWindow.dBTableWidget_2->item(1,0)->data(Qt::DisplayRole).toString());
    if(!query2.exec())//若SQL语句没有执行，则提示错误
        QMessageBox::warning(this,tr("failed2"),tr("Modify error"),QMessageBox::Close);
    else{
        QMessageBox::warning(this,tr("success2"),tr("Modify success"),QMessageBox::Close);
    }


    //update oilsampleinfo
    query4.prepare("update oilsampleinfo set oilsampleid=?,sampledepartid=?,planetype=?,planeid=?,monitorpartname=?,monitorpartid=?,sampleid=?,oilworktime=?,oiladdition=?,samplereason=?,sampledepartname=?,samplestuff=?,sampledate=?,sampletime=?,samplesituation=?,samplemethod=?,samplevolume=?,sampleinstruction=?,sendstuff=?,senddate=?,sendtime=? where id=?");//对数据库中Tuser中对应uid的一条记录进行修改

    query4.addBindValue(_bottomWindow.dBTableWidget_4->item(1,1)->data(Qt::DisplayRole).toString());//对其SQL语句中的每一项进行幅值操作
    query4.addBindValue(_bottomWindow.dBTableWidget_4->item(1,2)->data(Qt::DisplayRole).toString());
    query4.addBindValue(_bottomWindow.dBTableWidget_4->item(1,3)->data(Qt::DisplayRole).toString());
    query4.addBindValue(_bottomWindow.dBTableWidget_4->item(1,4)->data(Qt::DisplayRole).toString());
    query4.addBindValue(_bottomWindow.dBTableWidget_4->item(1,5)->data(Qt::DisplayRole).toString());
    query4.addBindValue(_bottomWindow.dBTableWidget_4->item(1,6)->data(Qt::DisplayRole).toString());
    query4.addBindValue(_bottomWindow.dBTableWidget_4->item(1,7)->data(Qt::DisplayRole).toString());
    query4.addBindValue(_bottomWindow.dBTableWidget_4->item(1,8)->data(Qt::DisplayRole).toString());
    query4.addBindValue(_bottomWindow.dBTableWidget_4->item(1,9)->data(Qt::DisplayRole).toString());
    query4.addBindValue(_bottomWindow.dBTableWidget_4->item(1,10)->data(Qt::DisplayRole).toString());//对其SQL语句中的每一项进行幅值操作
    query4.addBindValue(_bottomWindow.dBTableWidget_4->item(1,11)->data(Qt::DisplayRole).toString());
    query4.addBindValue(_bottomWindow.dBTableWidget_4->item(1,12)->data(Qt::DisplayRole).toString());
    query4.addBindValue(_bottomWindow.dBTableWidget_4->item(1,13)->data(Qt::DisplayRole).toString());
    query4.addBindValue(_bottomWindow.dBTableWidget_4->item(1,14)->data(Qt::DisplayRole).toString());
    query4.addBindValue(_bottomWindow.dBTableWidget_4->item(1,15)->data(Qt::DisplayRole).toString());
    query4.addBindValue(_bottomWindow.dBTableWidget_4->item(1,16)->data(Qt::DisplayRole).toString());
    query4.addBindValue(_bottomWindow.dBTableWidget_4->item(1,17)->data(Qt::DisplayRole).toString());
    query4.addBindValue(_bottomWindow.dBTableWidget_4->item(1,18)->data(Qt::DisplayRole).toString());
    query4.addBindValue(_bottomWindow.dBTableWidget_4->item(1,19)->data(Qt::DisplayRole).toString());
    query4.addBindValue(_bottomWindow.dBTableWidget_4->item(1,20)->data(Qt::DisplayRole).toString());
    query4.addBindValue(_bottomWindow.dBTableWidget_4->item(1,21)->data(Qt::DisplayRole).toString());

    query4.addBindValue(_bottomWindow.dBTableWidget_4->item(1,0)->data(Qt::DisplayRole).toString());
    if(!query4.exec())//若SQL语句没有执行，则提示错误
        QMessageBox::warning(this,tr("failed4"),tr("Modify error"),QMessageBox::Close);
    else{
        QMessageBox::warning(this,tr("success4"),tr("Modify success"),QMessageBox::Close);
    }


    //update oilanalyzeinfo
    query5.prepare("update oilanalyzeinfo set oilsampleid=?,analyzedepartname=?,senddepart=?,sendreason=?,sendstuff=?,receivedate=?,receivestuff=?,contaminationanalyzemethod=?,contaminationanalyzestuff=?,contaminationanalyzedate=?,contaminationanalyzeequipment=?,contaminationanalyzereportid=?,spectroscopymethod=?,spectroscopystuff=?,spectroscopydate=?,spectroscopyequipment=?,spectroscopyreportid=?,ferrographymethod=?,ferrographystuff=?,ferrographydate=?,ferrographyequipment=?,ferrographyreportid=?,physicochemicalmethod=?,physicochemicalstuff=?,physicochemicaldate=?,physicochemicalequipment=?,physicochemicalreportid=? where id=?");//对数据库中Tuser中对应uid的一条记录进行修改
    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,1)->data(Qt::DisplayRole).toString());//对其SQL语句中的每一项进行幅值操作
    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,2)->data(Qt::DisplayRole).toString());
    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,3)->data(Qt::DisplayRole).toString());
    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,4)->data(Qt::DisplayRole).toString());
    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,5)->data(Qt::DisplayRole).toString());
    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,6)->data(Qt::DisplayRole).toString());
    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,7)->data(Qt::DisplayRole).toString());
    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,8)->data(Qt::DisplayRole).toString());
    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,9)->data(Qt::DisplayRole).toString());
    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,10)->data(Qt::DisplayRole).toString());//对其SQL语句中的每一项进行幅值操作
    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,11)->data(Qt::DisplayRole).toString());
    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,12)->data(Qt::DisplayRole).toString());
    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,13)->data(Qt::DisplayRole).toString());
    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,14)->data(Qt::DisplayRole).toString());
    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,15)->data(Qt::DisplayRole).toString());
    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,16)->data(Qt::DisplayRole).toString());
    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,17)->data(Qt::DisplayRole).toString());
    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,18)->data(Qt::DisplayRole).toString());
    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,19)->data(Qt::DisplayRole).toString());
    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,20)->data(Qt::DisplayRole).toString());
    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,21)->data(Qt::DisplayRole).toString());
    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,22)->data(Qt::DisplayRole).toString());
    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,23)->data(Qt::DisplayRole).toString());
    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,24)->data(Qt::DisplayRole).toString());
    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,25)->data(Qt::DisplayRole).toString());
    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,26)->data(Qt::DisplayRole).toString());
    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,27)->data(Qt::DisplayRole).toString());

    query5.addBindValue(_bottomWindow.dBTableWidget_5->item(1,0)->data(Qt::DisplayRole).toString());

    if(!query5.exec())//若SQL语句没有执行，则提示错误
        QMessageBox::warning(this,tr("failed5"),tr("Modify error"),QMessageBox::Close);
    else{
        QMessageBox::warning(this,tr("success5"),tr("Modify success"),QMessageBox::Close);
    }

    //update ferrographyinfo
    query6.prepare("update ferrographyinfo set ferrographysheetid=?,ferrographyreportid=?,oilsampleid=?,ferrographyanalyzertype=?,ferrographymakeoilconsumption=?,ferrographymakemethod=?,ferrographymakestuff=? where id=?");//对数据库中Tuser中对应uid的一条记录进行修改
    query6.addBindValue(_bottomWindow.dBTableWidget_6->item(1,1)->data(Qt::DisplayRole).toString());//对其SQL语句中的每一项进行幅值操作
    query6.addBindValue(_bottomWindow.dBTableWidget_6->item(1,2)->data(Qt::DisplayRole).toString());
    query6.addBindValue(_bottomWindow.dBTableWidget_6->item(1,3)->data(Qt::DisplayRole).toString());
    query6.addBindValue(_bottomWindow.dBTableWidget_6->item(1,4)->data(Qt::DisplayRole).toString());
    query6.addBindValue(_bottomWindow.dBTableWidget_6->item(1,5)->data(Qt::DisplayRole).toString());
    query6.addBindValue(_bottomWindow.dBTableWidget_6->item(1,6)->data(Qt::DisplayRole).toString());
    query6.addBindValue(_bottomWindow.dBTableWidget_6->item(1,7)->data(Qt::DisplayRole).toString());
    query6.addBindValue(_bottomWindow.dBTableWidget_6->item(1,0)->data(Qt::DisplayRole).toString());
    if(!query6.exec())//若SQL语句没有执行，则提示错误
        QMessageBox::warning(this,tr("failed6"),tr("Modify error"),QMessageBox::Close);
    else{
        QMessageBox::warning(this,tr("success6"),tr("Modify success"),QMessageBox::Close);
    }

    //update ferrographypicinfo
    query7.prepare("update ferrographypicinfo set ferrographypicid=?,ferrographysheetid=?,ferrographyreportid=?,microscopictype=?,imageacquisitiontype=?,lightsourcetype=?,magnification=?,imageacquisitionarea=?,imageacquisitionstuff=?,ferrographypicpath=?,imagerecognitioninfoanalysis=?,imagesymbol=? where id=?");//对数据库中Tuser中对应uid的一条记录进行修改
    query7.addBindValue(_bottomWindow.dBTableWidget_7->item(1,1)->data(Qt::DisplayRole).toString());//对其SQL语句中的每一项进行幅值操作
    query7.addBindValue(_bottomWindow.dBTableWidget_7->item(1,2)->data(Qt::DisplayRole).toString());
    query7.addBindValue(_bottomWindow.dBTableWidget_7->item(1,3)->data(Qt::DisplayRole).toString());
    query7.addBindValue(_bottomWindow.dBTableWidget_7->item(1,4)->data(Qt::DisplayRole).toString());
    query7.addBindValue(_bottomWindow.dBTableWidget_7->item(1,5)->data(Qt::DisplayRole).toString());
    query7.addBindValue(_bottomWindow.dBTableWidget_7->item(1,6)->data(Qt::DisplayRole).toString());
    query7.addBindValue(_bottomWindow.dBTableWidget_7->item(1,7)->data(Qt::DisplayRole).toString());
    query7.addBindValue(_bottomWindow.dBTableWidget_7->item(1,8)->data(Qt::DisplayRole).toString());
    query7.addBindValue(_bottomWindow.dBTableWidget_7->item(1,9)->data(Qt::DisplayRole).toString());
    query7.addBindValue(_bottomWindow.dBTableWidget_7->item(1,10)->data(Qt::DisplayRole).toString());
    query7.addBindValue(_bottomWindow.dBTableWidget_7->item(1,11)->data(Qt::DisplayRole).toString());
    //query7.addBindValue(_bottomWindow.dBTableWidget_7->item(1,12)->data(Qt::DisplayRole).toString());
    _bottomWindow.dBTableWidget_7->setItem(1,12,new QTableWidgetItem("Y"));
    query7.addBindValue("Y");//默认标注过了之后都会点击保存标注结果进而将铁谱片标识符号设为Y
    query7.addBindValue(_bottomWindow.dBTableWidget_7->item(1,0)->data(Qt::DisplayRole).toString());
    if(!query7.exec())//若SQL语句没有执行，则提示错误
        QMessageBox::warning(this,tr("failed7"),tr("Modify error"),QMessageBox::Close);
    else{
        QMessageBox::warning(this,tr("success7"),tr("Modify success"),QMessageBox::Close);
    }


    //update abrasivemarkinfo
    query8.prepare("update abrasivemarkinfo set abrasiveid=?,ferrographypicid=?,ferrographysheetid=?,ferrographyreportid=?,abrasivemarkstuff=?,abrasivepicpath=?,abrasivematerial=?,abrasiveposition=?,abrasivesize=?,abrasivesperimeter=?,abrasiveshape=?,abrasivecolor=?,abrasivesurfacetexturetype=?,abrasiveweartype=?,abrasivedamagetype=?,abrasivemechanismtype=?,abrasivefaultinformationreflection=?,abrasivetypical=? where id=?");
    query8.addBindValue(_bottomWindow.dBTableWidget_8->item(1,1)->data(Qt::DisplayRole).toString());//对其SQL语句中的每一项进行幅值操作
    query8.addBindValue(_bottomWindow.dBTableWidget_8->item(1,2)->data(Qt::DisplayRole).toString());
    query8.addBindValue(_bottomWindow.dBTableWidget_8->item(1,3)->data(Qt::DisplayRole).toString());
    query8.addBindValue(_bottomWindow.dBTableWidget_8->item(1,4)->data(Qt::DisplayRole).toString());
    query8.addBindValue(_bottomWindow.dBTableWidget_8->item(1,5)->data(Qt::DisplayRole).toString());
    _bottomWindow.dBTableWidget_8->setItem(1,6,new QTableWidgetItem("F:/abrasivemarksystem/abrasivepic/"+_imageName));
    //query8.addBindValue(_bottomWindow.dBTableWidget_8->item(1,6)->data(Qt::DisplayRole).toString());
    query8.addBindValue("F:/abrasivemarksystem/abrasivepic/"+_imageName);//默认标注过了之后都会点击保存标注结果，进而将标注得到的磨粒对应的路径储存到数据库中
    query8.addBindValue(_bottomWindow.dBTableWidget_8->item(1,7)->data(Qt::DisplayRole).toString());
    query8.addBindValue(_bottomWindow.dBTableWidget_8->item(1,8)->data(Qt::DisplayRole).toString());
    query8.addBindValue(_bottomWindow.dBTableWidget_8->item(1,9)->data(Qt::DisplayRole).toString());
    query8.addBindValue(_bottomWindow.dBTableWidget_8->item(1,10)->data(Qt::DisplayRole).toString());//对其SQL语句中的每一项进行幅值操作
    query8.addBindValue(_bottomWindow.dBTableWidget_8->item(1,11)->data(Qt::DisplayRole).toString());
    query8.addBindValue(_bottomWindow.dBTableWidget_8->item(1,12)->data(Qt::DisplayRole).toString());
    query8.addBindValue(_bottomWindow.dBTableWidget_8->item(1,13)->data(Qt::DisplayRole).toString());
    query8.addBindValue(_bottomWindow.dBTableWidget_8->item(1,14)->data(Qt::DisplayRole).toString());
    query8.addBindValue(_bottomWindow.dBTableWidget_8->item(1,15)->data(Qt::DisplayRole).toString());
    query8.addBindValue(_bottomWindow.dBTableWidget_8->item(1,16)->data(Qt::DisplayRole).toString());
    query8.addBindValue(_bottomWindow.dBTableWidget_8->item(1,17)->data(Qt::DisplayRole).toString());
    query8.addBindValue(_bottomWindow.dBTableWidget_8->item(1,18)->data(Qt::DisplayRole).toString());
    query8.addBindValue(_bottomWindow.dBTableWidget_8->item(1,0)->data(Qt::DisplayRole).toString());
    if(!query8.exec())//若SQL语句没有执行，则提示错误
        QMessageBox::warning(this,tr("failed8"),tr("Modify error"),QMessageBox::Close);
    else{
        QMessageBox::warning(this,tr("success8"),tr("Modify success"),QMessageBox::Close);
    }

    QString connection = db.connectionName();
    db.close();
    QSqlDatabase::removeDatabase(connection);

    //_leftWindow.tableWidget->setItem(_cnt, 2, new QTableWidgetItem("Y"));
}

void ImageCompletionUI::saveLabelMap()
{
    QString filename = QFileDialog::getSaveFileName( this, "Save Image As...", QDir::currentPath(), tr("Images (*.bmp)") );
    if ( filename.isEmpty() )
    {
        return;
    }
    bool bSucessOrNot;
    bSucessOrNot = _editImageViewer->saveLabelMap(filename);
    if ( false == bSucessOrNot )
    {
        QMessageBox::information(this,"Error Saving",QString("Could not save to file: %1").arg(filename));
    }
}

void ImageCompletionUI::updateLog()
{
    char* log_str = getNewLogString();

    if (log_str!=NULL)
    {

        (*_logInformationString) = log_str;
        sprintf(log_str, "");
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
        delete [] log_str;
        log_str = NULL;
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
    if(!createConnection(db))
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("数据库连接失败!"),
                              QMessageBox::Cancel);
    }

    bool ret;
    QSqlRecord rec;

    QString tableNames[8] = { "equipmentinfo", "movepartinfo", "movepartrepairinfo", "oilsampleinfo", "oilanalyzeinfo", "ferrographyinfo", "ferrographypicinfo", "abrasivemarkinfo" };

    for(int k = 1; k <= 8; k++)
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
                    }
                }
                i++;
            }
        }
    }

    QString connection = db.connectionName();
    db.close();
    QSqlDatabase::removeDatabase(connection);
}

void ImageCompletionUI::loadLabelMap()
{
    QSettings settings("ImageCompletion","ImageCompletion");
    QString initialPath = settings.value("lastImportPath", QVariant(QDir::homePath())).toString();
    if(initialPath.isEmpty())
        initialPath = QDir::homePath() + "/untitled";

    QString fileName;
    fileName = QFileDialog::getOpenFileName( this, tr("Label Map"), initialPath, tr("Images (*.bmp)"));
    if (!fileName.isEmpty())
        settings.setValue("lastImportPath", QVariant(fileName));

    //If the file name is not empty //
    if (!fileName.isEmpty())
    {
        _editImageViewer->openLabelImage(fileName);
    }

    _step = MARKING;
}

void ImageCompletionUI::switchModule()
{
    switch( _moduleSelectBox->currentIndex() )
    {
    case 0 :
        _operationStackedWidget->setCurrentIndex(0);
        break;
    case 1 :
        _operationStackedWidget->setCurrentIndex(1);
        break;
    case 2 :
        _operationStackedWidget->setCurrentIndex(2);
        break;
    case 3 :
        _operationStackedWidget->setCurrentIndex(3);
        break;
    }
}

void ImageCompletionUI::_RegionupdateBrushSize()
{
    if ( _editImageViewer->image().isNull() )
        return;

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

void ImageCompletionUI::_SceneupdateBrushSize()
{
    if ( _editImageViewer->image().isNull() )
        return;

    _brushSize = _sceneCompletionDialog._brushSizeHSlider->value();

    _editImageViewer->setBrushWidth( _brushSize );
}

void ImageCompletionUI::Excute()
{

    if (_step == LOADFAILED)
    {
        QMessageBox::warning( this, tr("RegionCompetition"), tr("Load Image Error\n"));
        return;
    }


    QIcon iconGo, iconNext;
    iconGo.addPixmap(QPixmap(QString::fromUtf8("icons/go.png")), QIcon::Normal, QIcon::Off);
    iconNext.addPixmap(QPixmap(QString::fromUtf8("icons/next.png")), QIcon::Normal, QIcon::Off);

    QImage* tmp_image;

    //	_regionCompetitionDialog.operationBtn->setEnabled(false);
    tmp_image = _editImageViewer->RunRegionCompetition();
    //	_regionCompetitionDialog.operationBtn->setEnabled(true);
    updateLog();

    //switch (m_step)
    //{
    //case NONE:
    //	_regionCompetitionDialog.operationBtn->setIcon(iconGo);
    //	_regionCompetitionDialog.operationBtn->setIconSize(QSize(32, 32));
    //	break;
    //case MARKING:
    //	//_regionCompetitionDialog.operationBtn->setIcon(iconNext);
    //	_regionCompetitionDialog.operationBtn->setEnabled(false);
    //	tmp_image = _editImageViewer->RunRegionCompetition();
    //	_regionCompetitionDialog.operationBtn->setEnabled(true);
    //	//_resultImageViewer->setImage( *tmp_image );
    //	//_resultImageViewer->repaint();
    //	//_resultImageViewer->openImage("45454.jpg");
    //	//_centralTabWidget->setCurrentIndex(1);
    //	updateLog();
    //	break;
    //case BOOSTING:
    //	break;
    //case TESTING:
    //	//		_editImageViewer->boostTesting();
    //	break;
    //case MARCHING:
    //	//		_editImageViewer->fastMarching();
    //	break;
    //}

    //if (tmp_image) delete tmp_image;

}

void	ImageCompletionUI::keyPressEvent(QKeyEvent */* e */){}


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
    _regionCompetitionDialog.radioForeground->setEnabled(b);
    _regionCompetitionDialog.radioBackground->setEnabled(b);
    _regionCompetitionDialog.radioErazer->setEnabled(b);

}

void ImageCompletionUI::updateMethod()
{
    _editImageViewer->setPaintable(true);
    if (_regionCompetitionDialog.radioStrikeLabelling->isChecked())
    {
        _editImageViewer->setMethod(0);
        _editImageViewer->setLabelType(0);
        setStrikeOptionsEnabled(true);

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

void ImageCompletionUI::updateIsShowDetail()
{
    //	IsShowDetail = _regionCompetitionDialog.checkShowDetail;
}

void ImageCompletionUI::UsePrior(int state)
{
    if (state)
    {
        _editImageViewer->isUsePrior = true;
    }
    else
    {
        _editImageViewer->isUsePrior = false;
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
}

void ImageCompletionUI::brighting()
{
    _regionCompetitionDialog.sliderBasicOp->setMaximum(100);
    _regionCompetitionDialog.sliderBasicOp->setMinimum(-100);
    _regionCompetitionDialog.sliderBasicOp->setValue(0);

    _regionCompetitionDialog.labelSliderLeft->setText(tr("暗"));
    _regionCompetitionDialog.labelSliderRight->setText(tr("亮"));
}

void ImageCompletionUI::notColor()
{
    _regionCompetitionDialog.labelSliderLeft->setText(tr(""));
    _regionCompetitionDialog.labelSliderRight->setText(tr(""));

    _editImageViewer->imageNotColor( 0.0 );
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
        // TODO
    }
}

void ImageCompletionUI::userManagement()
{
    userMangementDlg = new UserManagement(this);
    userMangementDlg->setWindowFlags(userMangementDlg->windowFlags()&~Qt::WindowMinimizeButtonHint& ~Qt::WindowMaximizeButtonHint);
    userMangementDlg->show();
}

bool ImageCompletionUI::maybeSave()
{
    return true;
}

void ImageCompletionUI::updateLineWidth()
{
    if ( _editImageViewer->image().isNull() ) return;
    int w;
    if(_regionCompetitionDialog.radioLineWidthThick->isChecked())       w = 7;
    else if(_regionCompetitionDialog.radioLineWidthMedium->isChecked()) w = 5;
    else if(_regionCompetitionDialog.radioLineWidthThin->isChecked())   w = 3;

    _editImageViewer->setLineThickness( w );
}

void ImageCompletionUI::strikeComboChanged(int index)
{
    if(index != 0)
    {
        _editImageViewer->setPaintable(true);
        _editImageViewer->setMethod(0);
        _editImageViewer->setLabelType(0);
        setStrikeOptionsEnabled(true);

        if(index == 2)
        {
            _editImageViewer->changeObjectLabeling(0);
            QPixmap pixmap(30,30);
            pixmap.fill(qRgb(colorTable[0], colorTable[1], colorTable[2]));
            _regionCompetitionDialog.showColorLabel->setPixmap(pixmap);
        }
        else if(index == 1)
        {
            _editImageViewer->changeObjectLabeling(1);
            QPixmap pixmap(30,30);
            pixmap.fill(qRgb(colorTable[3], colorTable[4], colorTable[5]));
            _regionCompetitionDialog.showColorLabel->setPixmap(pixmap);
        }
        else if(index == 3)
        {
            _editImageViewer->setEraser();
            QPixmap pixmap(0,0);
            _regionCompetitionDialog.showColorLabel->setPixmap(pixmap);
        }
    }
}

void ImageCompletionUI::strikeThicknessComboboxChanged(int index)
{
    if ( !_editImageViewer || _editImageViewer->image().isNull() ) return;

    if(index == 0 || index == 1) _brushSize = 5;
    else if(index == 2) _brushSize = 15;
    else if(index == 3) _brushSize = 30;

    _editImageViewer->setBrushWidth( _brushSize );
}

void ImageCompletionUI::lineThicknessComboboxChanged(int index)
{
    if ( !_editImageViewer || _editImageViewer->image().isNull() ) return;
    int w;
    if(index == 0 || index == 1) w = 3;
    else if(index == 2) w = 5;
    else if(index == 3) w = 7;

    _editImageViewer->setLineThickness( w );
}

void ImageCompletionUI::labellingMethodChanged(QAction *a)
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

void ImageCompletionUI::saveLabelledResult()
{
    _editImageViewer->saveLabelledResult(_imageName, Global::ExtResult);
}

void ImageCompletionUI::saveUserLabels()
{
    //TODO
}

void ImageCompletionUI::saveMask()
{
    //_editImageViewer->saveAsMask();
}

void ImageCompletionUI::strikeThicknessChanged(QAction *a)
{
    if ( _editImageViewer->image().isNull() ) return;

    a->setChecked(true);

    if(a == _strikeThickness[0])      _brushSize = 5;
    else if(a == _strikeThickness[1]) _brushSize = 15;
    else if(a == _strikeThickness[2]) _brushSize = 30;

    _editImageViewer->setBrushWidth( _brushSize );
}

void ImageCompletionUI::lineThicknessChanged(QAction *a)
{
    a->setChecked(true);

    int w;
    if(a == _lineThickness[0])      w = 3;
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

// 从文件导入数据--zhyn
bool ImageCompletionUI::importDB(const QString &path)
{
    /**
     *@brief 读取sql文本内容,并写入至数据库
     *@param path sql文件路径
     */
    QSqlDatabase gAuthDB;
    if(!createConnection(gAuthDB))
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

// 导出数据数据到文件--zhyn
bool ImageCompletionUI::exportDB(const QString &path)
{
    /**
     *@brief 导出数据库数据到文件中
     *@param path 文件路径
     */
    //QMessageBox::warning(this,"warning","this is private function to export SQL Data",QMessageBox::Close);

    QSqlDatabase gAuthDB;
    if(!createConnection(gAuthDB))
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

void ImageCompletionUI::openImage(QString fileName)
{
    if(!fileName.isEmpty())
    {
        if ( _editImageViewer->openImage(fileName) )
        {
            _imagePath = fileName;
            _imageName = QFileInfo(fileName).fileName();

            _editImageViewer->repaint();
            _step = NONE;
            updateLog();

            QString status = this->labelStatus(fileName);
            if(status != "N" && status != "Y")
            {
                QMessageBox::StandardButton reply = QMessageBox::information(0, tr("提示"), "此图像为新图像,是否要导入数据库?", QMessageBox::Ok | QMessageBox::Cancel);
                if(reply == QMessageBox::Ok)
                {
                    (new ImageProperties)->showDlg(fileName);
                }
            }
        }
        else
        {
            //_editImageViewer->close();
            close();
            _step = LOADFAILED;
            _regionCompetitionDialog.radioForeground->setEnabled(false);
            _regionCompetitionDialog.radioBackground->setEnabled(false);
            _regionCompetitionDialog.radioErazer->setEnabled(false);
            return;
        }
    }

    int width = _editImageViewer->image().width();
    int height = _editImageViewer->image().height();
    this->setMinimumSize( width < 800 ? 800 : height, height < 600 ? 600 : height );
}

QColor ImageCompletionUI::getColor(QString status)
{
    if(status == "Y")
    {
        return Global::LabelledColor;
    }
    else
    {
        return Global::UnLabelledColor;
    }
}

void ImageCompletionUI::syncLabelledImage(QString pathOriginal, QString pathResult, QString pathMask)
{
    if(pathResult.isEmpty() && pathMask.isEmpty()) return;

    QSqlTableModel *_model = new QSqlTableModel;

    _model->setTable("abrasivemarkinfo");
    _model->setFilter(QString("abrasivepicpath = '%1'").arg(pathOriginal));

    if(_model->select())
    {
        if(_model->rowCount() == 1)
        {
            QSqlRecord record = _model->record(0);

            if(!pathResult.isEmpty())
            {
                QFile *file = new QFile(pathResult);
                file->open(QIODevice::ReadOnly);
                QByteArray data = file->readAll();
                record.setValue("abrasiveResultData", data);
            }

            if(!pathMask.isEmpty())
            {
                QFile *file = new QFile(pathMask);
                file->open(QIODevice::ReadOnly);
                QByteArray data = file->readAll();
                record.setValue("abrasiveMaskData", data);
            }
            _model->setRecord(0, record);
        }

        if(!_model->submitAll())
        {
            QMessageBox::warning(this, "保存", QString("保存数据库失败"), QMessageBox::Close);
        }
    }

    _model->setTable("ferrographypicinfo");
    _model->setFilter(QString("ferrographypicpath = '%1'").arg(pathOriginal));
    if(_model->select())
    {
        if(_model->rowCount() == 1)
        {
            QSqlRecord record = _model->record(0);
            record.setValue("imagesymbol", "Y");
            _model->setRecord(0, record);
        }

        if(!_model->submitAll())
        {
            QMessageBox::warning(this, "保存", QString("保存数据库失败"), QMessageBox::Close);
        }
    }
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

// 拷贝文件--zhyn
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

// 批量数据导入--zhyn
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

// 批量数据导出--zhyn
void ImageCompletionUI::exportData()
{
    QFileDialog *sourceDir = new QFileDialog(this,tr("选择铁谱图片目录"),"","");
    sourceDir->setFileMode(QFileDialog::DirectoryOnly);
    sourceDir->setViewMode(QFileDialog::Detail);
    QString sourcePath;
    if(sourceDir->exec())
    {
        QStringList sourcePaths = sourceDir->selectedFiles();
        sourcePath = sourcePaths.at(0);
        //qDebug()<<sourcePath;
    }
    else
        return;

    QFileDialog *resultDir = new QFileDialog(this,tr("选择磨粒标注结果目录"),"","");
    resultDir->setFileMode(QFileDialog::DirectoryOnly);
    resultDir->setViewMode(QFileDialog::Detail);
    QString resultPath;
    if(resultDir->exec())
    {
        QStringList resultPaths = resultDir->selectedFiles();
        resultPath = resultPaths.at(0);
        //qDebug()<<resultPath;
    }
    else
        return;

    QFileDialog *targetDir = new QFileDialog(this,tr("选择打包存档目录"),"","");
    targetDir->setFileMode(QFileDialog::DirectoryOnly);
    targetDir->setViewMode(QFileDialog::Detail);
    QString targetPath;
    if(targetDir->exec())
    {
        QStringList targetPaths = targetDir->selectedFiles();
        targetPath  = targetPaths.at(0);
        //qDebug()<<targetPath;
    }
    else
        return;

    if(resultPath.isEmpty() | resultPath.isEmpty()| targetPath.isEmpty())
        return;
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



void ImageCompletionUI::on_tableWidget_doubleClicked(const QModelIndex & /* index */)
{

}

void ImageCompletionUI::cellDoubleClicked_(int row, int /* col */)
{
    if(_leftWindow.tableWidget->item(row, 1) != 0)
    {
        QString absolutePath = _leftWindow.tableWidget->item(row, 1)->text();
        openImage(absolutePath);
    }
}

QString ImageCompletionUI::labelStatus(QString imagePath)
{
    if(imagePath.isEmpty()) return "";

    QSqlDatabase db;
    if( createConnection(db) )
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
            return "";
        }
        else return "";
    }
}

void ImageCompletionUI::editImageProperties(QString /* fileName */)
{
}
