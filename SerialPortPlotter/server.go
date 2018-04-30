package main

import (
	"bytes"
	"html/template"
	"log"
	"net/http"
	"strconv"
	"sync"

	"github.com/gorilla/websocket"
)

var upgrader = websocket.Upgrader{}
var data *[]uint16
var syn *sync.Mutex
var myTemplate = template.Must(template.ParseFiles("index.html"))

func echo(resp http.ResponseWriter, req *http.Request) {
	log.Print(req.Header)
	conn, err := upgrader.Upgrade(resp, req, nil)
	if err != nil {
		log.Print(err)
		return
	}
	defer conn.Close()

	syn.Lock()
	i := len(*data)
	syn.Unlock()
	for {
		var buf bytes.Buffer
		for {
			syn.Lock()
			if i < len(*data) {
				buf.WriteString(strconv.Itoa(int((*data)[i])))
				i++
				syn.Unlock()
				break
			}
			syn.Unlock()
		}
		err = conn.WriteMessage(websocket.TextMessage, buf.Bytes())
		if err != nil {
			log.Print(err)
			break
		}
	}
}

func index(resp http.ResponseWriter, req *http.Request) {
	log.Print(req.Header)
	myTemplate.Execute(resp, "ws://"+req.Host+"/echo")
}

// RunServer - init and run server
func RunServer(addr string, d *[]uint16, s *sync.Mutex) error {
	data = d
	syn = s
	http.HandleFunc("/echo", echo)
	http.HandleFunc("/", index)
	return http.ListenAndServe(addr, nil)
}
