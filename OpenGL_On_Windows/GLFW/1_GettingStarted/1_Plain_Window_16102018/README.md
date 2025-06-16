### Description:
The program is a demonstration of how to create a simple plain window using GLFW which we will use later for rendering OpenGL context.

### How To Run:
1. Open Visual Studio **Developer Command Prompt**
2. **Compile:** 
```
cl.exe /MD /c /EHsc /I ../../../../include Window.cpp glad.c

# include folder contains header files for below libraries: 
glad, GLFW, KHR  
```
This will create **Window.obj** and **glad.obj** files in your current directory.    

3. **Link:**
```
link.exe Window.obj glad.obj /LIBPATH:..\..\..\..\lib GLFW/glfw3.lib user32.lib gdi32.lib kernel32.lib shell32.lib opengl32.lib /NODEFAULTLIB:msvcrtd.lib

# lib folder contains .lib files for below libraries:
GLFW
```  
This will create **Window.exe** file in your current directory.    

4. **Run:** Window.exe  
You will see a new Window is popped up with Title "OpenGL Window Using GLFW"

---
