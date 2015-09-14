package statemachine

// Note: very naive state machine implementation - but hopefully readable and easy enough to debug?

// Source:  ../samples/pingpong/pingpong.sm1

// assembly = None
// basestatemachine = None
// comment = None
// fields = None
// hassubmachines = False
// implementationversion = 0.2
// modelfilename = pingpong.sm1
// modelguid = 78380831-ab1a-4d32-9677-8983c60d48bb
// namespace = MurphyPA.SM
// readonly = False
// statemachine = None
// statemachineversion = 2
// usingnamespaces = None

//note - not using transition.eventtype yet
//package MurphyPA
//{
//package SM
//{

import (
	"errors"
	"fmt"
)

type Model_xstatemachine_t interface {
	canPing(source *s1, ev *ping) bool
	canPong(source *s1, ev *pong) bool
	doPing(source *s1, ev *ping)
	doPong(source *s1, ev *pong)
}

type Ports_xstatemachine_t interface {

	//s1_port PortBinding<Owner, s1, EventBase>

}

type PortsModel_xstatemachine_t interface {
	Model_xstatemachine_t
	Ports_xstatemachine_t
}

type xstatemachine_t struct {
	// statemachine_t<EventSourceBase, EventBase>

	initialised bool
	state       uint64
	instanceid  string
	model       Model_xstatemachine_t
}

func (sm *xstatemachine_t) State() uint64 {
	return sm.state
}

func (sm *xstatemachine_t) SetState(value uint64) {
	sm.state = value
}

func (sm *xstatemachine_t) IsInitialised() bool {
	return sm.initialised
}

func (sm *xstatemachine_t) SetInitialised(value bool) {
	sm.initialised = value
}

func (sm *xstatemachine_t) Initialise() {
	sm.initialise_statemachine()
}

func (sm *xstatemachine_t) UnhandledEvent(source EventSource, ev Event) error {
	fmt.Printf("UNHANDLED_EVENT: %#v %#v\n", source, ev)
	return nil
}

func (sm *xstatemachine_t) Dispatch(eventSource EventSource, ev Event) error {
	switch source := eventSource.(type) {
	case *s1:
		return sm.dispatch_s1(source, ev)

	default:
		return errors.New("Unsupported source")
	}
}

// for use as an event source
func (sm *xstatemachine_t) Send(source EventSource, ev Event) error {
	return sm.Dispatch(source, ev)
}

func (sm *xstatemachine_t) InstanceId() string {
	return "sm.instanceid"
}

func (sm *xstatemachine_t) Model() (model Model_xstatemachine_t) {
	return sm.model
}

func (sm *xstatemachine_t) SetModel(model Model_xstatemachine_t) {
	sm.model = model
}

func (sm *xstatemachine_t) ModelName() string {
	return "xstatemachine_t"
}

func (sm *xstatemachine_t) ModelNamespace() string {
	return "MurphyPA::SM"
}

func (sm *xstatemachine_t) ModelGuid() string {
	return "78380831-ab1a-4d32-9677-8983c60d48bb"
}

func (sm *xstatemachine_t) ModelFilename() string {
	return "pingpong.sm1"
}

func (sm *xstatemachine_t) StatemachineVersion() string {
	return "2"
}

func (sm *xstatemachine_t) Comment() string {
	return "None"
}

func (sm *xstatemachine_t) process_event_s1_ping(source *s1, ev *ping) error {
	model := sm.model
	switch sm.State() {
	case Ping: // 6cccc56d-b82a-4e47-924d-38fbfed57103 1
		{
			if model.canPing(source, ev) {
				// FROM_LIST: 0 []
				model.doPing(source, ev) // transition
				// TO_LIST: 1 ['Pong']
				sm.SetState(Pong)
			}
		}
		break
	default:
		{
			sm.UnhandledEvent(source, ev)
		}
		break
	}
	return nil
}

func (sm *xstatemachine_t) process_event_s1_pong(source *s1, ev *pong) error {
	model := sm.model
	switch sm.State() {
	case Pong: // 5d031475-06ef-4191-a22a-f77615adf2eb 1
		{
			if model.canPong(source, ev) {
				// FROM_LIST: 0 []
				model.doPong(source, ev) // transition
				// TO_LIST: 1 ['Ping']
				sm.SetState(Ping)
			}
		}
		break
	default:
		{
			sm.UnhandledEvent(source, ev)
		}
		break
	}
	return nil
}

func (sm *xstatemachine_t) dispatch_s1(source *s1, event Event) error {
	if !sm.IsInitialised() {
		return errors.New("Statemachine " + sm.ModelName() + ", " + sm.ModelGuid() + sm.InstanceId() + " is not initialised")
	}
	switch ev := event.(type) {
	case *ping:
		return sm.process_event_s1_ping(source, ev)
	case *pong:
		return sm.process_event_s1_pong(source, ev)
	default:
		return errors.New("Unsupported event")
	}
}

func (sm *xstatemachine_t) initialise_statemachine() error {
	if sm.IsInitialised() {
		return errors.New("Statemachine " + sm.ModelName() + ", " + sm.ModelGuid() + sm.InstanceId() + " is not initialised")
	}
	sm.SetState(TOPSTATE)
	sm.SetState(Ping)
	sm.SetInitialised(true)
	return nil
}

// gen complete -- class  xstatemachine_t

//} // namespace SM
//} // namespace MurphyPA
