BINARY=plotter
BUILD_TIME=`date +%FT%T%z`
LDFLAGS=-ldflags "-X main.BuildTime=${BUILD_TIME}"
all:
		go install "github.com/gorilla/websocket" "github.com/jacobsa/go-serial/serial"
		go build ${LDFLAGS} -o ${BINARY} main.go server.go ttl.go
