#include "Delaunay_triangulation_2.hh"
#include "NeighbourEnumeration.hpp"

MainWindow::MainWindow()
  : DemosMainWindow(),
    selectedColor(CGAL::GREEN)
{
  setupUi(this);

  this->graphicsView->setAcceptDrops(false);

  // Add a GraphicItem for the Delaunay triangulation
  dgi = new CGAL::Qt::TriangulationGraphicsColoredItem<Delaunay>(&dt);

  QObject::connect(this, SIGNAL(changed()),
		   dgi, SLOT(modelChanged()));

  dgi->setVerticesPen(QPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  scene.addItem(dgi);

  // Add a GraphicItem for the Voronoi diagram
  vgi = new CGAL::Qt::VoronoiGraphicsItem<Delaunay>(&dt);

  QObject::connect(this, SIGNAL(changed()),
		   vgi, SLOT(modelChanged()));

  vgi->setEdgesPen(QPen(Qt::blue, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  scene.addItem(vgi);
  vgi->hide();

  // Setup input handlers. They get events before the scene gets them
  // and the input they generate is passed to the triangulation with 
  // the signal/slot mechanism    
  pi = new CGAL::Qt::TriangulationPointInputAndConflictZone<Delaunay>(&scene, &dt, this );

  QObject::connect(pi, SIGNAL(generate(CGAL::Object)),
		   this, SLOT(processInput(CGAL::Object)));

  ne = new CGAL::Qt::NeighbourEnumeration<Delaunay>(&dt, this);
  // NeighbourEnumeration<Delaunay> emits a modelChanged() signal each
  // time the moving point moves.
  // The following connection is for the purpose of emitting changed().
  QObject::connect(ne, SIGNAL(modelChanged()),
		   this, SIGNAL(changed()));

  trv = new CGAL::Qt::TriangulationRemoveVertex<Delaunay>(&dt, this);
  QObject::connect(trv, SIGNAL(modelChanged()),
		   this, SIGNAL(changed()));

  tcc = new CGAL::Qt::TriangulationCircumcircle<Delaunay>(&scene, &dt, this);
  tcc->setPen(QPen(Qt::red, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

  tl = new CGAL::Qt::TriangulationLocate<Delaunay>(&scene, &dt, this);
  QColor colort(Qt::blue);
  colort.setAlpha(150);
  tl->setBrush(colort);

  cz = new CGAL::Qt::TriangulationConflictZone<Delaunay>(&scene, &dt, this);

  // 
  // Manual handling of actions
  //

  QObject::connect(this->actionQuit, SIGNAL(triggered()), 
		   this, SLOT(close()));

  // We put mutually exclusive actions in an QActionGroup
  QActionGroup* ag = new QActionGroup(this);
  ag->addAction(this->actionSetColorGreen);
  ag->addAction(this->actionSetColorRed);
  ag->addAction(this->actionSetColorBlue);
  ag->addAction(this->actionSetColorBlack);


  QActionGroup* ag2 = new QActionGroup(this);
  ag2->addAction(this->actionInsertPoint);
  ag2->addAction(this->actionSelectPoint);
  
  // Check two actions 
  this->actionSetColorGreen->setChecked(true);
  this->actionInsertPoint->setChecked(true);

  //
  // Setup the scene and the view
  //
  scene.setItemIndexMethod(QGraphicsScene::NoIndex);
  scene.setSceneRect(-100, -100, 100, 100);
  this->graphicsView->setScene(&scene);
  this->graphicsView->setMouseTracking(true);

  // Turn the vertical axis upside down
  this->graphicsView->matrix().scale(1, -1);
                                                      
  // The navigation adds zooming and translation functionality to the
  // QGraphicsView
  this->addNavigation(this->graphicsView);

  this->setupStatusBar();
  this->setupOptionsMenu();
  this->addAboutDemo(":/cgal/help/about_Delaunay_triangulation_2.html");
  this->addAboutCGAL();

  this->addRecentFiles(this->menuFile, this->actionQuit);
  connect(this, SIGNAL(openRecentFile(QString)),
	  this, SLOT(open(QString)));
  dgi->setVisibleEdges(true);
}


void
MainWindow::processInput(CGAL::Object o)
{
  Point_2 p;
  if(CGAL::assign(p, o)){
    Delaunay::Vertex_handle vh = dt.insert(p);

    vh->info().color = selectedColor;
  }
  emit(changed());
}


/* 
 *  Qt Automatic Connections
 *  http://doc.trolltech.com/4.4/designer-using-a-component.html#automatic-connections
 * 
 *  setupUi(this) generates connections to the slots named
 *  "on_<action_name>_<signal_name>"
 */
void
MainWindow::on_actionSetColorGreen_toggled(bool checked)
{
  selectedColor = CGAL::GREEN;
  //CGAL::Qt::NeighbourEnumeration::expectedColor = CGAL::GREEN;
}


void
MainWindow::on_actionSetColorRed_toggled(bool checked)
{
  selectedColor = CGAL::RED;
  //CGAL::Qt::NeighbourEnumeration::expectedColor = CGAL::RED;
}


void
MainWindow::on_actionShowConflictZone_toggled(bool checked)
{

  if(checked){
    scene.installEventFilter(cz);
  } else {
    scene.removeEventFilter(cz);
  }
}

void
MainWindow::on_actionSetColorBlue_toggled(bool checked)
{
  selectedColor = CGAL::BLUE;
  //CGAL::Qt::NeighbourEnumeration::expectedColor = CGAL::BLUE;
}

void
MainWindow::on_actionSetColorBlack_toggled(bool checked)
{
  selectedColor = CGAL::BLACK;
  //CGAL::Qt::NeighbourEnumeration::expectedColor = CGAL::BLACK;
}


void
MainWindow::on_actionInsertPoint_toggled(bool checked)
{
  if(checked) {
    scene.installEventFilter(pi);
    scene.installEventFilter(trv);
  } else {  
    scene.removeEventFilter(pi);
    scene.removeEventFilter(trv);
  }
}


void
MainWindow::on_actionSelectPoint_toggled(bool checked)
{
  if(checked) {
    scene.installEventFilter(ne);
  } else {
    scene.removeEventFilter(ne);
  }
  //vgi->setVisible(checked);
}


void
MainWindow::on_actionClear_triggered()
{
  dt.clear();
  emit(changed());
}


void
MainWindow::on_actionInsertRandomPoints_triggered()
{
  CGAL::Color randomColors[] = {
      CGAL::GREEN,
      CGAL::BLUE,
      CGAL::RED,
  };
  QRectF rect = CGAL::Qt::viewportsBbox(&scene);
  CGAL::Qt::Converter<K> convert;  
  Iso_rectangle_2 isor = convert(rect);
  CGAL::Random_points_in_iso_rectangle_2<Point_2> pg(isor.min(), isor.max());
  bool ok = false;
  const int number_of_points = 
    QInputDialog::getInteger(this, 
                             tr("Number of random points"),
                             tr("Enter number of random points"),
			     100,
			     0,
			     std::numeric_limits<int>::max(),
			     1,
			     &ok);

  if(!ok) {
    return;
  }

  // wait cursor
  QApplication::setOverrideCursor(Qt::WaitCursor);
  std::vector<Point_2> points;
  points.reserve(number_of_points);
  for(int i = 0; i < number_of_points; ++i){
    points.push_back(*pg++);
  }
  for(std::vector<Point_2>::iterator it = points.begin(); it != points.end(); it++)
  {
      Delaunay::Vertex_handle vh = dt.insert(*it);
      int c = rand() % (sizeof(randomColors) / sizeof(randomColors[0]));
      vh->info().color = randomColors[c];
  }
  // default cursor
  QApplication::restoreOverrideCursor();
  emit(changed());
}


void
MainWindow::on_actionLoadPoints_triggered()
{
  QString fileName = QFileDialog::getOpenFileName(this,
						  tr("Open Points file"),
						  ".");
  if(! fileName.isEmpty()){
    open(fileName);
  }
}


void
MainWindow::open(QString fileName)
{
  // wait cursor
  QApplication::setOverrideCursor(Qt::WaitCursor);
  std::ifstream ifs(qPrintable(fileName));
  
  K::Point_2 p;
  std::vector<K::Point_2> points;
  while(ifs >> p) {
    points.push_back(p);
  }
  dt.insert(points.begin(), points.end());

  // default cursor
  QApplication::restoreOverrideCursor();
  this->addToRecentFiles(fileName);
  actionRecenter->trigger();
  emit(changed());
    
}

void
MainWindow::on_actionSavePoints_triggered()
{
  QString fileName = QFileDialog::getSaveFileName(this,
						  tr("Save points"),
						  ".");
  if(! fileName.isEmpty()){
    std::ofstream ofs(qPrintable(fileName));
    for(Delaunay::Finite_vertices_iterator 
          vit = dt.finite_vertices_begin(),
          end = dt.finite_vertices_end();
        vit!= end; ++vit)
    {
      ofs << vit->point() << std::endl;
    }
  }
}


void
MainWindow::on_actionRecenter_triggered()
{
  this->graphicsView->setSceneRect(dgi->boundingRect());
  this->graphicsView->fitInView(dgi->boundingRect(), Qt::KeepAspectRatio);  
}

CGAL::Color MainWindow::getSelectedColor()
{
    return selectedColor;
}


#include "Delaunay_triangulation_2.moc"

int main(int argc, char **argv)
{
  QApplication app(argc, argv);

  app.setOrganizationDomain("geometryfactory.com");
  app.setOrganizationName("GeometryFactory");
  app.setApplicationName("Delaunay_triangulation_2 demo");

  // Import resources from libCGALQt4.
  // See http://doc.trolltech.com/4.4/qdir.html#Q_INIT_RESOURCE
  Q_INIT_RESOURCE(File);
  Q_INIT_RESOURCE(Triangulation_2);
  Q_INIT_RESOURCE(Input);
  Q_INIT_RESOURCE(CGAL);

  MainWindow mainWindow;
  mainWindow.show();

  QStringList args = app.arguments();
  args.removeAt(0);
  Q_FOREACH(QString filename, args) {
    mainWindow.open(filename);
  }

  return app.exec();
}
