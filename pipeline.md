# OpenGL Rendering Pipeline

## Vertex Processing

### Vertex Shader
- (**in**): vertex attributes
- (**out**): vertex position, point size, clip distance

### Tesellation (*optional*)
Patches of vertex data are subdivided into smaller primitives
- (**in**): `N` vertices
- (**out**): `>=N` vertices

### Geometry Shader (*optional*)
- (**in**): `1` primitive
- (**out**): `>=0` primitives

## Vertex Post-Processing

### Transform Feedback
Process of capturing primitives recording data into Buffer Objects

### Primitive Assembly
Primitives are divided into a sequence of individual base primitives
- (**out**): orderes sequence of points, lines or triangles

### Clipping
`gl_Position`: Vertex clip space position

#### Perspective divide
$$ {\begin{pmatrix}x_{ndc}\\y_{ndc}\\z_{ndc}\end{pmatrix}}={\begin{pmatrix}{\tfrac {x_{c}}{w_{c}}}\\{\tfrac {y_{c}}{w_{c}}}\\{\tfrac {z_{c}}{w_{c}}}\end{pmatrix}} $$

#### Viewport transform

### Face Culling

## Rasterization

## Fragment Processing

### Fragment Shader (*optional*)

## Per-Sample Processing

## Draw to screen
