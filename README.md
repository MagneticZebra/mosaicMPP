# 🎨 Mosaic Image Generator (C++ with OpenCV)

![Mosaic Example](mosaic.jpg)

## 📌 Overview
This project creates a **photo mosaic** using a target image and a collection of smaller tile images. The program finds the best-matching tiles and arranges them to reconstruct the original image.

✅ **Features:**
- Uses **OpenCV** for image processing.
- Generates high-quality **grayscale** mosaic images.
- Supports **custom tile sizes** and resolutions.

Branch: main
- Single-threaded version of mosaic generator.
- Processes tiles and image in a single thread.

Branch: parallel
- Uses OpenMP to parallelize tile processing and mosaic building.
- Much faster on multi-core systems.


---

## 📁 Project Structure

```
/MPPProject/
│── mosaicMPP/
│   │── tiles/                  # Folder containing tile images
│   │── morganFreeman.jpg        # Target image
│   │── mosaic.jpg               # Final output mosaic
│   │── mosaic.cpp               # Main source code
│   │── image_finder.py          # Script to download tiles (optional)
│   │── README.md                # Documentation
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
Run the following command to compile the C++ program:

```sh
g++ -o mosaic mosaic.cpp `pkg-config --cflags --libs opencv4` -std=c++11
```

For **parallel processing** with OpenMP:
```sh
g++ -o mosaic mosaic.cpp -fopenmp `pkg-config --cflags --libs opencv4` -std=c++11
```

---

### **3️⃣ Run the Mosaic Generator**
Execute the program using:

```sh
./mosaic morganFreeman.jpg tiles/
```

📌 **Notes:**
- `morganFreeman.jpg` → The target image.
- `tiles/` → Folder containing small images to be used as tiles.

---

## 🖼 Sample Output

| Original Image | Mosaic Output |
|---------------|--------------|
| ![Original](morganFreeman.jpg) | ![Mosaic](mosaic.jpg) |

---

## ⚡ How It Works
1. **Loads the Target Image**: Reads the input image and resizes it for processing.
2. **Reads Tile Images**: Extracts small images from the `tiles/` folder.
3. **Finds Best Matches**: Compares each section of the target image with the tile dataset.
4. **Assembles the Mosaic**: Places the best-matching tile in each section of the image.
5. **Saves the Final Image**: Outputs the mosaic as `mosaic.jpg`.

---

## 🚀 Optional: Download Tile Images Automatically
You can use the Python script to **auto-download** black-and-white textures:

```sh
python image_finder.py
```

This script:
- **Searches for grayscale images** (textures, patterns, portraits).
- **Downloads them into the `tiles/` directory**.
- **Resizes them to the appropriate tile size**.

---

## 📜 License
This project is **open-source** and free to use. Feel free to modify and improve it! 🚀

---

### 🎯 Future Enhancements
✅ **Color Mosaic Support** 
✅ **More Image Filters & Effects**  

---

💡 **Have suggestions?** Feel free to fork, improve, and contribute to this project!

---
