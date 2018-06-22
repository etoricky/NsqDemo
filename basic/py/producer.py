import nsq
import tornado.ioloop
import time
 
def pub_message():
    writer.pub('test', (time.strftime('%H:%M:%S')+" Hello").encode(), finish_pub)
 
def finish_pub(conn, data):
    print (data)
 
writer = nsq.Writer(['172.31.118.243:4150'])
tornado.ioloop.PeriodicCallback(pub_message, 1000).start()
nsq.run()