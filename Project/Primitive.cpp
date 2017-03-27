#include "Primitive.hpp"
#include "polyroots.hpp"

#include <glm/ext.hpp>

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

Cylinder::~Cylinder()
{
}

bool Cylinder::intersect(const glm::vec3 origin, const glm::vec3 direction, Intersection &inter) const {
    NonhierCylinder cylinder(glm::vec3(0.0, 0.0, 0.0), 1.0, 1.0);
    return cylinder.intersect(origin, direction, inter);
}

Cone::~Cone()
{
}

bool Cone::intersect(const glm::vec3 origin, const glm::vec3 direction, Intersection &inter) const
{
    NonhierCone cone(glm::vec3(0.0, 0.0, 0.0), 1.0);
    return cone.intersect(origin, direction, inter);
}

Cube::~Cube()
{
}

bool Cube::intersect(const glm::vec3 origin, const glm::vec3 direction, Intersection &inter) const
{
    NonhierBox box(glm::vec3(0.0, 0.0, 0.0), 1.0);
    return box.intersect(origin, direction, inter);
}

Plane::~Plane()
{
}

bool Plane::intersect(const glm::vec3 origin, const glm::vec3 direction, Intersection &inter) const
{
    // This is a unit plane so position is centered at the origin and the size is 1.0-by-1.0 on the z-axis
    glm::vec3 pos = glm::vec3(0.0, 0.0, 0.0);
    double plane_size = 1.0/2.0 ;
    
    glm::vec3 normal = glm::vec3(0.0, 0.0, 1.0);
    
    // Check to see if line intersects with the infinite z-plane
    // First, make sure denominator isnt zero (or at least close to zero for floating point)
    double denominator = glm::dot(normal, direction);
    if (std::abs(denominator) < (1e-5)) return false;
    
    // Solve for t
    // https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
    double t = glm::dot(normal,(pos - origin)) / denominator;
    
    // Make sure point of intersection is in front of eye
    if (t > 0){

        // Check to see if point of intersection is within the bounded unit plane
        glm::vec3 possible_intersection = origin + (float)t*direction ;
        
        // Outside of the bounded x value
        if ((possible_intersection.x < pos.x-plane_size) ||
            (possible_intersection.x > pos.x+plane_size)){
            return false ;
        } else if ((possible_intersection.y < pos.y-plane_size) ||
                   (possible_intersection.y > pos.y+plane_size))
        {
            // Outside of the bounded y value
            return false ;
        } else {
            // Both x and y are inside the bounds
            inter.inter_point = possible_intersection;
            inter.inter_normal = glm::normalize(normal);
        
            inter.u = 0.5 + inter.inter_point[0];
            inter.v = 1 - (0.5 + inter.inter_point[1]);
            
            // Need the tangent vectors of the surface 
            // Note we are one a plane so the tangent vectors aren't anything special
            glm::vec3 O_u = glm::vec3(1.0, 0.0, 0.0);
            glm::vec3 O_v = glm::vec3(0.0, 1.0, 0.0);
            
            // inter.x = N x O_v
            // inter.y = N x O_u
            inter.x = O_u;
            inter.y = O_v;
            
            return true;
        }
    }
    
    return false ;
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

NonhierCone::~NonhierCone()
{
}

bool NonhierCylinder::intersect(const glm::vec3 origin, const glm::vec3 direction, Intersection &inter) const{
    
    // There are two intersections we need to check:
    // 1) with the curve of the cylinder
    // 2) with the face of the cylinder
    
    // Start with the curve of the cylinder
    
    double roots[2];
    glm::vec3 o = origin - m_pos;
    glm::vec3 d = direction;
    
    // Formula for an infinite cylinder is X^2 + Y^2 = R^2
    // Note: This is NOT the same as cone - this has a radius whereas cone radius is
    // determined by the height
    // A = d.x^2 + d.y^2
    // B = o.x*d.x + o.y*d.y
    // C = o.x^2 + a.y^2 - R^2
    
    double A = pow(d.x, 2) + pow(d.y, 2);
    double B = 2.0*o.x*d.x + 2.0*o.y*d.y ;
    double C = pow(o.x, 2) + pow(o.y, 2) - pow(m_radius, 2);
    
    // Get t
    size_t num_roots = quadraticRoots(A, B, C, roots);
    // Bound the cylinder
    double zmax = m_height / 2.0;
    double zmin = -zmax;
    
    // If no roots, then no intersection with the curve
    // THIS DOES NOT MEAN THERE IS NO INTERSECTION!
    // If one root, intersection is tangent to the cylinder curve
    // If two roots, goes in and out of the curve
    
    if (num_roots == 1){
        // Tangent
        // Check to make sure ray is tangent to a part of the finite cylinder
        double z = o.z + roots[0]*d.z;
        if (zmin <= z && z <= zmax){
            // Check if t > 0 (ie make sure in front of eye)
            if (roots[0] > 0) {
                inter.inter_point = o + (float)roots[0]*d;
                inter.inter_normal = glm::normalize(glm::vec3(inter.inter_point.x, inter.inter_point.y, 0));;
                std::cout << "INTERSECTION CYLINDER!! tangent" << std::endl;
                return true;
            }
        }
        // If there is only one root, but is not tangent to part of the finite cylinder
        return false;
    }
    
    if (num_roots == 2){
        
        // There are two roots, thus two intersections that go between the infinite cyinder
        // Need to check:
        // 1) if both roots go through the curve of the finite cylinder
        // 2) if one of the roots go through the min face
        // 3) if one of the roots go through the max face
        // 4) if both of the roots go through the two faces
        
        double z0 = o.z + roots[0]*d.z ;
        double z1 = o.z + roots[1]*d.z ;
        double zs_min = z0 < z1 ? z0 : z1;
        double zs_max = zs_min == z0 ? z1 : z0 ;
        
        bool z0_in = (zmin <= z0 && z0 <= zmax);
        bool z1_in = (zmin <= z1 && z1 <= zmax);
        double tmin = -INFINITY;
        double other= INFINITY;
        glm::vec3 face_norm = glm::vec3(0.0, 0.0, 1.0);
        bool n_min = false ;
        bool n_other = false;
        
        // Check 4) first
        // This means the smaller z value is less than zmin
        // and the larger z value is greater than zmax
        if (zs_min < zmin && zs_max > zmax) {
            //std::cout << "z0: " << z0 << " z1: " << z1 << std::endl;
            // Neither are in the cylinder, which means the line goes through the two faces
            // These are the t-values for the intersections of the face
            float t_min_face = (zmin - o.z) / d.z ;
            float t_max_face = (zmax - o.z) / d.z ;
            
            // Find the smaller face
            if (t_min_face < t_max_face){
                tmin = t_min_face;
                other = t_max_face;
            } else {
                tmin = t_max_face;
                other = t_min_face;
            }
            
            //std::cout << "tmin: " << tmin << " other: " << other << std::endl;
            
            // Check to see if eye in INSIDE the cone
            if (tmin < 0 && other > 0){
                inter.inter_point = o + (float)other*d;
                inter.inter_normal = glm::normalize(face_norm);
                //std::cout << "INTERSECTION CYLINDER!! inside cylinder" << std::endl;
                //return true;
            }else if (tmin > 0){
                // Looking at the whole cylinder
                inter.inter_point = o + (float)tmin*d;
                inter.inter_normal = glm::normalize(face_norm);
                //std::cout << "INTERSECTION CYLINDER!! outside cylinder" << std::endl;
                return true;
            } else {
                // The cylinder is completely behind us
                //std::cout << "INTERSECTION CYLINDER!! both outside cylinder" << std::endl;
                return false;
            }
            
        } else if (z0_in && z1_in){
            // Line goes through the curve in and out
            // Find the smaller t
            tmin = roots[0] < roots[1] ? roots[0] : roots[1];
            other = tmin == roots[0] ? roots[1] : roots[0];
        } else if (z0_in || z1_in){
            // Line goes through the curve and the face of the cylinder
            
            // Check if line goes through min face
            double t3 = INFINITY;
            if (zs_min < zmin && zs_max >= zmin && zs_max <= zmax){
                t3 = (zmin - o.z) / d.z;
            }
            // Check if line goes through max face
            if (zs_max > zmax && zs_min <= zmax && zs_min >= zmin){
                t3 = (zmax - o.z) / d.z ;
            }
            
            // If z0 is in the curve, then z1 is on the face
            if (z0_in){
                if (roots[0] < t3) {
                    tmin = roots[0];
                    other = t3 ;
                    n_other = true;
                } else {
                    tmin = t3;
                    other = roots[0] ;
                    n_min = true;
                }
            } else {
                if (roots[1] < t3) {
                    tmin = roots[1];
                    other = t3 ;
                    n_other = true;
                } else {
                    tmin = t3;
                    other = roots[1] ;
                    n_min = true;
                }
            }
        } else {
            // This means that the intersection with the curve happened
            // completely on one side of the finite cylinder
            return false;
        }
        
        // Check to see if eye in INSIDE the cone
        if (tmin < 0 && other > 0){
            inter.inter_point = o + (float)other*d;
            if (n_other) {
                inter.inter_normal = glm::normalize(face_norm);
            } else {
                inter.inter_normal = glm::normalize(glm::vec3(inter.inter_point.x, inter.inter_point.y, 0));
            }
            //std::cout << "INTERSECTION CYLINDER!! inside cylinder" << std::endl;
            return true;
        }
        
        // Make sure that tmin is still in view of the eye
        if (tmin > 0){
            inter.inter_point = o + (float)tmin*d;
            if (n_min) {
                inter.inter_normal = glm::normalize(face_norm);
            } else {
                inter.inter_normal = glm::normalize(glm::vec3(inter.inter_point.x, inter.inter_point.y, 0));
            }
            //std::cout << "INTERSECTION CYLINDER!! outside cylinder" << std::endl;
            return true;
        }
        
        return false ;
    }
    
    // Now, need to check if ray hit the faces instead of the cylinder curve
    float t4 = (zmin - o.z) / d.z ;
    float t5 = (zmax - o.z) / d.z ;
    
    // Check that t4 and t5 are within the circle of the cylinder face
    bool t4_result = pow(o.x + t4*d.x, 2) + pow(o.y + t4*d.y, 2) <= pow(m_radius, 2);
    bool t5_result = pow(o.x + t5*d.x, 2) + pow(o.y + t5*d.y, 2) <= pow(m_radius, 2);
    
    if (t4_result && t5_result){
        float tmin = t4 < t5 ? t4 : t5 ;
        float other = t4 == tmin ? t5 : t4 ;
        std::cout << "tmin: " << tmin << " other: " << other << std::endl;
        glm::vec3 norm = glm::vec3(0.0, 0.0, 1.0);
        
        // Check if inside
        if (tmin < 0 && other > 0){
            inter.inter_point = o + other*d;
            inter.inter_normal = glm::normalize(norm);
        } else if (tmin > 0){
            inter.inter_point = o + tmin*d;
            inter.inter_normal = glm::normalize(norm);
        } else {
            return false;
        }
        std::cout << "INTERSECTION CYLINDER!! completely cylinder" << std::endl;
        return true;
    }
    
    return false ;
}

NonhierCylinder::~NonhierCylinder()
{
}

bool NonhierCone::intersect(const glm::vec3 origin, const glm::vec3 direction, Intersection &inter) const
{
    double roots[2];
    glm::vec3 o = origin;
    glm::vec3 d = direction;
    
    //std::cout << "testing for intersection with cone" << std::endl;
    
    // Formula for an infinite cone is X^2 + Y^2 = Z^2
    // solve for t = (-B +- sqrt(B^2 - 4AC))/(2A)
    // A = d.x^2 + d.y^2 - d.z^2
    // B = 2*d.x*o.x + 2*d.y*o.y - 2*d.z*o.z
    // C = o.x^2 + o.y^2 - o.z^2
    // where o = origin of the ray
    //       d = direction - origin
    
    double A = pow(d.x, 2) + pow(d.y, 2) - pow(d.z, 2);
    double B = 2.0*d.x*o.x + 2.0*d.y*o.y - 2.0*d.z*o.z;
    double C = pow(o.x, 2) + pow(o.y, 2) - pow(o.z, 2);
    
    //std::cout << "A: " << A << " B: " << B << " C: " << C << std::endl;
    
    // Get t
    size_t num_roots = quadraticRoots(A, B, C, roots);
    // Bound the cone
    double zmax = 0.0;
    double zmin = -m_height;
    
    //std::cout << "Num roots: " << num_roots << std::endl;
    
    // If roots == 0, we missed the cone curve and there are no intersections
    // If roots == 1, the line is tangent to the cone curve and thus just hits it
    // If roots == 2, the line goes through the cone curve twice
    //   -> take the smaller of the roots (t) because we only want the front part where the ray hits not the back
    
    if (num_roots == 1){
        // Check to make sure ray is tangent to a part of the finite cone
        double z = o.z + roots[0]*d.z;
        if (zmin <= z && z <= zmax){
            // Check if t > 0 (ie make sure in front of eye)
            if (roots[0] > 0) {
                inter.inter_point = o + (float)roots[0]*d;
                // Radius for the current spot
                double mag = sqrt(std::pow(inter.inter_point.x, 2) + pow(inter.inter_point.y, 2));
                double x = inter.inter_point.x / mag ;
                double y = inter.inter_point.y / mag ;
                inter.inter_normal = glm::normalize(glm::vec3(x, y, 1.0));;
                //std::cout << "INTERSECTION CONE!! tangent" << std::endl;
                return true;
            }
        } else {
            return false ;
        }
        
    } else if (num_roots == 2){
    
        double z0 = o.z + (double)roots[0]*d.z;
        double z1 = o.z + (double)roots[1]*d.z;
        
        bool z0_in = zmin <= z0 && z0 <= zmax;
        bool z1_in = zmin <= z1 && z1 <= zmax;
        double tmin = -INFINITY;
        double other = INFINITY;
        bool face_intersected = false;
        bool face_back_inter = false;
        
        // If both z values are within the z_bounds, then ray intersects the curve of the cone twice
        // Thus choose the smaller of the two t's
        if (z0_in && z1_in){
            //std::cout << "BOTH IN: " << std::endl;
            //std::cout << "t1: " << roots[0] << " t2: " << roots[1] << std::endl;
            //std::cout << "z0: " << z0 << " z1: " << z1 << std::endl;
            //std::cout << "zmin: " << zmin << " zmax: " << zmax << std::endl;
            // Find the smaller t
            tmin = roots[0] < roots[1] ? roots[0] : roots[1];
            other = tmin == roots[0] ? roots[1] : roots[0];
        } else if (z0_in || z1_in){
            //std::cout << "The face should be showing" << std::endl;
            //std::cout << "t1: " << roots[0] << " t2: " << roots[1] << std::endl;
            //std::cout << "z0: " << z0 << " z1: " << z1 << std::endl;
            //std::cout << "zmin: " << zmin << " zmax: " << zmax << std::endl;
            
            double z_min = z0 < z1 ? z0 : z1 ;
            double z_max = z_min == z0 ? z1 : z0 ;
            
            // If one z value is in the bounds and the other out, then ray goes through the face
            // Goes through face if ...
            // zmax is larger than zmax and zmin less than zmax but larger than zmin
            // zmin is smaller than zmin and zmax is larger than zmin but smaller than zmax
            
            if ((z_max > zmax && z_min <= zmax && z_min >= zmin)||
                (z_min < zmin && z_max >= zmin && z_max <= zmax)) {
                
                //std::cout << "Goes through face! z0: " << z0 << " z1: " << z1 << " zmin: " << zmin << " zmax: " << zmax << std::endl;
                
                // Get t value for part of the ray that goes through the face
                double t3 = (zmin - o.z)/d.z;
                
                // Find the min t (the t with the smallest value)
                if (z0_in){
                    tmin = roots[0] < t3 ? roots[0] : t3 ;
                    other = tmin == roots[0] ? t3 : roots[0];
                    
                } else {
                    tmin = roots[1] < t3 ? roots[1] : t3 ;
                    other = tmin == roots[1] ? t3 : roots[1];
                }
                
                if (tmin == t3) {
                    face_intersected = true;
                } else {
                    face_back_inter = true;
                }
            } else {
                // THIS CASE SHOULD NEVER HAPPEN
                return false;
            }
        } else {
            // This means the ray doesn't intersect the finite cone anywhere
            return false ;
        }
        
        // Check to see if eye in INSIDE the cone
        if (tmin < 0.0 && other > 0.0){
            
            //std::cout << " this is tmin: " << tmin << " this is other: " << other << std::endl;
            
            inter.inter_point = o + (float)other*d;
            if (face_back_inter){
                inter.inter_normal = glm::normalize(glm::vec3(0.0, 0.0, zmin));
            } else {
                // Radius for the current spot
                double mag = sqrt(std::pow(inter.inter_point.x, 2) + pow(inter.inter_point.y, 2));
                double x = inter.inter_point.x / mag ;
                double y = inter.inter_point.y / mag ;
                inter.inter_normal = glm::normalize(glm::vec3(x, y, 1.0));
            }
            //std::cout << "INTERSECTION CONE!! inside cone" << std::endl;
            return true;
        }
        
        // Make sure that tmin is still in view of the eye
        if (tmin > 0){
            inter.inter_point = o + (float)tmin*d;
            if (face_intersected){
                inter.inter_normal = glm::normalize(glm::vec3(0.0, 0.0, zmin));
            } else {
                // Radius for the current spot
                double mag = sqrt(std::pow(inter.inter_point.x, 2) + pow(inter.inter_point.y, 2));
                double x = inter.inter_point.x / mag ;
                double y = inter.inter_point.y / mag ;
                inter.inter_normal = glm::normalize(glm::vec3(x, y, 1.0));
            }
           // std::cout << "INTERSECTION CONE!! outside cone" << std::endl;
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
        //min_normal = glm::vec3(-1.0, 0.0, 0.0);
        //max_normal = glm::vec3(-1.0, 0.0, 0.0);
        min_normal = glm::vec3(1.0, 0.0, 0.0);
        max_normal = glm::vec3(1.0, 0.0, 0.0);
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
            //min_normal = glm::vec3(0.0, -1.0, 0.0);
            min_normal = glm::vec3(0.0, 1.0, 0.0);
        }
    }
    
    // Check if the max t.y value is greater than current tmax
    if (ty_max < tmax){
        tmax = ty_max;
        if (direction.y < 0.0){
            max_normal = glm::vec3(0.0, 1.0, 0.0);
        } else {
            //max_normal = glm::vec3(0.0, -1.0, 0.0);
            max_normal = glm::vec3(0.0, 1.0, 0.0);
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
            //min_normal = glm::vec3(0.0, 0.0, -1.0);
            min_normal = glm::vec3(0.0, 0.0, 1.0);
        }
    }
    
    // Check is the max t.z is less than the tmax
    if (tz_max < tmax){
        tmax = tz_max;
        if (direction.z < 0.0){
            max_normal = glm::vec3(0.0, 0.0, 1.0);
        } else {
            //max_normal = glm::vec3(0.0, 0.0, -1.0);
            max_normal = glm::vec3(0.0, 0.0, 1.0);
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
    
    if (isnan(tmin)){
        return false;
    }
    
    // Calculate point of intersection using tmin (the smaller and thus "in front" of the sides
    inter.inter_point = origin + (float)tmin*direction;
    inter.inter_normal = min_normal;
    
    // To determine u and v I need to know what plane the ray has intersected of the cube
    // - if it intersects the x plane then the normal will be (1.0, 0.0, 0.0) and I will use y and z to calculate u and v
    // - if it intersects the y plane then the normal will be (0.0, 1.0, 0.0) and I will use x and z to calculate u and v
    // - if it intersects the z plane then the normal will be (0.0, 0.0, 1.0) and I will use x and y to calculate u and v
    
    int index_1, index_2;
    
    if (inter.inter_normal.x >= inter.inter_normal.y && inter.inter_normal.x >= inter.inter_normal.z) {
        index_1 = 1;
        index_2 = 2;
    } else if (inter.inter_normal.y >= inter.inter_normal.x && inter.inter_normal.y >= inter.inter_normal.z){
        index_1 = 0;
        index_2 = 2;
    } else {
        index_1 = 0;
        index_2 = 1;
    }
    
    if (isnan(inter.inter_point[0]) || isnan(inter.inter_point[1])){
        //std::cout << "origin: " << glm::to_string(origin) << std::endl;
        //std::cout << "tmin: " << tmin << std::endl;
        //std::cout << "direction: " << glm::to_string(direction) <<std::endl;
        //std::cout << "interpoint: " << glm::to_string(inter.inter_point) << std::endl;
        //std::cout << "index_1: " << index_1 << " index_2: " << index_2 << std::endl;
        //std::cout << "inder.point[index_1]: " << inter.inter_point[index_1] << " inter.point[index_2]: " << inter.inter_point[index_2] << std::endl;
    }

    inter.u = (inter.inter_point[index_1] - m_pos[index_1]) / m_size;
    inter.v = 1 - ((inter.inter_point[index_2] - m_pos[index_2]) / m_size);
    
    if(inter.inter_normal.z <= inter.inter_normal.x && inter.inter_normal.z <= inter.inter_normal.y) {
        inter.x = glm::vec3(-inter.inter_normal.y, inter.inter_normal.x, 0.0);
    } else if(inter.inter_normal.y <= inter.inter_normal.x){
        inter.x = glm::vec3(-inter.inter_normal.z, 0, inter.inter_normal.x);
    } else {
        inter.x = glm::vec3(0, -inter.inter_normal.z, inter.inter_normal.y);
    }
    
    inter.y = glm::cross(inter.inter_normal, inter.x);
    
    return true;
}
