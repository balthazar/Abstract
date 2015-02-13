/* ========================================================================== */
/*                                                                            */
/*    ██╗  ██╗██████╗   █████╗ ██╗   ██╗                                      */
/*    ██║  ██║╚════██╗ ██╔══██╗██║   ██║                                      */
/*    ███████║ █████╔╝ ███████║██║   ██║       by: bgronon                    */
/*    ╚════██║██╔═══╝  ██╔══██║╚██╗ ██╔╝       at: 2015/02/12 11:11:30        */
/*         ██║███████╗ ██║  ██║ ╚████╔╝                                       */
/*         ╚═╝╚══════╝ ╚═╝  ╚═╝  ╚═══╝                                        */
/*                                                                            */
/* ========================================================================== */

#ifndef OPERAND_CLASS
# define OPERAND_CLASS

# include "IOperand.hpp"
# include "abstract.hpp"
# include "Vm.hpp"
# include <sstream>
# include <iostream>
# include <math.h>
# include <limits>

template <typename T>
class Operand: public IOperand {

	public:

		Operand (eOperandType type, std::string const & val): _type(type), _str(val)
		{
			double				check;
			std::stringstream	ss;

			ss << val;
			ss >> check;
			if (check < std::numeric_limits<T>::lowest() || (type == INT8 && check < -127)) {
				throw Operand::UnderflowException(__FILE__, __LINE__);
			}
			else if (check > std::numeric_limits<T>::max() || (type == INT8 && check > 128)) {
				throw Operand::OverflowException(__FILE__, __LINE__);
			}
			else if (ss.fail()) {
				throw Operand::InvalidNumException(__FILE__, __LINE__);
			}
		}

		virtual ~Operand (void) {}

		int				getPrecision (void) const
		{
			return this->_type;
		}

		eOperandType	getType (void) const
		{
			return this->_type;
		}

		std::string const &	toString (void) const
		{
			return this->_str;
		}

		IOperand const *	operator+ (IOperand const & rhs) const
		{
			double				one;
			double				two;
			eOperandType		type;
			std::stringstream	res;

			this->_gulpParams(one, two, type, rhs);
			res << (one + two);
			return this->_renderOperand(type, res);
		}

		IOperand const *	operator- (IOperand const & rhs) const
		{
			double				one;
			double				two;
			eOperandType		type;
			std::stringstream	res;

			this->_gulpParams(one, two, type, rhs);
			res << (one - two);
			return this->_renderOperand(type, res);
		}

		IOperand const *	operator* (IOperand const & rhs) const
		{
			double				one;
			double				two;
			eOperandType		type;
			std::stringstream	res;

			this->_gulpParams(one, two, type, rhs);
			res << (one * two);
			return this->_renderOperand(type, res);
		}

		IOperand const *	operator/ (IOperand const & rhs) const
		{
			double				one;
			double				two;
			eOperandType		type;
			std::stringstream	res;

			this->_gulpParams(one, two, type, rhs);
			res << (one / two);
			return this->_renderOperand(type, res);
		}

		IOperand const *	operator% (IOperand const & rhs) const
		{
			double				one;
			double				two;
			eOperandType		type;
			std::stringstream	res;

			this->_gulpParams(one, two, type, rhs);
			res << fmod(one, two);
			return this->_renderOperand(type, res);
		}

		class UnderflowException: public ExecutionException {
			public:
				UnderflowException (const char * file, int line)
				{
					std::stringstream ss;
					ss << "\033[37m\033[41m" << "A wild underflow appears " << "\033[0m at \033[35mline " << line << "\033[0m of \033[35m" << file << "\033[0m.";
					this->_msg = ss.str();
				}

				virtual const char * what (void) const throw ()
				{
					return this->_msg.c_str();
				}

				UnderflowException (UnderflowException const & ref) { VOID(ref); }
				~UnderflowException (void) {}

			private:
				std::string		_msg;

				UnderflowException & operator= (UnderflowException const & ref);
				UnderflowException (void);
		};

		class InvalidNumException: public ExecutionException {
			public:
				InvalidNumException (const char * file, int line)
				{
					std::stringstream ss;
					ss << "\033[37m\033[41m" << "Invalid numeric representation " << "\033[0m at \033[35mline " << line << "\033[0m of \033[35m" << file << "\033[0m.";
					this->_msg = ss.str();
				}

				virtual const char * what (void) const throw ()
				{
					return this->_msg.c_str();
				}

				InvalidNumException (InvalidNumException const & ref) { VOID(ref); }
				~InvalidNumException (void) {}

			private:
				std::string		_msg;

				InvalidNumException & operator= (InvalidNumException const & ref);
				InvalidNumException (void);
		};

		class OverflowException: public ExecutionException {
			public:
				OverflowException (const char * file, int line)
				{
					std::stringstream ss;
					ss << "\033[37m\033[41m" << "A wild overflow appears " << "\033[0m at \033[35mline " << line << "\033[0m of \033[35m" << file << "\033[0m.";
					this->_msg = ss.str();
				}

				virtual const char * what (void) const throw ()
				{
					return this->_msg.c_str();
				}

				OverflowException (OverflowException const & ref) { VOID(ref); }
				~OverflowException (void) {}

			private:
				std::string		_msg;

				OverflowException & operator= (OverflowException const & ref);
				OverflowException (void);
		};

	private:

		eOperandType		_type;
		std::string			_str;

		void				_gulpParams (double & one, double & two, eOperandType & type, IOperand const & rhs) const
		{
			std::stringstream	ss(this->_str);
			std::stringstream	ss2(rhs.toString());

			ss >> one;
			ss2 >> two;

			type = (this->_type > rhs.getType() ? this->_type : rhs.getType());
		}

		IOperand const *	_renderOperand (eOperandType & type, std::stringstream & stream) const
		{
			std::stringstream ss;

			if (type == INT8 || type == INT16 || type == INT32) {
				int tmp;
				stream >> tmp;
				ss << tmp;
			}
			else if (type == FLOAT) {
				float tmp;
				stream >> tmp;
				ss << tmp;
			}
			else {
				ss << stream.str();
			}

			return Vm::single().createOperand(type, ss.str());
		}

		Operand (void);
		Operand (Operand const & ref);
		Operand & operator= (Operand const & ref);

};

#endif
