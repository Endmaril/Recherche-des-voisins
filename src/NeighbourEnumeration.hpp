#include "NeighbourEnumeration.h"

namespace CGAL {
namespace Qt {

template <typename T>
NeighbourEnumeration<T>::NeighbourEnumeration(T * dt_,
							  MainWindow* parent)
  :  GraphicsViewInput(parent), dt(dt_), pMainWindow(parent)
{}

template<typename T>
void NeighbourEnumeration<T>::findNeighborVertices(ClosestNeighbour<T>& closestEnumeration, Vertex_handle vhOrigin)
{
  Face_handle fh = vhOrigin->face();
  Vertex_handle vh0 = fh->vertex(fh->ccw(fh->index(vhOrigin))),
                vh = vh0;
  if(dt->is_infinite(vh))
  {
    fh = fh->neighbor(fh->index(vh));
    vh0 = vh = fh->vertex(fh->ccw(fh->index(vhOrigin)));
  }

  do
  {
    if(!dt->is_infinite(vh))
    {
      closestEnumeration.insert(vh);
    }

    fh = fh->neighbor(fh->index(vh));
    vh = fh->vertex(fh->ccw(fh->index(vhOrigin)));
  } while(vh->point() != vh0->point());
}

template <typename T>
void 
NeighbourEnumeration<T>::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  if(dt->number_of_vertices() < 3)
      return;
  typename T::Point p = typename T::Point(event->scenePos().x(), event->scenePos().y());
  Vertex_handle vh = dt->nearest_vertex(p);
  ClosestNeighbour<T> closestEnumeration(vh);
  bool res;

  for(typename T::Finite_vertices_iterator it = dt->finite_vertices_begin();
          it != dt->finite_vertices_end();
          ++it){
      it->info().order = -1;
  }

  int increment = 0;
  do
  {
    findNeighborVertices(closestEnumeration, vh);
    res = closestEnumeration.getNextNeighbour(vh);
    if(res)
    {
        vh->info().order = increment++;
    }
  } while(res && vh->info().color != pMainWindow->getSelectedColor());

  emit(modelChanged());
}

template <typename T>
bool 
NeighbourEnumeration<T>::eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() == QEvent::GraphicsSceneMousePress) {
    QGraphicsSceneMouseEvent *mouseEvent = static_cast<QGraphicsSceneMouseEvent *>(event);
    mousePressEvent(mouseEvent);
    return true;
  } else{
    // standard event processing
    return QObject::eventFilter(obj, event);
  }
} 


} // namespace Qt
} // namespace CGAL
