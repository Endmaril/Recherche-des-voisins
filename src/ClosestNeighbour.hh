

template <typename DT>
class ClosestNeighbour 
{
    public:
    typedef typename DT::Face_handle Face_handle;
    typedef typename DT::Vertex_handle Vertex_handle;
    typedef typename DT::Point Point;
    
    ClosestNeighbour(Vertex_handle origin) : originVertex(origin)
    {
        allreadyInsertedPoints.insert(origin->point());
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
        double distance = (pointToAdd - origin).squared_length();

        if(allreadyInsertedPoints.count(vertexToAdd->point()) > 0)
        {
            return;
        }
        untreatedNeighbours.insert(std::pair<double, Vertex_handle>(distance, vertexToAdd));
        allreadyInsertedPoints.insert(pointToAdd);
    }
    
    bool getNextNeighbour(Vertex_handle& nearNeighbour)
    {
        if(untreatedNeighbours.empty())
            return false;

        nearNeighbour = untreatedNeighbours.begin()->second;
        untreatedNeighbours.erase(untreatedNeighbours.begin());
        return true;
    }
    
    private:
    Vertex_handle originVertex;
    std::map<double, Vertex_handle> untreatedNeighbours;
    std::set<Point> allreadyInsertedPoints;
    
};
