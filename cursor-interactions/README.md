## Cursor Interactions

Building upon the [sphere collision](https://github.com/ashishkulkarnii/opengl/tree/main/physics) setup, I decided to implement drag-and-drop using mouse cursor.
This consisted of 3 steps:
1. Identifying the selected object.
2. Tracking the the cursor position.
3. Releasing the object at the correct location.

To select an object with a cursor left-click, I made use of the Stencil Buffer.
By using the ```glutMouseFunc``` with a callback function that accesses which stencil the cursor clicked on, I appropriately selected the object to be moved.

Next, I used the ```glutPassiveMouseFunc```, which passively calls a callback function which accesses the cursor's screen coordinates.
I mapped the screen coordinates of the cursor to the world coordinates of the selected object in such a way that the object cannot escape its confinement.

The callback function within ```glutMouseFunc``` identifies when the object is released, and it is allowed to move freely with its previous velocity.
The object while paused still has the ability to interact with other objects in the form of collisions.

Apart from cursor interactions, I implemented rotation and scaling of the objects as well. These actions work when the object is paused or when the object is held/selected:
* Pressing ```Q``` and ```E``` rotates the object in opposite directions about a random axis passing throught the centroid of the object.
* Using ```+``` and ```-``` scales the objects.

Here's a small demo:\
<img src = "https://raw.githubusercontent.com/ashishkulkarnii/opengl/main/cursor-interactions/images/cursor-interactions.gif" alt = "cursor-interactions.gif" height = "500">

<details>
    <summary><b>Camera Controls</b></summary>
    <p>
        In all of these projects, the camera position can be adjusted using the following keys:<br>
        <b>W</b> - to move the camera forwards.<br>
        <b>S</b> - to move the camera backwards.<br>
        <b>A</b> - to move the camera clockwise around subject.<br>
        <b>D</b> - to move the camera counterclockwise around subject.<br>
        <b>I</b> - to move the camera up.<br>
        <b>K</b> - to move the camera down.
    </p>
</details>
