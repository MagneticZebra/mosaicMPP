import os
import requests
from bs4 import BeautifulSoup
from PIL import Image
from io import BytesIO
import time
import random

# Configuration
SEARCH_QUERY = "black and white images"
SAVE_FOLDER = "tiles"
NUM_IMAGES = 200  # Number of images to download
TILE_SIZE = (50, 50)  # Resize each tile to 50x50 pixels
PAGES_TO_SCRAPE = 5  # Number of pages to scrape for more results

# Ensure save directory exists
if not os.path.exists(SAVE_FOLDER):
    os.makedirs(SAVE_FOLDER)

# Function to fetch image URLs from a page
def fetch_image_urls(page_num):
    # Bing Image Search URL with page number
    SEARCH_URL = f"https://www.bing.com/images/search?q={SEARCH_QUERY.replace(' ', '+')}&first={page_num*35}&form=HDRSC2"
    headers = {"User-Agent": "Mozilla/5.0"}
    response = requests.get(SEARCH_URL, headers=headers)
    soup = BeautifulSoup(response.text, "html.parser")
    return soup.find_all("img", {"class": "mimg"})

# Download images
downloaded = 0
for page_num in range(PAGES_TO_SCRAPE):
    image_tags = fetch_image_urls(page_num)
    print(f"Scraping page {page_num + 1}...")
    
    for idx, img in enumerate(image_tags):
        if downloaded >= NUM_IMAGES:
            break
        try:
            img_url = img["src"] if "src" in img.attrs else img.get("data-src", "")
            if not img_url:
                continue

            img_data = requests.get(img_url, timeout=10).content
            img_pil = Image.open(BytesIO(img_data)).convert("L")  # Convert to grayscale
            img_pil = img_pil.resize(TILE_SIZE)  # Resize

            # Save image
            img_pil.save(os.path.join(SAVE_FOLDER, f"tile_{downloaded+1}.jpg"))
            downloaded += 1
            print(f"Downloaded {downloaded}/{NUM_IMAGES}")
            
            # Adding a random sleep time between requests to avoid being blocked
            #time.sleep(random.uniform(0.5, 2.0))

        except Exception as e:
            print(f"Failed to download image {downloaded+1}: {e}")
    
    if downloaded >= NUM_IMAGES:
        break

print(f"Images saved to '{SAVE_FOLDER}'")
