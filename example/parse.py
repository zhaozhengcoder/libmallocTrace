import json

data = []
with open('pid_232977_trace.log') as f:
    for line in f:
        data.append(json.loads(line))

print(data)