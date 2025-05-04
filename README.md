# Scene Graphs: Interactive 2D Scene Editor

This project implements a hierarchical scene graph data structure for managing 2D graphical elements. The implementation focuses on the core tree-based architecture that will eventually support shape manipulation within a parent-child relationship system.

## What Are Scene Graphs?

Scene graphs are tree-based data structures widely used in computer graphics, animation, and game development. They organize graphical elements hierarchically, making it easier to:

- Group related objects together (like wheels on a car)
- Apply transformations that cascade through related objects (move the car, wheels move too)
- Efficiently manage complex scenes with many objects

In a complete implementation, when you rotate, scale, or move a parent object, all of its children inherit those transformations - just like in professional tools like Maya, Blender, or Unity.

## Current State & Planned Features

**Currently Implemented:**
- Core scene graph data structure with parent-child node relationships
- Basic transformation system (position, rotation, scale)
- Transformation propagation through the node hierarchy
- Framework for rendering and scene management

**Planned Features (Not Yet Implemented):**
- Interactive shape creation (rectangles and circles)
- Visual selection and transformation tools
- Grouping functionality for objects
- Complete tree view panel for hierarchy management

## Project Structure

```
SceneGraphs/
├── include/           # Header files defining classes and interfaces
│   ├── scene_graph/   # Core scene graph implementation
│   └── visualization/ # Rendering and UI components
├── src/               # Source implementations
├── test/              # Unit and integration tests
├── docs/              # Documentation
├── CMakeLists.txt     # CMake build configuration
└── Makefile           # Build automation
```

## Prerequisites

- C++ compiler with C++17 support (GCC 7+, Clang 5+, or MSVC 19.14+)
- CMake (version 3.10 or higher)
- Google Test (for running tests)
- Python 3.7+ (for documentation)

## Getting Started

### Building the Project

1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd SceneGraphs
   ```

2. Build the project:
   ```bash
   make
   ```
   
   This will create executables in the `build/` directory.

### Running the Application

Launch the current version of the application:
```bash
./build/scene_graphs_app
```

**Note:** The application is currently in development and has limited interactive functionality. The focus of this project is on the underlying scene graph data structure implementation.

## Implementation Details

This project demonstrates several key concepts:

- **Tree Data Structures**: Each node in our scene graph represents a graphical object with transformation properties
- **Transformation Matrices**: We use matrix operations to combine transformations through the hierarchy
- **Object-Oriented Design**: The implementation showcases inheritance, polymorphism, and encapsulation

## Testing

To run the test suite:
```bash
make test
```

Tests verify the correctness of:
- Node relationships and hierarchical operations
- Transformation calculations and propagation
- Core data structure functionality

## Documentation

### Setting Up Documentation Environment

1. Create and activate a Python virtual environment:
```bash
python3 -m venv venv
source venv/bin/activate  # On macOS/Linux
venv\Scripts\activate     # On Windows
```

2. Install required packages:
```bash
pip install -r requirements.txt
```

3. Serve the documentation locally:
```bash
mkdocs serve
```

The documentation will be available at `http://127.0.0.1:8000`

### Building Documentation

To build the documentation for deployment:
```bash
mkdocs build
```

This will create a `site` directory containing the static documentation files.

## Development Workflow

1. Make changes to the code
2. Run tests to ensure functionality: `make test`
3. Update documentation if necessary
4. Build and run the application to verify changes: `make && ./build/scene_graphs_app`

## Troubleshooting

- **Build Fails**: Ensure you have the required compiler and CMake versions
- **Missing Dependencies**: Check that all prerequisites are installed correctly
- **Runtime Errors**: The application logs errors to stderr; check for these messages

## Cleaning Up

To remove build files:
```bash
make clean
```

---

This project was created as a final project for CSPB 2270: Data Structures. It demonstrates the practical application of tree data structures in a real-world graphics context. While the interactive features are still under development, the core scene graph implementation showcases important data structure concepts.