# Scene Graphs

A C++ project implementing a scene graph data structure for managing hierarchical 2D scenes.

## Project Structure

```
SceneGraphs/
├── include/           # Header files
├── src/              # Source files
├── test/             # Test files
├── docs/             # Documentation files
├── CMakeLists.txt    # CMake configuration
└── Makefile          # Build automation
```

## Coding Conventions

- C++ Core Guidelines

## Prerequisites

- CMake (version 3.10 or higher)
- C++ compiler with C++17 support
- Google Test (for testing)
- Python 3.x (for documentation)

## Building the Project

1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd SceneGraphs
   ```

2. Build the project:
   ```bash
   make
   ```

## Running the Application

To run the main application:
```bash
./build/scene_graphs_app
```

## Running Tests

To run the test suite:
```bash
make test
```

## Documentation

### Setting Up Documentation Environment

1. Create and activate a Python virtual environment:
```bash
python3 -m venv venv
source venv/bin/activate  # On macOS/Linux
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

## Clean Build

To clean the build files:
```bash
make clean
```