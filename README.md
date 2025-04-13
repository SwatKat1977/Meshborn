# Meshborn
Wavefront Obj loader

## Dependency
GLM V1.0.1

## Development status

### Supported Wavefront tags
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

### Functionality implemented 
The following functionality is currently supported:

| Tag                  | Status             | Notes                   |
|:---------------------|:-------------------|:------------------------|
| Basic object Read    | :construction:     | Currently working on    |
| Basic material read  | :x:                | Needs mtllib and usemtl |
| Logging              | :white_check_mark: |                         |
