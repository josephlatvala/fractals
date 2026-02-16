# Fractals

Computing various fractals (currently only the Mandelbrot and Julia sets)

Fun way to learn a new language

## Current implementations

All implementations are in ascii unless otherwise noted

- Python
    - Mandelbrot set (slow, low resolution)
- C
    - Mandelbrot set
        - Has a png implementation!
    - Julia sets
- OCaml
    - Mandelbrot set

## PNG implementation

`mandelbrot-png.c` renders an image using libpng

![A low resolution rendering of the mandelbrot set](example-output/mandelbrot.png)

## Future plans

- OCaml Julia sets
- Combine Mandelbrot and Julia set into one program since they are the exact same core logic
    - CLI args
- Images in OCaml
    - Custom C bindings?
- Use `pthread.h` to increase cpu utilization
- fractal canopy (png image)
- Render as image instead of text
    - GPU rendering?
- Hilbert curve
