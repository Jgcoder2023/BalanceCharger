package server

import (
	"net"
	"strings"
)

type DeviceMsg struct {
	SetInV  string `json:"set_in_v"`
	SetOutV string `json:"set_out_v"`
	OutV    string `json:"out_v"`
	OutA    string `json:"out_a"`
}

var DeviceData DeviceMsg

func Hand(c net.Conn, Msg string) {
	//分包
	// 2,0,:1.47V,:0.00A#
	Value := strings.Split(Msg, ",")
	DeviceData.SetInV = Value[0]
	DeviceData.SetOutV = Value[1]
	DeviceData.OutV = Value[2]
	DeviceData.OutA = Value[3]

	//回发消息给客户端
	_, _ = c.Write([]byte("1"))
}
