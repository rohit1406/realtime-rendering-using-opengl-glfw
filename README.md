### 🌱 Description
---
This Repository contains the demos of OpenGL applications. These demos are focused on various aspects of real time rendering like **2D/3D geometry shapes rendering, applying textures, lighting, model loading, camera movements, advanced opengl concepts, advanced lighting, pbr**. In the end there is also one game developed which uses the concepts learned during these demo creation. 

## 🗃️ Demos
| #    | **Topic Link** | **Description** |
| --- | ---------------------|-------------------|
| 01 | [Getting Started](./OpenGL_On_Windows/GLFW/1_GettingStarted/README.md) | Set up simple rendering framework with capabilies like set up rendering pipeline, render basic shapes (like triangle, rectangle, cube, etc),  reading shaders from file, camera movements with keyboard and mouse |  
| 02 | [Lighting](./OpenGL_On_Windows/GLFW/2_Lighting/README.md) |  |  
| 03 | [Model Loading](./OpenGL_On_Windows/GLFW/3_ModelLoading/README.md) |  |  
| 04 | [Advanced OpenGL Concepts](./OpenGL_On_Windows/GLFW/4_AdvancedOpenGL/README.md) |  |  
| 05 | [Advanced Lighting](./OpenGL_On_Windows/GLFW/5_AdvancedLighting/README.md) |  |  
| 06 | [PBR](./OpenGL_On_Windows/GLFW/6_PBR/README.md) |  |  
| 07 | [OpenGL In Practice](./OpenGL_On_Windows/GLFW/7_InPractice/README.md) |  |  



### 🚀 Prerequisites
---
1. Windows 11 OS
2. Visual Studio installation: [click here to download the installer](https://visualstudio.microsoft.com/downloads/)  
    - this also installs Windows SDK
3. Cmake installation [click here to download the installer](https://cmake.org/download/#latest)

---
  
### 🧠 Challenges/Issues
---
**Linker error**  

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