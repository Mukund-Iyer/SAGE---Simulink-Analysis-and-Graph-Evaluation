# SAGE - Simulink Analysis and Graph Evaluation

**SAGE** is a lightweight, standalone C++17 tool for parsing and diffing Simulink `.slx` models without relying on MATLAB or Simulink. It is designed for developers and engineers working in model-based development environments who need fast, scriptable access to model structure and changes.

---

## 🚀 Features

- **SLX File Parsing**  
  Parses `.slx` files (ZIP archives containing XML) to extract block definitions, signal lines, parameters, and layout metadata.

- **Custom Model Representation**  
  Uses a unordered map data type to represent a block in a model. I call them "block". Each key is a property of the block and the value is the value of the property. Each model has a vector of such "block". Connections between the blocks in all layers is also stored in a special unordered map called connections. 

- **Model Differencing**  
  Compares two models to identify structural and parameter-level changes:
  - Added or removed blocks
  - Updated block parameters

- **No MATLAB Dependency** 
  Fully independent of MATLAB/Simulink, making it suitable for CI/CD pipelines and open-source workflows.

- **Cross-Platform Compatibility**  
  Built with standard C++17 and CMake, runs on Windows, Linux, and macOS. Need to modify the CMakeList.txt file for the environment used.

---

## 📦 Dependencies

This project uses the following open-source libraries:
- ✅ miniz
  - Author: Rich Geldreich
  - License: MIT License
  - Description: A single-file, public domain zlib-replacement library for compression and decompression.

- ✅ pugixml
  - Author: Arseny Kapoulkine
  - License: MIT License
  - Description: A light-weight C++ XML processing library with XPath support.

---

## 📜 License
This project is licensed under the terms of the MIT License.
Please refer to the individual libraries for their respective licenses.

## 🔧 Build Instructions
I am using Visual Studio 15 2017 in Windows 10 as seen in the below build instructions. Based on the environment, you might have to modify the below bash script accordingly.
```bash
git clone https://github.com/Mukund-Iyer/SAGE---Simulink-Analysis-and-Graph-Evaluation
cd sage-simulink-parser
mkdir build && cd build
cmake .. -G "Visual Studio 15 2017"
````
## Note
SAGE.cpp is the entry point. I have added interface to comparison function. So, a CLI should be up for usage. Type "help" for list of commands. I am planning to add further functionalities to the CLI.
