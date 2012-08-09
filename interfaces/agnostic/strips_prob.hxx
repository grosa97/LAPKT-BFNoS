#ifndef __STRIPS_PROBLEM__
#define __STRIPS_PROBLEM__

#include <string>
#include <types.hxx>
#include <map>

namespace aptk
{

	class STRIPS_Problem
	{
	public:
	
		STRIPS_Problem();
		~STRIPS_Problem();

		unsigned 		num_fluents() 			{ return m_num_fluents; }
		unsigned 		num_actions() 			{ return m_num_actions; }

		void			set_num_fluents( unsigned nf ) { m_num_fluents = nf; }
		void			set_num_actions( unsigned na ) { m_num_actions = na; }

		static unsigned 	add_action( STRIPS_Problem& p, std::string signature,
						    Fluent_Vec& pre, Fluent_Vec& add, Fluent_Vec& del,
						    Conditional_Effect_Vec& ceffs );

		static unsigned 	add_fluent( STRIPS_Problem& p, std::string signature );

		static void		set_init( STRIPS_Problem& p, Fluent_Vec& init );
		static void		set_goal( STRIPS_Problem& p, Fluent_Vec& goal, bool createEndOp = true );

	  	
		Fluent_Ptr_Vec&		fluents() 			{ return m_fluents; }
		Action_Ptr_Vec&		actions() 			{ return m_actions; }

		Fluent_Vec&		init()	  			{ return m_init; }
		Fluent_Vec&		goal()	  			{ return m_goal; }

		Action_Ptr_Vec&		actions_adding( unsigned f ) 	{ return m_adding[f]; }
		Action_Ptr_Vec&		actions_deleting( unsigned f ) 	{ return m_deleting[f]; }
		Action_Ptr_Vec&		actions_requiring( unsigned f ) { return m_requiring[f]; }

		bool			is_in_init( unsigned f )	{ return m_in_init[f]; }
		bool			is_in_goal( unsigned f )	{ return m_in_goal[f]; }

		void                    print_fluent_vec(const Fluent_Vec &a);
		unsigned                end_operator() { return m_end_operator_id; }
	        unsigned                get_fluent_index(std::string signature);

		void			make_action_tables();	
	protected:
	
		void			increase_num_fluents()        	{ m_num_fluents++; }
		void			increase_num_actions()        	{ m_num_actions++; }
		void			register_action_in_tables( Action* act );

	protected:

		unsigned		 	m_num_fluents;
		unsigned		 	m_num_actions;
		Action_Ptr_Vec		 	m_actions;
		Fluent_Ptr_Vec		 	m_fluents;
		Fluent_Vec		 	m_init;
		Fluent_Vec		 	m_goal;
		Fluent_Action_Table	 	m_adding;
		Fluent_Action_Table	 	m_requiring;
		Fluent_Action_Table	 	m_deleting;
		std::vector<bool>	 	m_in_init;
		std::vector<bool>	 	m_in_goal;
		unsigned                 	m_end_operator_id;
	  	std::map<std::string,int> 	m_fluents_map;
	  };

}

#endif // STRIPS_Problem.hxx