from flask import Flask, request
import os
from datetime import datetime


app = Flask(__name__)

@app.route('/upload', methods=['POST'])
def upload_file():
    file = request.files['file']
    if file:
        timestamp = datetime.now().strftime("%Y%m%d%H%M%S")
        folder_path = f"./uploads/{timestamp}"
        os.makedirs(folder_path,exist_ok=True)
        
        filename = file.filename
        file_path = os.path.join(folder_path, filename)

        file.save(file_path)
        print(f"File uploaded successfully Saved as: {file_path}")
        return 'File uploaded sucessfully'
    else:
        print("no file uploaded")
        return 'No file uploaded'

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5000)
