# N Body itterator

This is a program I wrote in about June 2018 that numerically integrates the gravitational forces between bodies (you can put in as many as you want to) and then draws a plot of their postion through time. It is not very reliable over a long time scale or if the incriments are too small as the whole system tends to gain energy.

To use it you make the bodies by calling `Body(name, mass, initial x position, y position, z position, x velocity, y velocity, z velocity)` and then `Body.simulate(start time, end time, incriments, output incriments, output - True/False, save- True/False)` and then finally `Body.output_all(to_show)`. 

## ToDo
This code is very messy and unreliable, I wrote it more as a learning task than for the actual product, so theres a lot I'd change about it
1. Re write with a more reliable simulation method - different integration method? completly different process like using energies and momentum? Lagrangian mechanics?
2. Re write in a more efficent faster way - C++?
3. Make more usable - useful output images as aposed to very slow graph not intended for it, useful functions to set up and run, etc
## Notes:
- The output incriments needs to be larger and a multiple of the incriments
- 
