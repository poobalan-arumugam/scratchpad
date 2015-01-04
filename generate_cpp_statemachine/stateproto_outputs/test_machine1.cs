//---------------------------------------------------------------------
//Begin[[StartOfFileBlock]]
//End[[StartOfFileBlock]]
//---------------------------------------------------------------------

using System;
using qf4net;

//---------------------------------------------------------------------
//Begin[[UsingNameSpaceCodeBlock]]
//End[[UsingNameSpaceCodeBlock]]
//---------------------------------------------------------------------

namespace MurphyPA.SM
{
	// SM: 
	// State: D
	// State: D.E
	// State: D.E.F
	// State: D.G
	// State: D.H
	// State: T
	// State: T.A
	// State: T.A.B
	// State: T.A.B.C
	// Transition: Source1.x/action2()
	// Transition: r1-Source1.x[test1()]/action1()
	// Transition: r2-Source1.x[test2()]/action5()
	// Transition: Source1.y/action3()
	// Transition: Source1.y/action6()
	// Transition: Source2.z/action4()
	[ModelInformation (@"test_machine1.sm1", "7e46f114-606e-4584-8492-dcf41ee57660", "17")]
	[TransitionEvent ("x", "Source1")]
	[TransitionEvent ("y", "Source1")]
	[TransitionEvent ("z", "Source2")]
	public class  : LQHsm, ISig
	//---------------------------------------------------------------------
	//Begin[[ImplementsInterfaces]]
	//End[[ImplementsInterfaces]]
	//---------------------------------------------------------------------
	{
		
		//---------------------------------------------------------------------
		//Begin[[ClassBodyCode]]
		//End[[ClassBodyCode]]
		//---------------------------------------------------------------------
		#region Boiler plate static stuff
		protected static new TransitionChainStore s_TransitionChainStore = 
			new TransitionChainStore(System.Reflection.MethodBase.GetCurrentMethod().DeclaringType);
		static object _MyStaticInstance;
		static  ()
		{
			s_TransitionChainStore.ShrinkToActualSize();
		    _MyStaticInstance = new ();
		}
		protected override TransitionChainStore TransChainStore
		{
			get { return s_TransitionChainStore; }
		}
		#endregion
		
		protected override void InitializeStateMachine()
		{
			InitializeState(s_T);
			//Begin[[InitialiseSM]]
			//End[[InitialiseSM]]
		}
		
		#region State Fields
		static protected QState s_D;
		static protected QState s_D_E;
		static protected QState s_D_E_F;
		static protected QState s_D_G;
		static protected QState s_D_H;
		static protected QState s_T;
		static protected QState s_T_A;
		static protected QState s_T_A_B;
		static protected QState s_T_A_B_C;
		
		#endregion
		
		#region Ports
		protected IQPort _Source0;
		public IQPort Source0 { get { if (_Source0 == null) { _Source0 = CreatePort ("Source0"); } return _Source0; } }
		protected IQPort _Source1;
		public IQPort Source1 { get { if (_Source1 == null) { _Source1 = CreatePort ("Source1"); } return _Source1; } }
		#endregion
		
		#region Constructors
		public  (){
			CreateStateFields ();
		}
		
		public  (bool createEventManager)
		  : base (createEventManager) {
			CreateStateFields ();
		}
		
		public  (IQEventManager eventManager)
		  : base (eventManager) {
			CreateStateFields ();
		}
		
		public  (string id, string groupId)
		  : base (id, groupId) {
			CreateStateFields ();
		}
		
		public  (string id, IQEventManager eventManager)
		  : base (id, eventManager) {
			CreateStateFields ();
		}
		
		public  (string id, string groupId, IQEventManager eventManager)
		  : base (id, groupId, eventManager) {
			CreateStateFields ();
		}
		
		public  (string id, IQHsmLifeCycleManager lifeCycleManager)
		  : base (id, lifeCycleManager) {
			CreateStateFields ();
		}
		
		public  (string id, string groupId, IQHsmLifeCycleManager lifeCycleManager)
		  : base (id, groupId, lifeCycleManager) {
			CreateStateFields ();
		}
		
		public  (string id, IQHsmExecutionContext executionContext)
		  : base (id, executionContext) {
			CreateStateFields ();
		}
		
		public  (string id, string groupId, IQHsmExecutionContext executionContext)
		  : base (id, groupId, executionContext) {
			CreateStateFields ();
		}
		
		#endregion // Constructors
		#region Create State Fields
		protected virtual void CreateStateFields (){
			if(null == _MyStaticInstance){
				s_D = new QState (S_D);
				s_D_E = new QState (S_D_E);
				s_D_E_F = new QState (S_D_E_F);
				s_D_G = new QState (S_D_G);
				s_D_H = new QState (S_D_H);
				s_T = new QState (S_T);
				s_T_A = new QState (S_T_A);
				s_T_A_B = new QState (S_T_A_B);
				s_T_A_B_C = new QState (S_T_A_B_C);
			}
		}
		#endregion
		
		#region IsFinalState
		public override bool IsFinalState (QState state){
			return false
			;
		}
		#endregion // IsFinalState
		
		
		#region State D
		protected static int s_trans_Source1_y_D_2_T_A_B = s_TransitionChainStore.GetOpenSlot ();
		[StateMethod ("D")]
		protected virtual QState S_D (IQEvent ev){
			switch (ev.QSignal){
			case QSignals.Init: {
				LogStateEvent (StateLogType.Init, s_D, s_D_G);
				InitializeState (s_D_G);
			} return null;
			case QSignals.Entry: {
				LogStateEvent (StateLogType.Entry, s_D, "d_entry()");
				d_entry();
			} return null;
			case QSignals.Exit: {
				d_exit();
				LogStateEvent (StateLogType.Exit, s_D, "d_exit()");
			} return null;
			case QualifiedSignals.Source1_y: {
				action3();
				LogStateEvent (StateLogType.EventTransition, s_D, s_T_A_B, "Source1.y", "Source1.y/action3()");
				TransitionTo (s_T_A_B, s_trans_Source1_y_D_2_T_A_B);
				return null;
			}  // Source1.y
			} // switch
			
			return TopState;
		} // S_D
		#endregion
	
		
		#region State D_E
		[StateMethod ("D_E")]
		protected virtual QState S_D_E (IQEvent ev){
			switch (ev.QSignal){
			case QSignals.Init: {
				LogStateEvent (StateLogType.Init, s_D_E, s_D_E_F);
				InitializeState (s_D_E_F);
			} return null;
			case QSignals.Entry: {
				LogStateEvent (StateLogType.Entry, s_D_E, "e_entry()");
				e_entry();
			} return null;
			case QSignals.Exit: {
				e_exit();
				LogStateEvent (StateLogType.Exit, s_D_E, "e_exit()");
			} return null;
			} // switch
			
			return s_D;
		} // S_D_E
		#endregion
	
		
		#region State D_E_F
		[StateMethod ("D_E_F")]
		protected virtual QState S_D_E_F (IQEvent ev){
			switch (ev.QSignal){
			case QSignals.Entry: {
				LogStateEvent (StateLogType.Entry, s_D_E_F, "f_entry()");
				f_entry();
			} return null;
			case QSignals.Exit: {
				f_exit();
				LogStateEvent (StateLogType.Exit, s_D_E_F, "f_exit()");
			} return null;
			} // switch
			
			return s_D_E;
		} // S_D_E_F
		#endregion
	
		
		#region State D_G
		protected static int s_trans_Source1_y_D_G_2_T_A = s_TransitionChainStore.GetOpenSlot ();
		[StateMethod ("D_G")]
		protected virtual QState S_D_G (IQEvent ev){
			switch (ev.QSignal){
			case QSignals.Entry: {
				LogStateEvent (StateLogType.Entry, s_D_G, "g_entry()");
				g_entry();
			} return null;
			case QSignals.Exit: {
				g_exit();
				LogStateEvent (StateLogType.Exit, s_D_G, "g_exit()");
			} return null;
			case QualifiedSignals.Source1_y: {
				action6();
				LogStateEvent (StateLogType.EventTransition, s_D_G, s_T_A, "Source1.y", "Source1.y/action6()");
				TransitionTo (s_T_A, s_trans_Source1_y_D_G_2_T_A);
				return null;
			}  // Source1.y
			} // switch
			
			return s_D;
		} // S_D_G
		#endregion
	
		
		#region State D_H
		[StateMethod ("D_H")]
		protected virtual QState S_D_H (IQEvent ev){
			switch (ev.QSignal){
			case QSignals.Entry: {
				LogStateEvent (StateLogType.Entry, s_D_H, "h_entry()");
				h_entry();
			} return null;
			case QSignals.Exit: {
				h_exit();
				LogStateEvent (StateLogType.Exit, s_D_H, "h_exit()");
			} return null;
			} // switch
			
			return s_D;
		} // S_D_H
		#endregion
	
		
		#region State T
		[StateMethod ("T")]
		protected virtual QState S_T (IQEvent ev){
			switch (ev.QSignal){
			case QSignals.Init: {
				LogStateEvent (StateLogType.Init, s_T, s_T_A);
				InitializeState (s_T_A);
			} return null;
			case QSignals.Entry: {
				LogStateEvent (StateLogType.Entry, s_T, "t_entry()");
				t_entry();
			} return null;
			case QSignals.Exit: {
				t_exit();
				LogStateEvent (StateLogType.Exit, s_T, "t_exit()");
			} return null;
			} // switch
			
			return TopState;
		} // S_T
		#endregion
	
		
		#region State T_A
		protected static int s_trans_r1_Source1_x_T_A_2_D_E = s_TransitionChainStore.GetOpenSlot ();
		protected static int s_trans_r2_Source1_x_T_A_2_D_H = s_TransitionChainStore.GetOpenSlot ();
		protected static int s_trans_Source1_x_T_A_2_D = s_TransitionChainStore.GetOpenSlot ();
		[StateMethod ("T_A")]
		protected virtual QState S_T_A (IQEvent ev){
			switch (ev.QSignal){
			case QSignals.Entry: {
				LogStateEvent (StateLogType.Entry, s_T_A, "a_entry()");
				a_entry();
			} return null;
			case QSignals.Exit: {
				a_exit();
				LogStateEvent (StateLogType.Exit, s_T_A, "a_exit()");
			} return null;
			case QualifiedSignals.Source1_x: {
				 if (test1()) {
					action1();
					LogStateEvent (StateLogType.EventTransition, s_T_A, s_D_E, "Source1.x", "r1-Source1.x[test1()]/action1()");
					TransitionTo (s_D_E, s_trans_r1_Source1_x_T_A_2_D_E);
					return null;
				}
				else if (test2()) {
					action5();
					LogStateEvent (StateLogType.EventTransition, s_T_A, s_D_H, "Source1.x", "r2-Source1.x[test2()]/action5()");
					TransitionTo (s_D_H, s_trans_r2_Source1_x_T_A_2_D_H);
					return null;
				}
				else {
					action2();
					LogStateEvent (StateLogType.EventTransition, s_T_A, s_D, "Source1.x", "Source1.x/action2()");
					TransitionTo (s_D, s_trans_Source1_x_T_A_2_D);
					return null;
				}
			}  // Source1.x
			} // switch
			
			return s_T;
		} // S_T_A
		#endregion
	
		
		#region State T_A_B
		[StateMethod ("T_A_B")]
		protected virtual QState S_T_A_B (IQEvent ev){
			switch (ev.QSignal){
			case QSignals.Entry: {
				LogStateEvent (StateLogType.Entry, s_T_A_B, "b_entry()");
				b_entry();
			} return null;
			case QSignals.Exit: {
				b_exit();
				LogStateEvent (StateLogType.Exit, s_T_A_B, "b_exit()");
			} return null;
			case QualifiedSignals.Source2_z: {
				action4();
				LogStateEvent (StateLogType.EventInternalTransition, s_T_A_B, s_T_A_B, "Source2.z", "Source2.z/action4()");
				return null;
			}  // Source2.z
			} // switch
			
			return s_T_A;
		} // S_T_A_B
		#endregion
	
		
		#region State T_A_B_C
		[StateMethod ("T_A_B_C")]
		protected virtual QState S_T_A_B_C (IQEvent ev){
			switch (ev.QSignal){
			case QSignals.Entry: {
				LogStateEvent (StateLogType.Entry, s_T_A_B_C, "c_entry()");
				c_entry();
			} return null;
			case QSignals.Exit: {
				c_exit();
				LogStateEvent (StateLogType.Exit, s_T_A_B_C, "c_exit()");
			} return null;
			} // switch
			
			return s_T_A_B;
		} // S_T_A_B_C
		#endregion
	
		#region ISig Members
		public void Sigx (object data) { AsyncDispatch (new QEvent (Signals.x, data)); }
		public void Sigy (object data) { AsyncDispatch (new QEvent (Signals.y, data)); }
		public void Sigz (object data) { AsyncDispatch (new QEvent (Signals.z, data)); }
		#endregion // ISig Members
	} // 
	public interface ISig
	{
		void Sigx (object data);
		void Sigy (object data);
		void Sigz (object data);
	}
	public class QualifiedSignals
	{
		public const string Source1_x = "Source1.x";
		public const string Source1_y = "Source1.y";
		public const string Source2_z = "Source2.z";
	}
	public class Signals
	{
		public const string x = "x";
		public const string y = "y";
		public const string z = "z";
	}
}
