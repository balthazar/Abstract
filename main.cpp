/* ========================================================================== */
/*                                                                            */
/*    ██╗  ██╗██████╗   █████╗ ██╗   ██╗                                      */
/*    ██║  ██║╚════██╗ ██╔══██╗██║   ██║                                      */
/*    ███████║ █████╔╝ ███████║██║   ██║       by: bgronon                    */
/*    ╚════██║██╔═══╝  ██╔══██║╚██╗ ██╔╝       at: 2015/02/12 10:59:07        */
/*         ██║███████╗ ██║  ██║ ╚████╔╝                                       */
/*         ╚═╝╚══════╝ ╚═╝  ╚═╝  ╚═══╝                                        */
/*                                                                            */
/* ========================================================================== */

#include <iostream>
#include "Vm.hpp"
#include "Parser.hpp"
#include "Operand.hpp"
#include "abstract.hpp"

int main (int ac, char ** av)
{
	if (ac > 2) {
		std::cout << "Please pass one or no arguments, is that complicated?" << std::endl;
		return (-1);
	}

	try {
		Parser p(av[1]);
	} catch (ExecutionException & e) {
		std::cout << e.what() << std::endl;
	} catch (SyntaxException & e) {
		std::cout << e.what() << std::endl;
	} catch (std::exception & e) {
		std::cout << e.what() << std::endl;
	}

	return (0);
}
