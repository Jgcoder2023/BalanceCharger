package home

import (
	"github.com/Jgcoder/gobc/server"
	"github.com/gin-gonic/gin"
	"math/rand"
)

// GetValue 获取value
func GetValue(c *gin.Context) {
	c.JSON(200, server.DeviceData)
	return
}

// SetValue App设置电压
func SetValue(c *gin.Context) {
	setValue := c.DefaultQuery("setValue", "0,0")
	server.CTCP.Write([]byte(setValue))
	c.JSON(200, gin.H{"code": 200, "msg": "ok"})
	return
}

// 生成随机数
func random(min, max int) int {
	return rand.Intn(max-min) + min
}
