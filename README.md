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
- Show used space in volume


### Platforms
<img src="https://skillicons.dev/icons?i=windows"/> <img src="https://skillicons.dev/icons?i=linux"/> <img src="https://skillicons.dev/icons?i=apple"/>  

This application is presently usable and buildable on Windows, Linux & macOS.

## Running:

### Windows:
- Download the latest SimpliFile-Windows.zip from the [Releases Section](https://github.com/kuwushagra/SimpliFile/releases)
- Extract both the `.exe` and the `includes` folder
- Get the latest version of `MinGW-w64`
- Double click the executable or execute `SimpliFile.exe` from the commandline
  
### Linux:
1. Download the latest zip and extract the `dist/SimpliFile.AppImage` from the [Releases Section](https://github.com/kuwushagra/SimpliFile/releases)
2. Run `sudo apt install libfuse2t64 libglfw3`
4. Run `chmod +x SimpliFile.AppImage`
5. Either double click on the AppImage or run `./SimpliFile.AppImage`

### macOS:

- Download the latest SimpliFile-macos-<x86_64/arm64>.zip from the [Releases Section](https://github.com/kuwushagra/SimpliFile/releases) depending on your architecture
- Run `brew install glfw`
- Extract the `SimpliFile.app` file (in most cases you will just see the .app file so no need to extract)
- Double click to launch

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
- (Font) FontAwesome: 
    - https://fontawesome.com/
