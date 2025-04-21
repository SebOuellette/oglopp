# ![OGLoPP](https://github.com/user-attachments/assets/7a3d17b3-6456-469d-9c74-48af6a8efdc2)

## What is OGLoPP?
[O]pen[GL] for [o]object-oriented C-[P]lus-[P]lus, is a library for C++ designed to abstract many of the functional programming annoyances of opengl into a neat, object-oriented interface. Programs built using OGLoPP can have a wide range of functionally, from CLI GPU-accelerated applications, to 2D desktop applications, up to 3D high-framerate simulations and games. OGLoPP was originally designed for science visualizations, but it can be used to build games as well.
<br>
OGLoPP is currently only capable of rendering and computing, however in the future it's intended to implement clean solutions for physics, networking, audio, and more.

## Building and Installing
OGLoPP uses `make` as its build system. A `make help` rule is available to view all available targets for building. Before building, you must ensure that the dependencies are installed.
### Required Dependencies
```
libglfw
libglm
```

### Installation Procedure
```bash
# Step 1: download source code
git clone https://github.com/SebOuellette/oglopp.git

# Step 2: Change directory into oglopp
cd oglopp

# Step 3: Build liboglopp.so
make

# Step 4: Install OGloPP
sudo make install

# OGLoPP can now be linked into your programs using '-loglopp' (and '-lglfw' if you use glfw directly)
```

### Uninstalling OGLoPP
```bash
# Reverse step 4, remove all traces of OGLoPP
sudo make uninstall
```

### Building Example Programs
```bash
# You can now build the examples, AFTER installing in step 4.
make examples
# Examples are placed in ./build
```

### Building Doxygen Documentation
```bash
# Compile HTML from source code
make docs

# You can now open the output in your favourite browser
firefox ./docs/doxygen/html/index.html
```


### Supported Platforms
OpenGL/GLFW/GLM are cross-platform, so ideally OGLoPP will eventually be available on all operating systems supported by OpenGL. OGLoPP was designed for Linux first, specifically Arch Linux, and may not work on other distributions, cpu architectures, or operating systems. Below is a list of systems and their known status.
<table>
  <tr>
    <th>Platform</th>
    <th>Status</th>
    <th>Description</th>
  </tr>
  <tr>
    <td>ArchLinux</td>
    <td>Supported</td>
    <td>OGLoPP is developed using ArchLinux systems. Issues are quickest to be detected for this distribution. Any distribution that allows packages to be installed to /usr/lib and /usr/include should in theory be able to run OGLoPP.</td>
  </tr>
  <tr>
    <td>Linux</td>
    <td>Likely Supported</td>
    <td>OGLoPP has not been tested on all Linux distributions, but it was designed for Linux. It is likely that OGLoPP can be configured to work properly on most or all distributions.</td>
  </tr>
  <tr>
    <td>Windows</td>
    <td>Unsupported</td>
    <td>The current 'Makefile' solution is Unix-specific. Will need a conversion to cmake and/or a visual studio solution to be created.</td>
  </tr>
</table>

## Projects using OGLoPP
<table>
  <tr>
    <th>Name</th>
    <th>Description</th>
    <th>Preview</th>
  </tr>
  <tr>
    <td><a href="https://github.com/SebOuellette/sketch-ml">Sketch-ML GitHub</a></td>
    <td>2D machine learning application capable of using OGLoPP compute shaders to reconstruct hand-drawn digits at upwards of 120 frames per second. (RX6900XT)</td>
    <td><img width=1000 src="https://github.com/user-attachments/assets/1f0250f6-49b9-48ea-be58-d5b3867a0c15"></td>
  </tr>
  <tr>
    <td>ParticleBox (Unreleased)</td>
    <td>Particle simulator capable of simulating upwards of 1 billion particles. (Although not all particles interact with each other)</td>
    <td><img width=1000 src="https://github.com/user-attachments/assets/70dc4e96-8208-44b0-a3fc-99d1c2da86a3"></td>
  </tr>
  <tr>
    <td>Cubegame (Unreleased)</td>
    <td>3D Infinite voxel world (vertically and horizonally) with textures and chunk generation.</td>
    <td><img width=1000 src="https://github.com/user-attachments/assets/891c9ae5-871e-4e6b-bea9-e2b43a6102cc"></td>
  </tr>
</table>

Have you used OGLoPP for a project? Tell me about it! Email: <code>i-used-oglopp [at] honeybeeks.net</code>
