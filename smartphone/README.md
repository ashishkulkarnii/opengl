### Smartphone

Here, the directory ```./image``` contains a program written in Python3 to convert an image from JPEG to a CSV file for easy parsing in C++.
Simply add an image to convert to CSV into the image directory, and run the code with the image name as a command-line argument (with its file extension).

Example: ```python3 jpg_to_csv.py image.jpg```.

The result will be stored in ```image.csv```.

In case the phone's screen resolution is not high enough for your image, it can be adjusted in ```smartphone.cpp``` by changing the values of ```screenresolution``` on line 117.

<img src = "https://raw.githubusercontent.com/ashishkulkarnii/opengl/main/smartphone/images/front.jpg" alt = "front.jpg" height = "500">
<img src = "https://raw.githubusercontent.com/ashishkulkarnii/opengl/main/smartphone/images/back.jpg" alt = "back.jpg" height = "500">
<img src = "https://raw.githubusercontent.com/ashishkulkarnii/opengl/main/smartphone/images/angled.jpg" alt = "angled.jpg" height = "500">
