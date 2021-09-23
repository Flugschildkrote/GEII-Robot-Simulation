# Robot Simulation
![Picture](picture.jpg?raw=true "Robot and Simulation")
## Context 
This project started in 2018 when I was student in first year of my Higher National Diploma of Electrical Engineering and Industrial Computer Science at the University of Poitiers (86), France. 
I was part of a group participating to a Robotic contest. We hat to build a Robot playing some kind of tennis with another robot.  
We had some issues figuring out how to place the sensors on the robot and deciding on a ball shooting mechanism, and iterative prototyping was taking way too long.

Because of that I decided to build a simulator with the following objectives : 
- Simulate several behaviors and select the most effective one. 
- Simulate the sensors as close as possible to reality in term of physical characteristics (ie: sensor range, camera resolution...) and data produced.
- Experiment on sensor placement to detect walls and the white line in the middle of the field. 
- Ensure the system was fitting under the maximum imposed size.
## Results
### The Simulation
This simulator helped us to choose the robot physical structure (chassis and shooting mechanism). It also helped us on the sensor placement especially for the white line detection in the middle of the field (we had to differentiate between thin and big lines) . 

Because of time constrains we were not able to experiment much on the robot behavior analysis. 

### The contest 
We has some issues during the contest. The main one was that artificial lights in the contest area were emitting too much in the UV range. This caused our camera to fail ball recognition as it was based on colors. We also had an issue with the shooting mechanism that failed mechanically. And at last our batteries were not strong enough to hold short repetitive charge-discharge cycles, so our robot was out of juice during the last match. Beyond that we arrived at the 8th place out of 20 teams. 
 


## Technologies Used 
This simulator is based on : 
- **OpenGL 3.3** for graphics rendering.
- **Bullet Physics** for the physics and collision detection.
- **SLD2** for the Window System and image loading.
- **TinyXML** for XML parsing.
- **GLAD** as the OpengGL loader.
- **GLM** for mathematics.

## Sensors Simulation 
Several sensors were simulated in this project : 
- **CNY70** : These were used to detect intensity change on the floor. They were simulated by a ray of a arbitrary length. Colliders were placed on the white line of the field to detect white location.
-  **SHARP_GP2Y** : These sensors are used to detect distances. They were simulated by a ray. The ray size was chosen according to the sensor datasheet.
- **PIXY Camera** : This camera is used perform object detection. This camera was simulated by a draw in an FBO with a texture color attachment. The resolution of the texture was the same as the camera resolution. The texture was then processed to detect balls and output "object block" approaching at best to the real camera data structure.
- **The motors** were simulated using Bullet Physics motors, but with arbitrary torque and speeds.

## Build from sources
To build this project you need premake5 (available [here](https://premake.github.io/download)) and Visual Studio. This project is only tested on Windows with the Visual Studio 2019 IDE. No guarantees are made regarding other platforms. To build this project run `premake5 vs2019`. This will generate a solution (.sln) in the projects/vs2019 directory. 
To run the binary outside of visual studio, the executable must be started from the root directory (the directory where the Data directory is located), or you must copy the Data folder on the same directory as the executable. 

## Controls 
- Start simulation : **Space**
- Camera : **Arrow keys** to move, **mouse** to change orientation
- Robot 1 :
	- Move : **I**, **J**, **K** and **L**
	- Open Shooter : **H**
	- Shoot : **U**
	- Manual mode : **Keypad 0**
	- Auto mode 1 : **Keypad 1**
	- Auto mode 2 : **Keypad 1**
- Robot 2 :
	- Move : **Z**, **Q**, **S** and **D**
	- Open Shooter : **A**
	- Shoot : **E**
	- Manual mode : **Keypad 9**
	- Auto mode 1 : **Keypad 7**
	- Auto mode 2 : **Keypad 8**
