#include <iostream>
#include <vector>
#include <sstream>
#include "base_statemachine.h"
#include <assert.h>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

class TEST{};

class MyEvent : public nsii::statemachine::Event {};

class MyEventSource : public nsii::statemachine::EventSource<MyEventSource, MyEvent>
{

};

class s1 : public MyEventSource
{

};

enum ABC
{
    TOPSTATE,
    Ping,
    Pong,
};

class ping : public MyEvent
{
public:
    int _typecode() const override { return 1; }
    static int _cpp_typecode() { return 1; }
};

class pong : public MyEvent
{
public:
    int _typecode() const override { return 2; }
    static int _cpp_typecode() { return 2; }
};

std::ostream& operator<<(std::ostream& os, const MyEvent& ev)
{
    std::string name = typeid(ev).name();
    os << " " << name; // << ": " << (&ev);
    return os;
}

std::ostream& operator<<(std::ostream& os, const MyEventSource& source)
{
    std::string name = typeid(source).name();
    os << " " << name; //<< ": " << (&source);
    return os;
}


#define audit(line) \
{ \
    std::ostringstream oss; \
    oss << line << std::endl; \
    _audit(oss.str()); \
    std::cout << oss.str(); \
}

template <typename s1>
class TestModel
{
public:
    TestModel()
        : _canPing(true),
          _canPong(true)
    {}

    bool canPing(const s1& source, const ping& ev)
    {
        audit("bool canPing(const s1& source, const ping& ev)"
              << " " << source
              << " " << ev);
        return _canPing;
    }
    bool canPong(const s1& source, const pong& ev)
    {
        audit("bool canPong(const s1& source, const pong& ev)"
              << " " << source
              << " " << ev);
        return _canPong;
    }
    void doPing(const s1& source, const ping& ev)
    {
        audit("void doPing(const s1& source, const ping& ev)"
              << " " << source
              << " " << ev);
    }
    void doPong(const s1& source, const pong& ev)
    {
        audit("void doPong(const s1& source, const pong& ev)"
              << " " << source
              << " " << ev);
    }

    // ----------------------

    void unhandled_event(const MyEventSource& source, const MyEvent& ev)
    {
        audit("unhandled event for source: " << source << " and event: " << ev);
    }

    void allowPing(bool value)
    {
        _canPing = value;
    }

    void allowPong(bool value)
    {
        _canPong = value;
    }


    const std::vector<std::string>& audit_trail() const
    {
        return _audit_trail;
    }

private:
    bool _canPing;
    bool _canPong;
    std::vector<std::string> _audit_trail;

    void _audit(const std::string& activity)
    {
        _audit_trail.push_back(activity);
    }
};


// ----------------------------------------------------------------------

#include "pingpong.sm1.cpp"

// ----------------------------------------------------------------------

/*
template <typename Owner, typename Source1, typename EventBase>
class TestModelWithPorts :
        public TestModel<Source1>,
        public MurphyPA::SM::xstatemachine_t_Ports<Owner, Source1, EventBase>
{

};
*/

// ---------------------------------------------------------------------

void test1()
{
    typedef s1 Source1;
    MurphyPA::SM::xstatemachine_t<
            TestModel<Source1>,
            MyEventSource, MyEvent, Source1, Source1> sm("x");
    sm.initialise();
    Source1 source1;

    typedef std::vector<std::string> string_vector_t;

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

    // start with a ping - followed by a pong transition
    std::cout << "State: " << sm.state() << std::endl;
    sm.dispatch(source1, ping());
    std::cout << "State: " << sm.state() << std::endl;
    sm.dispatch(source1, pong());
    std::cout << "State: " << sm.state() << std::endl;

    // must cause unhandled event
    sm.dispatch(source1, pong());
    std::cout << "State: " << sm.state() << std::endl;

    // ignore ping
    sm.model().allowPing(false);
    sm.dispatch(source1, ping());
    std::cout << "State: " << sm.state() << std::endl;

    // handle ping again
    sm.model().allowPing(true);
    sm.dispatch(source1, ping());
    std::cout << "State: " << sm.state() << std::endl;

    // must cause unhandled event
    sm.dispatch(source1, ping());
    std::cout << "State: " << sm.state() << std::endl;

    const string_vector_t& audit_trail = sm.model().audit_trail();

    assert(audit_trail.size() == expected.size());

    string_vector_t::const_iterator audit_iter = audit_trail.begin();
    string_vector_t::const_iterator expected_iter = expected.begin();
    for(;
        audit_iter != audit_trail.end() && expected_iter != expected.end();
        ++audit_iter, ++expected_iter)
    {
        if(! boost::starts_with(*audit_iter, *expected_iter))
        {
            std::cerr << "FAILURE: "
                      << *audit_iter
                      << " does not start with: "
                      << *expected_iter
                      << std::endl;
        }
    }
}

int main()
{
    test1();
}
