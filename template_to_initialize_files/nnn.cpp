#include "nnn.hpp"

nnn::nnn(void)
{
	return ;
}

nnn::nnn(const nnn& obj)
{
	*this = obj;
	return ;
}

nnn::~nnn(void)
{
	return ;
}

nnn& nnn::operator=(const nnn& obj)
{
	if (this != &obj)
	{
		this->_XXX = obj._XXX;
	}
	return (*this);
}

std::ostream&	operator<<(std::ostream& o, const nnn& i)
{
	(void)i;
	o << "something";
	return o;
}
