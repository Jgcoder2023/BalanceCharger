package server

import (
	"bufio"
	"fmt"
	"net"
	"strings"
	"sync"

	"github.com/panjf2000/ants/v2"
)

const Host = "0.0.0.0:8999"

var CTCP net.Conn

func handleConnection(c net.Conn) {
	fmt.Printf("Serving %s\n", c.RemoteAddr().String())
	CTCP = c
	for {
		//定义一个分包的结束符
		netData, err := bufio.NewReader(c).ReadString('#')
		if err != nil {
			fmt.Println(err)
			return
		}

		//获取收到的消息
		temp := strings.TrimSpace(string(netData))
		if temp == "STOP" {
			break
		}

		Hand(c, temp)
		//回发消息给客户端
		//_, err = c.Write([]byte("1"))
		if err != nil {
			fmt.Println(err.Error())
		}
	}
	err := c.Close()
	if err != nil {
		fmt.Println(err.Error())
	}
}

func CreateTcpServer() {
	l, err := net.Listen("tcp4", Host)
	if err != nil {
		fmt.Println(err)
		return
	}
	defer func(l net.Listener) {
		err := l.Close()
		if err != nil {
			fmt.Println(err.Error())
		}
	}(l)

	fmt.Println("Server Host:", Host)

	//设置 goroutine 池的容量为 100，最大阻塞队列长度为2
	p, _ := ants.NewPool(100, ants.WithMaxBlockingTasks(2))
	defer p.Release()

	var wg sync.WaitGroup
	for {
		c, err := l.Accept()
		if err != nil {
			fmt.Println(err)
			return
		}

		go func(c net.Conn) {
			wg.Add(1)
			err := p.Submit(wrapper(c, &wg))
			if err != nil {
				fmt.Printf("task:%d err:%v\n", c, err)
				wg.Done()
			}
		}(c)

	}
}

func wrapper(c net.Conn, wg *sync.WaitGroup) func() {
	return func() {
		handleConnection(c)
		wg.Done()
	}
}
