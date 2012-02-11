#ifndef CGAL_QT_TRIANGULATION_LOCATE_H
#define CGAL_QT_TRIANGULATION_LOCATE_H

#include <QGraphicsSceneMouseEvent> 
#include <QGraphicsScene>
#include <QEvent>
#include <QPen>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Qt/GraphicsViewInput.h>
#include <CGAL/Qt/Converter.h>

namespace CGAL {
namespace Qt {

template <typename DT>
class TriangulationLocate : public GraphicsViewInput
{
public:
  TriangulationLocate(QGraphicsScene* s, DT  * dt_, QObject* parent);
  ~TriangulationLocate();
 
  void setBrush(const QColor& color);

  void show();
  void hide();

protected:

  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  bool eventFilter(QObject *obj, QEvent *event);

private:

  DT * dt;
  typedef typename DT::Geom_traits K;
  typename DT::Vertex_handle hint;
  typename DT::Face_handle fh;
  Converter<K> convert;
  QGraphicsScene *scene_;
  QGraphicsPolygonItem* triangle;

};


template <typename T>
TriangulationLocate<T>::TriangulationLocate(QGraphicsScene* s,
                                            T * dt_,
                                            QObject* parent)
  :  GraphicsViewInput(parent), dt(dt_), scene_(s)
{
  hint = dt->infinite_vertex();
  triangle = new QGraphicsPolygonItem();
  triangle->hide();
  scene_->addItem(triangle);
}

template <typename T>
TriangulationLocate<T>::~TriangulationLocate()
{
}

template <typename T>
void
TriangulationLocate<T>::setBrush(const QColor& color)
{
  triangle->setBrush(color);
}

template <typename T>
void
TriangulationLocate<T>::show()
{
  triangle->show();
}

template <typename T>
void
TriangulationLocate<T>::hide()
{
  triangle->hide();
}


template <typename T>
void 
TriangulationLocate<T>::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  if(dt->dimension() != 2){
    triangle->hide();
    return;
  }
  typename T::Point p = typename T::Point(event->scenePos().x(), event->scenePos().y());
  fh = dt->infinite_vertex()->face(); 
  fh = fh->neighbor(fh->index(dt->infinite_vertex()));
  typename T::Geom_traits::Triangle_2 t=dt->triangle(fh);
  triangle->setPolygon(convert(t));
  triangle->show();
  hint = fh->vertex(0);
}


template <typename T>
bool 
TriangulationLocate<T>::eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() == QEvent::GraphicsSceneMouseMove) {
    QGraphicsSceneMouseEvent *mouseEvent = static_cast<QGraphicsSceneMouseEvent *>(event);
    mouseMoveEvent(mouseEvent);
    return false; // don't consume the event
  } else{
    // standard event processing
    return QObject::eventFilter(obj, event);
  }
} 

} // namespace Qt
} // namespace CGAL

#endif // CGAL_QT_TRIANGULATION_LOCATE_H
