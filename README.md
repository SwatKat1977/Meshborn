# Meshborn
Wavefront Obj loader

## Dependency
GLM V1.0.1

## Development status

### Supported Wavefront tags : object
The following Wavefront obj tags are currently supported:

| Tag                  | Status             | Notes                |
|:---------------------|:-------------------|:---------------------|
| f (face)             | :white_check_mark: |                      |
| g (group)            | :x:                | Not implemented      |
| l (line element)     | :x:                | Not implemented      |
| mtllib (materials)   | :x:                | Not implemented      |
| o (object)           | :x:                | Not implemented      |
| s (Smooth shading )  | :x:                | Not implemented      |
| usemtl (use material)| :x:                | Not implemented      |
| v (vertex)           | :white_check_mark: |                      |
| vn (vertex normal)   | :white_check_mark: |                      |
| vt (vertex texture)  | :x:                | Not implemented      |

### Supported Wavefront tags : material
The following Wavefront material tags are currently supported:

| Tag                        | Status             | Notes                |
|:---------------------------|:-------------------|:---------------------|
| newmtl (new material)      | :white_check_mark: |                      |
| Ka (ambient colour)        | :white_check_mark: |                      |
| Kd (diffuse colour)        | :white_check_mark: |                      |
| Ke (emissive colour)       | :white_check_mark: |                      |
| Ks (specular colour)       | :white_check_mark: |                      |
| Ns (specular exponent)     | :white_check_mark: |                      |
| d (transparent disolve)    | :white_check_mark: |                      |
| Ni (optical density)       | :white_check_mark: |                      |
| illum (illumination_model) | :white_check_mark: |                      |

### Functionality implemented 
The following functionality is currently supported:

| Tag                  | Status             | Notes                   |
|:---------------------|:-------------------|:------------------------|
| Basic object Read    | :construction:     | Currently working on    |
| Basic material read  | :x:                | Needs mtllib and usemtl |
| Logging              | :white_check_mark: |                         |
