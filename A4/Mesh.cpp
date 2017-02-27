#include <iostream>
#include <fstream>

#include <glm/ext.hpp>

// #include "cs488-framework/ObjFileDecoder.hpp"
#include "Mesh.hpp"

Mesh::Mesh( const std::string& fname )
	: m_vertices()
	, m_faces()
{
	std::string code;
	double vx, vy, vz;
	size_t s1, s2, s3;

	std::ifstream ifs( fname.c_str() );
	while( ifs >> code ) {
		if( code == "v" ) {
			ifs >> vx >> vy >> vz;
			m_vertices.push_back( glm::vec3( vx, vy, vz ) );
		} else if( code == "f" ) {
			ifs >> s1 >> s2 >> s3;
			m_faces.push_back( Triangle( s1 - 1, s2 - 1, s3 - 1 ) );
		}
	}
}

bool Mesh::intersect(const glm::vec3 origin, const glm::vec3 direction, Intersection &inter) const {
    
    // Go through each face of the mesh and check if the ray intersects with that face
    //  Steps to find intersection:
    // 1) Compute the face's normal
    // 2) Determine if the ray and the face are parallel
    // 3) Compute t
    // 4) Determine intersection point from p
    // 5) Do the inside out test to see if the point is on the left side of each of the triangle's edges
    // This means the ray intersects the triangle and the intersection point is INSIDE the triangle's boundaries
    
    //std::cout << "num faces " << m_faces.size() << std::endl;
    
    double min_t = std::numeric_limits<double>::infinity();
    
    for(Triangle face: m_faces){
        
        //std::cout << "checking faces" << std::endl;
        
        glm::vec3 p0 = m_vertices[face.v1];
        glm::vec3 p1 = m_vertices[face.v2];
        glm::vec3 p2 = m_vertices[face.v3];
        
        // 1) Compute the face's normal
        glm::vec3 face_normal = glm::cross((p1-p0),(p2-p0));
        
        // Find the point of intersection
        // 2) Determine if the ray and the face are parallel
        // 0 or almost 0
        if (fabs(glm::dot(face_normal, direction)) < std::numeric_limits<double>::epsilon()){
            continue;
        }
        
        // 3) Compute t
        double t = glm::dot(face_normal, p0 - origin) / glm::dot(face_normal, direction);
        
        // Check if the triangle is behind the ray
        // Also check if another face is closer (ie has a smaller t)
        if (t < 0 || min_t < t){
            continue;
        }
        
        // 4) Determine intersection point P
        glm::vec3 P = origin + t*direction;
        
        // 5) Inside out test
        // Vector that is perpendicular to the triangle's plane
        glm::vec3 C;
        
        // Side 0 p1 - p0
        glm::vec3 edge0 = p1 - p0;
        glm::vec3 vp0 = P - p0;
        C = glm::cross(edge0, vp0);
        // If the dot product of the normal and the perpendicular vector is < 0
        // then it is on the right side instead of the left
        if (glm::dot(face_normal, C) < 0){
            continue;
        }
        
        // Side 1 p2 - p1
        glm::vec3 edge1 = p2 - p1;
        glm::vec3 vp1 = P - p1;
        C = glm::cross(edge1, vp1);
        // If the dot product of the normal and the perpendicular vector is < 0
        // then it is on the right side instead of the left
        if (glm::dot(face_normal, C) < 0){
            continue;
        }
        
        // Side 2 p0 - p2
        glm::vec3 edge2 = p0 - p2;
        glm::vec3 vp2 = P - p2;
        C = glm::cross(edge2, vp2);
        // If the dot product of the normal and the perpendicular vector is < 0
        // then it is on the right side instead of the left
        if (glm::dot(face_normal, C) < 0){
            continue;
        }
        
        // Ray is in the face
        inter.inter_point = P;
        inter.inter_normal = glm::normalize(face_normal);
        min_t = t;
        return true;
    }
    
    return false;
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  out << "mesh {";
  
  /*(
  for( size_t idx = 0; idx < mesh.m_verts.size(); ++idx ) {
  	const MeshVertex& v = mesh.m_verts[idx];
  	out << glm::to_string( v.m_position );
	if( mesh.m_have_norm ) {
  	  out << " / " << glm::to_string( v.m_normal );
	}
	if( mesh.m_have_uv ) {
  	  out << " / " << glm::to_string( v.m_uv );
	}
  }
*/

  out << "}";
  return out;
}
