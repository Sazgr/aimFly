#include <bits/stdc++.h>
#include "raylib.h"

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#include "raymath.h"
#include "rlgl.h"

#include "spheretarget.h"
#include "hitscan.h"
#include "timer.h"

#define PLATFORM_DESKTOP

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

int main()
{
    
	
	float sensitivity = 0.19f;
	float sensitivityConstant = 0.00122f;
	
	int score = 0;
	Timer timer;

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_FULLSCREEN_MODE); // Multi Sampling Anti Aliasing 4x

    InitWindow(0, 0, "aimfly");
	const int screen_width = GetScreenWidth();
    const int screen_height = GetScreenHeight();

	SetExitKey(KEY_NULL);
	
	InitAudioDevice();
    Sound shootSound = LoadSound("assets\\ghost\\shoot.mp3");

    // Camera setup
    Camera camera = {0};
    camera.position = (Vector3){0.0f, 0.0f, 0.0f};    // Camera position
    camera.target = (Vector3){0.0f, 0.0f, -1.0f};     // Camera looking at point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};          // Camera up vector (rotation towards target)
    camera.fovy = 70.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type
	float yaw = 0;   // left/right
	float pitch = 0; // up/down
	
	Shader shader = LoadShader(TextFormat("assets\\shaders\\glsl%i\\normalmap.vs", GLSL_VERSION),
                               TextFormat("assets\\shaders\\glsl%i\\normalmap.fs", GLSL_VERSION));
	shader.locs[SHADER_LOC_MAP_NORMAL] = GetShaderLocation(shader, "normalMap");
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
	Vector3 lightPosition = { 0.0f, 4.0f, 0.0f };
    int lightPosLoc = GetShaderLocation(shader, "lightPos");
    Model model = LoadModel("assets\\ghost\\object.obj");
    Texture2D normalMap = LoadTexture("assets\\ghost\\bump.png");
	Texture2D diffuseMap = LoadTexture("assets\\ghost\\diffuse.png"); 

	model.materials[0].shader = shader;
	model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = diffuseMap;
	model.materials[0].maps[MATERIAL_MAP_NORMAL].texture = normalMap;
	
	GenTextureMipmaps(&model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture);
    GenTextureMipmaps(&model.materials[0].maps[MATERIAL_MAP_NORMAL].texture);

    SetTextureFilter(model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture, TEXTURE_FILTER_TRILINEAR);
    SetTextureFilter(model.materials[0].maps[MATERIAL_MAP_NORMAL].texture, TEXTURE_FILTER_TRILINEAR);
	
	float specularExponent = 128.0f;
    int specularExponentLoc = GetShaderLocation(shader, "specularExponent");

	int locDiffuseSampler = GetShaderLocation(shader, "diffuseMap");
	int locNormalSampler = GetShaderLocation(shader, "normalMap");
	
    Vector3 position = { 0.0f, 0.0f, 0.0f };    // Set model position
	
	std::vector<SphereTarget> sphereTargets{};
	sphereTargets.reserve(10); //reserve so targets dont get moved, invalidating GPU handles
	sphereTargets.emplace_back((Vector3){8.0f, 0.0f, 0.0f}, 0.3f);
	sphereTargets.emplace_back((Vector3){8.0f, 1.0f, 0.0f}, 0.3f);
	sphereTargets.emplace_back((Vector3){8.0f, 0.0f, 1.0f}, 0.3f);
	for (int i{}; i < sphereTargets.size(); ++i) {
		sphereTargets[i].addShader(shader);
	}
	bool targetPresent[3][3]{
		{0, 1, 0},
		{0, 1, 1},
		{0, 0, 0}
	};
	
    DisableCursor();
    bool cursorEnabled = false;
    SetTargetFPS(400);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) //detect window close button
    {
        //update camera
		Vector2 mouseDelta = GetMouseDelta();

		yaw   += mouseDelta.x * sensitivity * sensitivityConstant;
		pitch -= mouseDelta.y * sensitivity * sensitivityConstant;

		// clamp pitch so you can’t flip
		if (pitch > 0.5 * PI - 0.01) pitch = 0.5 * PI - 0.01;
		if (pitch < -0.5 * PI + 0.01) pitch = -0.5 * PI + 0.01;
		
		Vector3 forward = {
			cosf(pitch) * cosf(yaw),
			sinf(pitch),
			cosf(pitch) * sinf(yaw)
		};
		forward = Vector3Normalize(forward);

		Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, (Vector3){0,1,0}));
		
		float speed = 0.01f;
		if (IsKeyDown(KEY_W)) camera.position = Vector3Add(camera.position, Vector3Scale(forward, speed));
		if (IsKeyDown(KEY_S)) camera.position = Vector3Subtract(camera.position, Vector3Scale(forward, speed));
		if (IsKeyDown(KEY_D)) camera.position = Vector3Add(camera.position, Vector3Scale(right, speed));
		if (IsKeyDown(KEY_A)) camera.position = Vector3Subtract(camera.position, Vector3Scale(right, speed));
		
		camera.position = camera.position;
		camera.target = Vector3Add(camera.position, forward);
		camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };

		//toggle cursor on escape key
        if (IsKeyPressed(KEY_ESCAPE)) {
            EnableCursor();
            cursorEnabled = true;
			ToggleFullscreen();
        }

        //if user clicks inside window, hide cursor again
        if (cursorEnabled && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            DisableCursor();
            cursorEnabled = false;
			ToggleFullscreen();
        }
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(shootSound); // play sound on click
			for (int i{}; i < sphereTargets.size(); ++i) {
				if(hitscan(camera.position, forward, sphereTargets[i])) {
					++score;
					int newY = (std::rand() % 3) - 1;
					int newZ = (std::rand() % 3) - 1;
					while (targetPresent[newY + 1][newZ + 1]) {
						newY = (std::rand() % 3) - 1;
						newZ = (std::rand() % 3) - 1;
					}
					targetPresent[static_cast<int>(sphereTargets[i].position.y + 1.5)][static_cast<int>(sphereTargets[i].position.z + 1.5)] = false;
					sphereTargets[i].position.y = newY;
					sphereTargets[i].position.z = newZ;
					targetPresent[newY + 1][newZ + 1] = true;
				}
			}
        }
		
		

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(SKYBLUE);			
			
			BeginMode3D(camera);
				BeginShaderMode(shader);
					DrawCube(Vector3{3.0f, -4.0f, 0.0f}, 30.0, 2.0, 20.0, GRAY);
					DrawCube(Vector3{13.0f, 4.0f, 0.0f}, 2.0, 14.0, 10.0, GRAY);
					
				EndShaderMode();
				for (int i{}; i < sphereTargets.size(); ++i) {
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
						Vector3Scale(forward, 1.8f),   // 1.8 units in front
						Vector3Add(
							Vector3Scale(right, 0.8f), // 0.8 units to the right
							Vector3Scale(upVec, -0.5f) // 0.5 units down
						)
					)
				);

				model.transform = MatrixRotateXYZ((Vector3){0, -yaw, pitch});
				
				DrawModel(model, position, 0.1f, WHITE);
			
			EndMode3D();
			
			// Crosshair
			int centerX = screen_width / 2;
			int centerY = screen_height / 2;
			int size    = 10;   // half-length of crosshair arms
			int thickness = 2;

			DrawLineEx((Vector2){(float)(centerX - size), (float)(centerY)}, 
					   (Vector2){(float)(centerX + size), (float)(centerY)}, 
					   thickness, BLACK);

			DrawLineEx((Vector2){(float)(centerX), (float)(centerY - size)}, 
					   (Vector2){(float)(centerX), (float)(centerY + size)}, 
					   thickness, BLACK);
			
			DrawText((std::string{"Score: "} + std::to_string(score)).c_str(), 40, 40, 20, BLACK);
			DrawText((std::string{"Time: "} + std::to_string(timer.elapsed())).c_str(), 40, 80, 20, BLACK);
            DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(diffuseMap);
	UnloadTexture(normalMap);
    UnloadModel(model);         // Unload model

    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}