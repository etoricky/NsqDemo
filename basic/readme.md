Go
==

    wget https://dl.google.com/go/go1.10.2.linux-amd64.tar.gz
    sudo tar -C /usr/local -zxvf go1.10.2.linux-amd64.tar.gz

    Export Path
    /etc/profile (for a system-wide installation) or $HOME/.profile
    export PATH=$PATH:/usr/local/go/bin
    export GOROOT=/usr/local/go
    export GOPATH=$HOME/go

    Test Go
    mkdir -p $GOPATH/src/hello
    cd $GOPATH/src/hello
    vi hello.go
        package main
        import "fmt"
        func main() { fmt.Printf("hello, world\n") }
    go run hello.go
    go build
    ./hello


NSQ on CentOS 7
===============

    wget https://s3.amazonaws.com/bitly-downloads/nsq/nsq-1.0.0-compat.linux-amd64.go1.8.tar.gz
    tar -zxvf nsq-1.0.0-compat.linux-amd64.go1.8.tar.gz
    cd nsq-1.0.0-compat.linux-amd64.go1.8/bin/
    ./nsqlookupd
    ./nsqd --lookupd-tcp-address=127.0.0.1:4160 --broadcast-address=127.0.0.1
    sudo firewall-cmd --get-active-zones
    sudo firewall-cmd --zone=public --add-port=4150/tcp --permanent
    sudo firewall-cmd --zone=public --add-port=4160/tcp --permanent
    sudo firewall-cmd --zone=public --add-port=4161/tcp --permanent
    sudo firewall-cmd --zone=public --add-port=4171/tcp --permanent
    sudo firewall-cmd --reload
    sudo firewall-cmd --list-all
    ./nsqadmin --lookupd-http-address=127.0.0.1:4161
    http://172.31.118.243:4171/
    
    
NSQ on Windows
==============

    go1.10.2.windows-amd64.msi
    Git-2.17.1.2-64-bit.exe

    cmd
    nsqlookupd
    nsqd --lookupd-tcp-address=127.0.0.1:4160 --broadcast-address=127.0.0.1
    sudo firewall-cmd --get-active-zones
    sudo firewall-cmd --zone=public --add-port=4150/tcp --permanent
    sudo firewall-cmd --zone=public --add-port=4151/tcp --permanent
    sudo firewall-cmd --zone=public --add-port=4160/tcp --permanent
    sudo firewall-cmd --zone=public --add-port=4161/tcp --permanent
    sudo firewall-cmd --zone=public --add-port=4171/tcp --permanent
    sudo firewall-cmd --reload
    sudo firewall-cmd --list-all
    nsqadmin --lookupd-http-address=127.0.0.1:4161
    http://127.0.0.1:4171/
    http://172.31.118.249:4171/
    
    
    git bash
    curl -d 'hello world 1' 'http://127.0.0.1:4151/put?topic=test'
    curl -d 'hello world 2' 'http://127.0.0.1:4151/pub?topic=test'
    curl -d 'hello world 3' 'http://172.31.118.249:4151/pub?topic=test'
    
    
sender.go
=========

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

receiver.go
===========

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
    
    $ go run b.go
    2018/06/21 15:09:56 INF    1 [test/test_channel] querying nsqlookupd http://127.0.0.1:4161/lookup?topic=test
    2018/06/21 15:09:56 INF    1 [test/test_channel] (127.0.0.1:4150) connecting to nsqd
    2018/06/21 15:09:56 Received a message:
    &{[48 57 101 51 52 54 99 97 56 100 53 100 49 48 48 48] [104 101 108 108 111 44 32 119 111 114 108 100 33] 1529564172455138930 1 127.0.0.1:4150 0xc42000e140 0 0}
    2018/06/21 15:09:56 String verions of that message:
    hello, world!
