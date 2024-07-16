import os
import requests
import json
prompt = "Tell a short joke"
url = "http://localhost:3000/ollama/api/generate"
#172.17.0.1
api_key = 'sk-8090dded451a49528beee731402c7375'
#sk-8090dded451a49528beee731402c7375
headers = {
	'Authorization': f'Bearer {api_key}',
	"Content-Type": "application/json",
}
data = {
	"model": "tinyllama",
	"prompt": prompt ,
	"stream": False,
}
response = requests.post(url, headers=headers, data=json.dumps(data))
if response.status_code == 200:
	response_text = response.text
	data = json.loads(response_text)
	actual_response = data["response"]
	print(actual_response)
else:
	print("Error:", response.status_code, response.text)
