#include "Primitive.hpp"
#include "polyroots.hpp"

Primitive::~Primitive()
{
}

Sphere::~Sphere()
{
}


bool Sphere::intersect(const glm::vec3 origin, const glm::vec3 direction, Intersection &inter) const
{
    NonhierSphere sphere(glm::vec3(0.0, 0.0, 0.0), 1.0);
    return sphere.intersect(origin, direction, inter);
}

Cube::~Cube()
{
}

bool Cube::intersect(const glm::vec3 origin, const glm::vec3 direction, Intersection &inter) const
{
    NonhierBox box(glm::vec3(0.0, 0.0, 0.0), 1.0);
    return box.intersect(origin, direction, inter);
}

NonhierSphere::~NonhierSphere()
{
}

bool NonhierSphere::intersect(const glm::vec3 origin, const glm::vec3 direction, Intersection &inter) const{
    
    double roots[2];
    glm::vec3 og = origin;
    glm::vec3 di = direction;
    
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
        if (roots[0] > 0) {
            inter.inter_point = og + (float)roots[0]*di;
            inter.inter_normal = glm::normalize(inter.inter_point - m_pos);
            return true;
        }
    } else if (num_roots == 2){
        // find the smaller of the two ts
        // check if the min is < 0
        // -> if it is and the other root is > 0 then the eye is IN the sphere
        double min = (roots[0] < roots[1] ? roots[0] : roots[1]);
        double other = (min == roots[0] ? roots[1] : roots[0]);
        if (min > 0) {
            inter.inter_point = og + (float)min*di;
            inter.inter_normal = glm::normalize(inter.inter_point - m_pos);
            return true;
        }
        if (min < 0 && other > 0) {
            inter.inter_point = og + (float)other*di;
            inter.inter_normal = glm::normalize(inter.inter_point - m_pos);
            return true;
        }
    }
    
    return false;
}

NonhierBox::~NonhierBox()
{
}

bool NonhierBox::intersect(const glm::vec3 origin, const glm::vec3 direction, Intersection &inter) const{
    
    glm::vec3 min_bound = m_pos;
    glm::vec3 max_bound = m_pos + glm::vec3(m_size);
    
    double tmin;
    double tmax;
    
    glm::vec3 min_normal = glm::vec3();
    glm::vec3 max_normal = glm::vec3();
    
    // Get the two values for t where t intersects the x plane of the box
    double tx_min = (min_bound.x - origin.x) / direction.x ;
    double tx_max = (max_bound.x - origin.x) / direction.x ;
    
    // If min is bigger than max, swap them
    if (tx_min > tx_max){
        double tx_min_tmp = tx_min;
        tx_min = tx_max;
        tx_max = tx_min_tmp;
    }
    
    // Get the two values for t where tintersects the y plane of the box
    double ty_min = (min_bound.y - origin.y) / direction.y;
    double ty_max = (max_bound.y - origin.y) / direction.y ;
    
    // If min is bigger than max, swap them
    if (ty_min > ty_max){
        double ty_min_tmp = ty_min;
        ty_min = ty_max;
        ty_max = ty_min_tmp;
    }
    
    // Get the two values for t where tintersects the z plane of the box
    double tz_min = (min_bound.z - origin.z) / direction.z ;
    double tz_max = (max_bound.z - origin.z) / direction.z ;
    
    // If min is bigger than max, swap them
    if (tz_min > tz_max){
        double tz_min_tmp = tz_min;
        tz_min = tz_max;
        tz_max = tz_min_tmp;
    }
    
    // Set tmin to tx_min and tmax to tx_max to start
    tmin = tx_min;
    tmax = tx_max;
    if (direction.x < 0.0){
        min_normal = glm::vec3(1.0, 0.0, 0.0);
        max_normal = glm::vec3(1.0, 0.0, 0.0);
    } else {
        min_normal = glm::vec3(-1.0, 0.0, 0.0);
        max_normal = glm::vec3(-1.0, 0.0, 0.0);
    }
    
    // If the smaller t.x is larger than the larger t.y OR
    // the smaller t.y is larger than the larger t.x then
    // the ray misses the box completely
    if ((tmin > ty_max) || (ty_min > tmax)){
        return false;
    }
    
    // Check if the min t.y value is greater than current tmin
    if (ty_min > tmin){
        tmin = ty_min;
        if (direction.y < 0.0){
            min_normal = glm::vec3(0.0, 1.0, 0.0);
        } else {
            min_normal = glm::vec3(0.0, -1.0, 0.0);
        }
    }
    
    // Check if the max t.y value is greater than current tmax
    if (ty_max < tmax){
        tmax = ty_max;
        if (direction.y < 0.0){
            max_normal = glm::vec3(0.0, 1.0, 0.0);
        } else {
            max_normal = glm::vec3(0.0, -1.0, 0.0);
        }
    }
    
    // If the smaller t is larger than the larger t.z OR
    // the smaller t is larger than the larger t.z then
    // the ray misses the box completely
    if ((tmin > tz_max) || (tz_min > tmax)){
        return false;
    }
    
    // Check if the min t.z is greater than tmin
    if (tz_min > tmin){
        tmin = tz_min;
        if (direction.z < 0.0){
            min_normal = glm::vec3(0.0, 0.0, 1.0);
        } else {
            min_normal = glm::vec3(0.0, 0.0, -1.0);
        }
    }
    
    // Check is the max t.z is less than the tmax
    if (tz_max < tmax){
        tmax = tz_max;
        if (direction.z < 0.0){
            max_normal = glm::vec3(0.0, 0.0, 1.0);
        } else {
            max_normal = glm::vec3(0.0, 0.0, -1.0);
        }
    }
    
    
    // Need to check if we are inside the box (aka the back of the box is behind the eye)
    if (tmin < 0){
        if (tmax < 0) return false;
        else {
            tmin = tmax;
            min_normal = max_normal;
        }
    }
    
    // Calculate point of intersection using tmin (the smaller and thus "in front" of the sides
    inter.inter_point = origin + (float)tmin*direction;
    inter.inter_normal = min_normal;
    
    return true;
}
