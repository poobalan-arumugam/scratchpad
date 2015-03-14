#include <iostream>
#include <vector>
#include <sstream>
#include "base_statemachine.h"

class TEST{};

class MyEvent : public nsii::statemachine::Event {};

class MyEventSource : public nsii::statemachine::EventSource<MyEventSource, MyEvent>
{

};

class UserSource : public MyEventSource
{

};

class Source1x : public MyEventSource
{

};

class Source2x : public MyEventSource
{

};

class Source3x : public MyEventSource
{

};

class Source4x : public MyEventSource
{

};


enum ABC
{
    TOPSTATE,
    T,
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H
};

class x : public MyEvent
{
public:
    int _typecode() const override { return 1; }
    static int _cpp_typecode() { return 1; }
};

class y : public MyEvent
{
public:
    int _typecode() const override { return 2; }
    static int _cpp_typecode() { return 2; }
};

class z : public MyEvent
{
public:
    int _typecode() const override { return 3; }
    static int _cpp_typecode() { return 3; }
};

#define audit(line) \
{ \
    std::ostringstream oss; \
    oss << line << std::endl; \
    _audit(oss.str()); \
}

#define LOGFUNC(func) \
void func() \
{  \
    audit(#func); \
}


template <typename Source1, typename Source2>
class TestModel
{
public:
    TestModel()
        : _test1(),
          _test2()
    {}

    void a_entry()
    {
        audit("void a_entry()");
    }
    void a_exit()
    {
        audit("void a_exit()");
    }
    void b_entry()
    {
        audit("void b_entry()");
    }
    void b_exit()
    {
        audit("void b_exit()");
    }
    void c_entry()
    {
        audit("void c_entry()");
    }
    void c_exit()
    {
        audit("void c_exit()");
    }
    void d_entry()
    {
        audit("void d_entry()");
    }
    void d_exit()
    {
        audit("void d_exit()");
    }
    void e_entry()
    {
        audit("void e_entry()");
    }
    void e_exit()
    {
        audit("void e_exit()");
    }
    void f_entry()
    {
        audit("void f_entry()");
    }
    void f_exit()
    {
        audit("void f_exit()");
    }
    void g_entry()
    {
        audit("void g_entry()");
    }
    void g_exit()
    {
        audit("void g_exit()");
    }
    void h_entry()
    {
        audit("void h_entry()");
    }
    void h_exit()
    {
        audit("void h_exit()");
    }
    void t_entry()
    {
        audit("void t_entry()");
    }
    void t_exit()
    {
        audit("void t_exit()");
    }
    bool test1(const Source1& source, const x& ev)
    {
        audit("bool test1(const Source1& source, const x& ev)"
              << " " << source
              << " " << ev);
        return _test1;
    }
    bool test2(const Source1& source, const x& ev)
    {
        audit("bool test2(const Source1& source, const x& ev)"
              << " " << source
              << " " << ev);
        return _test2;
    }
    void action1(const Source1& source, const x& ev)
    {
        audit("void action1(const Source1& source, const x& ev)"
              << " " << source
              << " " << ev);
    }
    void action2(const Source1& source, const x& ev)
    {
        audit("void action2(const Source1& source, const x& ev)"
              << " " << source
              << " " << ev);
    }
    void action3(const Source1& source, const y& ev)
    {
        audit("void action3(const Source1& source, const y& ev)"
              << " " << source
              << " " << ev);
    }
    void action4(const Source2& source, const z& ev)
    {
        audit("void action4(const Source2& source, const z& ev)"
              << " " << source
              << " " << ev);
    }
    void action5(const Source1& source, const x& ev)
    {
        audit("void action5(const Source1& source, const x& ev)"
              << " " << source
              << " " << ev);
    }
    void action6(const Source1& source, const y& ev)
    {
        audit("void action6(const Source1& source, const y& ev)"
              << " " << source
              << " " << ev);
    }

    // ----------------------

    void unhandled_event(const MyEventSource& source, const MyEvent& ev)
    {
        audit("unhandled event for source: " << source << " and event: " << ev);
    }

    void test1(bool value)
    {
        _test1 = value;
    }

    void test2(bool value)
    {
        _test2 = value;
    }


    const std::vector<std::string>& audit_trail() const
    {
        return _audit_trail;
    }

private:
    bool _test1;
    bool _test2;
    std::vector<std::string> _audit_trail;

    void _audit(const std::string& activity)
    {
        _audit_trail.push_back(activity);
    }
};


// ----------------------------------------------------------------------

#include "result_test_statemachine1.cpp"

// ----------------------------------------------------------------------

template <typename Owner, typename Source1, typename Source2, typename EventBase>
class TestModelWithPorts :
        public TestModel<Source1, Source2>,
        public MurphyPA::SM::xstatemachine_t_Ports<Owner, Source1, Source2, EventBase>
{

};

// ---------------------------------------------------------------------

void test1()
{
    typedef Source1x Source1;
    MurphyPA::SM::xstatemachine_t<
            TestModel<Source1x, Source2x>,
            MyEventSource, MyEvent,
            Source4x, Source1x, Source2x
            > sm("x");
    sm.initialise();
    sm.model().test1(true);
    Source1 source1;
    sm.dispatch(source1, y());
    sm.dispatch(source1, x());
    sm.dispatch(source1, y());
    sm.model().test1(false);
    sm.dispatch(source1, x());
    sm.dispatch(source1, y());

    const std::vector<std::string>& audit_trail = sm.model().audit_trail();

    audit_trail.size();
}


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

int main()
{
    test1();
    test2();
    test3();
}
