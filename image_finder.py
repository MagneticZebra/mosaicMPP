import os
import requests
from bs4 import BeautifulSoup
from PIL import Image
from io import BytesIO

# Configuration
SEARCH_QUERY = "black and white texture"
SAVE_FOLDER = "tiles"
NUM_IMAGES = 200  # Number of images to download
TILE_SIZE = (50, 50)  # Resize each tile to 50x50 pixels

# Ensure save directory exists
if not os.path.exists(SAVE_FOLDER):
    os.makedirs(SAVE_FOLDER)

# Bing Image Search URL
SEARCH_URL = f"https://www.bing.com/images/search?q={SEARCH_QUERY.replace(' ', '+')}&form=HDRSC2"

# Fetch search results
headers = {"User-Agent": "Mozilla/5.0"}
response = requests.get(SEARCH_URL, headers=headers)
soup = BeautifulSoup(response.text, "html.parser")

# Find image URLs
image_tags = soup.find_all("img", {"class": "mimg"}, limit=NUM_IMAGES)

# Download images
for idx, img in enumerate(image_tags):
    try:
        img_url = img["src"] if "src" in img.attrs else img["data-src"]
        img_data = requests.get(img_url).content
        img_pil = Image.open(BytesIO(img_data)).convert("L")  # Convert to grayscale
        img_pil = img_pil.resize(TILE_SIZE)  # Resize

        # Save image
        img_pil.save(os.path.join(SAVE_FOLDER, f"tile_{idx+1}.jpg"))
        print(f"Downloaded {idx+1}/{NUM_IMAGES}")
    except Exception as e:
        print(f"Failed to download image {idx+1}: {e}")

print(f"Images saved to '{SAVE_FOLDER}'")
