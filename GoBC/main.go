package main

import (
	"github.com/Jgcoder/gobc/router"
	"github.com/Jgcoder/gobc/server"
	"sync"
)

var once sync.Once

func main() {
	//保证只执行一次
	onceFunc := func() {
		go router.RouteInit()
	}
	once.Do(onceFunc)
	server.CreateTcpServer()
}
