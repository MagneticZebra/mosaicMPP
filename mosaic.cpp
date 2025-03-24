#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include <opencv2/opencv.hpp>

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

void process_tile(const char *tile_path, Tile *tile)
{
    Mat img = imread(tile_path);
    if (img.empty())
        return;

    int min_dim = img.cols < img.rows ? img.cols : img.rows;
    Rect crop_region((img.cols - min_dim) / 2, (img.rows - min_dim) / 2, min_dim, min_dim);
    img = img(crop_region);

    resize(img, tile->large_tile, Size(TILE_SIZE, TILE_SIZE), 0, 0, INTER_LANCZOS4);
    resize(img, tile->small_tile, Size(TILE_SIZE / TILE_MATCH_RES, TILE_SIZE / TILE_MATCH_RES), 0, 0, INTER_LANCZOS4);

    strcpy(tile->path, tile_path);
}

void load_tiles(const char *dir_path)
{
    DIR *dir = opendir(dir_path);
    if (!dir)
    {
        printf("Could not open directory: %s\n", dir_path);
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strstr(entry->d_name, ".jpg") || strstr(entry->d_name, ".png"))
        {
            tile_count++;
        }
    }
    rewinddir(dir);

    tiles = (Tile *)malloc(tile_count * sizeof(Tile));
    int index = 0;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strstr(entry->d_name, ".jpg") || strstr(entry->d_name, ".png"))
        {
            char full_path[512];
            snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);
            process_tile(full_path, &tiles[index++]);
        }
    }
    closedir(dir);
}

Mat process_target_image(const char *image_path)
{
    Mat img = imread(image_path);
    if (img.empty())
    {
        printf("ERROR: Failed to load target image %s\n", image_path);
        exit(1);
    }
    printf("Target Image Size: %d x %d\n", img.cols, img.rows);

    int new_width = img.cols * ENLARGEMENT;
    int new_height = img.rows * ENLARGEMENT;
    resize(img, img, Size(new_width, new_height), 0, 0, INTER_LANCZOS4);
    return img;
}

int find_best_fit_tile(Mat small_img)
{
    int best_index = -1;
    double min_diff = DBL_MAX;

    for (int i = 0; i < tile_count; i++)
    {
        Mat diff;
        absdiff(small_img, tiles[i].small_tile, diff);
        Scalar diff_sum = sum(diff);
        double total_diff = diff_sum[0] + diff_sum[1] + diff_sum[2];

        if (total_diff < min_diff)
        {
            min_diff = total_diff;
            best_index = i;
        }
    }
    return best_index;
}

void build_mosaic(Mat *target_img)
{
    Mat mosaic = target_img->clone();

    for (int y = 0; y < target_img->rows; y += TILE_SIZE)
    {
        for (int x = 0; x < target_img->cols; x += TILE_SIZE)
        {
            if (x + TILE_SIZE > target_img->cols || y + TILE_SIZE > target_img->rows)
            {
                continue; // Skip tiles that go out of bounds
            }
            Rect roi(x, y, TILE_SIZE, TILE_SIZE);
            Mat small_img = (*target_img)(roi);
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