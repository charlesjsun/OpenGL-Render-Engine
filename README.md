# OpenGL Rendering Engine

A deferred rendering engine created using modern OpenGL.  

Deferred rendering engines greatly improves performances over traditional forward rendering engines, allowing for a massive amount of dynamic lighting without much performance loss. 

This is because instead of doing lighting calculations for each vertex/fragment that exist for every object in the scene with every light, lighting calculation is deferred to a post-processing pass, meaning lighting calculations are done only for every pixel of the screen, so complexity scales linearly.  

Also includes a framework for adding extra post-processing effects such as bloom, ambient occlusion, FXAA.

# Logistics

Source folder is in the OpenGLEngine folder. 

Main.cpp contains main function.

Wrapper classes for OpenGL objects such as VAOs, VBOs, FBOs, Textures.

