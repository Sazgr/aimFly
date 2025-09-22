#include "raylib.h"

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#include "raymath.h"
#include "rlgl.h"

#define PLATFORM_DESKTOP

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

int main()
{
    const int screen_width = 800;
    const int screen_height = 450;
	
	float sensitivity = 0.003f;

    SetConfigFlags(FLAG_MSAA_4X_HINT); // Multi Sampling Anti Aliasing 4x

    InitWindow(screen_width, screen_height, "aimfly");
	SetExitKey(KEY_NULL);

    // Camera setup
    Camera camera = {0};
    camera.position = (Vector3){8.0f, 8.0f, 8.0f};    // Camera position
    camera.target = (Vector3){0.0f, 1.0f, -1.0f};     // Camera looking at point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type
	Vector3 cameraPosition;
	float yaw;   // left/right
	float pitch; // up/down
	
	Shader shader = LoadShader(TextFormat("assets\\shaders\\glsl%i\\lighting.vs", GLSL_VERSION),
                               TextFormat("assets\\shaders\\glsl%i\\lighting.fs", GLSL_VERSION));
	
	// Ambient light
	int ambientLoc = GetShaderLocation(shader, "ambient");
    float ambient[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
    SetShaderValue(shader, ambientLoc, ambient, SHADER_UNIFORM_VEC4);
	
	//Directional light
	CreateLight(LIGHT_DIRECTIONAL,
                (Vector3){ 0.0f, 10.0f, 0.0f },   // position (not really used for directional)
                (Vector3){ 0.0f, -1.0f, 0.0f },  // shining downward
                WHITE, shader);
	
    Model model = LoadModel("assets\\ghost.obj");
    Texture2D texture = LoadTexture("assets\\t1.png");

    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture; // Bind texture to model
	model.materials[0].shader = shader;

    Vector3 position = { 0.0f, 0.0f, 0.0f };    // Set model position

    DisableCursor();
    bool cursorEnabled = false;
    SetTargetFPS(1000);
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
		
		float speed = 0.1f;
		if (IsKeyDown(KEY_W)) cameraPosition = Vector3Add(cameraPosition, Vector3Scale(forward, speed));
		if (IsKeyDown(KEY_S)) cameraPosition = Vector3Subtract(cameraPosition, Vector3Scale(forward, speed));
		if (IsKeyDown(KEY_D)) cameraPosition = Vector3Add(cameraPosition, Vector3Scale(right, speed));
		if (IsKeyDown(KEY_A)) cameraPosition = Vector3Subtract(cameraPosition, Vector3Scale(right, speed));
		
		//camera.position = cameraPosition;
		//camera.target   = Vector3Add(cameraPosition, forward);
		//camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
		//camera.up       = Vector3CrossProduct(right, forward);

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

                DrawGrid(10, 1.0f);     // Draw a grid

            EndMode3D();

			BeginMode3D(camera);
				
				Vector3 upVec = Vector3CrossProduct(right, forward);
				position = Vector3Add(cameraPosition,
					Vector3Add(
						Vector3Scale(forward, -5.0f),   // 0.5 units in front
						Vector3Add(
							Vector3Scale(right, 0.0f), // 0.3 units to the right
							Vector3Scale(upVec, 0.0f) // 0.2 units down
						)
					)
				);
				DrawModel(model, position, 0.1f, WHITE);
				//DrawLine3D(position, Vector3Add(position, (Vector3){0,0,-1}), RED); // -Z
				//DrawLine3D(position, Vector3Add(position, (Vector3){0,0,1}), BLUE); // +Z
				//DrawLine3D(position, Vector3Add(position, (Vector3){1,0,0}), GREEN); // +X
				//DrawLine3D(position, Vector3Add(position, (Vector3){0,1,0}), YELLOW); // +Y
				/*Matrix gunTransform = {
				   -forward.x, -forward.y, -forward.z, 0,  // local -X → camera forward
					upVec.x,    upVec.y,    upVec.z, 0,    // local +Y → camera up
				   -right.x,   -right.y,   -right.z, 0,    // local -Z → camera right
					position.x, position.y, position.z, 1
				};
				
				rlPushMatrix();
					rlMultMatrixf(MatrixToFloat(gunTransform));  // Apply camera-relative transform
					DrawModel(model, (Vector3){0,0,0}, 0.05f, WHITE); // draw at origin in transformed space
				rlPopMatrix();*/
				
			
			EndMode3D();
            DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(texture);     // Unload texture
    UnloadModel(model);         // Unload model

    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}