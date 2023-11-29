import requests

url = "https://api.dida365.com/api/v2/batch/task"
# url = "https://api.dida365.com/open/v1/task"

headers = {
    # "Authorization": "Bearer xxxx",
    "Cookie": "t=xxxxx; oai=xxxx; AWSALB=xxxx; AWSALBCORS=xxxx; SESSION=xxxx; tt_distid=help-xxxx",
    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:120.0) Gecko/20100101 Firefox/120.0",
}


from datetime import datetime, timezone

def create_task(title, content):
    current_time = datetime.utcnow().replace(tzinfo=timezone.utc)
    current_time_str = current_time.strftime("%Y-%m-%dT%H:%M:%S.%f")[:-3] + "Z"

    task_data = {
        "add": [
            {
                "assignee": None,
                "content": content,
                "createdTime": current_time_str,
                "dueDate": None,
                "exDate": [],
                "id": str(current_time.timestamp()), #"2023-11-28T23:24:06.000+0000"
                "isAllDay": True,
                "isFloating": False,
                "items": [],
                "kind": None,
                "modifiedTime": current_time_str,
                "priority": 0,
                "progress": 0,
                "projectId": "inbox1011896344",
                "reminders": [],
                "sortOrder": -1533295546269696,
                "startDate": current_time_str,
                "status": 0,
                "tags": [],
                "timeZone": "Asia/Shanghai",
                "title": title
            }
        ],
        "addAttachments": [],
        "delete": [],
        "deleteAttachments": [],
        "update": [],
        "updateAttachments": []
    }

    return task_data

# 示例使用
title_input = "Your Title"
content_input = "Your Content"

task_data = create_task(title_input, content_input)
# print(task_data)



try:
    response = requests.post(url, json=task_data, headers=headers)
    response.raise_for_status()

    print(response.status_code)
    try:
        json_response = response.json()
        print(json_response)
    except requests.exceptions.JSONDecodeError:
        print("Response is not a valid JSON:", response.text)

except requests.exceptions.HTTPError as errh:
    print("HTTP Error:", errh)

except requests.exceptions.RequestException as err:
    print("Request Error:", err)
    print(response.text)
