## CelestiaWorks: A 2D Rendering Framework Using Vulkan
CelestiaWorks is a 2D rendering framework inspired by SFML, designed purely for fun and learning. Built using the Vulkan API, this framework implements modern rendering techniques like bindless textures and a sprite batching system to improve performance when drawing multiple images. The project is currently only compatible with Windows operating systems. While it's not meant to be production-ready, and the code quality might not be the best, CelestiaWorks serves as a personal project for exploring new technologies and gaining experience in Vulkan development.

Disclaimer: This project is not guaranteed to work flawlessly. The performance may vary, and there are no promises of optimization, as this is more of a learning experiment rather than a polished product.

----
### **The list of things I want to add to this when I have the time for it:**
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

#### 1. Obtaining CelestiaWorks

You have two options to get CelestiaWorks:

**Option A: Download Pre-built Binaries**
- Go to the GitHub Releases page and download the latest release of CelestiaWorks
- Unzip the downloaded file to your desired location

**Option B: Build From Source**
- Follow the "Building from Source with CMake" instructions below to compile the library yourself
- This gives you the most up-to-date version and lets you customize the build

#### 2. Understanding the Library Structure

The CelestiaWorks library consists of:

- **Binary files**:
- The CelestiaWorks.dll file (runtime library)
- The CelestiaWorks.lib file (import library for linking)

- **Header files**:
- CelestiaWorks/Graphics/: Headers for sprites, textures, and window management
- CelestiaWorks/System/: Headers for vectors, matrices, input handling, and basic shapes

> Note: When building from source, these files will be in the build/bin/Release directory (DLL) and build/lib/Release directory (LIB), with headers in the original source tree.

#### 3. Link the Library

To use the CelestiaWorks library in your C++ project, follow these steps:

- Step 1: Add the include folder to your project's include directories.
    This will allow you to reference the header files for Graphics and System.

- Step 2: Add the lib folder to your project's library directories.
    Link against the .lib file located in the lib folder.

- Step 3: Ensure the .dll file from the bin folder is placed in the same directory as your compiled executable or set the path to the .dll in your system's environment variables.

#### 4. Usage Example

Check the example/ folder for a short working example that demonstrates how to use the library. The example goes through nearly every feature available, including setting up a window, creating sprites and handling input.


### Building from Source with CMake

If you prefer to build CelestiaWorks from source using CMake, follow these instructions.

#### Prerequisites

- CMake 3.15 or higher
- Visual Studio 2019 or 2022 with C++17 support
- Vulkan SDK installed
- Git (for cloning the repository)

#### 1. Clone the Repository

```
git clone https://github.com/ItsDuska/CelestiaWorks.git
cd CelestiaWorks
```

#### 2. Configure the Project

Create a build directory and configure the project:

```
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
```

> Note: You can replace "Visual Studio 17 2022" with your preferred generator.

#### 3. Building the DLL

To build just the CelestiaWorks library DLL:

```
cmake --build . --config Release --target CelestiaWorks
```

The compiled DLL will be located in the `build/bin/Release` directory.

#### 4. Building the Application (Bonus)

If you want to build the example application as well:

```
cmake --build . --config Release
```

This will build both the library and the application. The executable will be located in the `build/bin/Release` directory.

> Note: The CMake configuration will automatically download and build any missing dependencies (such as FreeType).

----

### **Celestia Works be like:**

![drawCallsBeLike](https://github.com/ItsDuska/CelestiaWorks/assets/89298953/c5180c1f-af51-4528-a84f-dfa329c910a6)

![8mw4jqom79wa1](https://github.com/ItsDuska/CelestiaWorks/assets/89298953/cf69f7b2-56b6-4525-9e5a-6880bbda1054)

![20231102_143716](https://github.com/ItsDuska/CelestiaWorks/assets/89298953/b372e3b7-eb25-48ee-888b-22fb08718482)
