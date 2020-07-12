# N Body itterator

This is a program I wrote in about June 2018 that numerically integrates the gravitational forces between bodies (you can put in as many as you want to) and then draws a plot of their position through time. It is not very reliable over a long time scale or if the increments are too small as the whole system tends to gain energy. I wrote an improved version in C++ which can be found [here](https://github.com/jagoosw/Gravitational-N-Body-Simulation)

To use it you make the bodies by calling `Body(name, mass, initial x position, y position, z position, x velocity, y velocity, z velocity)` and then `Body.simulate(start time, end time, incriments, output incriments, output - True/False, save- True/False)` and then finally `Body.output_all(to_show)`. 

## Examples
In the examples folder there are some examples of how the program can be run and some images of the outputs. In Starlink there is also a script that downloads the two line elements for the part of the constelation that is up at the moment and another that converts that into state vectors. The two scripts are in C++ becuase I'm learning it at the moment and it was a useful excersise.

## ToDo
This code is very messy and unreliable, I wrote it more as a learning task than for the actual product, so theres a lot I'd change about it
1. Re write with a more reliable simulation method - different integration method? completly different process like using energies and momentum? Lagrangian mechanics?
2. Re write in a more efficent faster way - C++?
3. Make more usable - useful output images as aposed to very slow graph not intended for it, useful functions to set up and run, maybe even add the objects actually having form (who'd have thought the Earth wasn't a point mass???)
## Notes:
- The output incriments needs to be larger and a multiple of the incriments
