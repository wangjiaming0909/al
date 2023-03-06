#include <iostream>

struct Renderer {
  virtual void render_circle(float x, float y, float radius) = 0;
  virtual void renderer_square(float x, float y, float width, float height) {}
};

struct VectorRenderer : public Renderer {
  void render_circle(float x, float y, float radius) override {
    std::cout << "drawing a vector circle of radius " << radius << std::endl;
  }
};

struct RasterRenderer : public Renderer {
  void render_circle(float x, float y, float radius) override {
    std::cout << "rasterizing a circle of radius " << radius << std::endl;
  }
};

class Shape {
  protected:
  Renderer &renderer;
  public:
    Shape(Renderer& renderer) : renderer(renderer) {}

    virtual void draw() = 0;
    virtual void resize(float factor) = 0;
};

class Circle : public Shape {
  public:
    Circle(Renderer &renderer) : Shape(renderer) {}
    float x, y, radius;
    void draw() override {
      renderer.render_circle(x, y, radius);
    }
    void resize(float factor) override {
      radius *= factor;
    }
};

void draw_cycle(float x, float y, float radius, Renderer &r) {
  Circle c(r);
  c.x = x;
  c.y = y;
  c.radius = radius;
  c.draw();
}

void test_bridge()
{
  VectorRenderer vr;
  RasterRenderer rr;

  draw_cycle(1, 2, 3, vr);
  draw_cycle(4, 5, 6, rr);
}
