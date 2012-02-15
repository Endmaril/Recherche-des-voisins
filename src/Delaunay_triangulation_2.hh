#ifndef DELAUNAY_TRIANGULATION_2_HH
#define DELAUNAY_TRIANGULATION_2_HH

#include <fstream>

// CGAL headers
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <CGAL/point_generators_2.h>

// Qt headers
#include <QtGui>
#include <QString>
#include <QActionGroup>
#include <QFileDialog>
#include <QInputDialog>

// GraphicsView items and event filters (input classes)
#include "TriangulationCircumcircle.h"
#include "TriangulationLocate.h"
#include "NeighbourEnumeration.h"
#include "TriangulationConflictZone.h"
#include "TriangulationRemoveVertex.h"
#include "TriangulationPointInputAndConflictZone.h"
#include "TriangulationGraphicsColoredItem.h"
#include <CGAL/Qt/VoronoiGraphicsItem.h>

// for viewportsBbox
#include <CGAL/Qt/utility.h>
  
// the two base classes
#include "ui_Delaunay_triangulation_2.h"
#include <CGAL/Qt/DemosMainWindow.h>

//~ namespace CGAL{
//~ namespace Qt{
//~ template<class T>
//~ class NeighbourEnumeration;
//~ }
//~ }

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef K::Iso_rectangle_2 Iso_rectangle_2;

typedef CGAL::Triangulation_vertex_base_with_info_2<CGAL::Color, K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Delaunay;

class MainWindow :
  public CGAL::Qt::DemosMainWindow,
  public Ui::Delaunay_triangulation_2
{
  Q_OBJECT
  
private:  
  CGAL::Color selectedColor;
  Delaunay dt; 
  QGraphicsScene scene;  

  CGAL::Qt::TriangulationGraphicsColoredItem<Delaunay> * dgi;
  CGAL::Qt::VoronoiGraphicsItem<Delaunay> * vgi;

  CGAL::Qt::NeighbourEnumeration<Delaunay> * ne;
  CGAL::Qt::TriangulationConflictZone<Delaunay> * cz;
  CGAL::Qt::TriangulationRemoveVertex<Delaunay> * trv;
  CGAL::Qt::TriangulationPointInputAndConflictZone<Delaunay> * pi;
  CGAL::Qt::TriangulationCircumcircle<Delaunay> *tcc;
  CGAL::Qt::TriangulationLocate<Delaunay> *tl;

public:
  MainWindow();
  CGAL::Color getSelectedColor();

public slots:

  void processInput(CGAL::Object o);

  void on_actionSetColorRed_toggled(bool checked);

  void on_actionShowConflictZone_toggled(bool checked);

  void on_actionSetColorBlue_toggled(bool checked);

  void on_actionSetColorBlack_toggled(bool checked);

  void on_actionInsertPoint_toggled(bool checked);

  void on_actionSelectPoint_toggled(bool checked);

  void on_actionSetColorGreen_toggled(bool checked);
  
  void on_actionInsertRandomPoints_triggered();

  void on_actionLoadPoints_triggered();

  void on_actionSavePoints_triggered();

  void on_actionClear_triggered();

  void on_actionRecenter_triggered();

  virtual void open(QString fileName);

signals:
  void changed();
};
#endif
