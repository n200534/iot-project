from flask import Flask, jsonify, request
import json
import os

app = Flask(__name__)

# Route for updating data in JSON file
@app.route('/update', methods=['GET'])
def update_data():
    try:
        # Check if file exists
        if os.path.exists('data.json'):
            # Load data from JSON file
            with open('data.json', 'r') as f:
                data = json.load(f)
                key_ = 0
                for i in data.keys():
                    if key_ > int(i):
                        key_ = int(i)
                if data != {}:
                    key_ = key_ + 1
        else:
            # Create empty data object
            data = {}
            key_ = 0
        print(request.args)

        # Update data with request data
        temp_ = {}
        temp_['CenterId'] = request.args['center_id']
        temp_['Status'] = request.args['status']
        temp_['Time'] = request.args['time']
        temp_['Paper'] = request.args['paper']
        temp_['PassRefId'] = request.args['ref_id']
        
        data[key_] = temp_

        # Save updated data to JSON file
        with open('data.json', 'w') as f:
            json.dump(data, f)

        return jsonify({'message': 'Data updated successfully'})

    except Exception as e:
        return jsonify({'error': str(e)})

# Route for displaying data from JSON file
@app.route('/display', methods=['GET'])
def display_data():
    try:
        # Check if file exists
        if os.path.exists('data.json'):
            # Load data from JSON file
            with open('data.json', 'r') as f:
                data = json.load(f)
        else:
            # Return empty object
            data = {}

        return jsonify(data)

    except Exception as e:
        return jsonify({'error': str(e)})

if __name__ == '__main__':
    app.run(debug=True)
