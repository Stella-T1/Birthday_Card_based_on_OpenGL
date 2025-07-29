# 🎉 2D Birthday Card using OpenGL

This project presents a 2D animated birthday card built with **OpenGL** using **C/C++**. It demonstrates how low-level graphics programming can be used to create interactive and visually appealing greeting experiences. The card features dynamic scenes, colorful animations, and personalized text rendered using OpenGL primitives and transformations.

## ✨ Project Highlights

- 🧱 **Built with OpenGL**: Direct rendering using OpenGL functions (`GLUT`, `GLU`, etc.)
- 💡 **Animation Effects**: Includes fireworks, color transitions, and camera movement.
- 🎂 **Interactive 2D Scene**: Birthday cake, candles, and festive objects arranged in 3D space.
- 🖋️ **Custom Text Rendering**: Personalized birthday message drawn with bitmap fonts.
- 📐 **Basic Transformation**: Demonstrates scaling, rotation, and translation in OpenGL.

## 🧪 How It Works

1. Initializes an OpenGL window using GLUT.
2. Sets up projection for 2D rendering.
3. Uses modular functions to draw:
   - Cake and candles using quads/cylinders.
   - Fireworks using particles or animated lines.
   - Text using bitmap character functions (`glutBitmapCharacter`).
4. Applies transformations to animate the scene and simulate depth.

## 🔧 Requirements

- C/C++ Compiler (e.g., `g++`)
- OpenGL and GLUT libraries

## 📚 References

- [📘 FreeGLUT API Documentation](https://freeglut.sourceforge.net/docs/api.php)  
  Reference for using GLUT functions, including window management, input handling, and drawing.

- [📖 LearnOpenGL](https://learnopengl.com/)  
  Comprehensive modern OpenGL tutorial series with examples, though more focused on OpenGL 3.3+.

- [📙 OpenGL-Tutorial.org](https://www.opengl-tutorial.org/)  
  Step-by-step OpenGL tutorials with code snippets, great for beginners and intermediate developers.
