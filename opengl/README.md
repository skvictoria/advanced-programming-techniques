## OPENGL Tutorial description by Seulgi Kim
- Super Useful tutorial guide: https://www.opengl-tutorial.org/beginners-tutorials/
- This README is made based on the above guide.

# Tutorial 1. First Window

# Tutorial 2. Red Triangle

# Tutorial 3. Matrices.
- Bug fix by applying radians, not degrees
```
# 68 lines
glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
```

- If we want to zoom in the triangle, adjust the FOV
```
# 68 lines
glm::mat4 Projection = glm::perspective(glm::radians(4.0f), 4.0f / 3.0f, 0.1f, 100.0f); # zoom in
glm::mat4 Projection = glm::perspective(glm::radians(90.0f), 4.0f / 3.0f, 0.1f, 100.0f); # zoom out
```
# Tutorial 4. Colored_cube
- If we want to see the disabled-depth version, try below.

```
# 1. 59, 61 line comment
# 2. Change 180 line
glClear(GL_COLOR_BUFFER_BIT);
```

# Tutorial 6. Keyboard and mouse
- OpenGL renders the two side of the object as default.
- If we only want to render the plane that is face towards the user, then use below code for optimization.

```
glEnable(GL_CULL_FACE);
```

- Bug fix for controlling our keyboard and mouse by editing common/controls.cpp
```
# 22 line
glm::vec3 origin = glm::vec3(0,0,0); // 

# 99 - 101 line
ViewMatrix = glm::lookAt(position,                    // Camera is here
						 origin,//position+direction, // and looks here : at the same position, plus "direction"
						 up                           // Head is up (set to 0,-1,0 to look upside-down)
						);
```

# Tutorial 8. basic_shading
- This time we will use shader: This file are loaded and compiled at run-time.
- If we want to put strong light, we can change the value of "ambient" light source.
```
// in StandardShading.fragmentshader file 24 line
vec3 MaterialAmbientColor = vec3(0.9,0.9,0.9) * MaterialDiffuseColor;
```