#pragma once

#include <ostream>
#include <vector>

class DataType
{
public:
	static DataType Invalid, Unit, Integer, String, Boolean;

	static DataType Variable(std::string value);
	static DataType Function(const DataType &return_type, const std::vector<DataType> &parameter_types);

	friend std::ostream &operator<<(std::ostream &stream, const DataType &type);
	bool operator==(const DataType &rhs) const;
	bool operator!=(const DataType &rhs) const;

	std::string value;
	bool is_function;
	std::vector<DataType> parameter_types;

private:
	DataType(std::string value);
	DataType(std::string value, const std::vector<DataType> &parameter_types);
};
