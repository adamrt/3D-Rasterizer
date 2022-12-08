#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "camera.h"
#include "display.h"
#include "matrix4.h"
#include "renderer.h"
#include "mesh.h"

// Display Window width and height
const int WIDTH  = 1280;
const int HEIGHT = 720;
const std::string MESH_PATH = "obj/monkey_smooth.obj";

std::string format_number(int n) {
    std::string number = std::to_string(n);
    int insertPosition = number.length() - 3;
    while (insertPosition > 0) {
        number.insert(insertPosition, ",");
        insertPosition-=3;
    }
    return number;
}

int main(int argc, char* argv[]) {

    // Create new display
    Display display(WIDTH, HEIGHT, "3D Rasterizer");

    // Create new Renderer and pass our display
    Renderer renderer(display);

    Camera camera;

    SDL_Event event;
    display.drawBackground();
    display.update();

    // Load mesh
    Mesh mesh(MESH_PATH);

    bool is_running = true;
    float zoom = 1.0f;
    int mouse_clicked = 0;
    bool animate = true;
    bool fill = true;

    // Main loop
    while (is_running) {

        float delta = display.getFrameDelta();
        
        // Poll SDL events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_running = false;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    mouse_clicked = 1;
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    mouse_clicked = 2;
                }
            }
            if (event.type == SDL_MOUSEBUTTONUP) {
                mouse_clicked = 0;
            }
            if (event.type == SDL_MOUSEWHEEL) {
                if (event.wheel.y == -1) {
                    zoom += 0.05;
                }
                if (event.wheel.y == 1) {
                    zoom -= 0.05;
                }
            }
            if (event.type == SDL_MOUSEMOTION) {
                if (mouse_clicked == 1) {
                    mesh.rotation.x -= event.motion.xrel * delta;
                    mesh.rotation.y -= event.motion.yrel * delta;
                }
                if (mouse_clicked == 2) {
                    mesh.translation.x += ((float)(event.motion.xrel)) * (4.0 / WIDTH);
                    mesh.translation.y -= ((float)(event.motion.yrel)) * (4.0 / HEIGHT);
                }

            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        is_running = false;
                    case SDLK_f:
                        fill = !fill;
                        break;
                    case SDLK_r:
                        animate = !animate;
                        break;
                    case SDLK_w:
                        camera.translation.z -= 0.1f;
                        break;
                    case SDLK_s:
                        camera.translation.z += 0.1f;
                        break;
                    case SDLK_a:
                        camera.translation.x += 0.1f;
                        break;
                    case SDLK_d:
                        camera.translation.x -= 0.1f;
                        break;
                    case SDLK_SPACE:
                        camera.translation.y -= 0.1f;
                        break;
                    case SDLK_LCTRL:
                        camera.translation.y += 0.1f;
                        break;
                    case SDLK_LEFT:
                        camera.rotation.y -= 0.1f;
                        break;
                    case SDLK_RIGHT:
                        camera.rotation.y += 0.1f;
                        break;
                    case SDLK_UP:
                        camera.rotation.x += 0.1f;
                        break;
                    case SDLK_DOWN:
                        camera.rotation.x -= 0.1f;
                }
            }
        }

        // Draw checkerboard background
        display.drawBackground();

        // Update rotation amount for animation
        if (!mouse_clicked && animate) {
            mesh.rotation.x -= delta;
        }

        // Create new transformation matrix
        Matrix4 transform =
            Matrix4::initTranslation(mesh.translation.x, mesh.translation.y, 4 * zoom)
                .mul(Matrix4::initRotation(mesh.rotation.y, mesh.rotation.x, 0)
                    .mul(Matrix4::initScale(1, 1, 1))
                );

        transform = Matrix4::initTranslation(camera.translation.x, camera.translation.y, camera.translation.z).mul(Matrix4::initRotation(camera.rotation.x, camera.rotation.y, camera.rotation.z).mul(transform));

        // Draw mesh
        renderer.drawMesh(mesh, transform, 0xFFFFFFFF, fill);

        // Display mesh information
        display.drawText("Triangles: " + format_number(mesh.shapes[0].mesh.indices.size()), 8, HEIGHT - 24);
        display.drawText(MESH_PATH, 8, HEIGHT - 44);

        // Update display and clear zbuffer
        display.update();
        renderer.clearZBuffer();
    }

    return 0;
}
