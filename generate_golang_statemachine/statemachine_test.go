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

type Source1 struct {
	MyEventSource
}

func (self *Source1) Send(source EventSource, event Event) error {
	return nil
}

type Source2 struct {
	MyEventSource
}

func (self *Source2) Send(source EventSource, event Event) error {
	return nil
}

type Source3 struct {
	MyEventSource
}

type Source4 struct {
	MyEventSource
}

const (
	TOPSTATE uint64 = iota
	T        uint64 = iota
	A        uint64 = iota
	B        uint64 = iota
	C        uint64 = iota
	D        uint64 = iota
	E        uint64 = iota
	F        uint64 = iota
	G        uint64 = iota
	H        uint64 = iota
)

type x struct {
	MyEvent
}

func (self *x) TypeCode() uint64 {
	return 1
}

type y struct {
	MyEvent
}

func (self *y) TypeCode() uint64 {
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
	_test1 bool
	_test2 bool
}

func (t *TestModel) a_entry() {
	audit("void a_entry()")
}

func (t *TestModel) a_exit() {
	audit("void a_exit()")
}

func (t *TestModel) b_entry() {
	audit("void b_entry()")
}

func (t *TestModel) b_exit() {
	audit("void b_exit()")
}

func (t *TestModel) c_entry() {
	audit("void c_entry()")
}

func (t *TestModel) c_exit() {
	audit("void c_exit()")
}

func (t *TestModel) d_entry() {
	audit("void d_entry()")
}

func (t *TestModel) d_exit() {
	audit("void d_exit()")
}

func (t *TestModel) e_entry() {
	audit("void e_entry()")
}

func (t *TestModel) e_exit() {
	audit("void e_exit()")
}

func (t *TestModel) f_entry() {
	audit("void f_entry()")
}

func (t *TestModel) f_exit() {
	audit("void f_exit()")
}

func (t *TestModel) g_entry() {
	audit("void g_entry()")
}

func (t *TestModel) g_exit() {
	audit("void g_exit()")
}

func (t *TestModel) h_entry() {
	audit("void h_entry()")
}

func (t *TestModel) h_exit() {
	audit("void h_exit()")
}

func (t *TestModel) t_entry() {
	audit("void t_entry()")
}

func (t *TestModel) t_exit() {
	audit("void t_exit()")
}

func (t *TestModel) test1(source *Source1, ev *x) bool {
	audit("bool test1(source %#v, ev %#v)",
		source,
		ev)

	return t._test1
}

func (t *TestModel) test2(source *Source1, ev *x) bool {
	audit("bool test2(source %#v, ev %#v)",
		source,
		ev)

	return t._test2
}

func (t *TestModel) action1(source *Source1, ev *x) {
	audit("action1(source %#v, ev %#v)",
		source,
		ev)
}

func (t *TestModel) action2(source *Source1, ev *x) {
	audit("action2(source %#v, ev %#v)",
		source,
		ev)
}
func (t *TestModel) action3(source *Source1, ev *y) {
	audit("action3(source %#v, ev %#v)",
		source,
		ev)
}
func (t *TestModel) action4(source *Source2, ev *z) {
	audit("action4(source %#v, ev %#v)",
		source,
		ev)
}
func (t *TestModel) action5(source *Source1, ev *x) {
	audit("action5(source %#v, ev %#v)",
		source,
		ev)
}
func (t *TestModel) action6(source *Source1, ev *y) {
	audit("action6(source %#v, ev %#v)",
		source,
		ev)
}

func (t *TestModel) UnhandledEvent(source EventSource, ev Event) {
	audit("UnhandledEvent(source %#v, ev %#v)",
		source,
		ev)
}

func (t *TestModel) SetTest1(value bool) {
	t._test1 = value
}

func (t *TestModel) SetTest2(value bool) {
	t._test2 = value
}

/*

void test2()
{
	MurphyPA::SM::xstatemachine_t<
		TestModel<Source1x, Source3x>,
	MyEventSource, MyEvent,
	Source2x, Source1x, Source3x
	> sm1("x1");
	MurphyPA::SM::xstatemachine_t<
		TestModel<Source2x, Source3x>,
	MyEventSource, MyEvent,
	Source1x, Source2x, Source3x
	> sm2("x2");

	sm1.model().test1(true);
	sm2.model().test1(false);

	sm1.initialise();
	sm2.initialise();

	sm1.dispatch(sm2, y());
	sm1.dispatch(sm2, y());
	sm1.dispatch(sm2, y());
	sm1.dispatch(sm2, y());

	sm2.dispatch(sm1, y());

	sm2.dispatch(sm1, x());
	sm2.dispatch(sm1, y());
	sm2.dispatch(sm1, x());
	sm2.dispatch(sm1, y());

	sm2.dispatch(sm1, y());

	const std::vector<std::string>& audit_trail = sm1.model().audit_trail();

	audit_trail.size();
}

void test3()
{
	MurphyPA::SM::xstatemachine_t<
		TestModelWithPorts<Source2x, Source1x, Source3x, MyEvent>,
	MyEventSource, MyEvent,
	Source2x, Source1x, Source3x
	> sm1("x1");
	MurphyPA::SM::xstatemachine_t<
		TestModelWithPorts<Source1x, Source2x, Source3x, MyEvent>,
	MyEventSource, MyEvent,
	Source1x, Source2x, Source3x
	> sm2("x2");

	sm1.model().source1_port.bind(sm1, sm2);
	sm2.model().source1_port.bind(sm2, sm1);

	sm1.model().test1(true);
	sm2.model().test1(false);

	sm1.initialise();
	sm2.initialise();

	sm2.model().source1_port.send(y());
	sm1.model().source1_port.send(y());

	sm2.model().source1_port.send(x());
	sm2.model().source1_port.send(y());
	sm2.model().source1_port.send(x());
	sm2.model().source1_port.send(y());

	const std::vector<std::string>& audit_trail = sm2.model().audit_trail();

	audit_trail.size();
}

*/

func TestStateMachine001(t *testing.T) {
	sm := new(xstatemachine_t)
	source1 := new(Source1)
	model := new(TestModel)

	sm.SetModel(model)
	sm.Initialise()
	sm.Model().(*TestModel).SetTest1(true)
	sm.Dispatch(source1, &y{})
	sm.Dispatch(source1, &x{})
	sm.Dispatch(source1, &y{})
	sm.Model().(*TestModel).SetTest1(false)
	sm.Dispatch(source1, &x{})
	sm.Dispatch(source1, &y{})

}

type EventTuple struct {
	source EventSource
	event  Event
}

type EventTest struct {
	test1  bool
	test2  bool
	events []EventTuple
}

func TestStateMachine002(t *testing.T) {
	sm := new(xstatemachine_t)
	source1 := new(Source1)
	source2 := new(Source2)
	model := new(TestModel)

	sm.SetModel(model)
	sm.Initialise()
	sm.Model().(*TestModel).SetTest1(true)

	tests := make([]EventTest, 0)

	{
		events := make([]EventTuple, 1)

		events = append(events, EventTuple{source1, &y{}})
		events = append(events, EventTuple{source1, &x{}})
		events = append(events, EventTuple{source1, &y{}})
		events = append(events, EventTuple{source2, &z{}})

		tests = append(tests, EventTest{true, false, events})
	}

	{
		events := make([]EventTuple, 1)

		events = append(events, EventTuple{source1, &x{}})
		events = append(events, EventTuple{source1, &y{}})
		events = append(events, EventTuple{source2, &z{}})

		tests = append(tests, EventTest{false, false, events})
	}

	for _, test := range tests {
		sm.Model().(*TestModel).SetTest1(test.test1)
		sm.Model().(*TestModel).SetTest2(test.test2)

		for _, evr := range test.events {
			fmt.Printf("\ndispatch: %#v %#v\n", evr.source, evr.event)
			if err := sm.Dispatch(evr.source, evr.event); err != nil {
				fmt.Println("FAIL")
			}
		}
		fmt.Println()
	}

}
