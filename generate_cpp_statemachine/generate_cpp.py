from __future__ import print_function
import logging

def make_state_tree(states):

    d = {}
    
    class Node(object):
        def __init__(self, parent, child):
            self.parent = parent
            self.state = child
            self.children = None
    
    for opl in states:
        if opl.parent not in d:
            d[opl.parent] = []
            
        node = Node(opl.parent, opl)
        d[opl.parent].append(node)
    
    drop_map = set()
    for parent_guid, children in d.items():
        for child in children:
            if child.state.guid in d:
                child.children = d[child.state.guid]
                drop_map.add(child.state.guid)
    
    all_guids = d.copy()
    
    for guid in drop_map:
        del d[guid]
    
    assert d.keys() == ["NOPARENT"]

    return all_guids


def print_state_attributes(state, indent, logger):
    values = [("isstartstate", state.isstartstate),
              ("childStartStateName: ", state.childStartStateName),
              ("entry", state.entry),
              ("exit", state.exit)]
    for name, value in values:
        if value:
            logger.debug("%s | %s = %s", indent,  name, value)
    logger.debug("")


def print_all_using_all_guids(all_guids, parent_guid, name, node, level, logger):
    logger.debug("%s %s %s", level, parent_guid, name)
    if node:
        print_state_attributes(node.state, level, logger)
    if parent_guid in all_guids:
        for child in all_guids[parent_guid]:
            print_all_using_all_guids(all_guids, 
                                      child.state.guid, 
                                      child.state.name,
                                      child, 
                                      level + "\t",
                                      logger)


def print_all_using_root(state_tree_root, logger):
    def print_node(node, indent):
        logger.debug("%s %s %s", indent, node.state().guid, node.name())
        print_state_attributes(node.state(), indent, logger)

        node.do(print_node, indent + "\t")

    print_node(state_tree_root, ">>")

def get_state_node_from_guid(parsed_model, state_guid):
    state_node = parsed_model.stateTreeRoot().findNodeForState(state_guid)
    return state_node

def get_state_name_from_guid(parsed_model, state_guid):
    return parsed_model.ByGuid(state_guid).name


def get_hops_to_parent(state, parent_state):
    assert type(state) == type(parent_state), (type(state),
                                               type(parent_state))
    hop_count = 0
    while state != parent_state:
        state = state.parentNode()
        hop_count += 1
    return hop_count


def print_transition(transition, parsed_model, offset, logger):
    logger.debug("%s%s", offset, "transition:")
    for name, value in [
            ("from", "%s_%s" % (transition.fromstate, 
                                get_state_name_from_guid(parsed_model, 
                                                         transition.fromstate))),
            ("to", "%s_%s" % (transition.tostate, 
                              get_state_name_from_guid(parsed_model, 
                                                       transition.tostate))),
            ("guid", transition.guid),
            ("guard", transition.guard),
            ("action", transition.action),
            ("timeoutexpression", transition.timeoutexpression),
            ("eventsource", transition.eventsource),
            ("isInternalTransition", transition.isInternalTransition),
            ("transitiontype", transition.transitiontype),
            ("evaluationorderpriority", transition.evaluationorderpriority),
            ("event", transition.event),
            ("eventtype", transition.eventtype),
            ("name", transition.name)]:
        logger.debug("%s    %s = %s", offset, name, value)
        pass

    generate_path_for_transition(transition, parsed_model, offset, logger)


def get_leaf_states_from(stateNode):
    collect = []
    def _get_leaf_states(stateNode, arg):
        if stateNode.has_children():
            _walk_children(stateNode, arg)
        else:
            collect.append(stateNode)

    def _walk_children(stateNode, arg):
        if stateNode.has_children():
            stateNode.do(_get_leaf_states, arg)

    _walk_children(stateNode, None)
    return collect

def get_all_source_child_states_from(stateNode):
    collect = []
    def _get_descendent_states(stateNode, arg):
        if stateNode.has_children():
            _walk_children(stateNode, arg)
        collect.append(stateNode)
            
    def _walk_children(stateNode, arg):
        if stateNode.has_children():
            stateNode.do(_get_descendent_states, arg)

    _walk_children(stateNode, None)
    return collect

def get_linear_source_child_states_from_child_to_parent(stateNode, parent):
    collect = []
    while stateNode != parent:
        collect.append(stateNode)
        stateNode = stateNode.parentNode()
        assert stateNode is not None
            
    assert stateNode == parent

    return collect

def get_start_state_descendent_node_list_from(stateNode):
    collect = []
    def _get_start_state_child_states(stateNode, arg):
        assert stateNode.state().isstartstate in ["True", "False"]
        if stateNode.state().isstartstate == "True":
           collect.append(stateNode)
           # only walk children of start states
           _walk_children(stateNode, arg)
           

    def _walk_children(stateNode, arg):
        if stateNode.has_children():
            stateNode.do(_get_start_state_child_states, arg)

    _walk_children(stateNode, None)
    return collect

def get_state_to_root_node_list(stateNode):
    nodes = []
    while stateNode is not None:
        nodes.append(stateNode)
        stateNode = stateNode.parentNode()

    return nodes

def find_common_stateNode(from_list, to_list, logger):
    assert len(from_list) > 0
    assert len(to_list) > 0

    while len(from_list) > 0 and len(to_list) > 0:
        if to_list[-1] != from_list[-1]:
            break

        logger.debug("DROP %s", from_list[-1].state().name)
        del from_list[-1]
        del to_list[-1]


    #print("A", [n.state().name for n in from_list])
    #print("B", [n.state().name for n in to_list])
    
    return from_list, to_list

def generate_path_for_transition(transition, parsed_model, offset, logger):
    #from_state = parsed_model.ByGuid(transition.fromstate)
    #to_state = parsed_model.ByGuid(transition.tostate)

    logger.debug("%s Generate Path: %s", offset, transition.event)

    from_state_node = parsed_model.stateTreeRoot().findNodeForState(transition.fromstate)
    to_state_node = parsed_model.stateTreeRoot().findNodeForState(transition.tostate)

    if from_state_node == to_state_node:
        logger.debug("%s SAME!!!!!!!!!!!!!!", offset)

    if transition.isInternalTransition:
        assert from_state_node == to_state_node, (transition.isInternalTransition,
                                                  transition,
                                                  from_state_node.state().name,
                                                  to_state_node.state().name)


    leaf_from_states = get_leaf_states_from(from_state_node)
    logger.debug("%s LEAF_FROM_STATES: %s %s", offset, len(leaf_from_states), 
                 [n.state().name for n in leaf_from_states])
    for state_node in leaf_from_states:
        #print("F:", end=" ")
        while state_node != from_state_node:
            #print(" ",state_node.state().name, end=" > ")
            state_node = state_node.parentNode()
            pass
        assert state_node == from_state_node
        #print(from_state_node.state().name)

    all_source_child_states = get_all_source_child_states_from(from_state_node)
    logger.debug("%s SOURCE_CHILD_STATES: %s %s", offset, len(all_source_child_states), 
                 [n.state().name for n in all_source_child_states])
    for state_node in all_source_child_states:
        #print("F:", end=" ")
        while state_node != from_state_node:
            #print(" ",state_node.state().name, end=" > ")
            state_node = state_node.parentNode()
            pass
        assert state_node == from_state_node
        #print(from_state_node.state().name)
    all_source_child_states.append(from_state_node)


    to_state_descendent_list = get_start_state_descendent_node_list_from(to_state_node)
    #print("T:", len(start_state_descendent_list), to_state_node.state().name, end=", ")
    #for stateNode in start_state_descendent_list:
    #    print(stateNode.name(), end=", ")
    #print()


    from_state_to_root_node_list = get_state_to_root_node_list(from_state_node)
    to_state_to_root_node_list = get_state_to_root_node_list(to_state_node)


    from_list, to_list = find_common_stateNode(from_state_to_root_node_list,
                                               to_state_to_root_node_list,
                                               logger)

    logger.debug("%s FROM_LIST: %s %s", offset, len(from_list), 
                 [n.state().name for n in from_list])
    logger.debug("%s TO_LIST: %s %s", offset, len(to_list), 
                 [n.state().name for n in to_list])


    # This is the correct actions:
    # for each leaf_from_state:
    #     react to the event
    #     by transitioning from the leaf state through the from_list
    #     and the to_list and finally the descent list.
    #
    # print()
    # if len(leaf_from_states) == 0:
    #     leaf_from_states = [from_state_node]
    # for stateNode in leaf_from_states:
    #     print()
    #     print(offset, "======================")
    #     print(offset, parsed_model.ByGuid(transition.fromstate).name, 
    #           " -> ",
    #           transition.event, '[%s]' % (transition.guard,),
    #           " / ", 
    #           transition.action,
    #           " -> ",
    #           parsed_model.ByGuid(transition.tostate).name)
    #     print(offset, "======================")

    #     while stateNode.state().guid != from_state_node.state().guid:
    #         print (offset, "A", stateNode.state().name)
    #         stateNode = stateNode.parentNode()

    #     for stateNode in from_list:
    #         print (offset, "B", stateNode.state().name)

    #     for stateNode in to_list:
    #         print (offset, "C", stateNode.state().name)

    #     for stateNode in start_state_descendent_list:
    #         print (offset, "D", stateNode.state().name)
    
    #     print(offset, "----------------------")


    logger.debug ("%s A: %s", offset, from_state_node.state().name)

    logger.debug("leaf_from_states! %s", [n.state().name for n in leaf_from_states])
    logger.debug("to_state_descendent_list! %s", [n.state().name for n in to_state_descendent_list])

    if from_list == []:
        from_list = [from_state_node]

    if to_list == []:
        to_list = [to_state_node]

    assert from_list[0] == from_state_node

    assert to_list[0] == to_state_node

    for stateNode in from_list[1:]:
        logger.debug ("%s B: %s", offset, stateNode.state().name)
        
    for stateNode in to_list[1:]:
        logger.debug ("%s C: %s", offset, stateNode.state().name)

    logger.debug ("%s D: %s", offset, to_state_node.state().name)

    current_state = from_list[0]
    del from_list[0]

    linear_child_state_lists = []
    for child_state in all_source_child_states:
        linear_child_list_for_current_state = get_linear_source_child_states_from_child_to_parent(child_state, current_state)
        linear_child_list_for_current_state.append(current_state)
        linear_child_state_lists.append(linear_child_list_for_current_state)
        
    logger.debug("move from_list[0] to each sublist in linear_child_state_lists")

    to_list = [n for n in reversed(to_list)]
    logger.debug("reverse to_list")

    return (leaf_from_states, all_source_child_states, 
            linear_child_state_lists, current_state,
            from_list, to_list, to_state_descendent_list)


def generate_fromstate_transitions(transitions):
    fromstate_transitions = {}
    
    for transition in transitions:
        statename = transition.fromstate
        if statename not in fromstate_transitions:
            fromstate_transitions[statename] = []
            
        fromstate_transitions[statename].append(transition)

        pass

    return fromstate_transitions

def generate_event_list_transitions_from_transitions(transitions):
    event_list_transitions = {}
    
    for transition in transitions:
        eventname = (transition.eventsource, transition.event)
        if eventname not in event_list_transitions:
            event_list_transitions[eventname] = []
            
        event_list_transitions[eventname].append(transition)

        pass

    return event_list_transitions

def get_options():
    import argparse
    description="""Generate basic cpp code for state machine passed as input."""
    epilog="""This is a very naive implementation"""
    parser = argparse.ArgumentParser(description=description,
                                     epilog=epilog)

    help = "The state machine description filename (generally ends in extension .sm1)"
    parser.add_argument("--input_filename", required=True, help=help)

    help = "The destination filename to generate the cpp code into."
    parser.add_argument("--cpp_output_filename", required=True, help=help)

    return parser.parse_args()

class Test1(object):

    def run_test(self):

        self.debug = False

        self.logger = logging.getLogger("Test1")

        #filename = "../../FixOrderStateMatrixHsms/More/PrincipalOrder.sm1"
        #filename = "../../FixOrderStateMatrixHsms/More/PrincipalTrade.sm1"        
        #filename = "test_machine1.sm1"        

        #cpp_filename = "test1x.cpp"

        
        options = get_options()
        input_filename = options.input_filename
        cpp_filename = options.cpp_output_filename

        (parsed_model,
         state_list,
         transition_list,
         state_tree_root,
         reader) = self.read_model_from_filenamed(input_filename)
        
        if self.debug:
            self.print_model(parsed_model,
                             state_list,
                             transition_list,
                             state_tree_root,
                             reader)

        class Printer(object):
            def __init__(self, hFile):
                self.hFile = hFile
                self._lines = []
            def __call__(self, *args):
                print(*args, file=self.hFile)
                self._lines.append(args)
                    

        with open(cpp_filename, "w") as hFile:
            scprint = Printer(hFile)

            self.print_cpp_code(input_filename, 
                                parsed_model,
                                state_list,
                                transition_list,
                                state_tree_root,
                                reader,
                                scprint)


    def read_model_from_filenamed(self, filename):

        import reader
        
        parsed_model = reader.ParsedModel(filename)

        state_list = parsed_model.stateList()

        transition_list = parsed_model.transitionList()

        state_tree_root = reader.buildStateTree(parsed_model)
        
        assert state_tree_root.name() == "TOPSTATE", (state_tree_root,)

        return (parsed_model,
                state_list,
                transition_list,
                state_tree_root,
                reader)

    def print_model(self, 
                    parsed_model,
                    state_list,
                    transition_list,
                    state_tree_root,
                    reader):
        
        print("#ifdef __PRINT_MODEL_STRUCTURES__")
        print()

        print("Root State:", state_tree_root.name())

        print_visitor = reader.GenericPrintVisitor(parsed_model)
        parsed_model.do(lambda item, arg: item.accept(print_visitor, arg), 
                        None)
        
        print("parsed_model: ", 
              type(parsed_model))
        print("parsed_model.stateList: ",
              type(parsed_model.stateList))
        
        print("parsed_model.stateList(): ",
              type(state_list))
        
        print("parsed_model.transitionList: ",
              type(parsed_model.transitionList))

        print("parsed_model.transitionList(): ",
              type(transition_list))

        self.print_model_i(parsed_model,
                           state_list,
                           transition_list,
                           state_tree_root)
        print("#endif // __PRINT_MODEL_STRUCTURES__")
        print()

    def print_model_i(self, 
                      parsed_model,
                      state_list,
                      transition_list,
                      state_tree_root):
        
        #######
        
        states = []
        transitions = []
        
        def collect1(m, pl):
            pl.append(m)
        
        state_list.do(collect1, states)
        
        transition_list.do(collect1, transitions)
        
        parsed_model.ByGuid(transitions[2].fromstate)
        
        print([opl.fromstate 
               for opl in transitions])
        
        print([(opl.fromstate, 
                opl.tostate) 
               for opl in transitions])
        
        
        print([(("P", opl.parent),
                ("T", opl.guid))
               for opl in states])
        
        #tt.ByGuid(pl[2].tostate)
        #tt.ByGuid(pl[2].fromstate)
        
        all_guids = make_state_tree(states)
        print("all_guids:", all_guids)
        print_all_using_all_guids(all_guids, "NOPARENT", "", None, ":", self.logger)
        print_all_using_root(state_tree_root, self.logger)
        
        for transition in transitions:
            print_transition(transition, parsed_model, "", self.logger)
        
        
        fromstate_transitions = generate_fromstate_transitions(transitions)
            
        
        print()
        
        for statename, transition_items in fromstate_transitions.items():
            print("state:", statename, len(transition_items))
            for transition in transition_items:
                print("    transition: %s [%s] %s -> %s" % (transition.evaluationorderpriority, 
                                                            transition.guard,
                                                            transition.event,
                                                            parsed_model.ByGuid(transition.tostate).name))
                #print_transition(transition, parsed_model, "    ")


        print()

        event_list_transitions = generate_event_list_transitions_from_transitions(transitions)

        for eventname, transition_items in event_list_transitions.items():
            print("event:", eventname, len(transition_items))
            for transition in transition_items:
                print("    transition: %s [%s] %s -> %s" % (transition.evaluationorderpriority, 
                                                            transition.guard,
                                                            transition.event,
                                                            parsed_model.ByGuid(transition.tostate).name))
                #print_transition(transition, parsed_model, "    ")

        print()
        print()


        for eventname, ev_transition_items in event_list_transitions.items():
            print("event:", eventname, len(ev_transition_items))
            fromstate_transitions = generate_fromstate_transitions(ev_transition_items)
            for statename, transition_items in fromstate_transitions.items():
                print("    state: ", statename, parsed_model.ByGuid(statename).name, len(transition_items))
                for transition in transition_items:
                    offset = "        "
                    print_transition(transition, parsed_model, offset, self.logger)

                    (leaf_from_states, 
                     all_source_child_states, 
                     linear_child_state_lists, 
                     current_state,
                     from_list, 
                     to_list, 
                     to_state_descendent_list) = generate_path_for_transition(
                         transition, parsed_model, "", self.logger)
    
                    if transition.isInternalTransition:
                        print("%sINTERNAL" % (offset,))
                        pass

                    print("%sGUARD: %s" % (
                        offset, transition.guard))
                    print("%sFROM_LIST: %s %s" % (
                        offset, len(from_list),
                        [n.state().name for n in from_list]))
                    print("%sTO_LIST: %s %s" % (
                        offset, len(to_list), 
                        [n.state().name for n in to_list]))
                    print()
                    pass
                                                                    

        
    def print_cpp_code(self, 
                       input_filename,
                       parsed_model,
                       state_list,
                       transition_list,
                       state_tree_root, 
                       reader,
                       print):

        print('#include "base_statemachine.h"')
        print()

        print("// Note: very naive state machine implementation - but hopefully readable and easy enough to debug?")
        print()
        print("// Source: ", input_filename)
        print()
        for name, value in sorted(vars(parsed_model.header()).items()):
            if name == "self":
                continue
            assert value is None or type(value) == type(""), type(value)
            print("//", name, "=", value)
        print()

        transitions = []
        
        def collect1(m, pl):
            pl.append(m)
        
        transition_list.do(collect1, transitions)

        all_states = []
        state_list.do(collect1, all_states)


        methods = []
        for lstate in all_states:
            if lstate.entry:
                methods.append(("state", lstate.entry, "entry"))
            if lstate.exit:
                methods.append(("state", lstate.exit, "exit"))
            
        print("//note - not using transition.eventtype yet")

        for ltransition in transitions:
            if ltransition.guard:
                methods.append(("transition-0", ltransition.guard, "guard", 
                                ltransition.eventsource, ltransition.event, ltransition.eventtype))
            #print (ltransition.eventsource, ltransition.event, ltransition.eventtype)

            if ltransition.action:
                methods.append(("transition-1", ltransition.action, "action",
                                ltransition.eventsource, ltransition.event, ltransition.eventtype))


        # This does a bit more code gen than necessary. 
        # more efficient would be to print the transitions first - then within a transition test on the states
        
        # What this does is print the states and then per state - does the transitions.
        
        event_list_transitions = generate_event_list_transitions_from_transitions(transitions)

        # -----------------------------------------

        sources = set()
        for eventname, ev_transition_items in sorted(event_list_transitions.items()):
            source, eventname = eventname
            sources.add(source)

        SOURCES = ", ".join(["typename " + source for source in sorted(sources)])

        DISPATCH = ""

        for source in sorted(sources):
            dispatch_string = """
        {{
        const {source_type}* {source_name} = dynamic_cast<const {source_type}*>(&source);
        if(nullptr != {source_name})
        {{
            dispatch(*this, _model, *{source_name}, ev);
            return;
        }}
        }}
"""
            dispatch_string = dispatch_string.format(source_type=source,
                                                     source_name="source_instance")
            DISPATCH += dispatch_string

            pass



        methods_list = []

        for method in sorted(methods):
            methodname = method[1]
            assert methodname.endswith("()")
            original_methodname = methodname.replace("()", "").strip()
            if method[0].startswith("transition"):
                assert method[2] in ["guard", "action"]
                args = "(const %s& source, const %s& ev)" 
                args = args % (method[3], method[4],)
                methodname = methodname.replace("()", args)
            
            if method[2] == "guard":
                methodname = "bool " + methodname
            else:
                methodname = "void " + methodname

            if method[0].startswith("transition"):
                if method[2] == "guard":
                    methodname = methodname + """
    {
        std::cout << "%s" << " " << ev << std::endl;
        return _%s;
    }""" % (methodname, original_methodname)
                else:
                    methodname = methodname + """
    {
        std::cout << "%s" << " " << ev << std::endl;
    }""" % (methodname,)
            else:
                methodname = methodname + """
    {
        std::cout << "%s" << std::endl;
    }""" % (methodname,)
                
            methodname = "    " + methodname


            methods_list.append("%s" % (methodname,))
        methods_string = "\n".join(methods_list)

        # ports class

        #ports = []
        #parsed_model.statetransitionportList().do(collect1, ports)

        PORTS = ""
        for portname in sorted(sources):
            PORTS += """
    PortBinding<Owner, %s, EventBase> %s_port;
""" % (portname, portname.lower(),)

        ports_class = """
template <typename Owner,
          {SOURCES},
          typename EventBase>
class {machine_name}_Ports
{{
public:
    /*
{methods_string}
    */
{PORTS}
}};
"""

        # state class

        machine_name = parsed_model.header().statemachine
        if machine_name is None:
            machine_name = "xstatemachine_t"

        baseclassname = parsed_model.header().basestatemachine
        if baseclassname is None:
            baseclassname = "statemachine_t<EventSourceBase, EventBase>"

        namespace = parsed_model.header().namespace
        if namespace is None:
            namespace = "murphypa::sm1"

        namespace = namespace.replace(".", "::")
        for ns in namespace.split("::"):
            print ("namespace", ns)
            print ("{")

        print()
        print("using namespace nsii::statemachine;")

        print(ports_class.format(**locals()))


        class_template = """
template <typename UnderlyingModel, typename EventSourceBase, typename EventBase,
          typename MY_SOURCE_REPRESENTATION, {SOURCES}>
class {machine_name} :
        public {baseclassname},
        public MY_SOURCE_REPRESENTATION
{{
public:
    typedef {baseclassname} inherited;

public:
    {machine_name}(const std::string& instanceid)
        : inherited(instanceid)
    {{}}

    void initialise() override
    {{
        initialise_statemachine(*this, _model);
    }}

    void dispatch(EventSourceBase& source, const EventBase& ev) override
    {{
        // slow hack -- need a dispatch method per source type
        {DISPATCH}

        //unhandled_event(source, ev);
        throw std::logic_error("Unsupported source");
    }}

    // for use as an event source
    void send(EventSourceBase& source, const EventBase& ev) override
    {{
        dispatch(source, ev);
    }}

    UnderlyingModel& model() {{ return _model; }}
    const UnderlyingModel& model() const {{ return _model; }}

    std::string modelname() const override {{ return "{machine_name}"; }}
    std::string modelnamespace() const override {{ return "{namespace}"; }}
    std::string modelguid() const override {{ return "{header.modelguid}"; }}
    std::string modelfilename() const override {{ return "{header.modelfilename}"; }}
    std::string statemachineversion() const override {{ return "{header.statemachineversion}"; }}
    std::string comment() const override {{ return "{header.comment}"; }}

protected:
    void _model_unhandled_event(
            const EventSourceBase& source,
            const EventBase& ev)
    {{
        _model.unhandled_event(source, ev);
    }}

private:
        UnderlyingModel _model;
"""

        header = parsed_model.header()
        class_string = class_template.format(**locals())

        print(class_string)
        """
        comment = None;
        assembly = None;
        modelguid = 7e46f114-606e-4584-8492-dcf41ee57660;
        statemachine = None;
        usingnamespaces = None;
        fields = None;
        namespace = MurphyPA.SM;
        modelfilename = test_machine1.sm1;
        implementationversion = 0.2;
        readonly = False;
        basestatemachine = None;
        hassubmachines = False;
        statemachineversion = 14;
        """

        #print(dir(parsed_model.header()))

        #print("class 

        def make_call(name):
            assert name.endswith("()")
            return name.replace("()", "(source, ev)")

        for eventname, ev_transition_items in sorted(event_list_transitions.items()):
            print()
            print("template <typename StateMachine, typename Underlying>")
            print("void process_event(StateMachine& sm, Underlying& model, const {eventsource}& source, const {eventname}& ev)"
                  .format(eventname=eventname[1], 
                          eventsource=eventname[0]))
            print("{")
            fromstate_transitions = generate_fromstate_transitions(ev_transition_items)
            offset = "    "
            print("{offset}switch(sm.state())".format(**locals()))
            print("{offset}{{".format(**locals()))


            state_transition_info = {}
            for xstateguid, transition_items in fromstate_transitions.items():    
                #print(xstateguid, get_state_name_from_guid(parsed_model, xstateguid))
                def transition_guard_sorter(transition):
                    return (-int(transition.evaluationorderpriority),
                            transition.guard if transition.guard is not None else "")
                transition_items = list(sorted(transition_items, 
                                               key=transition_guard_sorter, 
                                               reverse=True))
                for transition in transition_items:
                    
                    #print(dir(transition))

                    (xleaf_from_states, 
                     xall_source_child_states, 
                     linear_child_state_lists, 
                     current_state,
                     from_list, 
                     to_list, 
                     to_state_descendent_list) = generate_path_for_transition(
                         transition, parsed_model, "", self.logger)

                    #print("_____", get_state_name_from_guid(parsed_model, 
                    #                                        xstateguid), 
                    #      [n.state().name for n in source_states])

                    for s_index, linear_state_list in enumerate(linear_child_state_lists):
                        state = linear_state_list[0]
                        if state not in state_transition_info:
                            state_transition_info[state] = []

                        # find competing transitions
                        do_not_add_outer_transition = False
                        drop_list = []
                        for xindex, xinfo in enumerate(state_transition_info[state]):
                            xtransition = xinfo[5]
                            if xtransition.event == transition.event:
                                if xtransition.guard == transition.guard:
                                    if xtransition.eventsource == transition.eventsource:
                                        # found two competing transitions
                                        # who is closer to the state
                                        hops_to_parent_xtransition = get_hops_to_parent(
                                            state, # child
                                            get_state_node_from_guid(parsed_model, xtransition.fromstate)) # parent
                                        hops_to_parent_transition = get_hops_to_parent(
                                            state, # child
                                            get_state_node_from_guid(parsed_model, transition.fromstate)) # parent
                                        if hops_to_parent_transition < hops_to_parent_xtransition:
                                            #print("// Drop transition:", xindex, xtransition.guid)
                                            print("// Drop transition:", xtransition.guid, "in favour of", transition.guid)
                                            drop_list.append(xindex)
                                        else:
                                            # outer transition is further from parent state
                                            print("// Drop transition:", transition.guid, "in favour of", xtransition.guid)
                                            do_not_add_outer_transition = True

                        for drop_index in reversed(drop_list):
                            del state_transition_info[state][drop_index]

                        if do_not_add_outer_transition:
                            continue

                        state_transition_info[state].append((state, 
                                                             linear_state_list,
                                                             from_list, 
                                                             to_list, 
                                                             to_state_descendent_list, 
                                                             transition,
                                                             len(transition_items),
                                                             transition == transition_items[0]))

            def state_name_key(value):
                return value[0].state().name
            for xstate, info_list in sorted(state_transition_info.items(), key=state_name_key):
                offset = "    "
                print("    case %s:" % (xstate.name(),), "//", xstate.state().guid, len(transition_items))
                print("    {")

                for (state, 
                     source_states,
                     from_list, 
                     to_list, 
                     to_state_descendent_list, 
                     transition, 
                     transitions_count,
                     is_first_transition_for_event) in info_list:

                    assert xstate == state

                    if True:
                        offset = "        "
    
                        #print_transition(transition, parsed_model, "        ", self.logger)
        
                        if transition.isInternalTransition:
                            # Rules for internal transitions are different for guards
                            # Not competition - if a guard matches - its transition runs?

                            # is this assertion valid?
                            assert len(transition_items) == 1, ("Only one internal transition is allowed",
                                                                transition_items)

                            if transition.guard:
                                print("%sif(model.%s)" % (
                                    offset, make_call(transition.guard)))
                            print("%s{" % (offset,))
                            print("%s%smodel.%s; // internal" % (offset, offset, make_call(transition.action)))
                            print("%s}" % (offset,))

                            continue # could be more internal transitions
        
                        if transition.guard:
                            if not is_first_transition_for_event:
                                print("%selse" % (
                                    offset))
                                
                            print("%sif(model.%s)" % (
                                offset, make_call(transition.guard)))
                        elif transitions_count > 1:
                            print("%selse // %s" % (
                                offset, transition.guard))

                        print("%s{" % (offset,))

    
                        offset = "    " * 3
    
                        print("%s// FROM_LIST: %s %s" % (
                            offset, len(from_list),
                            [n.state().name for n in from_list]))

                        for state in source_states:
                            if state.state().exit:
                                print("%smodel.%s;" % (offset, state.state().exit,), "// exit-1")

                        for fstate in from_list:
                            if fstate.state().exit:
                                print("%smodel.%s;" % (offset, fstate.state().exit,), "// exit-2")

    
                        if transition.action:
                            print("%smodel.%s; // transition" % (offset, make_call(transition.action)))
    
                        print("%s// TO_LIST: %s %s" % (
                            offset, len(to_list), 
                            [n.state().name for n in to_list]))

                        final_state = None
                        for fstate in to_list:
                            final_state = fstate
                            if fstate.state().entry:
                                print("%smodel.%s;" % (offset, fstate.state().entry,), "// entry-1")

                        for fstate in to_state_descendent_list:
                            final_state = fstate
                            if fstate.state().entry:
                                print("%smodel.%s;" % (offset, fstate.state().entry,), "// entry-2")
    
                        print("%ssm.set_state(%s);" % (offset, 
                                                       final_state.state().name,))


                        offset = "    " * 2
                        print("%s} // %s" % (
                            offset, transition.guard))
    
                        print()
                        pass
                    pass

                print("    }; break;")

                offset = "    "
            print("{offset}default:".format(**locals()))
            print("{offset}{{".format(**locals()))
            print("{offset}{offset}sm.unhandled_event(source, ev);".format(**locals()))
            print("{offset}}}; break;".format(**locals()))
            print("{offset}}}".format(**locals()))
            print("}")


        for source in sorted(sources):
            print()
            print("template <typename StateMachine, typename Underlying>")
            print("void dispatch(StateMachine& sm, Underlying& model, const %s& source, const EventBase& ev)" % (source,))
            print("{")
            print("%sif(!sm.is_initialised())" % (offset,))
            print("%s{" % (offset,))
            print("%s%sthrow std::logic_error(\"Statemachine \" + sm.modelname() + \", \" + sm.modelguid()" % (offset, offset))
            print("%s%s                       + \", \" + sm.instanceid() + \" is not initialised\");" % (offset, offset))
            print("%s}" % (offset,))
            ifstatement = "if"
            offset = "    "
            for eventname, ev_transition_items in sorted(event_list_transitions.items()):
                if eventname[0] != source:
                    continue
                print("%s%s(ev._typecode() == %s::_cpp_typecode())" % (offset, 
                                                                         ifstatement, 
                                                                         eventname[1],))
                print("%s{" % (offset,))
                print("%s%sprocess_event(sm, model, source, *dynamic_cast<const %s*>(&ev));" % (offset, 
                                                                                         offset,
                                                                                         eventname[1],))
                print("%s%sreturn;" % (offset, offset,))
                print("%s}" % (offset,))
                ifstatement = "else if"
                
                pass
            print("%selse" % (offset,))
            print("%s{" % (offset,))
            print("%s%sthrow std::logic_error(\"Unsupported event\");" % (offset, 
                                                                          offset,))
            print("%s}" % (offset,))
    
            print("}")
    
            print()

            pass
    
        print("template <typename StateMachine, typename Underlying>")
        print("void initialise_statemachine(StateMachine& sm, Underlying& model)")
        print("{")
        print("%sif(sm.is_initialised())" % (offset,))
        print("%s{" % (offset,))
        print("%s%sthrow std::logic_error(\"Statemachine \" + sm.modelname() + \", \" + sm.modelguid()" % (offset, offset))
        print("%s%s                       + \", \" + sm.instanceid() + \" is already initialised\");" % (offset, offset))
        print("%s}" % (offset,))
        ifstatement = "if"
        offset = "    "
        print("%ssm.set_state(%s);" % (offset, "TOPSTATE",))
        for fstate in get_start_state_descendent_node_list_from(state_tree_root):
            if fstate.state().entry:
                print("%smodel.%s;" % (offset, fstate.state().entry,), "// initialise by walking default states")
            print("%ssm.set_state(%s);" % (offset, fstate.state().name,))
        print("%ssm.set_initialised();" % (offset,))
        print("}")

        print()

        print("}; // class ", machine_name)
        print()

        for ns in reversed(namespace.split("::")):
            print ("} // namespace", ns)

                
if __name__ == "__main__":
    logging.basicConfig()

    test1 = Test1()
    test1.run_test()

