# GlitchArtist
Image editor that loads BMP files as audio samples, allowing you to apply audio effects like reverb or delay. Written in C++ with [GLFW](https://github.com/glfw/glfw) and [Dear ImGui](https://github.com/ocornut/imgui).

<img width="1392" height="860" alt="Capture d’écran 2025-08-21 à 01 17 08" src="https://github.com/user-attachments/assets/7cc6c769-594d-40df-8cb1-6b2032db1f17" />

The inspiration behind this project came from this video : https://www.youtube.com/watch?v=Z_Rut5gjwfE

I basically wanted to recreate a real time version of that process, turning an image into audio samples and apply audio effects just to see what it looks like. Maybe it has some potential for artists.

Only tested on MacOS.

## Features : 
- BMP import (files < 2MB recommended)
- Delay, Reverb, Distorsion, Phaser, Chorus
- Audio samples visualizer
- Hex viewer
- Real time display (with a toggle if you wanna stack a lot of effects)
- Glitcher effect (Creates glitchy lines)

## Planned features :
- BMP export
- More effects
- Cutting/Pasting for the hex viewer and audio visualizer
- Full blown hex editor
- Glitcher effect
- I might wanna add other accepted file formats, mostly images but I also wanna explore the reverse process (audio -> image) and 3D models

## How to install
Basically you need a C++ compiler, cmake and vcpkg I think that's all, vcpkg and cmake do all the work right ? 
