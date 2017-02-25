#include "Primitive.hpp"
#include "polyroots.hpp"

Primitive::~Primitive()
{
}

Sphere::~Sphere()
{
}


bool Sphere::intersect(const glm::vec4 origin, const glm::vec4 direction) const
{
    NonhierSphere sphere(glm::vec3(0.0, 0.0, 0.0), 1.0);
    return sphere.intersect(origin, direction);
}

Cube::~Cube()
{
}

bool Cube::intersect(const glm::vec4 origin, const glm::vec4 direction) const
{
    NonhierBox box(glm::vec3(0.0, 0.0, 0.0), 1.0);
    return box.intersect(origin, direction);
}

NonhierSphere::~NonhierSphere()
{
}

bool NonhierSphere::intersect(const glm::vec4 origin, const glm::vec4 direction) const{
    
    double roots[2];
    glm::vec3 og = glm::vec3(origin);
    glm::vec3 di = glm::vec3(direction);
    
    // solve for t = (-B +- sqrt(B^2 - 4AC))/(2A)
    // A = (b-a)(b-a)
    // B = 2(b-a)(a-c)
    // C = (a-c)(a-c) - R^2
    // where a = origin of ray
    //       b = direction + origin
    //       c = centre of sphere (m_pos)
    //       R = radius of sphere
    
    // (b-a)(b-a) == direction * direction
    double A = glm::dot(di, di);
    // 2(b-a)(a-c) == 2 * direction * (origin - centre)
    double B = 2.0 * glm::dot(di, og - m_pos);
    // (a-c)(a-c) - R^2 == (origin - centre) (origin - centre) - R*R
    double C = glm::dot(og - m_pos, og - m_pos) - m_radius*m_radius;
    
    size_t num_roots = quadraticRoots(A, B, C, roots);
    
    // If roots == 0, we missed the sphere and there are no intersections
    // If roots == 1, the line is tangent to the sphere and thus just hits it
    // If roots == 2, the line goes through the sphere
    //   -> take the smaller of the roots (t) because we only want the front part where the ray hits not the back
    if (num_roots == 1){
        // Check if t > 0 (ie make sure in front of eye)
        if (roots[0] > 0) return true;
    } else if (num_roots == 2){
        // find the smaller of the two ts
        // check if the min is < 0
        // -> if it is and the other root is > 0 then the eye is IN the sphere
        double min = (roots[0] < roots[1] ? roots[0] : roots[1]);
        double other = (min == roots[0] ? roots[1] : roots[0]);
        if (min > 0) return true;
        if (min < 0 && other > 0) return true;
    }
    
    return false;
}

NonhierBox::~NonhierBox()
{
}

bool NonhierBox::intersect(const glm::vec4 origin, const glm::vec4 direction) const{
    return false;
}
