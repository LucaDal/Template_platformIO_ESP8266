import json
from requests import get, post
import sys

IP = "IP_ADDRESS"

def get_version_from_TOKEN(TOKEN):
    data_TOKEN = get("https://{}/ota/api/get/version/{}".format(IP,TOKEN)).text
    if data_TOKEN == "none" or data_TOKEN is None:
        return None
    return json.loads(data_TOKEN)["version"]


def get_latest_version(current_version : list ):
    if current_version[-1] < 255:
        current_version[-1] = current_version[-1] + 1
    else:
        current_version[-1] = 0
        if current_version[-2] < 255:
            current_version[-2] = current_version[-2] + 1
    current_version = list(map(lambda x: str(x),current_version))
    return ".".join(current_version)


def start_upload(firmware_path, token):

    version = get_version_from_TOKEN(token)
    if version is None:
        print("No board for token: {}".format(token))
        return
    
    current_int_version = list(map(lambda x: int(x),version.split(".")))
    new_version = get_latest_version(current_int_version)
    print("New version: {} => {}\n".format(version,new_version))

    res = post(
        url="https://{}/ota/uploader".format(IP),
        data={'token': token, 'version': new_version},
        files= {'file': open(firmware_path,'rb')})
    
    print("Server status code : {}\n".format(str(res.status_code)))
    

def main():
    start_upload(sys.argv[1],sys.argv[2])


if __name__ == "__main__":
    main()

