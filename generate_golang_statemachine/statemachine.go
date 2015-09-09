package statemachine

type Event interface {
	TypeCode() uint64
}

type EventSource interface {
	Send(source EventSource, event Event) error
}

type PortBinding interface {
	Send(event Event) error
}

type StateMachine interface {
	Initialise() error
	Dispatch(eventSource EventSource, event Event) error
	UnhandledEvent(eventSource EventSource, event Event) error

	Instanceid() string
	SetInstanceid(value string)

	State() uint64
	SetState(value uint64)

	IsInitialised() bool
	SetIsInitialised(value bool)

	// informational
	ModelName() string
	ModelNamespace() string
	ModelGuid() string
	ModelFilename() string
	StatemachineVersion() string
	Comment() string
}
