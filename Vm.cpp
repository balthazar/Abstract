/* ========================================================================== */
/*                                                                            */
/*    ██╗  ██╗██████╗   █████╗ ██╗   ██╗                                      */
/*    ██║  ██║╚════██╗ ██╔══██╗██║   ██║                                      */
/*    ███████║ █████╔╝ ███████║██║   ██║       by: bgronon                    */
/*    ╚════██║██╔═══╝  ██╔══██║╚██╗ ██╔╝       at: 2015/02/12 12:15:03        */
/*         ██║███████╗ ██║  ██║ ╚████╔╝                                       */
	/*         ╚═╝╚══════╝ ╚═╝  ╚═╝  ╚═══╝                                        */
/*                                                                            */
/* ========================================================================== */

#include "Vm.hpp"
#include "Operand.hpp"
#include "Instruction.hpp"
#include <iterator>

Vm::Vm (void)
{
}

Vm::~Vm (void)
{
}

Vm::Vm (Vm const & ref)
{
	*this = ref;
}

Vm & Vm::operator= (Vm const & ref)
{
	VOID(ref);
	return *this;
}

Vm & Vm::single (void)
{
	static Vm i;
	return i;
}

void Vm::pushInstruction (VmBasics fn)
{
	this->_instructs.push_back(new Instruction(fn, NULL, NULL));
}

void Vm::pushInstruction (VmParams fn, IOperand const * op)
{
	this->_instructs.push_back(new Instruction(NULL, fn, op));
}

void Vm::execute (void)
{
	std::list<Instruction *>::iterator it = this->_instructs.begin();
	while (it != this->_instructs.end()) {
		Instruction * ins = *it;
		if (ins->param) {
			(this->*(ins->extra))(ins->param);
		} else {
			(this->*(ins->basic))();
		}
		++it;
	}
}

IOperand const *	Vm::createOperand (eOperandType type, std::string const & value) const
{
	static std::map<eOperandType, VmFn> map = {
		{ INT8, &Vm::createInt8 },
		{ INT16, &Vm::createInt16 },
		{ INT32, &Vm::createInt32 },
		{ DOUBLE, &Vm::createDouble },
		{ FLOAT, &Vm::createFloat }
	};

	return (this->*(map[type]))(value);
}

void Vm::push (eOperandType type, std::string const & value)
{
	this->_stack.push_front(this->createOperand(type, value));
}

void Vm::push (IOperand const * op)
{
	this->_stack.push_front(op);
}

void Vm::pop (void)
{
	if (this->_stack.size() < 1) {
		EXECEXCEPT("The stack is empty, cannot perform your operation");
	}
	delete *(this->_stack.begin());
	this->_stack.pop_front();
}

void Vm::dump (void)
{
	std::list<IOperand const *>::const_iterator it = this->_stack.begin();
	while (it != this->_stack.end()) {
		std::cout << (*it)->toString() << std::endl;
		++it;
	}
}

void Vm::assert (IOperand const * op)
{
	IOperand const * top = *(this->_stack.begin());
	if (top->getType() != op->getType() && top->toString() != op->toString()) {
		EXECEXCEPT("Assert failed");
	}
}

void Vm::print (void)
{
	IOperand const * top = *(this->_stack.begin());
	IOperand const * tmp = this->createOperand(INT8, top->toString());
	this->assert(tmp);
	std::stringstream ss;
	int res;
	ss << top->toString();
	ss >> res;
	std::cout << static_cast<char>(res) << std::endl;
}

void Vm::exit (void)
{
	std::cout << "Exiting..." << std::endl;
}

void Vm::add (void)
{
	if (this->_stack.size() < 2) { EXECEXCEPT("Not enough operands on the stack to make this operation"); }
	IOperand const * op = **(this->_stack.begin()) + **(std::next(this->_stack.begin()));
	this->pop();
	this->pop();
	this->push(op);
}

void Vm::sub (void)
{
	if (this->_stack.size() < 2) { EXECEXCEPT("Not enough operands on the stack to make this operation"); }
	IOperand const * op = **(this->_stack.begin()) - **(std::next(this->_stack.begin()));
	this->pop();
	this->pop();
	this->push(op);
}

void Vm::mul (void)
{
	if (this->_stack.size() < 2) { EXECEXCEPT("Not enough operands on the stack to make this operation"); }
	IOperand const * op = **(this->_stack.begin()) * **(std::next(this->_stack.begin()));
	this->pop();
	this->pop();
	this->push(op);
}

void Vm::div (void)
{
	if (this->_stack.size() < 2) { EXECEXCEPT("Not enough operands on the stack to make this operation"); }
	IOperand const * op = **(this->_stack.begin()) / **(std::next(this->_stack.begin()));
	this->pop();
	this->pop();
	this->push(op);
}

void Vm::mod (void)
{
	if (this->_stack.size() < 2) { EXECEXCEPT("Not enough operands on the stack to make this operation"); }
	IOperand const * op = **(this->_stack.begin()) % **(std::next(this->_stack.begin()));
	this->pop();
	this->pop();
	this->push(op);
}

IOperand const * Vm::createInt8 (std::string const & value) const
{
	return new Operand<int8_t>(INT8, value);
}

IOperand const * Vm::createInt16 (std::string const & value) const
{
	return new Operand<int16_t>(INT16, value);
}

IOperand const * Vm::createInt32 (std::string const & value) const
{
	return new Operand<int32_t>(INT32, value);
}

IOperand const * Vm::createFloat (std::string const & value) const
{
	return new Operand<float>(FLOAT, value);
}

IOperand const * Vm::createDouble (std::string const & value) const
{
	return new Operand<double>(DOUBLE, value);
}
