#pragma once

#include <glm/glm.hpp>

#include "SceneNode.hpp"
#include "Light.hpp"
#include "Image.hpp"

void A4_Render(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		// Viewing parameters
		const glm::vec3 & eye,
		const glm::vec3 & view,
		const glm::vec3 & up,
		double fovy,

		// Lighting parameters
		const glm::vec3 & ambient,
		const std::list<Light *> & lights,
               
        // Ray parameters
        const int num_threads,
        const int reflect_rays,
        const int refract_rays,
        const int glossy_rays,
        const int soft_rays,
               
        // Adaptive AA params
        const int alias_count,
        const float threshold,
        const bool highlight,
        const bool anti
);
