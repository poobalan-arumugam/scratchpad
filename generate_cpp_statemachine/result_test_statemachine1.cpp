#include "base_statemachine.h"

// Note: very naive state machine implementation - but hopefully readable and easy enough to debug?

// Source:  test_machine1.sm1

// assembly = None
// basestatemachine = None
// comment = None
// fields = None
// hassubmachines = False
// implementationversion = 0.2
// modelfilename = test_machine1.sm1
// modelguid = 7e46f114-606e-4584-8492-dcf41ee57660
// namespace = MurphyPA.SM
// readonly = False
// statemachine = None
// statemachineversion = 16
// usingnamespaces = None

//note - not using transition.eventtype yet
namespace MurphyPA
{
namespace SM
{

template <typename Owner, typename Source1, typename Source2>
class xstatemachine_t_Ports
{
public:
    /*
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
    */

    PortBinding<Owner, Source1> source1_port;

    PortBinding<Owner, Source2> source2_port;

};


template <typename UnderlyingModel, typename EventSource, typename EventBase, 
          typename MY_SOURCE_REPRESENTATION, typename Source1, typename Source2>
class xstatemachine_t : public statemachine_t<EventSource, EventBase>, public MY_SOURCE_REPRESENTATION
{
public:
    typedef statemachine_t<EventSource, EventBase> inherited;

public:
    xstatemachine_t(const std::string& instanceid)
        : inherited(instanceid)
    {}

    void initialise() override
    {
        initialise_statemachine(*this, _model);
    }

    void dispatch(EventSource& source, const EventBase& ev) override
    {
        // slow hack -- need a dispatch method per source type
        
        {
        const Source1* source_instance = dynamic_cast<const Source1*>(&source);
        if(nullptr != source_instance)
        {
            dispatch(*this, _model, *source_instance, ev);
            return;
        }
        }

        {
        const Source2* source_instance = dynamic_cast<const Source2*>(&source);
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
    void send(EventSource& source, const EventBase& ev) override 
    {
        dispatch(source, ev);
    }

    UnderlyingModel& model() { return _model; }
    const UnderlyingModel& model() const { return _model; }

    std::string modelname() const override { return "xstatemachine_t"; }
    std::string modelnamespace() const override { return "MurphyPA::SM"; }
    std::string modelguid() const override { return "7e46f114-606e-4584-8492-dcf41ee57660"; }
    std::string modelfilename() const override { return "test_machine1.sm1"; }
    std::string statemachineversion() const override { return "16"; }
    std::string comment() const override { return "None"; }

protected:
    void _model_unhandled_event(
            const EventSource& source,
            const EventBase& ev)
    {
        _model.unhandled_event(source, ev);
    }

private:
        UnderlyingModel _model;


template <typename StateMachine, typename Underlying>
void process_event(StateMachine& sm, Underlying& model, const Source1& source, const x& ev)
{
    switch(sm.state())
    {
    case A: // bbd966da-e5dc-4e16-9434-09f19ed47960 3
    {
        if(model.test2(source, ev))
        {
            // FROM_LIST: 1 ['T']
            model.a_exit(); // exit-1
            model.t_exit(); // exit-2
            model.action5(source, ev); // transition
            // TO_LIST: 2 ['D', 'H']
            model.d_entry(); // entry-1
            model.h_entry(); // entry-1
            sm.set_state(H);
        } // test2()

        else
        if(model.test1(source, ev))
        {
            // FROM_LIST: 1 ['T']
            model.a_exit(); // exit-1
            model.t_exit(); // exit-2
            model.action1(source, ev); // transition
            // TO_LIST: 2 ['D', 'E']
            model.d_entry(); // entry-1
            model.e_entry(); // entry-1
            model.f_entry(); // entry-2
            sm.set_state(F);
        } // test1()

        else // None
        {
            // FROM_LIST: 1 ['T']
            model.a_exit(); // exit-1
            model.t_exit(); // exit-2
            model.action2(source, ev); // transition
            // TO_LIST: 1 ['D']
            model.d_entry(); // entry-1
            model.g_entry(); // entry-2
            sm.set_state(G);
        } // None

    }; break;
    case B: // e5f5bec1-3279-42a2-95da-15d9805aad79 3
    {
        if(model.test2(source, ev))
        {
            // FROM_LIST: 1 ['T']
            model.b_exit(); // exit-1
            model.a_exit(); // exit-1
            model.t_exit(); // exit-2
            model.action5(source, ev); // transition
            // TO_LIST: 2 ['D', 'H']
            model.d_entry(); // entry-1
            model.h_entry(); // entry-1
            sm.set_state(H);
        } // test2()

        else
        if(model.test1(source, ev))
        {
            // FROM_LIST: 1 ['T']
            model.b_exit(); // exit-1
            model.a_exit(); // exit-1
            model.t_exit(); // exit-2
            model.action1(source, ev); // transition
            // TO_LIST: 2 ['D', 'E']
            model.d_entry(); // entry-1
            model.e_entry(); // entry-1
            model.f_entry(); // entry-2
            sm.set_state(F);
        } // test1()

        else // None
        {
            // FROM_LIST: 1 ['T']
            model.b_exit(); // exit-1
            model.a_exit(); // exit-1
            model.t_exit(); // exit-2
            model.action2(source, ev); // transition
            // TO_LIST: 1 ['D']
            model.d_entry(); // entry-1
            model.g_entry(); // entry-2
            sm.set_state(G);
        } // None

    }; break;
    case C: // 0b075041-aa7c-4538-b0b2-81db66fc97b0 3
    {
        if(model.test2(source, ev))
        {
            // FROM_LIST: 1 ['T']
            model.c_exit(); // exit-1
            model.b_exit(); // exit-1
            model.a_exit(); // exit-1
            model.t_exit(); // exit-2
            model.action5(source, ev); // transition
            // TO_LIST: 2 ['D', 'H']
            model.d_entry(); // entry-1
            model.h_entry(); // entry-1
            sm.set_state(H);
        } // test2()

        else
        if(model.test1(source, ev))
        {
            // FROM_LIST: 1 ['T']
            model.c_exit(); // exit-1
            model.b_exit(); // exit-1
            model.a_exit(); // exit-1
            model.t_exit(); // exit-2
            model.action1(source, ev); // transition
            // TO_LIST: 2 ['D', 'E']
            model.d_entry(); // entry-1
            model.e_entry(); // entry-1
            model.f_entry(); // entry-2
            sm.set_state(F);
        } // test1()

        else // None
        {
            // FROM_LIST: 1 ['T']
            model.c_exit(); // exit-1
            model.b_exit(); // exit-1
            model.a_exit(); // exit-1
            model.t_exit(); // exit-2
            model.action2(source, ev); // transition
            // TO_LIST: 1 ['D']
            model.d_entry(); // entry-1
            model.g_entry(); // entry-2
            sm.set_state(G);
        } // None

    }; break;
    default:
    {
        sm.unhandled_event(source, ev);
    }; break;
    }
}

template <typename StateMachine, typename Underlying>
void process_event(StateMachine& sm, Underlying& model, const Source1& source, const y& ev)
{
    switch(sm.state())
    {
// Drop transition: 0 1a6ca210-bca6-4a41-9567-f3dae0ff5037
    case D: // 7915e533-f297-4ef2-84eb-4a6341ebdbb1 1
    {
        {
            // FROM_LIST: 0 []
            model.d_exit(); // exit-1
            model.action3(source, ev); // transition
            // TO_LIST: 3 ['T', 'A', 'B']
            model.t_entry(); // entry-1
            model.a_entry(); // entry-1
            model.b_entry(); // entry-1
            sm.set_state(B);
        } // None

    }; break;
    case E: // 4750b88a-14ac-464e-99e8-369fd59a6708 1
    {
        {
            // FROM_LIST: 0 []
            model.e_exit(); // exit-1
            model.d_exit(); // exit-1
            model.action3(source, ev); // transition
            // TO_LIST: 3 ['T', 'A', 'B']
            model.t_entry(); // entry-1
            model.a_entry(); // entry-1
            model.b_entry(); // entry-1
            sm.set_state(B);
        } // None

    }; break;
    case F: // dd0657e6-adcd-4e94-bca3-5326be56e1e5 1
    {
        {
            // FROM_LIST: 0 []
            model.f_exit(); // exit-1
            model.e_exit(); // exit-1
            model.d_exit(); // exit-1
            model.action3(source, ev); // transition
            // TO_LIST: 3 ['T', 'A', 'B']
            model.t_entry(); // entry-1
            model.a_entry(); // entry-1
            model.b_entry(); // entry-1
            sm.set_state(B);
        } // None

    }; break;
    case G: // ed8cd3e7-f252-4a9d-8878-e8621f2ad378 1
    {
        {
            // FROM_LIST: 1 ['D']
            model.g_exit(); // exit-1
            model.d_exit(); // exit-2
            model.action6(source, ev); // transition
            // TO_LIST: 2 ['T', 'A']
            model.t_entry(); // entry-1
            model.a_entry(); // entry-1
            sm.set_state(A);
        } // None

    }; break;
    case H: // 34e01365-a6de-4d5d-818e-b336e5f885c5 1
    {
        {
            // FROM_LIST: 0 []
            model.h_exit(); // exit-1
            model.d_exit(); // exit-1
            model.action3(source, ev); // transition
            // TO_LIST: 3 ['T', 'A', 'B']
            model.t_entry(); // entry-1
            model.a_entry(); // entry-1
            model.b_entry(); // entry-1
            sm.set_state(B);
        } // None

    }; break;
    default:
    {
        sm.unhandled_event(source, ev);
    }; break;
    }
}

template <typename StateMachine, typename Underlying>
void process_event(StateMachine& sm, Underlying& model, const Source2& source, const z& ev)
{
    switch(sm.state())
    {
    case B: // e5f5bec1-3279-42a2-95da-15d9805aad79 1
    {
        {
                model.action4(source, ev); // internal
        }
    }; break;
    case C: // 0b075041-aa7c-4538-b0b2-81db66fc97b0 1
    {
        {
                model.action4(source, ev); // internal
        }
    }; break;
    default:
    {
        sm.unhandled_event(source, ev);
    }; break;
    }
}

template <typename StateMachine, typename Underlying>
void dispatch(StateMachine& sm, Underlying& model, const Source1& source, const EventBase& ev)
{
    if(!sm.is_initialised())
    {
        throw std::logic_error("Statemachine " + sm.modelname() + ", " + sm.modelguid()
                               + ", " + sm.instanceid() + " is not initialised");
    }
    if(ev._typecode() == x::_cpp_typecode())
    {
        process_event(sm, model, source, *dynamic_cast<const x*>(&ev));
        return;
    }
    else if(ev._typecode() == y::_cpp_typecode())
    {
        process_event(sm, model, source, *dynamic_cast<const y*>(&ev));
        return;
    }
    else
    {
        throw std::logic_error("Unsupported event");
    }
}


template <typename StateMachine, typename Underlying>
void dispatch(StateMachine& sm, Underlying& model, const Source2& source, const EventBase& ev)
{
    if(!sm.is_initialised())
    {
        throw std::logic_error("Statemachine " + sm.modelname() + ", " + sm.modelguid()
                               + ", " + sm.instanceid() + " is not initialised");
    }
    if(ev._typecode() == z::_cpp_typecode())
    {
        process_event(sm, model, source, *dynamic_cast<const z*>(&ev));
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
    model.t_entry(); // initialise by walking default states
    sm.set_state(T);
    model.a_entry(); // initialise by walking default states
    sm.set_state(A);
    sm.set_initialised();
}

}; // class  xstatemachine_t

} // namespace SM
} // namespace MurphyPA
