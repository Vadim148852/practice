from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from cpp_module.filter import apply_filter_cpp

app = FastAPI()

# Додаємо підтримку CORS для дозволу запитів з вашого фронтенду
app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://127.0.0.1:8001"],  # Дозволяємо запити тільки з цього домену
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Константа для ROOM_ID
ROOM_ID = "room_6000"

room_commands = {}

# Обробка малювання на дошці
@app.post("/filter/{room_id}")
def filter_image(room_id: str, payload: dict):
    if room_id != ROOM_ID:
        raise HTTPException(status_code=404, detail="Room not found")
    
    image_data = payload.get("image_data")
    filter_name = payload.get("filter_name")
    width = payload.get("width")
    height = payload.get("height")

    if not image_data:
        raise HTTPException(status_code=400, detail="No image data provided")
    if not width or not height:
        raise HTTPException(status_code=400, detail="Missing width or height")

    print(f"Filter requested: {filter_name}")
    print(f"Image data length: {len(image_data)}")
    print(f"Dimensions: {width}x{height}")

    filtered = apply_filter_cpp(image_data, width, height, filter_name)

    print(f"Filtered data length: {len(filtered)}")

    return {"image_data": filtered}

@app.get("/draw/{room_id}")
def get_commands(room_id: str):
    if room_id != ROOM_ID:
        raise HTTPException(status_code=404, detail="Room not found")

    return room_commands.get(room_id, [])

@app.post("/draw/{room_id}")
def save_command(room_id: str, cmd: dict):
    if room_id != ROOM_ID:
        raise HTTPException(status_code=404, detail="Room not found")

    room_commands.setdefault(room_id, []).append(cmd)
    return {"status": "ok"}
