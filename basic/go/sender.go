package main
import (
    "log"
    "github.com/nsqio/go-nsq"
)
func main() {
    config := nsq.NewConfig()
    w, _ := nsq.NewProducer("172.31.118.243:4150", config)
    err := w.Publish("test", []byte("hello, world!"))
    if err != nil {
        log.Panic("Publish() failed")
    }
    w.Stop()
}