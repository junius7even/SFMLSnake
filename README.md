# Summary
 This is a snake game written with the SFML graphics library and C++.
![SFMLSnake](https://github.com/junius7even/SFMLSnake/assets/33168288/d13be957-f986-4bbb-a561-d01ffb2a4927)

## Drawing the environment
The screen(window) is divided by me into blocks of a certain height, then a file is read describing the proper textured block to be used at each given block location. The corresponding and appropriate wall objects are also created and stuffed into a vector.

## Inputs
The snake's inputs are handled using the Command pattern, which involves pushing each command on and off a stack of commands ready to execute at the next available moment. 

## Collisions
Collisions are handled using a very basic implementation (and coupled, which isn't good) of the AABB collision detection method. This is true for both the apple-snake interaction as well as the wall-snake interaction. Note that a collision tree is not used, meaning that checking collisions each frame is O(n^2) instead of O(nlogn). This is because I only found out about them afterwards.
