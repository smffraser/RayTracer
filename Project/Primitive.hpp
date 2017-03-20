#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <math.h>
#include "Helpers.hpp"

class Primitive {
public:
    virtual ~Primitive();
    
    virtual bool intersect(const glm::vec3 origin, const glm::vec3 direction, Intersection &inter) const
    {
        return false;
    }
    
    
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();
    
    virtual bool intersect(const glm::vec3 origin, const glm::vec3 direction, Intersection &inter) const;
    
};

class Cube : public Primitive {
public:
  virtual ~Cube();
    
    virtual bool intersect(const glm::vec3 origin, const glm::vec3 direction, Intersection &inter) const;
    
};

class Cone : public Primitive {
public:
    virtual ~Cone();
    
    virtual bool intersect(const glm::vec3 origin, const glm::vec3 direction, Intersection &inter) const;
    
};

class Cylinder : public Primitive {
public:
    virtual ~Cylinder();
    
    virtual bool intersect(const glm::vec3 origin, const glm::vec3 direction, Intersection &inter) const;
    
};


class NonhierSphere : public Primitive {
public:
    NonhierSphere(const glm::vec3& pos, double radius): m_pos(pos), m_radius(radius){}
    virtual ~NonhierSphere();
    
    virtual bool intersect(const glm::vec3 origin, const glm::vec3 direction, Intersection &inter) const;

private:
  glm::vec3 m_pos;
  double m_radius;
};

class NonhierCylinder : public Primitive {
public:
    NonhierCylinder(const glm::vec3& pos, double radius, double height): m_pos(pos), m_radius(radius), m_height(height){}
    virtual ~NonhierCylinder();
    
    virtual bool intersect(const glm::vec3 origin, const glm::vec3 direction, Intersection &inter) const;
    
private:
    glm::vec3 m_pos;
    double m_radius;
    double m_height;
};

class NonhierCone : public Primitive {
public:
    NonhierCone(const glm::vec3& pos, double height): m_pos(pos), m_height(height){}
    virtual ~NonhierCone();
    
    virtual bool intersect(const glm::vec3 origin, const glm::vec3 direction, Intersection &inter) const;
    
private:
    glm::vec3 m_pos;
    double m_height;
};

class NonhierBox : public Primitive {
public:
  NonhierBox(const glm::vec3& pos, double size)
    : m_pos(pos), m_size(size)
  {
  }
  
  virtual ~NonhierBox();
    
    virtual bool intersect(const glm::vec3 origin, const glm::vec3 direction, Intersection &inter) const;

private:
  glm::vec3 m_pos;
  double m_size;
};
