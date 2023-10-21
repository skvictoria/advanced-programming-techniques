## OPENGL Tutorial description by Seulgi Kim

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