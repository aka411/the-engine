# TheEngine (Working Title)

A lightweight, modular game engine being built from the ground up with a focus on **Data-Oriented Design (DOD)** and high-performance rendering.

---

## Core Philosophy: Data-Oriented Design

Unlike traditional **OOP** based engines, TheEngine prioritizes memory layout and cache efficiency. By utilizing **Data-Oriented Design (DOD)** patterns, the engine organizes data in contiguous arrays to maximize cache locality and CPU throughput.
* **Custom ECS System:** [aka411/ecs-engine](https://github.com/aka411/ecs-engine)
* **Memory Layout:** Contiguous component arrays for minimal cache misses.

---

##  Current Feature Set

### Rendering & Graphics

* **glTF 2.0 (Only Core Profile):** The engine exclusively supports the glTF (Graphics Library Transmission Format), focusing on this standard for efficient asset loading.
* **Batched Rendering:** Utilizes Multi-Draw Indirect (MDI) to significantly reduce draw call overhead and batch submission.
* **Bindless Texturing:** Leverages modern OpenGL extensions to reduce CPU overhead and draw call state changes.
* **PBR Material Support:** PBR Metallic-Roughness workflow support.
* **SDF Text Rendering(Single-Channel):** High-quality, resolution-independent typography using Signed Distance Fields.
* **Dynamic Shader Generation:** Shaders are automatically generated based on the specific **Vertex Format** of the mesh.
* **Skeletal Animation:** Support for skinned meshes and bone hierarchies.

### Memory Management
* **Custom GPU Sub-Allocators:** Bespoke GPU memory buffer management to reduce fragmentation and driver overhead.
* **Efficient Buffer Management:** Specialized handling for vertex, index, and uniform data.

### Systems
* **Custom UI System:** A lightweight, UI framework built to integrate directly with the engine's data flow.
* **Lighting:** Simple single-light implementation.

---

## Demo

Check out the latest progress in the video below:

https://github.com/user-attachments/assets/4fefce97-4bf6-4443-8949-c109522d898b
* **Asset:** ["Reap the Whirlwind"](https://sketchfab.com/3d-models/reap-the-whirlwind-297e1595d6cf4466bf5c7803db8c5bd8) by [toomanydemons](https://sketchfab.com/toomanydemons)
* **License:** [CC BY 4.0](https://creativecommons.org/licenses/by/4.0/)
* *Note: This asset is not included in this repository. It is showcased for demonstration purposes only.*

---

## Project Status: Active Redesign

**TheEngine is currently undergoing a major architectural redesign.** I am applying lessons learned from the initial implementation to make the engine more modular, cleaner, and even more performant.
* **Current State:** The main branch may not be in an easily buildable state as focus has shifted to the new design in a separate development branch.
* **Goal:** A more robust modular design with clean code that expands on the existing DOD principles while adding new features.

---

## Building & Requirements

**Note:** As the engine is currently undergoing a major architectural redesign, the main branch is not in a stable, buildable state. Active development is occurring in a separate feature branch.

### Prerequisites
* **Compiler:** C++20 or higher (MSVC, Clang, or GCC).
* **Build System:** CMake 3.17+.
* **Graphics:** OpenGL 4.6 with **Bindless Texture** extension support.

---

## License

This project is currently under the  [Apache 2.0 License](LICENSE).

> **Note:** "TheEngine" is a working title. The project is an exploration of how modern hardware can be pushed using DOD patterns.