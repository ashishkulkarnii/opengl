
## Convex Hull Using Graham's Scan

The Graham's scan is a way of finding the convex hull of a set of coplanar points in *O*(_n_ log(_n_)) time.
We start by having a collection of points.
In my case, I created a function to randomly plot _n_ points.

1. Select the left-most point out of the set of points.
2. Sort all the remaining points by the angle subtended at the left-most point.
3. Now start at the left most point and go in order of the remaining points. We know that on the convex hull, we should have only left turns as we move through the points. So we keep pushing points into the stack as we move through them, but if a particular point causes us to take a right turn with the next point, then we pop it from the stack.
4. Finally, we join the last point to the initial point and we have ourselves a convex hull.

This is how the output looks for 20 randomly generated points:\
<img src = "https://raw.githubusercontent.com/ashishkulkarnii/opengl/main/convex-hull/images/convex-hull.jpg" alt = "convex-hull.jpg" height = "500">
