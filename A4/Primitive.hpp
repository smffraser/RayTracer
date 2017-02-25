#pragma once

#include <glm/glm.hpp>

class Primitive {
public:
    virtual ~Primitive();
    
    virtual bool intersect(const glm::vec4 origin, const glm::vec4 direction) const
    {
        return false;
    }
    
    
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();
    
    virtual bool intersect(const glm::vec4 origin, const glm::vec4 direction) const;
    
};

class Cube : public Primitive {
public:
  virtual ~Cube();
    
    virtual bool intersect(const glm::vec4 origin, const glm::vec4 direction) const;
    
};

class NonhierSphere : public Primitive {
public:
    NonhierSphere(const glm::vec3& pos, double radius): m_pos(pos), m_radius(radius){}
    virtual ~NonhierSphere();
    
    virtual bool intersect(const glm::vec4 origin, const glm::vec4 direction) const;

private:
  glm::vec3 m_pos;
  double m_radius;
};

class NonhierBox : public Primitive {
public:
  NonhierBox(const glm::vec3& pos, double size)
    : m_pos(pos), m_size(size)
  {
  }
  
  virtual ~NonhierBox();
    
    virtual bool intersect(const glm::vec4 origin, const glm::vec4 direction) const;

private:
  glm::vec3 m_pos;
  double m_size;
};
