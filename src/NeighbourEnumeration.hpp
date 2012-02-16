#include "NeighbourEnumeration.h"

namespace CGAL {
namespace Qt {

template <typename T>
NeighbourEnumeration<T>::NeighbourEnumeration(T * dt_,
							  MainWindow* parent)
  :  GraphicsViewInput(parent), dt(dt_), /*vh(),*/ movePointToInsert(false), insertedPoint(false), pMainWindow(parent)
{}


/*
template <typename T>
void 
NeighbourEnumeration<T>::localize_and_insert_point(QPointF qt_point)
{
  Point p(qt_point.x(), qt_point.y());
  typename T::Locate_type lt;
  int li;
  Face_handle fh = (vh == Vertex_handle()) ? Face_handle() : vh->face();
  fh = dt->locate(p, lt, li, fh); // fh serves as a hint
  if(lt != T::VERTEX){
    vh = dt->insert(p, lt, fh, li);
    insertedPoint = true;
    emit(modelChanged());
  } else {
    vh = fh->vertex(0);
    insertedPoint = false;
  }
}
*/

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
  typename T::Point p = typename T::Point(event->scenePos().x(), event->scenePos().y());
  Vertex_handle vh = dt->nearest_vertex(p);
  std::cout << vh->point() << std::endl;
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
}


template <typename T>
void 
NeighbourEnumeration<T>::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

  //if(! movePointToInsert) return;

  // fh will be destroyed by the removal of vh.
  // Let us take a neighbor that is not in the star of vh.
  //const Face_handle fh = vh->face();
  //Vertex_handle next_hint = fh->vertex((fh->index(vh)+1)&3);
  //if(insertedPoint){
  //  dt->remove(vh);
  //}
  //vh = next_hint;
  //localize_and_insert_point(event->scenePos());
}


template <typename T>
void 
NeighbourEnumeration<T>::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  /*if(! movePointToInsert ||
     event->button() != ::Qt::LeftButton) {
    return;
  }

  if(insertedPoint){
    dt->remove(vh);
  }
  vh = Vertex_handle();
  */
  emit(modelChanged());
 
  //movePointToInsert = false;
}



template <typename T>
bool 
NeighbourEnumeration<T>::eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() == QEvent::GraphicsSceneMousePress) {
    QGraphicsSceneMouseEvent *mouseEvent = static_cast<QGraphicsSceneMouseEvent *>(event);
    mousePressEvent(mouseEvent);
    return true;
  } else if (event->type() == QEvent::GraphicsSceneMouseMove) {
    QGraphicsSceneMouseEvent *mouseEvent = static_cast<QGraphicsSceneMouseEvent *>(event);
    mouseMoveEvent(mouseEvent);
    return false; // do not eat move event!
  } else if (event->type() == QEvent::GraphicsSceneMouseRelease) {
    QGraphicsSceneMouseEvent *mouseEvent = static_cast<QGraphicsSceneMouseEvent *>(event);
    mouseReleaseEvent(mouseEvent);
    return true;
  } else{
    // standard event processing
    return QObject::eventFilter(obj, event);
  }
} 


} // namespace Qt
} // namespace CGAL
