import sys
import timeit
import os
import requests
import json

#url = "http://localhost:3000/ollama/api/generate"
url = "http://192.168.0.229:3000/ollama/api/generate"

#api_key = 'sk-8090dded451a49528beee731402c7375'
api_key = 'sk-9b8e2f13268846a1b2898f06eaa3d6a9'

def AskPola(prompt):
	headers = {	'Authorization': f'Bearer {api_key}',	"Content-Type": "application/json"}
	data = {"model": "tinyllama", "prompt": prompt , "stream": False}
	start = timeit.default_timer()
	response = requests.post(url, headers=headers, data=json.dumps(data))
	if response.status_code == 200:
		response_text = response.text
		data = json.loads(response_text)
		actual_response = data["response"]
		text = actual_response
		stop = timeit.default_timer()
		request_time = stop - start
	else:
		text ="Error:" + response.status_code + response.text
	return text 
    #+ f" request time: {request_time:.6f}"

if __name__ == "__main__":
	if len(sys.argv) != 2:
		sys.exit(1)	
	question = sys.argv[1]
	#print("You have Pola asked: " + question)
	antwort = AskPola(question)
	#print(antwort, end="")
	f = open("/mnt/ramdisk/response.txt", "w")
	f.write(antwort)
	f.close()
	print("ask",end="");
		
	sys.exit()	
	
