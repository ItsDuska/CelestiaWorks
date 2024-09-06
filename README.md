## Silly 2D rendering framework like SFML made for fun.
Please don't expect this to work because my code is hot garbage.
The performance will probably be horrible but this project is just for having fun and learning new things.

### **The list of things I want to add to this when I have the time for it:**
  - Mouse support
  - Text rendering
  - RenderTextures
  - Custom shader support
  - Vertex arrays
  - Music / sound system
  - Camera system? (Maybe not)
  - sprite rotations?
  - optimations / whole rewrite
  - Controller support?

-------

### How to Link CelestiaWorks in Your Project
#### 1. Download and Extract the Latest Version

Go to the GitHub Releases page and download the latest release of CelestiaWorks. Then unzip the downloaded file.

#### 2. Folder Structure

Once extracted, the following folders will be available:

- bin/: Contains the .dll file for the library.
- lib/: Contains the .lib file for linking the library.
- include/: Contains all the header files necessary for using the library.
  - include/Graphics/: Contains headers for working with sprites, textures, and window handling.
  - include/System/: Contains headers for vectors, matrices, keyboard input, squares, and vertex shapes.
- example/: Contains a short example that demonstrates how the library works and walks through almost every feature currently available.

#### 3. Link the Library

To use the CelestiaWorks library in your C++ project, follow these steps:

- Step 1: Add the include folder to your project's include directories.
    This will allow you to reference the header files for Graphics and System.

- Step 2: Add the lib folder to your project's library directories.
    Link against the .lib file located in the lib folder.

- Step 3: Ensure the .dll file from the bin folder is placed in the same directory as your compiled executable or set the path to the .dll in your system's environment variables.

#### 4. Usage Example

Check the example/ folder for a short working example that demonstrates how to use the library. The example goes through nearly every feature available, including setting up a window, creating sprites and handling input.


----

### **Celestia Works be like:**

![drawCallsBeLike](https://github.com/ItsDuska/CelestiaWorks/assets/89298953/c5180c1f-af51-4528-a84f-dfa329c910a6)

![8mw4jqom79wa1](https://github.com/ItsDuska/CelestiaWorks/assets/89298953/cf69f7b2-56b6-4525-9e5a-6880bbda1054)

![20231102_143716](https://github.com/ItsDuska/CelestiaWorks/assets/89298953/b372e3b7-eb25-48ee-888b-22fb08718482)
