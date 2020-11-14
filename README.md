# AutoLabel
A label tool enable to do image annotation for rectangle and generate polygon annotation automatically
 
## Environment
### Compiler: 
|Win 10|Ubuntu 1804|
|:--:|:--:|
|Visual studio 2017|GCC 7.5.0|  

### Third Party: 
|Package|Version|
|:--:|:--:|
|OpenCV| 3.4.9|  

## Description
AutoLabel is a graphical image annotation tool, 
it aim to reduce semantic segmentation label time.
For now, the feature list below have been implement 
and more function will be added continuously.
It is written in c++ and Qt for its graphical interface,
and this project is inspired by https://github.com/wkentaro/labelme

## Feature
- Rectangle annotation
- Generate a polygon annotation automatically with rectangle ROI and enable to modify the generated polygon
![Execute](result/flower.gif)
![Execute](result/dog.gif)  
- Save rectangle and polygon annotation data according to the [labelme](https://github.com/wkentaro/labelme) json format

## How to contribute
Send a pull request

  
