## Sierpinski Triangle

The Sierpinski Triangle is a triangular pattern that can be formed algorithmically.
Here, I'm using the chaos game approach to generate the Sierpinski Triangle:

1. A point is chosen (at random) within a triangular region (bounded by 3 vertices).
2. One of the 3 vertices is randomly chosen.
3. The midpoint of the randomly chosen point and vertex is plotted.
4. The plotted point is used as the randomly chosen from step 1, and the consequent steps are repeated for several iterations.

I ran the algorithm for 50,000 iterations, and this is how the output looks:\
<img src = "https://raw.githubusercontent.com/ashishkulkarnii/opengl/main/sierpinski-triangle/images/sierpinski-triangle.jpg" alt = "sierpinski-triangle.jpg" height = "500">
