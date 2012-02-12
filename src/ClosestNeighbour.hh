

template <typename DT>
class ClosestNeighbour 
{
    public:
    typedef typename DT::Face_handle Face_handle;
    typedef typename DT::Vertex_handle Vertex_handle;
    typedef typename DT::Point Point;
    
    ClosestNeighbour(Vertex_handle origin) : originVertex(origin)
    {
    }
    
    ClosestNeighbour()
    {
    }
    
    void setOrigin(Vertex_handle origin)
    {
        originVertex = origin;
    }
    
    void insert(Vertex_handle vertexToAdd)
    {
        Point origin = originVertex -> point();
        Point pointToAdd = vertexToAdd -> point();
        double distance = (pointToAdd - originVertex).squared_length();
        if(allreadyInsertedPoints.find(vertexToAdd->point()) != allreadyInsertedPoints.end())
        {
            return;
        }
        untreatedNeighbours.insert(std::pair<double, Vertex_handle>(distance, vertexToAdd));
        allreadyInsertedPoints.insert(pointToAdd);
    }
    
    Vertex_handle getNextNeighbour()
    {
        Vertex_handle nearNeighbour = untreatedNeighbours.begin()->second();
        return nearNeighbour;
    }
    
    private:
    Vertex_handle originVertex;
    std::map<double, Vertex_handle> untreatedNeighbours;
    std::set<Point> allreadyInsertedPoints;
    
};
