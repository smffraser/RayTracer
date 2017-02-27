//
//  Helpers.hpp
//  CS488-Projects
//
//  Created by Sarah Fraser on 2017-02-25.
//  Copyright © 2017 none. All rights reserved.
//

#ifndef Helpers_hpp
#define Helpers_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include "Material.hpp"

class Intersection {
public:
    
    Intersection():
        inter_point(glm::vec3(INFINITY, INFINITY, INFINITY)),
        inter_normal(glm::vec3(0.0, 0.0, 0.0)),
        material(NULL){};
    
    Intersection(glm::vec3 inter_point, glm::vec3 inter_normal, const Material * material):
        inter_point(inter_point),
        inter_normal(inter_normal),
        material(material){};
    
    // Intersection point between ray and object
    glm::vec3 inter_point;
    // Surface normal at intersection point
    glm::vec3 inter_normal;
    // Material of the object the ray hit at this intersection
    const Material *material;
};


#endif /* Helpers_hpp */
