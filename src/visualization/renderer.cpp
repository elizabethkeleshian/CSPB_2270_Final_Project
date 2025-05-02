#include "visualization/renderer.h"
#include "scene_graph/rectangle.h"
#include "scene_graph/circle.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <iostream>
namespace visualization {

using namespace std;
using namespace scene_graph;

struct Renderer::Impl {
    int viewportWidth = 800;
    int viewportHeight = 600;
    bool initialized = false;

    // testing helper
    bool frameActive = false;
    int shapesRendered = 0;
};

Renderer::Renderer() : impl_(make_unique<Impl>()) {};

Renderer::~Renderer() {
    cleanup();
}

bool Renderer::initialize() {
    impl_->initialized = true;
    cout << "Renderer initialized with viewport " << 
        impl_->viewportWidth << "x" << impl_->viewportHeight << endl;
    return true;
}

void Renderer::cleanup() {
    impl_->initialized = false;
    impl_->frameActive = false;
    impl_->shapesRendered = 0;
    cout << "Renderer cleaned up" << endl;
}

void Renderer::beginFrame() {
    if (!impl_->initialized) {
        cerr << "Renderer not initialized!" << endl;
        return;
    }
    impl_->frameActive = true;
    impl_->shapesRendered = 0;
    cout << "Frame started" << endl;
}

void Renderer::endFrame() {
    if (!impl_->initialized) {
        cerr << "Renderer not initialized!" << endl;
        return;
    }
    if (!impl_->frameActive) {
        cerr << "Frame not active!" << endl;
        return;
    }
    impl_->frameActive = false;
    cout << "Frame ended, " << impl_->shapesRendered << " shapes rendered" << endl;
}

void Renderer::setViewport(int width, int height) {
    impl_->viewportWidth = width;
    impl_->viewportHeight = height;
    cout << "Viewport set to " << width << "x" << height << endl;
}

void Renderer::renderShape(const Shape& shape) {
    if (!impl_->initialized) {
        cerr << "Renderer not initialized!" << endl;
        return;
    }
    if (!impl_->frameActive) {
        cerr << "Frame not active!" << endl;
        return;
    }
    Transform transform = shape.getGlobalTransform();
    Vector4 color = shape.getColor();
    
    // Log shape information
    cout << "Rendering " << shape.getName() << ": ";
    cout << "Position(" << transform.getPosition().x << ", " << transform.getPosition().y << ") ";
    cout << "Rotation(" << transform.getRotation() << ") ";
    cout << "Scale(" << transform.getScale().x << ", " << transform.getScale().y << ") ";
    cout << "Color(" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << ")" << endl;
    
    // Check shape type for specific rendering
    if (auto rect = dynamic_cast<const Rectangle*>(&shape)) {
        Vector2 size = rect->getSize();
        cout << "Rectangle size: " << size.x << "x" << size.y << endl;
    } 
    else if (auto circle = dynamic_cast<const Circle*>(&shape)) {
        float radius = circle->getRadius();
        cout << "Circle radius: " << radius << endl;
    }
    
    // Increment render count
    impl_->shapesRendered++;
    

}

} // namespace visualization