Ray Tracer is a basic ray tracer used to render a 3D inputted scene into a 2D image. It was created as the final project for CS488 taught by Gladimir V. G. Baranoski at the University of Waterloo. 

This project is a (large) extension of my completed A4 assignment for CS488. The only source code given to me were frameworks used for reading in Lua files, creating a hierarchical nodes object, and creating a PNG image from a 2D array of RGB values. The ray tracer and all elements pertaining to pixel colour were done by myself. All source code and assets pertaining to the ray tracer can be found within the "Project" directory. All other "first layer" directories are shared libraries and provided frameworks. 

- - - -

This ray tracer features basic ray tracer elements such as shading, hierarchical node objects, and bounding boxes. It also includes 8 additional features listed as follows:

1. Primitives
   * Boxes
   * Spheres
   * Planes
   * Cylinders
   * Cones
   * Meshes
2. Texture Mapping
   * Implemented for Planes, Boxes, and Spheres
3. Bump Mapping
   * Implemented for Planes, Boxes, and Spheres
4. Reflection
   * With varying reflection ray depth
5. Refraction
   * Implemented for Spheres only
   * With varying refraction ray depth
   * Combines with reflection using Fresnel Equations
6. Adaptive Anti-Aliasing
   * With varying division depth
7. Soft Shadow
   * Using non-zero rectangular light sources • With varying shadow ray amounts
8. Glossy Reflection
   * With varying glossy reflection ray amounts
   
- - - -

To Build:

   cd \<path to Project directory>
   premake4 gmake
   make

To Build a Clean Build:
   
   make clean
   make

To Run:

   ./Project Assets/\<lua filename>
   

To view renders created by this ray tracer, please visit http://www.toyboxray. wordpress.com
