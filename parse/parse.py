import json

LOG_FILE_NAME="pid_443866_trace.log"

malloc_dict = {}
free_dict   = {}

def add_load_data(line):
    json_info = json.loads(line)
    # print("addr : ", json_info['Addr'])
    # print("type : ", json_info['type'])
    # print("size : ", json_info['size'])
    # print("backtrace : ", json_info['backtrace'])

    if json_info['type'] == "alloc":
        malloc_dict[json_info['Addr']] = json_info
    
    if json_info['type'] == "free":
        free_dict[json_info['Addr']] = json_info

def load_data():
    with open(LOG_FILE_NAME) as f:
        for line in f:
            if line.strip():
                add_load_data(line)

def parse():
    print("maybe leak memery info :")
    for key in malloc_dict:
        if key not in free_dict:
            print(">>>>>  ")
            print("addr : ", key)
            print("info : ", malloc_dict[key])

if __name__ == "__main__":
    load_data()
    parse()
