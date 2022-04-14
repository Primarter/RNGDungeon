#include "game.hpp"
#include "stdInclude.hpp"
#include "lib/Lib.hpp"
#include "Generator.hpp"


#define CELL_SIZE (1.0f)
#define CELL_SIZE2 (CELL_SIZE/2.0f)

#define WALL_THICCNESS (0.1f)


void drawGridPoints(game_t &game)
{
    for (int i = 0 ; i < game.map.size() ; ++i) {
        int x = i % game.size_x;
        int z = floor(i / game.size_x);

        Vector3 p = {
            x - game.size_x/2.f,
            0.0f,
            z - game.size_z/2.f
        };

        if (game.map[i])
            DrawCubeV(p, {.1f, .1f, .1f}, RED);
        else
            DrawCubeV(p, {.1f, .1f, .1f}, WHITE);
    }
}

void drawWalls(game_t &game)
{
    for (int i = 0 ; i < game.map.size() ; ++i) {
        int x = i % game.size_x;
        int z = floor(i / game.size_x);

        // x axis
        if (x < game.size_x - 1 && game.map[i] != game.map[i + 1]) {
            DrawModelEx(game.wall,
                {x - game.size_x/2.f + CELL_SIZE2, 0, z - game.size_z/2.f},
                {0, 1, 0}, 0,
                {1, 1, 1},
                WHITE);
        }
        // z axis
        if (z < game.size_z-1 && game.map[i] != game.map[i + game.size_x]) {
            DrawModelEx(game.wall,
                {x - game.size_x/2.f, 0, z - game.size_z/2.f + CELL_SIZE2},
                {0,1,0}, 90,
                {1,1,1},
                WHITE);
        }
    }
}

void drawGrounds(game_t &game)
{
    const float GROUND_HEIGHT = -0.25f;

    for (int i = 0 ; i < game.map.size() ; ++i) {
        int x = i % game.size_x;
        int z = floor(i / game.size_x);

        DrawModelEx(game.ground,
            {
                x * CELL_SIZE - game.size_x/2.f,
                GROUND_HEIGHT,
                z * CELL_SIZE - game.size_z/2.f
            },
            {0, 1, 0}, 0,
            {1, 1, 1},
            WHITE);
    }
}

void update(game_t &game)
{
    UpdateCamera(&game.camera);
    UpdateLightValues(game.shader_material, game.light);

    float cameraPos[3] = {game.camera.position.x, game.camera.position.y, game.camera.position.z };
    SetShaderValue(game.shader_material, game.shader_material.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
}

int main(void)
{
    rngd::Generator generator;
    generator.setup();
    rngd::Generator::ExportGrid eg = generator.getRandomMap();
    game_t game;

    InitWindow(1280, 720, "walls");
    SetTargetFPS(60);

    game.camera.position = {0.0f, 10.0f, 10.0f};
    game.camera.target = {0.0f, 0.0f, 0.0f};
    game.camera.up = {0.0f, 1.0f, 0.0f};
    game.camera.fovy = 45.0f;
    game.camera.projection = CAMERA_PERSPECTIVE;
    game.size_x = eg.width;
    game.size_z = eg.height;
    game.map = eg.grid;

    SetCameraMode(game.camera, CAMERA_ORBITAL);

    game.shader_material = LoadShader(
        "resources/shader/wall/wall.vert",
        "resources/shader/wall/wall.frag");

    Texture texture = LoadTexture("resources/texel_checker.png");

    // cannot tell what are usefull
    game.shader_material.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(game.shader_material, "matModel");
    game.shader_material.locs[SHADER_LOC_MATRIX_NORMAL] = GetShaderLocation(game.shader_material, "matNormal");
    game.shader_material.locs[SHADER_LOC_MATRIX_MVP] = GetShaderLocation(game.shader_material, "mvp");
    game.shader_material.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(game.shader_material, "viewPos");

    game.wall = LoadModelFromMesh(GenMeshCube(WALL_THICCNESS, 0.5f, 1.0f));
    game.wall.materials[0].shader = game.shader_material;
    game.wall.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = GREEN;

    game.ground = LoadModelFromMesh(GenMeshPlane(1.0f, 1.0f, 1, 1));
    game.ground.materials[0].shader = game.shader_material;
    game.ground.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = WHITE;
    game.ground.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    game.light = CreateLight(LIGHT_POINT, {10.0f, 5.0f, 0.0f}, Vector3Zero(), WHITE, game.shader_material);

    while (!WindowShouldClose())
    {
        update(game);

        BeginDrawing();
            ClearBackground({60, 60, 60, 255});
            BeginMode3D(game.camera);

            drawGridPoints(game);
            drawWalls(game);
            drawGrounds(game);
            DrawSphere({game.light.position}, 0.5f, RAYWHITE);
            // DrawGrid(SIZE, 1.0f);

            EndMode3D();
        EndDrawing();
    }

    CloseWindow();

    // generator.stop();

    return 0;
}


// CAMERA_CUSTOM = 0,
// CAMERA_FREE,
// CAMERA_ORBITAL,
// CAMERA_FIRST_PERSON,
// CAMERA_THIRD_PERSON
