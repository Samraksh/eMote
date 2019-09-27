package services

//status messages for Gateway service
const (
	GSGood = iota + 1
	GSNoNet
	CoreGood
	CoreNotGood
)

//StatusLookup int to string conversion of the const
var StatusLookup = map[uint16]string{
	GSGood:      `GSGood`,
	GSNoNet:     `GSNoNet`,
	CoreGood:    `CoreGood`,
	CoreNotGood: `CoreNotGood`,
}

//Service port numbers
const (
	GSPort = iota + 1234
	CorePort
	MSPort
)

//message types exchanges between various services. Possibly not needed. Since we are not doing any marshalling
type E_SMsgType int

const (
	GatewayStausM E_SMsgType = 1
)

//GtwyStatusResponse type to be used as response to status api
type GtwyStatusResponse struct {
	Status uint16
}

//GtwyStatusRequest type to be used as first param to status rpc
type GtwyStatusRequest struct {
	Name string
}

//GtwyStatusResponse type to be used as response to status api
type CoreStatusResponse struct {
	Status int
}

//GtwyStatusRequest type to be used as first param to status rpc
type CoreStatusRequest struct {
	Name string
}
