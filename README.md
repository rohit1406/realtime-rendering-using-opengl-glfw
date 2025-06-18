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


### 🚀 Prerequisites
---
1. Windows 11 OS
2. Visual Studio installation: [click here to download the installer](https://visualstudio.microsoft.com/downloads/)  
    - this also installs Windows SDK
3. Cmake installation [click here to download the installer](https://cmake.org/download/#latest)

---
  
### 🧠 Challenges/Issues
---
#### GIT
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