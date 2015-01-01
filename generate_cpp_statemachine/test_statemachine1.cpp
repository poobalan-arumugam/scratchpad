#include <iostream>
#include "base_statemachine.h"

class TEST{};

class UserSource : public EventSource
{

};

class Source1x : public EventSource
{

};

class Source2x : public EventSource
{

};

class Source3x : public EventSource
{

};

class Source4x : public EventSource
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

class MessageBase {};

class x : public MessageBase {};
class y : public MessageBase {};
class z : public MessageBase {};

#define LOGFUNC(func) \
void func() \
{  \
    std::cout << #func << std::endl; \
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
        std::cout << "void a_entry()" << std::endl;
    }
    void a_exit()
    {
        std::cout << "void a_exit()" << std::endl;
    }
    void b_entry()
    {
        std::cout << "void b_entry()" << std::endl;
    }
    void b_exit()
    {
        std::cout << "void b_exit()" << std::endl;
    }
    void c_entry()
    {
        std::cout << "void c_entry()" << std::endl;
    }
    void c_exit()
    {
        std::cout << "void c_exit()" << std::endl;
    }
    void d_entry()
    {
        std::cout << "void d_entry()" << std::endl;
    }
    void d_exit()
    {
        std::cout << "void d_exit()" << std::endl;
    }
    void e_entry()
    {
        std::cout << "void e_entry()" << std::endl;
    }
    void e_exit()
    {
        std::cout << "void e_exit()" << std::endl;
    }
    void f_entry()
    {
        std::cout << "void f_entry()" << std::endl;
    }
    void f_exit()
    {
        std::cout << "void f_exit()" << std::endl;
    }
    void g_entry()
    {
        std::cout << "void g_entry()" << std::endl;
    }
    void g_exit()
    {
        std::cout << "void g_exit()" << std::endl;
    }
    void h_entry()
    {
        std::cout << "void h_entry()" << std::endl;
    }
    void h_exit()
    {
        std::cout << "void h_exit()" << std::endl;
    }
    void t_entry()
    {
        std::cout << "void t_entry()" << std::endl;
    }
    void t_exit()
    {
        std::cout << "void t_exit()" << std::endl;
    }
    bool test1(const Source1& source, const x& ev)
    {
        std::cout << "bool test1(const Source1& source, const x& ev)" << " " << ev << std::endl;
        return _test1;
    }
    bool test2(const Source1& source, const x& ev)
    {
        std::cout << "bool test2(const Source1& source, const x& ev)" << " " << ev << std::endl;
        return _test2;
    }
    void action1(const Source1& source, const x& ev)
    {
        std::cout << "void action1(const Source1& source, const x& ev)" << " " << ev << std::endl;
    }
    void action2(const Source1& source, const x& ev)
    {
        std::cout << "void action2(const Source1& source, const x& ev)" << " " << ev << std::endl;
    }
    void action3(const Source1& source, const y& ev)
    {
        std::cout << "void action3(const Source1& source, const y& ev)" << " " << ev << std::endl;
    }
    void action4(const Source2& source, const z& ev)
    {
        std::cout << "void action4(const Source2& source, const z& ev)" << " " << ev << std::endl;
    }
    void action5(const Source1& source, const x& ev)
    {
        std::cout << "void action5(const Source1& source, const x& ev)" << " " << ev << std::endl;
    }
    void action6(const Source1& source, const y& ev)
    {
        std::cout << "void action6(const Source1& source, const y& ev)" << " " << ev << std::endl;
    }

    // ----------------------

    void unhandled_event(const EventSource& source, const MessageBase& ev)
    {
        std::cout << ev << std::endl;
    }

    void test1(bool value)
    {
        _test1 = value;
    }

    void test2(bool value)
    {
        _test2 = value;
    }

private:
    bool _test1;
    bool _test2;
};


// ----------------------------------------------------------------------

#include "result_test_statemachine1.cpp"

// ----------------------------------------------------------------------

template <typename Owner, typename Source1, typename Source2>
class TestModelWithPorts :
        public TestModel<Source1, Source2>,
        public MurphyPA::SM::xstatemachine_t_Ports<Owner, Source1, Source2>
{

};

// ---------------------------------------------------------------------

GTEST_TEST_SUITE( statemachine1_suite )

GTEST_TEST_CASE(test_statemachine_1)
{
    typedef Source1x Source1;
    MurphyPA::SM::xstatemachine_t<
            TestModel<Source1x, Source2x>,
            EventSource, MessageBase,
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
}


GTEST_TEST_CASE(test_statemachine_2)
{
    MurphyPA::SM::xstatemachine_t<
            TestModel<Source1x, Source3x>,
            EventSource, MessageBase,
            Source2x, Source1x, Source3x
            > sm1("x1");
    MurphyPA::SM::xstatemachine_t<
            TestModel<Source2x, Source3x>,
            EventSource, MessageBase,
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
}

GTEST_TEST_CASE(test_statemachine_3_ports)
{
    MurphyPA::SM::xstatemachine_t<
            TestModelWithPorts<Source2x, Source1x, Source3x>,
            EventSource, MessageBase,
            Source2x, Source1x, Source3x
            > sm1("x1");
    MurphyPA::SM::xstatemachine_t<
            TestModelWithPorts<Source1x, Source2x, Source3x>,
            EventSource, MessageBase,
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
}

GTEST_TEST_SUITE_END()
