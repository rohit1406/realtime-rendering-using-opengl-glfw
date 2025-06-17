### 🌱 Description
---
This Repository contains the demos of OpenGL applications. These demos are focused on various aspects of real time rendering like **2D/3D geometry shapes rendering, applying textures, lighting, model loading, camera movements, advanced opengl concepts, advanced lighting, pbr**. In the end there is also one game developed which uses the concepts learned during these demo creation. 

## 🗃️ Demos
| #    | **Demo Link** | **Description** |  **Output**
| --- | ---------------------|-------------------|-------|
| 01 | [Plain Window using GLFW](./OpenGL_On_Windows/GLFW/1_GettingStarted/1_Plain_Window_16102018/README.md) | Create a plain Window using GLFW | [Click me to see output](./OpenGL_On_Windows/GLFW/1_GettingStarted/1_Plain_Window_16102018/images/output.png)


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
