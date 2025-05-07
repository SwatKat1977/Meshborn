# Meshborn
Wavefront Obj loader

Open source light-weight Wavefront obj loader with no dependencies.

## Version Information

**Stable Release:** None

**Bleeding-edge Release:** None

## Development status
This library is currently in the early stages of development and is likely to
contain bugs or errors.


## What does Meshborn support?

### Supported Wavefront tags : object
The following Wavefront obj tags are currently supported:

| Tag                  | Status             | Notes           |
|:---------------------|:-------------------|:----------------|
| f (face)             | :white_check_mark: |                 |
| g (group)            | :white_check_mark: |                 |
| l (line element)     | :x:                | Not implemented |
| mtllib (materials)   | :white_check_mark: |                 |
| o (object)           | :white_check_mark: |                 |
| s (Smooth shading)   | :white_check_mark: |                 |
| usemtl (use material)| :white_check_mark: |                 |
| v (vertex)           | :white_check_mark: |                 |
| vn (vertex normal)   | :white_check_mark: |                 |
| vt (vertex texture)  | :white_check_mark: |                 |

### Supported Wavefront tags : material
The following Wavefront material tags are currently supported:

| Tag                                   | Status             | Notes                |
|:--------------------------------------|:-------------------|:---------------------|
| newmtl (new material)                 | :white_check_mark: |                      |
| Ka (ambient colour)                   | :white_check_mark: |                      |
| Kd (diffuse colour)                   | :white_check_mark: |                      |
| Ke (emissive colour)                  | :white_check_mark: |                      |
| Ks (specular colour)                  | :white_check_mark: |                      |
| Ns (specular exponent)                | :white_check_mark: |                      |
| d (transparent disolve)               | :white_check_mark: |                      |
| Ni (optical density)                  | :white_check_mark: |                      |
| illum (illumination model)            | :white_check_mark: |                      |
| map_Ka (ambient texture map)          | :white_check_mark: |                      |
| map_Kd (diffuse texture map)          | :white_check_mark: |                      |
| map_Ks (specular color texture map)   | :white_check_mark: |                      |
| map_Ns (specular highlight component) | :white_check_mark: |                      |
| map_d (alpha texture map)             | :white_check_mark: |                      |
| map_bump/bump (bump map)              | :white_check_mark: |                      |
| disp (displacement map)               | :white_check_mark: |                      |
| decal (stencil decal texture)         | :white_check_mark: |                      |

### Functionality implemented 
The following functionality is currently supported:

| Functionality          | Status             | Notes                                  |
|:-----------------------|:-------------------|:---------------------------------------|
| Basic object Read      | :construction:     | Line (L) not implemented               |
| Basic material read    | :white_check_mark: |                                        |
| Logging                | :white_check_mark: |                                        |
| Materials class        | :white_check_mark: |                                        |
| Mesh class             | :white_check_mark: |                                        |
| Validate object values | :construction:     | Work in progress                       |
| Model class            | :white_check_mark: |                                        |
| Unit tests             | :x:                | To be started once main core is stable |
