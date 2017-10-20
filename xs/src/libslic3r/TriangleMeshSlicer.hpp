#ifndef slic3r_TriangleMeshSlicer_hpp_
#define slic3r_TriangleMeshSlicer_hpp_

#include "libslic3r.h"
#include <vector>

#include "Line.hpp"
#include "Point.hpp"
#include "Polygon.hpp"
#include "ExPolygon.hpp"

namespace Slic3r {

class TriangleMesh;

enum FacetEdgeType { feNone, feTop, feBottom, feHorizontal };

class IntersectionPoint : public Point
{
    public:
    int point_id;
    int edge_id;
    IntersectionPoint() : point_id(-1), edge_id(-1) {};
};

class IntersectionLine : public Line
{
    public:
    int             a_id;
    int             b_id;
    int             edge_a_id;
    int             edge_b_id;
    FacetEdgeType   edge_type;
    bool            skip;
    IntersectionLine() : a_id(-1), b_id(-1), edge_a_id(-1), edge_b_id(-1), edge_type(feNone), skip(false) {};
};

typedef std::vector<IntersectionLine> IntersectionLines;
typedef std::vector<IntersectionLine*> IntersectionLinePtrs;

/// \brief Class for processing TriangleMesh objects.
template <Axis A>
class TriangleMeshSlicer
{
    public:
    TriangleMesh* mesh;
    TriangleMeshSlicer(TriangleMesh* _mesh);
    ~TriangleMeshSlicer();
    void slice(const std::vector<float> &z, std::vector<Polygons>* layers) const;
    void slice(const std::vector<float> &z, std::vector<ExPolygons>* layers) const;
    void slice(float z, ExPolygons* slices) const;
    void slice_facet(float slice_z, const stl_facet &facet, const int &facet_idx,
        const float &min_z, const float &max_z, std::vector<IntersectionLine>* lines,
        boost::mutex* lines_mutex = NULL) const;

	/// \brief Splits the current mesh into two parts.
	/// \param[in] z Coordinate plane to cut along.
	/// \param[out] upper TriangleMesh object to add the mesh > z. NULL suppresses saving this.
	/// \param[out] lower TriangleMesh object to save the mesh < z. NULL suppresses saving this.
    void cut(float z, TriangleMesh* upper, TriangleMesh* lower) const;

    private:
    typedef std::vector< std::vector<int> > t_facets_edges;
    t_facets_edges facets_edges;
    stl_vertex* v_scaled_shared;
    void _slice_do(size_t facet_idx, std::vector<IntersectionLines>* lines, boost::mutex* lines_mutex, const std::vector<float> &z) const;
    void _make_loops_do(size_t i, std::vector<IntersectionLines>* lines, std::vector<Polygons>* layers) const;
    void make_loops(std::vector<IntersectionLine> &lines, Polygons* loops) const;
    void make_expolygons(const Polygons &loops, ExPolygons* slices) const;
    void make_expolygons_simple(std::vector<IntersectionLine> &lines, ExPolygons* slices) const;
    void make_expolygons(std::vector<IntersectionLine> &lines, ExPolygons* slices) const;

    float& _x(stl_vertex &vertex) const;
    float& _y(stl_vertex &vertex) const;
    float& _z(stl_vertex &vertex) const;
    const float& _x(stl_vertex const &vertex) const;
    const float& _y(stl_vertex const &vertex) const;
    const float& _z(stl_vertex const &vertex) const;
};

template<> inline float& TriangleMeshSlicer<X>::_x(stl_vertex &vertex) const { return vertex.y; }
template<> inline float& TriangleMeshSlicer<X>::_y(stl_vertex &vertex) const { return vertex.z; }
template<> inline float& TriangleMeshSlicer<X>::_z(stl_vertex &vertex) const { return vertex.x; }
template<> inline float const& TriangleMeshSlicer<X>::_x(stl_vertex const &vertex) const { return vertex.y; }
template<> inline float const& TriangleMeshSlicer<X>::_y(stl_vertex const &vertex) const { return vertex.z; }
template<> inline float const& TriangleMeshSlicer<X>::_z(stl_vertex const &vertex) const { return vertex.x; }

template<> inline float& TriangleMeshSlicer<Y>::_x(stl_vertex &vertex) const { return vertex.z; }
template<> inline float& TriangleMeshSlicer<Y>::_y(stl_vertex &vertex) const { return vertex.x; }
template<> inline float& TriangleMeshSlicer<Y>::_z(stl_vertex &vertex) const { return vertex.y; }
template<> inline float const& TriangleMeshSlicer<Y>::_x(stl_vertex const &vertex) const { return vertex.z; }
template<> inline float const& TriangleMeshSlicer<Y>::_y(stl_vertex const &vertex) const { return vertex.x; }
template<> inline float const& TriangleMeshSlicer<Y>::_z(stl_vertex const &vertex) const { return vertex.y; }

template<> inline float& TriangleMeshSlicer<Z>::_x(stl_vertex &vertex) const { return vertex.x; }
template<> inline float& TriangleMeshSlicer<Z>::_y(stl_vertex &vertex) const { return vertex.y; }
template<> inline float& TriangleMeshSlicer<Z>::_z(stl_vertex &vertex) const { return vertex.z; }
template<> inline float const& TriangleMeshSlicer<Z>::_x(stl_vertex const &vertex) const { return vertex.x; }
template<> inline float const& TriangleMeshSlicer<Z>::_y(stl_vertex const &vertex) const { return vertex.y; }
template<> inline float const& TriangleMeshSlicer<Z>::_z(stl_vertex const &vertex) const { return vertex.z; }

}

#endif
