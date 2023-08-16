# Lane Detection with OpenCV in ROS



## Description

This project aims to develop a lane tracking system using OpenCV integrated with ROS (Robot Operating System). The system is designed to perform real-time lane detection and provide a PID (Proportional-Integral-Derivative) value. The software is built to work with a USB camera, capturing images from the camera port, and then detecting lane markings. It calculates a PID value based on the detected lane position.

## System Requirements

- ROS Melodic
- Ubuntu 18.04.6 LTS
- OpenCV 4.0 and above
- C++

## Region of Interest (ROI) Configuration

For accurate lane detection, the region of interest (ROI) pixels should be identified and integrated into the algorithm. This involves selecting the relevant pixels from the camera sensor, which are essential for lane detection.

## Usage

1. Install ROS Melodic on your system.
2. Clone this repository.
3. Configure the USB camera to work with ROS.
4. Identify and set the region of interest (ROI) pixels in the algorithm.
5. Build and run the lane detection system.

## Some application contents

<img src="https://github.com/mr-ozdemir/Lane-detection-with-Opencv-in-Ros/blob/main/ilgi%20alanı%20belirlenmesi.PNG" width="250" height="300" > <img src="https://github.com/mr-ozdemir/Lane-detection-with-Opencv-in-Ros/blob/main/bilatlıgri.PNG" width="250" height="300" ><img src="https://github.com/mr-ozdemir/Lane-detection-with-Opencv-in-Ros/blob/main/Morfoljikislemvebianry.PNG" width="250" height="300" > <img src="https://github.com/mr-ozdemir/Lane-detection-with-Opencv-in-Ros/blob/main/pid.PNG" width="250" height="300" >

## Note

This project was initially developed for the Teknofest Robotaxi Autonomous Vehicle Competition. Further development and optimization are required to enhance its performance.

## License

This project is licensed under the [MIT License](LICENSE).

For questions or suggestions, please contact (mr.ozdemir34@gmail.com)
