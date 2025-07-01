# INFO

This file contains information about the graphics API and how it works.

## OpenGL Specific Info

- The GL_ELEMENT_ARRAY_BUFFER binding point is itself a bit unusual.
  It's not part of global context state; it's part of VAO state. So if you have no VAO bound (under a core profile context),
  then you can't bind anything to that binding point. And when you bind to that binding point,
  you are affecting the state of the currently bound VAO. And if you change the currently bound VAO, 
  you will be changing which buffer is bound to the element array binding point.