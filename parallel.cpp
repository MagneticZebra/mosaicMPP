#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include <opencv2/opencv.hpp>

#include <omp.h>
#include <vector>
#include <string>

#define TILE_SIZE 50
#define TILE_MATCH_RES 5
#define ENLARGEMENT 8
#define OUT_FILE "mosaic.jpg"

using namespace cv;

typedef struct
{
    char path[256];
    Mat large_tile;
    Mat small_tile;
} Tile;

Tile *tiles;
int tile_count = 0;

// Loads an image, crops it square, and resizes into both large and small versions
void process_tile(const char *tile_path, Tile *tile)
{
    Mat img = imread(tile_path);
    if (img.empty())
        return;

    // Crops the center image to a square
    int min_dim = img.cols < img.rows ? img.cols : img.rows;
    Rect crop_region((img.cols - min_dim) / 2, (img.rows - min_dim) / 2, min_dim, min_dim);
    img = img(crop_region);

    // Resizes the image to the large and small tile sizes
    resize(img, tile->large_tile, Size(TILE_SIZE, TILE_SIZE), 0, 0, INTER_LANCZOS4);
    resize(img, tile->small_tile, Size(TILE_SIZE / TILE_MATCH_RES, TILE_SIZE / TILE_MATCH_RES), 0, 0, INTER_LANCZOS4);

    strcpy(tile->path, tile_path);
}

//  Scans the provided directory, counts and loads .jpg or .png files, and processes each tile
void load_tiles(const char *dir_path)
{
    DIR *dir = opendir(dir_path);
    if (!dir)
    {
        printf("Could not open directory: %s\n", dir_path);
        return;
    }

    struct dirent *entry;
    std::vector<std::string> tile_paths;

    // Step 1: Collect all .jpg and .png paths
    while ((entry = readdir(dir)) != NULL)
    {
        if (strstr(entry->d_name, ".jpg") || strstr(entry->d_name, ".png"))
        {
            char full_path[512];
            snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);
            tile_paths.push_back(full_path);
        }
    }
    closedir(dir);

    tile_count = tile_paths.size();
    tiles = (Tile *)malloc(tile_count * sizeof(Tile));

// Step 2: Process each tile in parallel
#pragma omp parallel for
    for (int i = 0; i < tile_paths.size(); i++)
    {
        process_tile(tile_paths[i].c_str(), &tiles[i]);
    }
}

// Loads the target image, resizes it to the enlarged size
Mat process_target_image(const char *image_path)
{
    Mat img = imread(image_path);
    if (img.empty())
    {
        printf("ERROR: Failed to load target image %s\n", image_path);
        exit(1);
    }
    printf("Target Image Size: %d x %d\n", img.cols, img.rows);

    // Resize the target image to the enlarged size (gives more detail so you can fit more tiles into the mosaic)
    int new_width = img.cols * ENLARGEMENT;
    int new_height = img.rows * ENLARGEMENT;
    resize(img, img, Size(new_width, new_height), 0, 0, INTER_LANCZOS4);
    return img;
}

// Finds the best fit tile for a given small image by comparing it with all tiles
int find_best_fit_tile(Mat small_img)
{
    int best_index = -1;
    double min_diff = DBL_MAX;

    for (int i = 0; i < tile_count; i++)
    {
        Mat diff;
        absdiff(small_img, tiles[i].small_tile, diff);               // Uses pixel-wise absolute difference (absdiff) and color sum to find the most similar tile
        Scalar diff_sum = sum(diff);                                 // Sums the differences across all channels
        double total_diff = diff_sum[0] + diff_sum[1] + diff_sum[2]; // picks the tile with the least difference in color

        if (total_diff < min_diff)
        {
            min_diff = total_diff;
            best_index = i;
        }
    }
    return best_index;
}

// Builds the mosaic by iterating over the target image and replacing each tile with the best fit tile
void build_mosaic(Mat *target_img)
{
    Mat mosaic = target_img->clone();

#pragma omp parallel for collapse(2)
    for (int y = 0; y < target_img->rows; y += TILE_SIZE)
    {
        for (int x = 0; x < target_img->cols; x += TILE_SIZE)
        {
            if (x + TILE_SIZE > target_img->cols || y + TILE_SIZE > target_img->rows)
            {
                continue; // Skip tiles that go out of bounds
            }
            Rect roi(x, y, TILE_SIZE, TILE_SIZE);
            Mat small_img = (*target_img)(roi).clone(); // clone avoids shared memory issues
            resize(small_img, small_img, Size(TILE_SIZE / TILE_MATCH_RES, TILE_SIZE / TILE_MATCH_RES));

            int best_fit = find_best_fit_tile(small_img);
            if (best_fit != -1)
            {
                tiles[best_fit].large_tile.copyTo(mosaic(roi));
            }
        }
    }
    imwrite(OUT_FILE, mosaic);
    printf("Mosaic image saved as %s\n", OUT_FILE);
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Usage: %s <target image> <tiles directory>\n", argv[0]);
        return 1;
    }

    load_tiles(argv[2]);
    if (tile_count == 0)
    {
        printf("No tiles found in %s\n", argv[2]);
        return 1;
    }

    Mat target_img = process_target_image(argv[1]);
    build_mosaic(&target_img);

    free(tiles);
    return 0;
}