
/*
Lightweight Automated Planning Toolkit

Copyright 2022
Miquel Ramirez <miquel.ramirez@unimelb.edu.au>Nir Lipovetzky <nirlipo@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files 
(the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, 
publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, subject
 to the following conditions:

The above copyright notice and this permission notice shall be included 
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

// MRJ: In this example, we'll show how to create a search problem out of
// a planning problem we acquired from some external source
#include <iostream>
#include <fstream>
#include <vector>
#include <common/nwn.hxx>

#include <strips_prob.hxx>
#include <watched_lit_succ_gen.hxx>
#include <fluent.hxx>
#include <action.hxx>
#include <cond_eff.hxx>
#include <strips_state.hxx>
#include <reachability.hxx>

#include <fwd_search_prob.hxx>
#include <aptk/string_conversions.hxx>
#include <aptk/time.hxx>
using	aptk::STRIPS_Problem;
using	aptk::State;
using	aptk::Action;
using	aptk::agnostic::Fwd_Search_Problem;
using   aptk::agnostic::Reachability_Test;

using aptk::WatchedLitSuccGen;

int main( int argc, char** argv ) {

	int 	dim = 10;
	float 	block_prob = 0.1f;
	int	n_items = 12;
	int	n_goal_items = 5;
	int 	n_goal_locs = 4;
	int 	i = 1;
	
	int TRIALS = 2000;

	while ( i < argc ) {
		std::string parm = argv[i];
		if ( parm == "--dim" ) {
			i++;
			std::string value = argv[i];
			aptk::from_string( dim, value, std::dec );
			std::cout << "Grid dimensions set to " << dim << std::endl;
			i++;
			continue;	
		}
		if ( parm == "--block-prob" ) {
			i++;
			std::string value = argv[i];
			aptk::from_string( block_prob, value, std::dec );
			std::cout << "Block probability set to " << block_prob << std::endl;
			i++;
			continue;
		}
		if ( parm == "--num-items" ) {
			i++;
			std::string value = argv[i];
			aptk::from_string( n_items, value, std::dec );
			std::cout << "Number of items set to " << n_items << std::endl;
			i++;
			continue;
		}
		if ( parm == "--num-goal-items" ) {
			i++;
			std::string value = argv[i];
			aptk::from_string( n_goal_items, value, std::dec );
			std::cout << "Number of items relevant to goal set to " << n_goal_items << std::endl;
			i++;
			continue;
		}
		if ( parm == "--num-goal-locations" ) {
			i++;
			std::string value = argv[i];
			aptk::from_string( n_goal_locs, value, std::dec );
			std::cout << "Number of locations relevant to goal set to " << n_goal_locs << std::endl;
			i++;
			continue;
		}
		if ( parm == "--trials" ) {
		    i++;
		    std::string value = argv[i];
		    aptk::from_string( TRIALS, value, std::dec );
		    std::cout << "Number of trials set to " << TRIALS << std::endl;
		    i++;
		    continue;
		}
		if( parm == "--help"){
		    std::cout << "::Options::" << std::endl;
		    std::cout << "\t--dim <int> \t\t\t default 10" << std::endl;
		    std::cout << "\t--block-prob <float> \t\t default 0.1f" << std::endl;
		    std::cout << "\t--num-items <int> \t\t default 12" << std::endl;
		    std::cout << "\t--num-goal-items <int> \t\t default 5" << std::endl;
		    std::cout << "\t--num-goal-locations <int> \t default 4" << std::endl;
		    std::cout << "\t--trials <int> \t\t\t default 2000" << std::endl;
		    return 0;
		 
		}
		    
	}

	NWN_Mockup fake_nwn_situation;
	STRIPS_Problem plan_prob;
	

	fake_nwn_situation.setup_nav_graph( dim, dim, block_prob );
	fake_nwn_situation.add_items( n_items );
	fake_nwn_situation.build_strips_problem( n_goal_items, n_goal_locs, plan_prob );
	
	Fwd_Search_Problem	search_prob( &plan_prob );

	Reachability_Test reach( plan_prob );

	int total_action_pres = 0;
	for ( int i = 0; i < search_prob.num_actions(); i++ ) {
		total_action_pres += plan_prob.actions()[i]->prec_vec().size();
	}

	std::cout << "Dumping STRIPS problem on file 'problem.strips'" << std::endl;
	std::ofstream outstream( "problem.strips" );
	plan_prob.print( outstream );
	outstream.close();
	std::cout << "Problem statistics:" << std::endl;
	std::cout << "\t# Fluents: " << plan_prob.num_fluents() << std::endl;
	std::cout << "\t# Actions: " << plan_prob.num_actions() << std::endl;
	std::cout << "\t# Avg Prec Size: " << (float(total_action_pres) / float(search_prob.num_actions())) << std::endl;
	std::cout << "Initial state: " << std::endl;
	plan_prob.print_fluent_vec( std::cout, plan_prob.init() );
	std::cout << std::endl;
	std::cout << "Goal state: " << std::endl;
	plan_prob.print_fluent_vec( std::cout, plan_prob.goal() );
	std::cout << std::endl;

	float old_time;

	// MRJ: Example of different successor generator modalities
	std::cout << std::endl << "Applicable actions at root by direct checking: " << std::endl;
	State* s0 = search_prob.init();
	old_time = aptk::time_used();
	int n = 0;
	
	
	/** NIR: currently by default we use match tree, 
	 * therefore we have to explicitly initialize the old successor generator
	 */
	plan_prob.initialize_successor_generator();

	//NIR: Initialize both successor generators
	plan_prob.make_action_tables();

	WatchedLitSuccGen w_succ_gen(plan_prob);

	std::vector<int> app_set;


	std::vector<State*> states;
	states.push_back(s0);
	for (int i = 0; states.size() < TRIALS; i++) {
		State* s = states[i%states.size()];
		app_set.clear();
		w_succ_gen.applicable_actions(*s, app_set);
		for(auto op :app_set){
			auto act = plan_prob.actions()[op];
			states.push_back(s0->progress_through( *act ));
			n++;
		}
	}
	states.resize(TRIALS);
	std::cout << "Generated " << states.size() << " states by breadth-first search" << std::endl;

	old_time = aptk::time_used();
	std::cout << "root  Delete-free Reachablility  with watched literals: " << std::endl;
	n = 0;
	for (int trial = 0; trial < TRIALS; trial++) {
		w_succ_gen.is_reachable( *(states[trial]) );
	}
	std::cout << "Time: " << (aptk::time_used() - old_time) << std::endl << std::endl;

	old_time = aptk::time_used();
	std::cout << "root Delete-free Reachablility: " << std::endl;
	n = 0;

	for (int trial = 0; trial < TRIALS; trial++) {
		reach.is_reachable( states[trial]->fluent_vec(), plan_prob.goal() );
	}
	std::cout << "Time: " << (aptk::time_used() - old_time) << std::endl << std::endl;

	
	for(auto s : states)
		delete s;
	states.clear();
	
	return 0;
}
