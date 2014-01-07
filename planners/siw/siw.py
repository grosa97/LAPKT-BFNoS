#! /usr/bin/env python
import fd.grounding
import sys
from libsiw import SIW_Planner 

def main( domain_file, problem_file) :
	task = SIW_Planner( )

	fd.grounding.default( domain_file, problem_file, task )

	# MRJ: Uncomment to check what actions are being loaded
	#for i in range( 0, task.num_actions() ) :
	#	task.print_action( i )

	# MRJ: Setting planner parameters is as easy as setting the values
	# of Python object attributes
	
	# MRJ: Maximum bound on width is set to 2
	task.iw_bound = 2
	# MRJ: log filename set
	task.log_filename = 'iw.log'

	# MRJ: We call the setup method in SIW_Planner
	task.setup()

	# MRJ: And then we're ready to go
	task.solve()

def debug() :
	main( "/home/bowman/Sandboxes/Fast-Downward/benchmarks/miconic-simpleadl/domain.pddl",
		"/home/bowman/Sandboxes/Fast-Downward/benchmarks/miconic-simpleadl/s3-0.pddl" )

if __name__ == "__main__":
	main( sys.argv[1], sys.argv[2])
