# 🎨 Mosaic Image Generator (C++ with OpenCV)

![Mosaic Example](mosaic.jpg)

## 📌 Overview

This project creates a **photo mosaic** using a target image and a collection of smaller tile images. The program finds the best-matching tiles and arranges them to reconstruct the original image.

> Turn **any image** into a beautiful mosaic using smaller tile images.  
> Now supports **both grayscale and color** input!

## 🚀 Features

✅ Works with both **grayscale and color images**  
✅ Includes both **serial and parallel (OpenMP)** versions  
✅ Uses **OpenCV** for image processing  
✅ Supports customizable tile resolution and matching precision  
✅ Optional Python script to fetch tile images for you

---

## 📁 Project Structure

```
/MPPProject/
│── mosaicMPP/
│   │── images/
│   │── output/
│   │── tiles/
│   │── morganFreeman.jpg
│   │── mosaic.jpg
│   │── serial.cpp
│   │── parallel.cpp
│   │── run_mosaic.sh
│   │── image_finder.py
│   │── timing_results.txt
│   │── README.md
```

---

## 🛠 Installation & Setup

### **1️⃣ Install Dependencies**

Ensure you have **OpenCV** and **G++** installed.

#### 🖥 Linux (Ubuntu/Debian)

```sh
sudo apt update
sudo apt install libopencv-dev g++
```

#### 🍎 macOS (Homebrew)

```sh
brew install opencv
brew install gcc
```

---

### **2️⃣ Compile the Program**

Run the following command to compile the Serial C++ program:

```sh
g++ serial.cpp -o serial `pkg-config --cflags --libs opencv4` -std=c++11
```

For **parallel processing** with OpenMP:

```sh
g++ parallel.cpp -o parallel -fopenmp `pkg-config --cflags --libs opencv4` -std=c++11
```

---

### **3️⃣ Run the Mosaic Generator**

Execute the program using:

```sh
./<serial or parallel> <target_image> <tiles_directory> <output_image>
```

📌 **Notes:**

- `morganFreeman.jpg` → The target image.
- `tiles/` → Folder containing small images to be used as tiles.

### 🔁 Automation Script

The `run_mosaic.sh` script automatically compiles both the serial and parallel versions of the mosaic generator, processes every image in the `images/` folder, and saves the results in the `output/` directory. It runs each version 4 times per image and logs execution times in `timing_results.txt`.

---

## 🖼 Sample Output

| Original Image                 | Mosaic Output         |
| ------------------------------ | --------------------- |
| ![Original](morganFreeman.jpg) | ![Mosaic](mosaic.jpg) |

---

## ⚡ How It Works

1. **Loads the Target Image**: Reads the input image and resizes it for processing.
2. **Reads Tile Images**: Extracts small images from the `tiles/` folder.
3. **Finds Best Matches**: Compares each section of the target image with the tile dataset.
4. **Assembles the Mosaic**: Places the best-matching tile in each section of the image.
5. **Saves the Final Image**: Outputs the mosaic as whatever you named it.

---

## 🚀 Optional: Download Tile Images Automatically

You can use the Python script to **auto-download** black-and-white textures:

```sh
python image_finder.py
```

This script:

- **Searches for images** (can adjust to find grayscale or color tiles depending on what you need).
- **Downloads them into the `tiles/` directory**.
- **Resizes them to the appropriate tile size**.

---

## 📜 License

This project is **open-source** and free to use. Feel free to modify and improve it! 🚀

---

### 🎯 Future Enhancements

✅ **More Image Filters & Effects**

---

💡 **Have suggestions?** Feel free to fork, improve, and contribute to this project!

---
