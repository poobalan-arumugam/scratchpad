package statemachine

import (
	"fmt"
	"testing"
)

type MyEvent struct {
}

type MyEventSource struct {
}

type UserSource struct {
	MyEventSource
}

type s1 struct {
	MyEventSource
}

func (self *s1) Send(source EventSource, event Event) error {
	return nil
}

const (
	TOPSTATE uint64 = iota
	Ping     uint64 = iota
	Pong     uint64 = iota
)

type ping struct {
	MyEvent
}

func (self *ping) TypeCode() uint64 {
	return 1
}

type pong struct {
	MyEvent
}

func (self *pong) TypeCode() uint64 {
	return 2
}

type z struct {
	MyEvent
}

func (self *z) TypeCode() uint64 {
	return 3
}

type Audit struct {
}

func (t *Audit) audit(formatString string, args ...interface{}) {
	fmt.Printf(formatString, args...)
}

func audit(formatString string, args ...interface{}) {
	fmt.Printf(formatString, args...)
	fmt.Println()
}

var logger = new(Audit)

type TestModel struct {
	_canPing bool
	_canPong bool
}

func (t *TestModel) canPing(source *s1, ev *ping) bool {
	audit("bool canPing(source %#v, ev %#v)",
		source,
		ev)

	return t._canPing
}

func (t *TestModel) canPong(source *s1, ev *pong) bool {
	audit("bool canPong(source %#v, ev %#v)",
		source,
		ev)

	return t._canPong
}

func (t *TestModel) doPing(source *s1, ev *ping) {
	audit("doPing(source %#v, ev %#v)",
		source,
		ev)
}

func (t *TestModel) doPong(source *s1, ev *pong) {
	audit("doPong(source %#v, ev %#v)",
		source,
		ev)
}

func (t *TestModel) UnhandledEvent(source EventSource, ev Event) {
	audit("UnhandledEvent(source %#v, ev %#v)",
		source,
		ev)
}

func (t *TestModel) SetCanPing(value bool) {
	t._canPing = value
}

func (t *TestModel) SetCanPong(value bool) {
	t._canPong = value
}

func TestStateMachine001(t *testing.T) {
	sm := new(xstatemachine_t)
	source1 := new(s1)
	model := new(TestModel)

	/*
	   string_vector_t expected;
	   expected.push_back("bool canPing");
	   expected.push_back("void doPing");
	   expected.push_back("bool canPong");
	   expected.push_back("void doPong");
	   expected.push_back("unhandled event");
	   expected.push_back("bool canPing");
	   expected.push_back("bool canPing");
	   expected.push_back("void doPing");
	   expected.push_back("unhandled event");
	*/

	sm.SetModel(model)
	sm.Initialise()
	sm.Model().(*TestModel).SetCanPing(true)
	sm.Model().(*TestModel).SetCanPong(true)

	sm.Dispatch(source1, &ping{})
	sm.Dispatch(source1, &pong{})
	// unhandled
	sm.Dispatch(source1, &pong{})
	sm.Model().(*TestModel).SetCanPing(false)
	sm.Dispatch(source1, &ping{})
	sm.Model().(*TestModel).SetCanPing(true)
	sm.Dispatch(source1, &ping{})
	// unhandled
	sm.Dispatch(source1, &ping{})

}

type EventTuple struct {
	source EventSource
	event  Event
}

type EventTest struct {
	canPing bool
	canPong bool
	events  []EventTuple
}

func TestStateMachine002(t *testing.T) {
	sm := new(xstatemachine_t)
	source1 := new(s1)
	model := new(TestModel)

	sm.SetModel(model)
	sm.Initialise()

	tests := make([]EventTest, 0)

	{
		events := make([]EventTuple, 0)

		events = append(events, EventTuple{source1, &ping{}})
		events = append(events, EventTuple{source1, &pong{}})
		// unhandled
		events = append(events, EventTuple{source1, &pong{}})

		tests = append(tests, EventTest{true, true, events})
	}

	{
		events := make([]EventTuple, 0)

		events = append(events, EventTuple{source1, &ping{}})

		tests = append(tests, EventTest{false, true, events})
	}

	{
		events := make([]EventTuple, 0)

		events = append(events, EventTuple{source1, &ping{}})
		// unhandled
		events = append(events, EventTuple{source1, &ping{}})

		tests = append(tests, EventTest{true, true, events})
	}

	for _, test := range tests {
		sm.Model().(*TestModel).SetCanPing(test.canPing)
		sm.Model().(*TestModel).SetCanPong(test.canPong)

		for _, evr := range test.events {
			fmt.Printf("\ndispatch: %#v %#v\n", evr.source, evr.event)
			if err := sm.Dispatch(evr.source, evr.event); err != nil {
				fmt.Println("FAIL")
			}
		}
		fmt.Println()
	}

}
