#include "base_statemachine.h"

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
namespace MurphyPA
{
namespace SM
{

using namespace nsii::statemachine;

template <typename Owner,
          typename s1,
          typename EventBase>
class xstatemachine_t_Ports
{
public:
    /*
    bool canPing(const s1& source, const ping& ev)
    {
        std::cout << "bool canPing(const s1& source, const ping& ev)" << " " << ev << std::endl;
        return _canPing;
    }
    bool canPong(const s1& source, const pong& ev)
    {
        std::cout << "bool canPong(const s1& source, const pong& ev)" << " " << ev << std::endl;
        return _canPong;
    }
    void doPing(const s1& source, const ping& ev)
    {
        std::cout << "void doPing(const s1& source, const ping& ev)" << " " << ev << std::endl;
    }
    void doPong(const s1& source, const pong& ev)
    {
        std::cout << "void doPong(const s1& source, const pong& ev)" << " " << ev << std::endl;
    }
    */

    PortBinding<Owner, s1, EventBase> s1_port;

};


template <typename UnderlyingModel, typename EventSourceBase, typename EventBase,
          typename MY_SOURCE_REPRESENTATION, typename s1>
class xstatemachine_t :
        public statemachine_t<EventSourceBase, EventBase>,
        public MY_SOURCE_REPRESENTATION
{
public:
    typedef statemachine_t<EventSourceBase, EventBase> inherited;

public:
    xstatemachine_t(const std::string& instanceid)
        : inherited(instanceid)
    {}

    void initialise() override
    {
        initialise_statemachine(*this, _model);
    }

    void dispatch(EventSourceBase& source, const EventBase& ev) override
    {
        // slow hack -- need a dispatch method per source type
        
        {
        const s1* source_instance = dynamic_cast<const s1*>(&source);
        if(nullptr != source_instance)
        {
            dispatch(*this, _model, *source_instance, ev);
            return;
        }
        }


        //unhandled_event(source, ev);
        throw std::logic_error("Unsupported source");
    }

    // for use as an event source
    void send(EventSourceBase& source, const EventBase& ev) override
    {
        dispatch(source, ev);
    }

    UnderlyingModel& model() { return _model; }
    const UnderlyingModel& model() const { return _model; }

    std::string modelname() const override { return "xstatemachine_t"; }
    std::string modelnamespace() const override { return "MurphyPA::SM"; }
    std::string modelguid() const override { return "78380831-ab1a-4d32-9677-8983c60d48bb"; }
    std::string modelfilename() const override { return "pingpong.sm1"; }
    std::string statemachineversion() const override { return "2"; }
    std::string comment() const override { return "None"; }

protected:
    void _model_unhandled_event(
            const EventSourceBase& source,
            const EventBase& ev)
    {
        _model.unhandled_event(source, ev);
    }

private:
        UnderlyingModel _model;


template <typename StateMachine, typename Underlying>
void process_event(StateMachine& sm, Underlying& model, const s1& source, const ping& ev)
{
    switch(sm.state())
    {
    case Ping: // 6cccc56d-b82a-4e47-924d-38fbfed57103 1
    {
        if(model.canPing(source, ev))
        {
            // FROM_LIST: 0 []
            model.doPing(source, ev); // transition
            // TO_LIST: 1 ['Pong']
            sm.set_state(Pong);
        } // canPing()

    }; break;
    default:
    {
        sm.unhandled_event(source, ev);
    }; break;
    }
}

template <typename StateMachine, typename Underlying>
void process_event(StateMachine& sm, Underlying& model, const s1& source, const pong& ev)
{
    switch(sm.state())
    {
    case Pong: // 5d031475-06ef-4191-a22a-f77615adf2eb 1
    {
        if(model.canPong(source, ev))
        {
            // FROM_LIST: 0 []
            model.doPong(source, ev); // transition
            // TO_LIST: 1 ['Ping']
            sm.set_state(Ping);
        } // canPong()

    }; break;
    default:
    {
        sm.unhandled_event(source, ev);
    }; break;
    }
}

template <typename StateMachine, typename Underlying>
void dispatch(StateMachine& sm, Underlying& model, const s1& source, const EventBase& ev)
{
    if(!sm.is_initialised())
    {
        throw std::logic_error("Statemachine " + sm.modelname() + ", " + sm.modelguid()
                               + ", " + sm.instanceid() + " is not initialised");
    }
    if(ev._typecode() == ping::_cpp_typecode())
    {
        process_event(sm, model, source, *dynamic_cast<const ping*>(&ev));
        return;
    }
    else if(ev._typecode() == pong::_cpp_typecode())
    {
        process_event(sm, model, source, *dynamic_cast<const pong*>(&ev));
        return;
    }
    else
    {
        throw std::logic_error("Unsupported event");
    }
}

template <typename StateMachine, typename Underlying>
void initialise_statemachine(StateMachine& sm, Underlying& model)
{
    if(sm.is_initialised())
    {
        throw std::logic_error("Statemachine " + sm.modelname() + ", " + sm.modelguid()
                               + ", " + sm.instanceid() + " is already initialised");
    }
    sm.set_state(TOPSTATE);
    sm.set_state(Ping);
    sm.set_initialised();
}

}; // class  xstatemachine_t

} // namespace SM
} // namespace MurphyPA
