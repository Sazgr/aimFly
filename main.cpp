#include <bits/stdc++.h>
#include "raylib.h"

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#include "raymath.h"
#include "rlgl.h"

#include "spheretarget.h"

#define PLATFORM_DESKTOP

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

int main()
{
    const int screen_width = 1600;
    const int screen_height = 900;
	
	float sensitivity = 0.003f;

    SetConfigFlags(FLAG_MSAA_4X_HINT); // Multi Sampling Anti Aliasing 4x

    InitWindow(screen_width, screen_height, "aimfly");
	SetExitKey(KEY_NULL);

    // Camera setup
    Camera camera = {0};
    camera.position = (Vector3){0.0f, 0.0f, 0.0f};    // Camera position
    camera.target = (Vector3){0.0f, 0.0f, -1.0f};     // Camera looking at point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};          // Camera up vector (rotation towards target)
    camera.fovy = 30.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type
	float yaw = 0;   // left/right
	float pitch = 0; // up/down
	
	Shader shader = LoadShader(TextFormat("assets\\shaders\\glsl%i\\lighting.vs", GLSL_VERSION),
                               TextFormat("assets\\shaders\\glsl%i\\lighting.fs", GLSL_VERSION));
	
    Model model = LoadModel("assets\\ghost\\object.obj");
    //Texture2D normalMap = LoadTexture("assets\\ghost\\bump.png");
	Texture2D albedoMap = LoadTexture("assets\\ghost\\diffuse.png"); 
	//Texture2D roughnessMap = LoadTexture("assets\\ghost\\roughness.png");

    for (int i = 0; i < model.materialCount; i++) {
		model.materials[i].shader = shader;
		model.materials[i].maps[MATERIAL_MAP_ALBEDO].texture = albedoMap;
		//model.materials[i].maps[MATERIAL_MAP_NORMAL].texture  = normalMap;
		// store roughness in specular map slot (many people do this)
		//model.materials[i].maps[MATERIAL_MAP_ROUGHNESS].texture = roughnessMap;
	}
	
	// Create a 1x1 white texture
	//Image whiteImg = GenImageColor(1, 1, WHITE);
	//Texture2D whiteTex = LoadTextureFromImage(whiteImg);
	//UnloadImage(whiteImg); // we don't need the CPU-side image anymore

	// Assign to shader
	//SetShaderValueTexture(shader, GetShaderLocation(shader, "albedoMap"), whiteTex);
	
	SetShaderValueTexture(shader, GetShaderLocation(shader, "albedoMap"), albedoMap);
	//SetShaderValueTexture(shader, GetShaderLocation(shader, "normalMap"), normalMap);
	//SetShaderValueTexture(shader, GetShaderLocation(shader, "roughnessMap"), roughnessMap);
	
	// Get uniform locations
	int locCameraPos = GetShaderLocation(shader, "cameraPos");
	int locLightDir = GetShaderLocation(shader, "lightDir");
	int locLightColor = GetShaderLocation(shader, "lightColor");
	int locAmbientColor = GetShaderLocation(shader, "ambientColor");
	int locAmbient = GetShaderLocation(shader, "ambient");

	// IMPORTANT: bind samplers names to texture units if necessary
	// If your shader uses sampler names 'albedoMap','normalMap','roughnessMap', ensure those names are bound.
	// Raylib may automatically bind material maps to :
	//  - MATERIAL_MAP_DIFFUSE → "texture0" or the shader's sampler at location 0.
	// Safer: set shader samplers explicitly (if your Raylib version exposes this):
	int locAlbedoSampler = GetShaderLocation(shader, "albedoMap");
	//int locNormalSampler = GetShaderLocation(shader, "normalMap");
	//int locRoughnessSampler = GetShaderLocation(shader, "roughnessMap");
	
	//Directional light
	Vector3 lightDirection = (Vector3){ 0.0f, 1.0f, 0.0f }; //downwards
	CreateLight(LIGHT_DIRECTIONAL,
                (Vector3){ 0.0f, 10.0f, 0.0f },   // position (not really used for directional)
                lightDirection,
                WHITE, shader);
	
	Vector3 lightCol = { 30.0f, 30.0f, 30.0f }; // bright white light
	lightsCount = 1;
	
    Vector3 position = { 0.0f, 0.0f, 0.0f };    // Set model position
	
	
	SphereTarget target((Vector3){8.0f, 0.0f, 0.0f}, 0.2f);
	target.addShader(shader);
    DisableCursor();
    bool cursorEnabled = false;
    SetTargetFPS(100);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        //update camera
		Vector2 mouseDelta = GetMouseDelta();

		yaw   += mouseDelta.x * sensitivity;
		pitch -= mouseDelta.y * sensitivity;

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

		// Toggle cursor on ESC
        if (IsKeyPressed(KEY_ESCAPE)) {
            EnableCursor();
            cursorEnabled = true;
        }

        // If user clicks inside window, hide cursor again
        if (cursorEnabled && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            DisableCursor();
            cursorEnabled = false;
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                //DrawGrid(10, 1.0f);     // Draw a grid

            EndMode3D();

			BeginMode3D(camera);
			
				target.draw();
				
				SetShaderValue(shader, locCameraPos, &camera.position, SHADER_UNIFORM_VEC3);
				SetShaderValue(shader, locLightDir, &lightDirection, SHADER_UNIFORM_VEC3);
				SetShaderValue(shader, locLightColor, &lightCol, SHADER_UNIFORM_VEC3);
				
				Vector3 ambientColor = (Vector3){1.0f, 1.0f, 1.0f};
				float ambient = 0.1f;
				SetShaderValue(shader, locAmbientColor, &ambientColor, SHADER_UNIFORM_VEC3);
				SetShaderValue(shader, locAmbient, &ambient, SHADER_UNIFORM_FLOAT);
				
				Vector3 upVec = Vector3CrossProduct(right, forward);
				position = Vector3Add(camera.position,
					Vector3Add(
						Vector3Scale(forward, 5.0f),   // 0.5 units in front
						Vector3Add(
							Vector3Scale(right, 1.0f), // 0.3 units to the right
							Vector3Scale(upVec, -1.0f) // 0.2 units down
						)
					)
				);

				model.transform = MatrixRotateXYZ((Vector3){0, -yaw, pitch});
				
				DrawModel(model, position, 0.1f, WHITE);
			
			EndMode3D();
			
			// Crosshair
			int centerX = GetScreenWidth()/2;
			int centerY = GetScreenHeight()/2;
			int size    = 10;   // half-length of crosshair arms
			int thickness = 2;

			DrawLineEx((Vector2){centerX - size, centerY}, 
					   (Vector2){centerX + size, centerY}, 
					   thickness, BLACK);

			DrawLineEx((Vector2){centerX, centerY - size}, 
					   (Vector2){centerX, centerY + size}, 
					   thickness, BLACK);
			
            DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(albedoMap);
	//UnloadTexture(normalMap);
	//UnloadTexture(roughnessMap);
    UnloadModel(model);         // Unload model

    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}