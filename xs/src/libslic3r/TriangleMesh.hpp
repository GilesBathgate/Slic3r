#ifndef slic3r_TriangleMesh_hpp_
#define slic3r_TriangleMesh_hpp_

#include "libslic3r.h"
#include <admesh/stl.h>
#include <vector>
#include "BoundingBox.hpp"
#include "Line.hpp"
#include "Point.hpp"
#include "Polygon.hpp"
#include "ExPolygon.hpp"
#include "TriangleMeshSlicer.hpp"

namespace Slic3r {

class TriangleMesh;
typedef std::vector<TriangleMesh*> TriangleMeshPtrs;

class TriangleMesh
{
    public:
    TriangleMesh();
    TriangleMesh(const Pointf3s &points, const std::vector<Point3> &facets);
    TriangleMesh(const TriangleMesh &other);
    TriangleMesh& operator= (TriangleMesh other);
    void swap(TriangleMesh &other);
    ~TriangleMesh();
    void ReadSTLFile(const std::string &input_file);
    void write_ascii(const std::string &output_file);
    void write_binary(const std::string &output_file);
    void repair();
    void check_topology();
    float volume();
    bool is_manifold() const;
    void WriteOBJFile(const std::string &output_file);
    void scale(float factor);
    void scale(const Pointf3 &versor);
    void translate(float x, float y, float z);
    void rotate(float angle, const Axis &axis);
    void rotate_x(float angle);
    void rotate_y(float angle);
    void rotate_z(float angle);
    void mirror(const Axis &axis);
    void mirror_x();
    void mirror_y();
    void mirror_z();
    void align_to_origin();
    void center_around_origin();
    void rotate(double angle, Point* center);
    TriangleMeshPtrs split() const;
    TriangleMeshPtrs cut_by_grid(const Pointf &grid) const;
    void merge(const TriangleMesh &mesh);
    ExPolygons horizontal_projection() const;
    Polygon convex_hull();
    BoundingBoxf3 bounding_box() const;
    void reset_repair_stats();
    bool needed_repair() const;
    size_t facets_count() const;
    void extrude_tin(float offset);
    void require_shared_vertices();
    void reverse_normals();
	
	/// Generate a mesh representing a cube with dimensions (x, y, z), with one corner at (0,0,0).
    static TriangleMesh make_cube(double x, double y, double z);
	
	/// Generate a mesh representing a cylinder of radius r and height h, with the base at (0,0,0). 
	/// param[in] r Radius 
	/// param[in] h Height 
	/// param[in] fa Facet angle. A smaller angle produces more facets. Default value is 2pi / 360.  
    static TriangleMesh make_cylinder(double r, double h, double fa=(2*PI/360));
	
	/// Generate a mesh representing a sphere of radius rho, centered about (0,0,0). 
	/// param[in] rho Distance from center to the shell of the sphere. 
	/// param[in] fa Facet angle. A smaller angle produces more facets. Default value is 2pi / 360.  
    static TriangleMesh make_sphere(double rho, double fa=(2*PI/360));
    
    stl_file stl;
	/// Whether or not this mesh has been repaired.
    bool repaired;
    
    private:
    friend class TriangleMeshSlicer<X>;
    friend class TriangleMeshSlicer<Y>;
    friend class TriangleMeshSlicer<Z>;
};


}

#endif
