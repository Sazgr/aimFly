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

    SetConfigFlags(FLAG_WINDOW_UNDECORATED);

    InitWindow(0, 0, "aimfly");
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    RenderTexture2D aspectScreen = LoadRenderTexture(NATIVE_WIDTH, NATIVE_HEIGHT);
    GenTextureMipmaps(&aspectScreen.texture);
    SetTextureFilter(aspectScreen.texture, TEXTURE_FILTER_TRILINEAR); // TRILINEAR and mipmaps has similar performance to BILINEAR

    float aspectScale = static_cast<float>(screenWidth) / static_cast<float>(NATIVE_WIDTH);
    int scaledWidth = static_cast<int>(NATIVE_WIDTH * aspectScale);
	int scaledHeight = static_cast<int>(NATIVE_HEIGHT * aspectScale);
	
    if (scaledHeight > screenHeight) {
        aspectScale = static_cast<float>(screenHeight) / static_cast<float>(NATIVE_HEIGHT);
        scaledWidth = static_cast<int>(NATIVE_WIDTH * aspectScale);
        scaledHeight = static_cast<int>(NATIVE_HEIGHT * aspectScale);
    }

    const int offsetX = (screenWidth - scaledWidth) / 2;
    const int offsetY = (screenHeight - scaledHeight) / 2;

    SetExitKey(KEY_NULL);

    InitAudioDevice();
    Sound shootSound = LoadSound("assets\\ghost\\shoot.mp3");

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

    DisableCursor();
    bool cursorEnabled = false;
    SetTargetFPS(0);

    InputManager input;

    while (!WindowShouldClose()) {
        input.update();
        Vector2 mouseDelta = input.getMouseDelta();

        yaw   += mouseDelta.x * sensitivity * sensitivityConstant;
        pitch -= mouseDelta.y * sensitivity * sensitivityConstant;

        if (pitch > 0.5 * PI - 0.01) pitch = 0.5 * PI - 0.01;
        if (pitch < -0.5 * PI + 0.01) pitch = -0.5 * PI + 0.01;

        Vector3 forward = {
            cosf(pitch) * cosf(yaw),
            sinf(pitch),
            cosf(pitch) * sinf(yaw)
        };
        forward = Vector3Normalize(forward);

        Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, Vector3{0, 1, 0}));

        float speed = 0.01f;
        if (input.isKeyHeld(KEY_W)) camera.position = Vector3Add(camera.position, Vector3Scale(forward, speed));
        if (input.isKeyHeld(KEY_S)) camera.position = Vector3Subtract(camera.position, Vector3Scale(forward, speed));
        if (input.isKeyHeld(KEY_D)) camera.position = Vector3Add(camera.position, Vector3Scale(right, speed));
        if (input.isKeyHeld(KEY_A)) camera.position = Vector3Subtract(camera.position, Vector3Scale(right, speed));

        camera.target = Vector3Add(camera.position, forward);
        camera.up = {0.0f, 1.0f, 0.0f};

        if (input.isKeyPressed(KEY_ESCAPE)) {
            EnableCursor();
            cursorEnabled = true;
            ToggleFullscreen();
        }

        if (cursorEnabled && input.isMousePressed(MOUSE_LEFT_BUTTON)) {
            DisableCursor();
            cursorEnabled = false;
            ToggleFullscreen();
        }

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

        BeginTextureMode(aspectScreen);
            ClearBackground(SKYBLUE);

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

            int centerX = NATIVE_WIDTH / 2;
            int centerY = NATIVE_HEIGHT / 2;
            int size = 10;
            int thickness = 2;

            DrawLineEx(Vector2{static_cast<float>(centerX - size), static_cast<float>(centerY)},
                    Vector2{static_cast<float>(centerX + size), static_cast<float>(centerY)},
                    thickness, BLACK);

            DrawLineEx(Vector2{static_cast<float>(centerX), static_cast<float>(centerY - size)},
                    Vector2{static_cast<float>(centerX), static_cast<float>(centerY + size)},
                    thickness, BLACK);

            DrawText((std::string{"Score: "} + std::to_string(score)).c_str(), 40, 40, 20, BLACK);
            DrawText((std::string{"Accuracy: "} + std::to_string(shots == 0 ? 0 : hits * 100 / shots) + "%").c_str(), 40, 80, 20, BLACK);
            DrawText((std::string{"Time: "} + std::to_string(timer.elapsed())).c_str(), 40, 120, 20, BLACK);

        EndTextureMode();

        BeginDrawing();
            ClearBackground(BLACK);

            Rectangle sourceRect = {0.0f, 0.0f, static_cast<float>(NATIVE_WIDTH), static_cast<float>(-NATIVE_HEIGHT)};
            Rectangle destRect = {static_cast<float>(offsetX), static_cast<float>(offsetY), static_cast<float>(scaledWidth), static_cast<float>(scaledHeight)};

            DrawTexturePro(aspectScreen.texture, sourceRect, destRect, Vector2{0, 0}, 0.0f, WHITE);

            DrawFPS(10, 10);
        EndDrawing();
    }


    CloseWindow();
    return 0;
}