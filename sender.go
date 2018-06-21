package main
    import (
        "log"
        "github.com/nsqio/go-nsq"
    )
    func main() {
        config := nsq.NewConfig()
        w, _ := nsq.NewProducer("127.0.0.1:4150", config)
        err := w.Publish("test", []byte("hello, world!"))
        if err != nil {
            log.Panic("Publish() failed")
        }
        w.Stop()
    }
    
    $ go run a.go
    2018/06/21 14:56:12 INF    1 (127.0.0.1:4150) connecting to nsqd
    2018/06/21 14:56:12 INF    1 stopping
    2018/06/21 14:56:12 INF    1 exiting router