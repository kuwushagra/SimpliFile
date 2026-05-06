# SimpliFile - The Simple File Manager

<img src="https://github.com/kuwushagra/SimpliFile/blob/main/screenshots/screenshot.png?raw=true"/>

## About 
This File Manager was made as a college project and can perform very basic operations such as:
- Creation of New Folders
- Deletion of Folders
- Cut/Copy/Paste (on both files and folders)
- Renaming of files and folders
- Navigation of folders
- Launching/opening files of various types by double click (opens in the default app set by your DE)


### Platforms
<img src="https://skillicons.dev/icons?i=linux"/>
This application is presently usable and buildable only on linux.

## Running:
1. Download the latest AppImage from the [Releases Section](https://github.com/kuwushagra/SimpliFile/releases)
2. Run `sudo apt install libfuse2t64 libglfw3`
3. Run `chmod +x SimpliFile.AppImage`
4. Either double click on the AppImage or run `./SimpliFile.AppImage`

## Compiling:
Make sure you have the following installed and working on your system:  
- [OpenGL](https://www.opengl.org/)  
- [GLFW](https://www.glfw.org/)  
- [Dear ImGui](https://github.com/ocornut/imgui)  

and then simply run the following commands in your terminal emulator of choice:

```
git clone 'https://github.com/kuwushagra/SimpliFile' SimpliFile
cd 'SimpliFile'
make
```

### Components
This project relies on the [Dear ImGui](https://github.com/ocornut/imgui) library by [ocornut](https://github.com/ocornut). 

### Assets
Here are the various assets used by this project and where they have been taken from:
- Logo:
    - https://t4.ftcdn.net/jpg/04/59/22/61/360_F_459226178_mVXOk4GRSyp6cY1SKuurbXpKFcSOTYQ3.jpg
- (Font) Unbounded:
    - https://fonts.google.com/specimen/Unbounded
- (Font) Onest: 
    - https://fonts.google.com/specimen/Onest
