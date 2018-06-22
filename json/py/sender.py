import nsq
import tornado.ioloop
import time

import json

class EmailMessage():
    def __init__(self):
        self.Id = "d69d7de3-1219-4fe6-9dbf-89e5866b9c26"
        self.TemplateID = "2760F1A1-0DC8-48B5-B9F2-29144E0C5CC9"
        self.KeyValues = []
        self.SendTo = "abssdf@rhyta.com"
        self.Subject = "Verify your email address"
    def toJSON(self):
        return json.dumps(self, default=lambda o: o.__dict__, 
            sort_keys=False, indent=2)
            
message = EmailMessage()
message.KeyValues.append({"field": "{Title}","value": "Mrs"})
message.KeyValues.append({"field": "{LastName}","value": "Let"})
message.KeyValues.append({"field": "{VerifyUrl}","value": "http://baidu.com"})
 
def pub_message():
    writer.pub('test', message.toJSON().encode(), finish_pub)
 
def finish_pub(conn, data):
    print (data)
    
writer = nsq.Writer(['172.31.118.243:4150'])
tornado.ioloop.PeriodicCallback(pub_message, 1000).start()
nsq.run()

