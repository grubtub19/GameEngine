Peter Robe
3/19/19
Game Engine Milestone 2

Run "Project3/OpenGL Game Engine.exe"

Controls:
W - Forward
S - Backward
A - Strafe Left
D - Strafe Right
Left Shift - Down
Space - Up
Q - Roll Left
E - Roll Right

Things to note:
I use Quaternions for rotation so you can do like flips and stuff. It's pretty cool.

The pitch of the camera is always vertical in world space, so when your roll and then pitch up, it's kinda funky.

I use assimp to read model files and I extract the vertices, textures, etc. from assimp.

Assimp takes a while to load one of the models so be patient.

I'm missing the sphere and pyramid from the rubric, but I hope u'll forgive me.

I still use my math library exclusively.

Singletons are bad, so I instead use a static bool to check if a class has already been instantiated or not. I use this for Manager classes.

I do however have a singleton "Game" class that has all the Managers inside it, so I have access to every variable from anywhere