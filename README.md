# 3D Render Sandbox

<img width="1274" height="712" alt="OpenGL_Phong" src="https://github.com/user-attachments/assets/ee964454-bc73-41ec-9051-5005e7059421" />

## Overview

This is my hands-on journey through computer graphics, where I implement concepts from scratch to deeply understand how modern OpenGL works under the hood.

This repository focuses on **learning by doing**. The goal is not just to render things, but to understand *why* they work.

This project also serves as a foundation for future engine and rendering work.

---

## What This Project Covers

The repository incrementally implements core graphics programming concepts, including:

- Modern OpenGL (Core Profile 3.3+)
- Shader-based rendering (GLSL)
- Vertex Buffers (VBO), Vertex Arrays (VAO)
- Texture loading with **stb_image**
- Multiple textured objects with shared shaders
- Model / View / Projection matrix pipeline
- Camera system with:
  - WASD movement
  - Mouse look
  - Scroll-wheel FOV zoom
- Basic lighting concepts:
  - Object color
  - Light color
  - Moving light source
- Transformation hierarchy:
  - Translation
  - Rotation
  - Scaling
- Depth testing
- Multiple renderable objects using shared geometry

---

## Current Features

## Lighting
https://github.com/user-attachments/assets/a7aa44d3-4c2a-407c-9135-455bd4dcc7c8

## Lighting
https://github.com/user-attachments/assets/d5f2e7ed-37b8-4f0c-a7ee-93f0ddbc6dcd

## Model Loading With Assimp
https://github.com/user-attachments/assets/68313c20-4ff4-44ba-b2e3-ca97dcaa16b2

## Shader Outline
https://github.com/user-attachments/assets/2dda4c73-f936-47ff-a6b3-b0404822618a

- Textured cubes rendered in 3D space
- Scalable ground plane with tiled textures
- Orbiting light source visualized as a cube
- Reusable texture loading function
- Clean separation of:
  - Shaders
  - Geometry
  - Rendering logic
- Real-time camera movement and input handling

---

## Controls

| Input | Action |
|------|--------|
| `W / A / S / D` | Move camera |
| `Space` | Move camera up |
| Mouse Move | Look around |
| Mouse Scroll | Adjust FOV |
| `F` | Toggle wireframe |
| `L` | Toggle light background |
| `ESC` | Exit |

---

## Why This Repo Exists

- To **build strong fundamentals in graphics programming**
- To avoid black-box engines early on

Everything here is intentionally verbose and explicit — clarity over convenience.

---

## References

- [LearnOpenGL](https://learnopengl.com/)
- OpenGL 3.3 Core Profile
- GLM (OpenGL Mathematics)
- GLFW, GLAD, stb_image

---

## Status

🚧 Actively being developed while learning and experimenting.

