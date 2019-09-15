#include "type.h"

DataType DataType::Invalid = DataType::Variable("Invalid");
DataType DataType::Unit = DataType::Variable("()");
DataType DataType::Integer = DataType::Variable("Int");
DataType DataType::String = DataType::Variable("String");
DataType DataType::Boolean = DataType::Variable("Bool");

DataType DataType::Variable(std::string value)
{
	return DataType(value);
}

DataType DataType::Function(const DataType &return_type, const std::vector<DataType> &parameter_types)
{
	return DataType(return_type.value, parameter_types);
}

DataType::DataType(std::string value)
	: value(value),
	  is_function(false),
	  parameter_types({})
{
}

DataType::DataType(std::string value, const std::vector<DataType> &parameter_types)
	: value(value),
	  is_function(true),
	  parameter_types(parameter_types)
{
}

std::ostream &operator<<(std::ostream &stream, const DataType &type)
{
	if(type.is_function)
	{
		stream << "(";
		for(size_t i = 0; i < type.parameter_types.size(); i++)
		{
			stream << type.parameter_types[i];
			if(i != type.parameter_types.size() - 1) stream << ", ";
		}
		stream << ") -> ";
	}
	stream << type.value;
	return stream;
}

bool DataType::operator==(const DataType &rhs) const
{
	return value == rhs.value;
}

bool DataType::operator!=(const DataType &rhs) const
{
	return !(*this == rhs);
}
