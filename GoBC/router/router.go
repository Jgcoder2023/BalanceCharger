package router

import (
	"fmt"
	"github.com/Jgcoder/gobc/home"
	"github.com/gin-gonic/gin"
)

// RouteInit  路由初始化
func RouteInit() {
	r := gin.Default()
	//r.Use(Cors())
	r.GET("/getValue", home.GetValue)
	r.GET("/SetValue", home.SetValue)

	//监听端口默认为8080
	err := r.Run(":8998")
	if err != nil {
		fmt.Println(err.Error())
	}
}
