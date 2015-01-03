#include <stdexcept>

#define SM_UNUSED_ARG(arg) \
{ (void)arg; }


class EventBase
{
public:
    virtual ~EventBase(){}

    virtual int _typecode() const = 0;
    //static int _cpp_typecode() { return 0; }
};


std::ostream& operator <<(std::ostream& os, const EventBase& ev)
{
    os << "Event? " << ((void*)&ev);
    return os;
}

class EventSource
{
public:
    virtual ~EventSource(){}

    virtual void send(EventSource& source, const EventBase& ev)
    {
        SM_UNUSED_ARG(source);
        SM_UNUSED_ARG(ev);
        throw std::logic_error("Must be overriden");
    }
};


std::ostream& operator <<(std::ostream& os, const EventSource& source)
{
    os << "EventSource? " << ((void*)&source);
    return os;
}

template <typename Owner, typename Target>
class PortBinding
{
public:
    PortBinding()
        : _owner(),
          _target()
    {}

    void bind(Owner& owner, Target& target)
    {
        _owner = &owner;
        _target = &target;
    }

    void bind(Owner* owner, Target* target)
    {
        _owner = owner;
        _target = target;
    }

    void send(const EventBase& ev)
    {
        if(nullptr == _target)
        {
            throw std::logic_error("Target for Port not bound");
        }
        if(nullptr == _owner)
        {
            throw std::logic_error("Owner for Port not bound");
        }
        _target->send(*_owner, ev);
    }

private:
    Owner* _owner;
    Target* _target;
};

template <typename EventSource, typename EventBase>
class statemachine_t
{
public:
    statemachine_t(const std::string& instanceid)
        : _is_initialised(),
          _state(),
          _instanceid(instanceid)
    {}

    virtual void initialise() = 0;
    virtual void dispatch(EventSource& source, const EventBase& ev) = 0;

    virtual std::string modelname() const = 0;
    virtual std::string modelnamespace() const = 0;
    virtual std::string modelguid() const = 0;
    virtual std::string modelfilename() const = 0;
    virtual std::string statemachineversion() const = 0;
    virtual std::string comment() const = 0;

    std::string instanceid() const { return _instanceid; }
    void instanceid(const std::string& value) { _instanceid = value; }

    inline int state() const { return _state; }
    inline void set_state(int value){ _state = value; }

    inline bool is_initialised() const { return _is_initialised; }
    inline void set_initialised() { _is_initialised = true; }

    virtual void unhandled_event(const EventSource& source, const EventBase& ev)
    {
        _model_unhandled_event(source, ev);
    }

protected:
    virtual void _model_unhandled_event(
            const EventSource& source,
            const EventBase& ev) = 0;

private:
    bool _is_initialised;
    int _state;
    std::string _instanceid;
};

