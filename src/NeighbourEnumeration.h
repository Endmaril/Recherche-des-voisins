
#ifndef CGAL_QT_NEIGHBOUR_ENUMERATION
#define CGAL_QT_NEIGHBOUR_ENUMERATION

#include <CGAL/Qt/GraphicsViewInput.h>
#include <QGraphicsSceneMouseEvent>
#include <QEvent>
#include <list>

#include "ClosestNeighbour.hh"
//~ #include "Delaunay_triangulation_2.hh"

class MainWindow;

namespace CGAL {
namespace Qt {

template <typename DT>
class NeighbourEnumeration : public GraphicsViewInput
{
public:
  typedef typename DT::Face_handle Face_handle;
  typedef typename DT::Vertex_handle Vertex_handle;
  typedef typename DT::Point Point;

  NeighbourEnumeration(DT  * dt_, MainWindow* parent);

protected:
  void findNeighborVertices(ClosestNeighbour<DT>& closestEnumeration, Vertex_handle vhOrigin);
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  bool eventFilter(QObject *obj, QEvent *event);

  DT * dt;
  bool movePointToInsert;
  bool insertedPoint;
  MainWindow* pMainWindow;
};

}
}

#endif // CGAL_QT_NEIGHBOUR_ENUMERATION
