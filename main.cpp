// g++ Main.cpp -o Main -I raylib/ -L raylib/ -lraylib -lopengl32 -lgdi32 -lwinmm
//   ./Main.exe

#include "raylib.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

struct Tile
{
    Rectangle location_in_sprite_sheet;
};

int main()
{
    std::ifstream tile_map_file;
    tile_map_file.open("Tilemap.txt");

    std::string image_name;
    int tile_count;
    Tile *tile_list;
    Vector2 grid_size;
    std::vector<std::vector<int>> grid_list;

    std::string line;
    // Read IMAGE_NAME line (keyword should be "IMAGE_NAME")
    if (tile_map_file.is_open())
    {
        std::string searchTarget;
        for (int i = 0; std::getline(tile_map_file, line); i++)
        {
            if (line.find("IMAGE_NAME ") == 0)
            {
                searchTarget = "IMAGE_NAME ";
                image_name = (line.erase(0, searchTarget.length()));
            }
            if (line.find("TILE_COUNT ") == 0)
            {
                searchTarget = "TILE_COUNT ";
                tile_count = std::stoi(line.erase(0, searchTarget.length()));

                // Putting tile data into tile_list
                tile_list = new Tile[tile_count];
                for (int i = 0; i < tile_count; i++)
                {
                    std::getline(tile_map_file, line, ' ');
                    tile_list[i].location_in_sprite_sheet.x = std::stoi(line);
                    std::getline(tile_map_file, line, ' ');
                    tile_list[i].location_in_sprite_sheet.y = std::stoi(line);
                    std::getline(tile_map_file, line, ' ');
                    tile_list[i].location_in_sprite_sheet.width = std::stoi(line);
                    std::getline(tile_map_file, line);
                    tile_list[i].location_in_sprite_sheet.height = std::stoi(line);
                    std::cout << "Tile List: " << tile_list[i].location_in_sprite_sheet.x << tile_list[i].location_in_sprite_sheet.y
                              << tile_list[i].location_in_sprite_sheet.width << tile_list[i].location_in_sprite_sheet.height << "\n";
                }
            }
            if (line.find("GRID ") == 0)
            {
                searchTarget = "GRID ";
                grid_size.x = std::stoi(line.erase(0, searchTarget.length()));
                grid_size.y = std::stoi(line.erase(0, 2));
                std::cout << "grid_size: " << grid_size.x << "," << grid_size.y << "\n";

                // Putting grid data into grid_list
                std::cout << "grid list:\n";
                for (int i = 0; i < grid_size.y; i++)
                {
                    std::vector<int> temp_row;
                    for (int j = 0; j < grid_size.x; j++)
                    {
                        if (j + 1 == grid_size.x) // If we're at the last element of the row
                            std::getline(tile_map_file, line);
                        else
                            std::getline(tile_map_file, line, ' ');

                        temp_row.push_back(std::stoi(line));
                    }
                    grid_list.push_back(temp_row);
                }

                for (int i = 0; i < grid_size.y; i++)
                {
                    for (int j = 0; j < grid_size.x; j++)
                    {
                        std::cout << grid_list[i][j];
                    }
                    std::cout << std::endl;
                }
            }
        }
    }

    tile_map_file.close();
    // Assuming each tile is 32x32
    int WINDOW_WIDTH = grid_size.x * 32 * 5;
    int WINDOW_HEIGHT = grid_size.y * 32 * 5;

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Exercise 04 - Tilemap");
    SetTargetFPS(60);

    Texture2D tile_texture = LoadTexture(image_name.c_str());

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        for (int row = 0; row < grid_size.y; row++)
        {
            for (int col = 0; col < grid_size.x; ++col)
            {
                // int tileIndex = grid_list[row][row * grid_size.x + col];
                int tileIndex = grid_list[row][col];
                if (tileIndex >= 0 && tileIndex < tile_count)
                {
                    Rectangle source_rect = tile_list[tileIndex].location_in_sprite_sheet;
                    Rectangle dest_rect = {col * 32.0f * 5.0f, row * 32.0f * 5.0f, 32.0f * 5.0f, 32.0f * 5.0f};
                    DrawTexturePro(tile_texture, source_rect, dest_rect, {0.0f, 0.0f}, 0.0f, WHITE);
                }
            }
        }

        EndDrawing();
    }

    UnloadTexture(tile_texture);
    CloseWindow();

    return 0;
}
