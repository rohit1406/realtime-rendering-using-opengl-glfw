### 🌱 Description
---
This Repository contains the demos of OpenGL applications. These demos are focused on various aspects of real time rendering like **2D/3D geometry shapes rendering, applying textures, lighting, model loading, camera movements, advanced opengl concepts, advanced lighting, pbr**. In the end there is also one game developed which uses the concepts learned during these demo creation. 

## 🗃️ Demos
| #    | **Demo Link** | **Description** |  **Output**
| --- | ---------------------|-------------------|-------|
| 01 | [Plain Window using GLFW](./OpenGL_On_Windows/GLFW/1_GettingStarted/1_Plain_Window_16102018/README.md) | Create a plain Window using GLFW | [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/1_Plain_Window_16102018/images/output.png) |  
| 02 | [Colored Triangle](./OpenGL_On_Windows/GLFW/1_GettingStarted/2_Triangle_17102018/README.md) | Render a colored triangle on Window | [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/2_Triangle_17102018/images/output.png) |
| 03 | [Square](./OpenGL_On_Windows/GLFW/1_GettingStarted/3_SQUARE_EBO_18102018/README.md) | Render a Square using EBO | [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/3_SQUARE_EBO_18102018/images/output.png) |
| 04 | [Two Triangles Next To Each Other](./OpenGL_On_Windows/GLFW/1_GettingStarted/4_e1_Two_Triangles_18102018/README.md) | Render two triangles using single VAO | [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/4_e1_Two_Triangles_18102018/images/output.png) |
| 05 | [Two Triangles Next To Each Other](./OpenGL_On_Windows/GLFW/1_GettingStarted/5_e2_Two_Triangles_18102018/README.md) | Render two triangles using seperate VAO for each triangle | [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/5_e2_Two_Triangles_18102018/images/output.png) |
| 06 | [Two Triangles Next To Each Other](./OpenGL_On_Windows/GLFW/1_GettingStarted/6_e3_Two_Triangles_18102018/README.md) | Render two triangles with different colors using seperate VAO for each triangle. Color values are hardcoded in fragment shaders | [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/6_e3_Two_Triangles_18102018/images/output.png) |
| 07 | [Two Triangles Next To Each Other](./OpenGL_On_Windows/GLFW/1_GettingStarted/7_GLSL_Two_Triangles_18102018/README.md) | Render two triangles with different colors. Assign color value to one of the triangle via "uniform" | [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/7_GLSL_Two_Triangles_18102018/images/output.png) |
| 08 | [Two Triangles Next To Each Other with interleaved data](./OpenGL_On_Windows/GLFW/1_GettingStarted/8_GLSL_Two_Triangles_Interleaved_18102018/README.md) | Render two triangles with different colors and pass interleaved data for position and color in same VBO for one of the triangle | [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/8_GLSL_Two_Triangles_Interleaved_18102018/images/output.png) |
| 09 | [Triangle](./OpenGL_On_Windows/GLFW/1_GettingStarted/9_GL_Framework_18102018/README.md) | Render a triangle with shader code seperated from a cpp file | [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/9_GL_Framework_18102018/images/output.png) |
| 10 | [Triangle (upside down)](./OpenGL_On_Windows/GLFW/1_GettingStarted/10_e1_Triangle_18102018/README.md) | Render a triangle (upside down) | [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/10_e1_Triangle_18102018/images/output.png) |
| 11 | [Triangle with offset](./OpenGL_On_Windows/GLFW/1_GettingStarted/11_e2_Triangle_With_Offset_18102018/README.md) | Render a triangle with offset | [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/11_e2_Triangle_With_Offset_18102018/images/output.png) |
| 12 | [Colorful Triangle](./OpenGL_On_Windows/GLFW/1_GettingStarted/12_e3_Triangle_18102018/README.md) | Render a Colorful triangle | [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/12_e3_Triangle_18102018/images/output.png) |
| 13 | [Textured Rectangle](./OpenGL_On_Windows/GLFW/1_GettingStarted/13_Texturing_Rectangle_19102018/README.md) | Render a textured rectangle | [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/13_Texturing_Rectangle_19102018/images/output.png) |
| 14 | [Funcky Colored Textured Rectangle](./OpenGL_On_Windows/GLFW/1_GettingStarted/14_Texturing_Rectangle_FunkyColor_19102018/README.md) | Render a funcky colored textured rectangle | [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/14_Texturing_Rectangle_FunkyColor_19102018/images/output.png) |
| 16 | [Blended Textured Rectangle](./OpenGL_On_Windows/GLFW/1_GettingStarted/16_e1_Texturing_Rectangle_MultiTextureSampling_19102018/README.md) | Render a blended multi textured rectangle | [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/16_e1_Texturing_Rectangle_MultiTextureSampling_19102018/images/output.png) |
| 18 | [Textured Rectangle with image clamping](./OpenGL_On_Windows/GLFW/1_GettingStarted/18_e3_Texturing_Rectangle_MultiTextureSampling_19102018/README.md) | Render an indexed textured rectangle | [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/18_e3_Texturing_Rectangle_MultiTextureSampling_19102018/images/output.png) |
| 19 | [Textured Rectangle with transformation](./OpenGL_On_Windows/GLFW/1_GettingStarted/19_Texturing_Rectangle_Transformations_19102018/README.md) | Render a textured rectangle and applied transformation to it - V1 | [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/19_Texturing_Rectangle_Transformations_19102018/images/output.png) |
| 20 | [Textured Rectangle with transformation V2](./OpenGL_On_Windows/GLFW/1_GettingStarted/20_e1_Texturing_Rectangle_Transformations_19102018/README.md) | Render a textured rectangle and applied transformation to it - V2 | [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/20_e1_Texturing_Rectangle_Transformations_19102018/images/output.png) |
| 21 | [Textured Rectangle with transformation V3](./OpenGL_On_Windows/GLFW/1_GettingStarted/21_e2_Texturing_Rectangle_Transformations_19102018/README.md) | Render a textured rectangle and applied transformation to it - V3 | [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/21_e2_Texturing_Rectangle_Transformations_19102018/images/output.png) |
| 22 | [Going 3D with Rectangle](./OpenGL_On_Windows/GLFW/22_Going_3D_20102018/README.md) | Render a textured rectangle in 3D space | [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/22_Going_3D_20102018/images/output.png) |
| 23 | [Rotating Textured Cube](./OpenGL_On_Windows/GLFW/1_GettingStarted/23_More_3D_20102018/README.md) | Render a rotating textured cube in 3D space | [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/23_More_3D_20102018/images/output.png) |
| 24 | [More Textured Cube](./OpenGL_On_Windows/GLFW/1_GettingStarted/24_More_3D_Cubes_20102018/README.md) | Render more textured cube in 3D space | [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/24_More_3D_Cubes_20102018/images/output.png) |
| 25 | [More Textured Cube V2](./OpenGL_On_Windows/GLFW/1_GettingStarted/25_e1_More_3D_Cubes_25102018/README.md) | Render more textured cube in 3D space - V2| [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/25_e1_More_3D_Cubes_25102018/images/output.png) |
| 26 | [More Textured Cube V3](./OpenGL_On_Windows/GLFW/1_GettingStarted/26_e2_More_3D_Cubes_25102018/README.md) | Render more textured cube in 3D space - V3| [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/26_e2_More_3D_Cubes_25102018/images/output.png) |
| 27 | [Rotating More Textured Cube](./OpenGL_On_Windows/GLFW/1_GettingStarted/27_e3_More_3D_Cubes_25102018/README.md) | Render rotating more textured cube in 3D space| [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/27_e3_More_3D_Cubes_25102018/images/output.png) |
| 28 | [Set up Camera](./OpenGL_On_Windows/GLFW/1_GettingStarted/28_Setting_Up_Camera_25102018/README.md) | Camera Setup introduced and movement with keyboad keys| [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/28_Setting_Up_Camera_25102018/images/output.png) |
| 29 | [Set up Camera](./OpenGL_On_Windows/GLFW/1_GettingStarted/29_Setting_Up_Camera_With_Mouse_Movements_26102018/README.md) | Camera Setup introduced and movement with keyboad keys and mouse movement| [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/29_Setting_Up_Camera_With_Mouse_Movements_26102018/images/output.png) |
| 30 | [Set up Camera](./OpenGL_On_Windows/GLFW/1_GettingStarted/30_Camera_Abstraction_26102018/README.md) | Camera Object abstration from graphics rendering code| [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/30_Camera_Abstraction_26102018/images/output.png) |
| 31 | [Set up Camera](./OpenGL_On_Windows/GLFW/1_GettingStarted/31_e1_Camera_Abstraction_WIthout_Fly_26102018/README.md) | Camera Object abstration from graphics rendering code V2| [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/31_e1_Camera_Abstraction_WIthout_Fly_26102018/images/output.png) |

### 🚀 Prerequisites
---
1. Windows 11 OS
2. Visual Studio installation: [click here to download the installer](https://visualstudio.microsoft.com/downloads/)  
    - this also installs Windows SDK
3. Cmake installation [click here to download the installer](https://cmake.org/download/#latest)

---
  
### 🧠 Challenges/Issues
---
** Linker error  
LINK : warning LNK4098: defaultlib 'MSVCRTD' conflicts with use of other libs; use /NODEFAULTLIB:library in command line
---
**Untrack already committed files to the repository from GIT**
1. Add **.gitignore** file to the root of the repository
2. Add files which needs to be ignored/untracked e.g. *.obj, *.exe
3. execute command:
```
 git rm --cached *.obj *.exe
 git commit -m "stop tracking ignored files"
 git push
```
---

## 🌟 Developer/Contributor
Name: Rohit Shamrao Muneshwar  
Email: rohit.muneshwar1406@gmail.com  
LinkedIn Profile: [Click Here](https://www.linkedin.com/in/rohit-muneshwar-a9079258/)  
Other Github repositories: [Click Here](https://github.com/rohit1406?tab=repositories)  

---