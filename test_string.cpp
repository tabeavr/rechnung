#include <string>
#include <iostream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char * argv[]){
	po::options_description desc("stinr");
	desc.add_options()
		("string", po::value<int>(), "zum testen")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argc, desc), vm);
	notify(vm);

	if(vm.count("string"){
			std::cout << vm["string"].as<int>() <<"\n" ;
			}
	else
	std::cout << "blah"<< std::endl;
	return 0;
	}

