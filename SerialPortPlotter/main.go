package main

import (
	"flag"
	"fmt"
	"log"
	"os"
	"os/signal"
	"sync"
)

const defPort = "3000"
const defHost = "localhost"
const defSerialPort = "/dev/ttyUSB0"

func main() {
	port := flag.String("p", defPort, "port to serve on")
	serialPort := flag.String("s", defSerialPort, "serial port")
	host := flag.String("h", defHost, "host to serve on")
	addr := fmt.Sprint(*host, ":", *port)
	log.Print(addr)
    flag.Parse()

	var syn = &sync.Mutex{}
	var data = make([]uint16, 0)
	go RunTTLListener(&data, syn, *serialPort)
	go RunServer(addr, &data, syn)

	// Wait for interrupt
	termCh := make(chan os.Signal)
	signal.Notify(termCh, os.Interrupt)
	<-termCh
}
