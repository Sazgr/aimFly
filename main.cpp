#define RLIGHTS_IMPLEMENTATION
#define PLATFORM_DESKTOP
#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION 330
#else
    #define GLSL_VERSION 100
#endif

#include "gamestate.h"
#include "inputmanager.h"
#include "menuscreen.h"
#include "target.h"
#include "task.h"
#include "timer.h"
#include "uicomponents.h"
#include "weapon.h"

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "rlights.h"

#include "assets/fonts/DOHYEON_REGULAR.h"
#include "assets/fonts/ORBITRON_BOLD.h"

#include <iostream>
#include <string>
#include <vector>

Font DOHYEON_REGULAR;
Font ORBITRON_BOLD;

void loadFonts();

int main() {
    constexpr int NATIVE_WIDTH = 1920;
    constexpr int NATIVE_HEIGHT = 1080;
    constexpr float TARGET_ASPECT = 16.0f / 9.0f;

    float sensitivity = 0.19f;
    float sensitivityConstant = 0.00122f;

    int score = 0;
    int shots = 0;
    int hits = 0;
	double timePerTarget = 0;
    Timer timer;

    SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_MSAA_4X_HINT);

    InitWindow(0, 0, "aimfly");
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    // 16:9 aspect ratio scaling 
    float screenAspect = (float)screenWidth / screenHeight;
    int scaledWidth, scaledHeight, offsetX, offsetY;
    
    if (screenAspect >= TARGET_ASPECT) {
        // Screen is wider than 16:9, fit to height
        scaledHeight = screenHeight;
        scaledWidth = (int)(screenHeight * TARGET_ASPECT);
        offsetX = (screenWidth - scaledWidth) / 2;
        offsetY = 0;
    } else {
        // Screen is taller than 16:9, fit to width
        scaledWidth = screenWidth;
        scaledHeight = (int)(screenWidth / TARGET_ASPECT);
        offsetX = 0;
        offsetY = (screenHeight - scaledHeight) / 2;
    }

    // Calculate scale factor for UI elements
    float aspectScale = (float)scaledWidth / NATIVE_WIDTH;

    SetExitKey(KEY_NULL);

    InitAudioDevice();
    Sound shootSound = LoadSound("assets/ghost/shoot.mp3");
	Sound killSound = LoadSound("assets/sounds/kill.mp3");

    // create render texture for 3D scene
    RenderTexture2D gameTexture = LoadRenderTexture(NATIVE_WIDTH, NATIVE_HEIGHT);

    // set up camera
    Camera camera = {0};
    camera.position = {0.0f, 0.0f, 0.0f};
    camera.target = {0.0f, 0.0f, -1.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 70.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    float yaw = 0;
    float pitch = 0;

    Shader shader = LoadShader(
        TextFormat("assets/shaders/glsl%i/normalmap.vs", GLSL_VERSION),
        TextFormat("assets/shaders/glsl%i/normalmap.fs", GLSL_VERSION)
    );
    shader.locs[SHADER_LOC_MAP_NORMAL] = GetShaderLocation(shader, "normalMap");
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

   Shader gradientShader = LoadShader(0, "assets/shaders/glsl330/gradient.fs");
    int locStart = GetShaderLocation(gradientShader, "startColor");
    int locEnd   = GetShaderLocation(gradientShader, "endColor");
    int locVertical = GetShaderLocation(gradientShader, "vertical");

    Vector3 lightPosition = {0.0f, 4.0f, 0.0f};
    int lightPosLoc = GetShaderLocation(shader, "lightPos");

    float specularExponent = 128.0f;
    int specularExponentLoc = GetShaderLocation(shader, "specularExponent");

    Weapon weapon("assets/ghost/object.obj", "assets/ghost/diffuse.png", "assets/ghost/bump.png", shader);
    int locDiffuseSampler = GetShaderLocation(shader, "diffuseMap");
    int locNormalSampler = GetShaderLocation(shader, "normalMap");

    Vector3 position = {0.0f, 0.0f, 0.0f};

	Task task(TaskId::HEADSHOT, shader);

    bool cursorEnabled = true;
    SetTargetFPS(1444);

    InputManager input;
    GameStateManager stateManager;
    MenuScreen menuScreen;

    loadFonts();

    while (!WindowShouldClose()) {
        input.update();

        // input handling
        if (stateManager.isState(GameState::MENU)) {
            if (input.isKeyPressed(KEY_ESCAPE)) {
                break; // exit game
            }
			
			BeginDrawing();
				ClearBackground(BLACK);
				
				// handle menu rendering and clicking with correct 16:9 viewport
                MenuAction action = menuScreen.render(
                    screenWidth, screenHeight, offsetX, offsetY, 
                    scaledWidth, scaledHeight, input,
                    NATIVE_WIDTH, NATIVE_HEIGHT,
                    gradientShader
                );

                switch (action) {
                    case MenuAction::PRACTICE:
						menuScreen.menus[1].visible = true;
						break;
                    case MenuAction::CALIBRATE:
						menuScreen.menus[1].visible = false;
						break;
                    case MenuAction::SETTINGS:
						menuScreen.menus[1].visible = false;
						break;
                    case MenuAction::CREDITS:
						menuScreen.menus[1].visible = false;
						break;
					case MenuAction::TRACKING:
						task.setTask(TaskId::STRAFESHOT, shader);
						break;
					case MenuAction::FLICKING:
						task.setTask(TaskId::GRIDSHOT, shader);
						break;
					case MenuAction::HEADSHOT:
						task.setTask(TaskId::HEADSHOT, shader);
						break;
					case MenuAction::BEGIN:
						stateManager.setState(GameState::PLAYING);
						DisableCursor();
						cursorEnabled = false;
						break;
                    default:
                        break;
                }
                if (action == MenuAction::EXIT) {
                    break; // exit main loop
                }
			EndDrawing();
        } else if (stateManager.isState(GameState::PLAYING)) {			
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
			Vector3 walk_w = forward;
			walk_w.y = 0;
			walk_w = Vector3Normalize(walk_w);
			Vector3 walk_d = Vector3CrossProduct(walk_w, Vector3{0, 1, 0});
            if (input.isKeyHeld(KEY_W)) camera.position = Vector3Add(camera.position, Vector3Scale(walk_w, speed));
            if (input.isKeyHeld(KEY_S)) camera.position = Vector3Subtract(camera.position, Vector3Scale(walk_w, speed));
            if (input.isKeyHeld(KEY_D)) camera.position = Vector3Add(camera.position, Vector3Scale(walk_d, speed));
            if (input.isKeyHeld(KEY_A)) camera.position = Vector3Subtract(camera.position, Vector3Scale(walk_d, speed));

			task.tick();
			
            // shooting
            if (input.isMousePressed(MOUSE_LEFT_BUTTON) ||input.isKeyPressed(KEY_SPACE)) {
                PlaySound(shootSound);
                ++shots;
                for (int i = 0; i < task.targets.size(); ++i) {
					int hitType = task.targets[i].hitscan(camera.position, forward);
                    if (hitType) {
						task.targets[i].hitType = hitType;
						task.targets[i].lastHit = timer.elapsed();
                        ++hits;
                        bool targetKilled = task.processHit(task.targets[i], hitType, score);
						if (targetKilled) {
							if (task.taskId == TaskId::HEADSHOT)
								PlaySound(killSound);
							score += 10;
							timePerTarget = timer.elapsed() * 10 / score;
						}
                    } else {
						if (timer.elapsed() - task.targets[i].lastHit >= 0.1) {
							task.targets[i].hitType = MISS;
						}
					}
                }
            } else {
				for (int i = 0; i < task.targets.size(); ++i) {
					if (timer.elapsed() - task.targets[i].lastHit >= 0.1) {
						task.targets[i].hitType = MISS;
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

			// rendering
			BeginDrawing();
				ClearBackground(BLACK);
				
                // render 3D scene to texture
                BeginTextureMode(gameTexture);
                    ClearBackground(SKYBLUE);

                    BeginMode3D(camera);
                        BeginShaderMode(shader);
                            DrawCube(Vector3{0.0f, -4.0f, 0.0f}, 54.0, 2.0, 20.0, GRAY);
                            DrawCube(Vector3{25.0f, 4.0f, 0.0f}, 2.0, 14.0, 20.0, GRAY);
                        EndShaderMode();

                        task.draw();

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
                EndTextureMode();

                // draw the texture centered on screen with proper 16:9 letterboxing
                DrawTexturePro(
                    gameTexture.texture,
                    Rectangle{0, 0, (float)NATIVE_WIDTH, -(float)NATIVE_HEIGHT}, // flip Y
                    Rectangle{(float)offsetX, (float)offsetY, (float)scaledWidth, (float)scaledHeight},
                    Vector2{0,0},
                    0.0f,
                    WHITE
                );

                // crosshair - centered in the 16:9 viewport
                int centerX = offsetX + scaledWidth / 2;
                int centerY = offsetY + scaledHeight / 2;
                DrawLineEx(Vector2{static_cast<float>(centerX - 10.0f), static_cast<float>(centerY)},
                        Vector2{static_cast<float>(centerX + 10), static_cast<float>(centerY)}, 2, BLACK);
                DrawLineEx(Vector2{static_cast<float>(centerX), static_cast<float>(centerY - 10)},
                        Vector2{static_cast<float>(centerX), static_cast<float>(centerY + 10)}, 2, BLACK);

                // ui positioned relative to 16:9 game area with scaling
                int uiX = offsetX + (int)(40 * aspectScale);
                int uiY = offsetY + (int)(40 * aspectScale);
                float fontSize = (float)(20 * aspectScale);
                int lineSpacing = (int)(40 * aspectScale);

                DrawTextEx(DOHYEON_REGULAR, (std::string{"Score: "} + std::to_string(score)).c_str(), (Vector2){(float)uiX, (float)uiY}, fontSize, 0.0f, BLACK);
                DrawTextEx(DOHYEON_REGULAR, (std::string{"Accuracy: "} + std::to_string(shots == 0 ? 0 : hits * 100 / shots) + "%").c_str(), (Vector2){(float)uiX, (float)uiY + lineSpacing}, fontSize, 0.0f, BLACK);
                DrawTextEx(DOHYEON_REGULAR, (std::string{"Time per target: "} + std::to_string(static_cast<int>(timePerTarget * 1000)) + "ms").c_str(), (Vector2){(float)uiX, (float)uiY + lineSpacing * 2}, fontSize, 0.0f, BLACK);
                DrawTextEx(DOHYEON_REGULAR, (std::string{"Time: "} + std::to_string(timer.elapsed())).c_str(), (Vector2){(float)uiX, (float)uiY + lineSpacing * 3}, fontSize, 0.0f, BLACK);

                DrawFPS(10, 10);
			EndDrawing();
		}
    }

    UnloadRenderTexture(gameTexture);
    UnloadSound(shootSound);
    CloseAudioDevice();
    UnloadShader(shader);
    UnloadShader(gradientShader);
    UnloadFont(DOHYEON_REGULAR);
    UnloadFont(ORBITRON_BOLD);
    CloseWindow();
    return 0;
}

void loadFonts() {
    DOHYEON_REGULAR = LoadFontFromMemory(".ttf", dohyeon_regular, sizeof(dohyeon_regular), 48, nullptr, 0);
    ORBITRON_BOLD = LoadFontFromMemory(".ttf", orbitron_bold, sizeof(orbitron_bold), 96, nullptr, 0);

    SetTextureFilter(DOHYEON_REGULAR.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(ORBITRON_BOLD.texture, TEXTURE_FILTER_BILINEAR);

    GenTextureMipmaps(&DOHYEON_REGULAR.texture);
    GenTextureMipmaps(&ORBITRON_BOLD.texture);
}