from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route("/saiga-ui", methods=["POST"])
def put_response():
    data = request.get_json()
    
    if not data:
        return jsonify({"error": "JSON verisi bulunamadı"}), 400

    process_list = data.get("process_list", [])
    
    for process in process_list:
        pid = process.get("pid")
        name = process.get("name")
        duration = process.get("duration")

        print(f"pid: {pid} name: {name}, duration: {duration}")
         
    return jsonify({"status": "ok"})

if __name__ == "__main__":
    # Geliştirme ortamında debug moduyla çalıştırmak için
    app.run(debug = True, host = "127.0.0.1", port = 6666)
