
https://github.com/GauthierGesquiere/Tron

The controls are WASD to move, q and e to rotate arm and you can press F to shoot, 
controller use the left, right, up, down to move, left trigger/righttrigger to rotate arm, and the x to shoot.

I use the component pattern defining behaviours, states.

I use the observer to connect the state machine with the player controller.

then the event queue for more global things like reset level, killed player,..

my game supports keyboard, also controller, multiple players...

The AI is very simple could definitely be improved. 

I mapped the levels with Inkscape, then I use raycasts to see if i hit a crossroad.