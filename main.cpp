#define RLIGHTS_IMPLEMENTATION
#define PLATFORM_DESKTOP
#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION 330
#else
    #define GLSL_VERSION 100
#endif

#include "spheretarget.h"
#include "hitscan.h"
#include "timer.h"
#include "weapon.h"
#include "InputManager.h"
#include "GameState.h"
#include "MenuScreen.h"

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "rlights.h"

#include <string>
#include <vector>

int main() {
    constexpr int NATIVE_WIDTH = 1920;
    constexpr int NATIVE_HEIGHT = 1080;

    float sensitivity = 0.19f;
    float sensitivityConstant = 0.00122f;

    int score = 0;
    int shots = 0;
    int hits = 0;
    Timer timer;

    SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_MSAA_4X_HINT);

    InitWindow(0, 0, "aimfly");
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    // aspect ratio scaling 
    float scaleX = static_cast<float>(screenWidth) / static_cast<float>(NATIVE_WIDTH);
    float scaleY = static_cast<float>(screenHeight) / static_cast<float>(NATIVE_HEIGHT);
    float aspectScale = fminf(scaleX, scaleY);

    int scaledWidth = static_cast<int>(NATIVE_WIDTH * aspectScale);
    int scaledHeight = static_cast<int>(NATIVE_HEIGHT * aspectScale);
    const int offsetX = (screenWidth - scaledWidth) / 2;
    const int offsetY = (screenHeight - scaledHeight) / 2;

    SetExitKey(KEY_NULL);

    InitAudioDevice();
    Sound shootSound = LoadSound("assets\\ghost\\shoot.mp3");

    // set up camera with the scaled viewport
    Camera camera = {0};
    camera.position = {0.0f, 0.0f, 0.0f};
    camera.target = {0.0f, 0.0f, -1.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 70.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    float yaw = 0;
    float pitch = 0;

    Shader shader = LoadShader(
        TextFormat("assets\\shaders\\glsl%i\\normalmap.vs", GLSL_VERSION),
        TextFormat("assets\\shaders\\glsl%i\\normalmap.fs", GLSL_VERSION)
    );
    shader.locs[SHADER_LOC_MAP_NORMAL] = GetShaderLocation(shader, "normalMap");
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

    Vector3 lightPosition = {0.0f, 4.0f, 0.0f};
    int lightPosLoc = GetShaderLocation(shader, "lightPos");

    float specularExponent = 128.0f;
    int specularExponentLoc = GetShaderLocation(shader, "specularExponent");

    Weapon weapon("assets\\ghost\\object.obj", "assets\\ghost\\diffuse.png", "assets\\ghost\\bump.png", shader);
    int locDiffuseSampler = GetShaderLocation(shader, "diffuseMap");
    int locNormalSampler = GetShaderLocation(shader, "normalMap");

    Vector3 position = {0.0f, 0.0f, 0.0f};

    std::vector<SphereTarget> sphereTargets{};
    sphereTargets.reserve(10);
    sphereTargets.emplace_back(Vector3{8.0f, 0.0f, 0.0f}, 0.3f);
    sphereTargets.emplace_back(Vector3{8.0f, 1.0f, 0.0f}, 0.3f);
    sphereTargets.emplace_back(Vector3{8.0f, 0.0f, 1.0f}, 0.3f);
    for (int i = 0; i < sphereTargets.size(); ++i) {
        sphereTargets[i].addShader(shader);
    }

    bool targetPresent[3][3]{
        {0, 1, 0},
        {0, 1, 1},
        {0, 0, 0}
    };

    bool cursorEnabled = false;
    SetTargetFPS(144);

    InputManager input;
    GameStateManager stateManager;
    MenuScreen menuScreen;

    while (!WindowShouldClose()) {
        input.update();

        // input handling
        if (stateManager.isState(GameState::MENU)) {
            // keyboard input for menu
            if (input.isKeyPressed(KEY_ENTER) || input.isKeyPressed(KEY_SPACE)) {
                stateManager.setState(GameState::PLAYING);
                DisableCursor();
                cursorEnabled = false;
            }
            if (input.isKeyPressed(KEY_ESCAPE)) {
                break; // exit game
            }
        }
        else if (stateManager.isState(GameState::PLAYING)) {
            // calculate forward/right vectors only when playing
            Vector3 forward = {
                cosf(pitch) * cosf(yaw),
                sinf(pitch),
                cosf(pitch) * sinf(yaw)
            };
            forward = Vector3Normalize(forward);
            Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, Vector3{0, 1, 0}));
            // mouse look
            Vector2 mouseDelta = input.getMouseDelta();
            yaw   += mouseDelta.x * sensitivity * sensitivityConstant;
            pitch -= mouseDelta.y * sensitivity * sensitivityConstant;

            if (pitch > 0.5 * PI - 0.01) pitch = 0.5 * PI - 0.01;
            if (pitch < -0.5 * PI + 0.01) pitch = -0.5 * PI + 0.01;

            // movement
            float speed = 0.01f;
            if (input.isKeyHeld(KEY_W)) camera.position = Vector3Add(camera.position, Vector3Scale(forward, speed));
            if (input.isKeyHeld(KEY_S)) camera.position = Vector3Subtract(camera.position, Vector3Scale(forward, speed));
            if (input.isKeyHeld(KEY_D)) camera.position = Vector3Add(camera.position, Vector3Scale(right, speed));
            if (input.isKeyHeld(KEY_A)) camera.position = Vector3Subtract(camera.position, Vector3Scale(right, speed));

            // shooting
            if (input.isMousePressed(MOUSE_LEFT_BUTTON)) {
                PlaySound(shootSound);
                ++shots;
                for (int i = 0; i < sphereTargets.size(); ++i) {
                    if (hitscan(camera.position, forward, sphereTargets[i])) {
                        ++hits;
                        score += 10;
                        int newY = (std::rand() % 3) - 1;
                        int newZ = (std::rand() % 3) - 1;
                        while (targetPresent[newY + 1][newZ + 1]) {
                            newY = (std::rand() % 3) - 1;
                            newZ = (std::rand() % 3) - 1;
                        }
                        targetPresent[static_cast<int>(sphereTargets[i].position.y + 1.5)]
                                    [static_cast<int>(sphereTargets[i].position.z + 1.5)] = false;
                        sphereTargets[i].position.y = newY;
                        sphereTargets[i].position.z = newZ;
                        targetPresent[newY + 1][newZ + 1] = true;
                    }
                }
            }

            // esc to return to menu
            if (input.isKeyPressed(KEY_ESCAPE)) {
                stateManager.setState(GameState::MENU);
                EnableCursor();
                cursorEnabled = true;
            }

            // update camera target
            camera.target = Vector3Add(camera.position, forward);
            camera.up = {0.0f, 1.0f, 0.0f};
        }

        // rendering
        BeginDrawing();
            ClearBackground(BLACK);
            
            if (stateManager.isState(GameState::MENU)) {
                // handle menu rendering and clicking
                MenuAction action = menuScreen.render(screenWidth, screenHeight, offsetX, offsetY, scaledWidth, scaledHeight, input);
                
                // process menu actions
                if (action == MenuAction::START_3D) {
                    stateManager.setState(GameState::PLAYING);
                    DisableCursor();
                    cursorEnabled = false;
                }
                else if (action == MenuAction::EXIT_GAME) {
                    break; // exit main loop
                }
            }
            else if (stateManager.isState(GameState::PLAYING)) {
                // recalculate vectors for rendering (they're the same as input)
                Vector3 forward = {
                    cosf(pitch) * cosf(yaw),
                    sinf(pitch),
                    cosf(pitch) * sinf(yaw)
                };
                forward = Vector3Normalize(forward);
                Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, Vector3{0, 1, 0}));

                // set viewport to maintain aspect ratio with letterboxing
                rlViewport(offsetX, offsetY, scaledWidth, scaledHeight);
                
                // clear just the game area
                DrawRectangle(offsetX, offsetY, scaledWidth, scaledHeight, SKYBLUE);

                BeginMode3D(camera);
                    BeginShaderMode(shader);
                        DrawCube(Vector3{3.0f, -4.0f, 0.0f}, 30.0, 2.0, 20.0, GRAY);
                        DrawCube(Vector3{13.0f, 4.0f, 0.0f}, 2.0, 14.0, 10.0, GRAY);
                    EndShaderMode();

                    for (int i = 0; i < sphereTargets.size(); ++i) {
                        sphereTargets[i].draw();
                    }

                    float lightPos[3] = {lightPosition.x, lightPosition.y, lightPosition.z};
                    SetShaderValue(shader, lightPosLoc, lightPos, SHADER_UNIFORM_VEC3);

                    float camPos[3] = {camera.position.x, camera.position.y, camera.position.z};
                    SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], camPos, SHADER_UNIFORM_VEC3);

                    SetShaderValue(shader, specularExponentLoc, &specularExponent, SHADER_UNIFORM_FLOAT);

                    Vector3 upVec = Vector3CrossProduct(right, forward);
                    position = Vector3Add(camera.position,
                        Vector3Add(
                            Vector3Scale(forward, 1.8f),
                            Vector3Add(
                                Vector3Scale(right, 0.8f),
                                Vector3Scale(upVec, -0.5f)
                            )
                        )
                    );

                    weapon.model.transform = MatrixRotateXYZ(Vector3{0, -yaw, pitch});
                    weapon.draw(position, 0.1f);
                EndMode3D();

                // reset viewport for ui rendering
                rlViewport(0, 0, screenWidth, screenHeight);

                // crosshair
                int centerX = offsetX + scaledWidth / 2;
                int centerY = offsetY + scaledHeight / 2;
                DrawLineEx(Vector2{static_cast<float>(centerX - 10), static_cast<float>(centerY)},
                        Vector2{static_cast<float>(centerX + 10), static_cast<float>(centerY)}, 2, BLACK);
                DrawLineEx(Vector2{static_cast<float>(centerX), static_cast<float>(centerY - 10)},
                        Vector2{static_cast<float>(centerX), static_cast<float>(centerY + 10)}, 2, BLACK);

                // ui positioned relative to game area
                int uiX = offsetX + 40;
                int uiY = offsetY + 40;
                
                DrawText((std::string{"Score: "} + std::to_string(score)).c_str(), uiX, uiY, 20, BLACK);
                DrawText((std::string{"Accuracy: "} + std::to_string(shots == 0 ? 0 : hits * 100 / shots) + "%").c_str(), uiX, uiY + 40, 20, BLACK);
                DrawText((std::string{"Time: "} + std::to_string(timer.elapsed())).c_str(), uiX, uiY + 80, 20, BLACK);

                DrawFPS(10, 10);
            }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}