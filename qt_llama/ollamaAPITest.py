import os
import requests
import json
prompt = "Your name is Pola, you're a personal assistant that answers briefly, concisely and politely to this: Where is Bochum?"
url = "http://192.168.1.111:3000/ollama/api/generate"
#172.17.0.1
api_key = 'sk-9b8e2f13268846a1b2898f06eaa3d6a9'
#sk-8090dded451a49528beee731402c7375
headers = {
	'Authorization': f'Bearer {api_key}',
	"Content-Type": "application/json",
}
data = {
	"model": "gemma:2b",
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
