package main
    import (  
        "log"
        "github.com/nsqio/go-nsq"
    )
    func main() {
        received := make(chan bool) // 建立「已接收」頻道，作為是否接收到訊息的一個開關。
        config := nsq.NewConfig()
        q, _ := nsq.NewConsumer("test", "test_channel", config)
        q.AddHandler(nsq.HandlerFunc(func(message *nsq.Message) error {
            log.Printf("Received a message:\n%v", message)
            log.Printf("String verions of that message:\n%v", string(message.Body))
            received <- true
            return nil
        }))
        err := q.ConnectToNSQLookupd("127.0.0.1:4161") // 連線到 NSQ 叢集，而不是單個 NSQ，這樣更安全與可靠。
        if err != nil {
            log.Panic("連線失敗。")
        }
        <-received // 除非接收到訊息，不然我們就讓程式卡住。
    }