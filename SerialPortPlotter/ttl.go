package main

import (
	"io"
	"log"
	"sync"

	"github.com/jacobsa/go-serial/serial"
)
import "encoding/binary"

const baudRate = 9600
const startByteVal = 0xFF
const countOfStartBytes = 2
const (
	startByteState = iota
	highPartState  = iota
	lowPartState   = iota
)

// 0xFF, 0xFF, HighByte, LowByte, ..., 0xFF, 0xFF, HighByte, LowByte, ...
func inputAnalyser() func(byte) (uint16, bool) {
	var startBytesCount = 0
	var highVal byte
	var state = startByteState
	return func(curByte byte) (uint16, bool) {
		switch state {
		case startByteState:
			if curByte == startByteVal {
				startBytesCount++
			} else {
				startBytesCount = 0
			}
			if startBytesCount == countOfStartBytes {
				state = highPartState
			}
		case highPartState:
			highVal = curByte
			state = lowPartState
		case lowPartState:
			val := binary.BigEndian.Uint16([]byte{highVal, curByte})
			startBytesCount = 0
			state = startByteState
			return val, true
		}
		return 0, false
	}
}

func listenTTLStream(port io.ReadWriteCloser, data *[]uint16, syn *sync.Mutex) error {
	var processByte = inputAnalyser()
	for buf := make([]byte, 32); ; {
		n, err := port.Read(buf)
		if err != nil {
			log.Printf("Error: %v", err)
			return err
		}
		syn.Lock()
		for i := 0; i < n; i++ {
			val, hasVal := processByte(buf[i])
			if hasVal {
				log.Print("TTL data received: ", val)
				*data = append(*data, val)
			}
		}
		syn.Unlock()
	}
}

// RunTTLListener - Read data from USB-TTL converter
func RunTTLListener(d *[]uint16, s *sync.Mutex, portName string) error {
	serialOptions := serial.OpenOptions{
		PortName:        portName,
		BaudRate:        baudRate,
		DataBits:        8,
		StopBits:        1,
		MinimumReadSize: 1,
	}

	port, err := serial.Open(serialOptions)
	if err != nil {
		log.Fatalf("Error: %v", err)
	}
	defer port.Close()

	return listenTTLStream(port, d, s)
}
