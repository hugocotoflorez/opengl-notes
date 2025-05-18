# OpenGL Rendering Pipeline

[OpenGL wiki - rendering](https://www.khronos.org/opengl/wiki/Rendering_Pipeline_Overview)

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
$$ { \begin{pmatrix} x_{ndc}\\ y_{ndc}\\ z_{ndc} \end{pmatrix} } = {
    \begin{pmatrix} { \tfrac {x_{c}}{w_{c}} }\\ { \tfrac {y_{c}}{w_{c}} }\\ {
        \tfrac {z_{c}}{w_{c}} } \end{pmatrix} } $$

#### Viewport transform
Vertex possitions from NDC to window space

#### Shape clipping
- **Point**: discared
- **Line**: clipped
- **Triangle**: generates triangles on the boundary of the clipping space

### Face Culling
Discard triangles facing away from the viewer

## Rasterization
Process whereby each individual primitive is broken down into discrete
elements called Fragments (at least one fragment produced for every pixel area
covered by the primitive being rasterized

## Fragment Processing
- **Fragment**: Set of state that is used to compute the final data for a
  pixel (or sample) in the output Framebuffer.

### Fragment Shader (*optional*)
- (**in**): Fragment
- (**out**): color, deph value and unmodified stencil value

## Per-Sample Processing
### Pixel Ownership Test
### Scissor Test
### Multisample operations
### Stencil Test
### Depth Test
### Occlusion Query Updating
### Blending
### sRGB Conversion
### Dithering
### Logical Operations

## Draw to screen
Finally you will be able to see that it does not work as expected.
