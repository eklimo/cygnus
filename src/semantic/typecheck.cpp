#include "typecheck.h"

#include "util/error.h"
#include "lang.h"

TypeChecker::TypeChecker(std::string_view file, std::string_view source)
	: file(file),
	  source(source),
	  error(false),
	  type(DataType::Invalid)
{
}

bool TypeChecker::failed() const
{
	return error;
}

DataType TypeChecker::get_type(Node &node)
{
	node.accept(*this);
	auto temp = type;
	type = DataType::Invalid;
	return temp;
}

DataType TypeChecker::check_infix(const Token &token, const Expression *const left, const DataType &left_type, const Expression *const right, const DataType &right_type)
{
	auto op = token.value;

	if(Lang::is_arithmetic(op))
	{
		if(left_type == right_type && right_type == DataType::Integer)
			return DataType::Integer;
		else if(left_type == DataType::String || right_type == DataType::String)
			return DataType::String;
	}
	else if(Lang::is_assignment(op))
	{
		if(!dynamic_cast<const Identifier *>(left))
		{
			error = true;
			Util::Error::At(
			    token,
			    "left-hand expression must be assignable"
			).print(file, source);
			return DataType::Invalid;
		}

		if(left_type == right_type)
			return left_type;
	}
	else if(Lang::is_boolean_op(op))
	{
		if(left_type == right_type)
		{
			if(token.value == "==" || token.value == "!=")
				return DataType::Boolean;
			else if(right_type == DataType::Integer && (token.value == ">" || token.value == ">=" || token.value == "<" || token.value == "<="))
				return DataType::Boolean;
			else if(right_type == DataType::Boolean)
				return DataType::Boolean;
		}
	}

	if(left_type != DataType::Invalid && right_type != DataType::Invalid)
	{
		error = true;
		Util::Error::At(
		    token,
		    "mismatched types '", left_type,
		    "' and '", right_type,
		    "' for operator '", token.value, "'"
		).print(file, source);
	}
	return DataType::Invalid;
}

DataType TypeChecker::check_prefix(const Token &token, const Expression *const operand, const DataType &operand_type)
{
	auto op = token.value;

	if(Lang::is_arithmetic(op))
	{
		if(operand_type == DataType::Integer)
			return operand_type;
	}
	else if(Lang::is_boolean_op(op))
	{
		if(operand_type == DataType::Boolean)
			return operand_type;
	}

	if(operand_type != DataType::Invalid)
	{
		error = true;
		Util::Error::At(
		    token,
		    "mismatched type '", operand_type,
		    "' for operator '", token.value, "'"
		).print(file, source);
	}
	return DataType::Invalid;
}

DataType TypeChecker::check_postfix(const Token &token, const Expression *const operand, const DataType &operand_type)
{
	auto op = token.value;

	if(Lang::is_arithmetic(op))
	{
		if(operand_type == DataType::Integer)
			return operand_type;
	}

	if(operand_type != DataType::Invalid)
	{
		error = true;
		Util::Error::At(
		    token,
		    "mismatched type '", operand_type,
		    "' for operator '", token.value, "'"
		).print(file, source);
	}
	return DataType::Invalid;
}

// main

void TypeChecker::visit(Program &node)
{
	for(const auto &stmt : node.statements)
	{
		stmt->accept(*this);
	}
}

// statements

void TypeChecker::visit(ExprStatement &node)
{
	print(str.stringify(node));
	tab_level++;

	node.expr->accept(*this);

	tab_level--;
}
void TypeChecker::visit(VariableDef &node)
{
	print(str.stringify(node));
	tab_level++;

	auto variable_type = node.type
	                     ? get_type(*node.type)
	                     : get_type(*node.value);

	type = variable_type;

	if(node.type && node.value)
	{
		auto inferred_type = get_type(*node.value);
		if(variable_type != DataType::Invalid && inferred_type != DataType::Invalid && variable_type != inferred_type)
		{
			error = true;
			Util::Error::At(
			    node.name->token,
			    "inferred type '", inferred_type,
			    "' does not match explicit type '", variable_type, "'"
			).print(file, source);
		}

		type = variable_type != DataType::Invalid
		       ? variable_type
		       : inferred_type;
	}

	node.name->symbol =
	    std::make_shared<SymbolData>
	    (
	        nullptr,
	        0,
	        node.name.get(),
	        type
	    );

	tab_level--;
	print(": ", type);
}
void TypeChecker::visit(FunctionDef &node)
{
	print(str.stringify(node));
	tab_level++;

	std::vector<DataType> parameter_types;

	for(const auto &param : node.parameters)
	{
		auto param_type = get_type(*param);
		param->name->symbol =
		    std::make_shared<SymbolData>
		    (
		        nullptr,
		        0,
		        param->name.get(),
		        param_type
		    );
		parameter_types.push_back(param_type);
	}

	auto return_type = node.return_type
	                   ? get_type(*node.return_type)
	                   : DataType::Unit;

	auto _type = DataType::Function(return_type, parameter_types);

	node.name->symbol =
	    std::make_shared<SymbolData>
	    (
	        nullptr,
	        0,
	        node.name.get(),
	        _type
	    );

	auto body_return_type = get_type(*node.body);
	if(body_return_type != DataType::Invalid && return_type != DataType::Invalid && body_return_type != return_type)
	{
		error = true;
		Util::Error::At(
		    node.name->token,
		    "body return type '", body_return_type,
		    "' does not match signature return type '", return_type, "'"
		).print(file, source);
	}

	type = _type;

	tab_level--;
	print(": ", type);
}

// expressions
#include "util/stringifier.h"
void TypeChecker::visit(NumberLiteral &node)
{
	type = DataType::Integer;

	print(str.stringify(node), " : ", type);
}
void TypeChecker::visit(StringLiteral &node)
{
	type = DataType::String;

	print(str.stringify(node), " : ", type);
}
void TypeChecker::visit(BooleanLiteral &node)
{
	type = DataType::Boolean;

	print(str.stringify(node), " : ", type);
}
void TypeChecker::visit(UnitLiteral &node)
{
	type = DataType::Unit;

	print(str.stringify(node), " : ", type);
}
void TypeChecker::visit(Identifier &node)
{
	if(node.symbol->type.is_function || node.symbol->type != DataType::Invalid)
	{
		type = node.symbol->type;

		print(str.stringify(node), " : ", type);
	}
	else if(&node != node.symbol->node)
	{
		print(str.stringify(node));
		tab_level++;

		type = get_type(*node.symbol->node);

		tab_level--;
		print(": ", type);
	}
}
void TypeChecker::visit(FunctionCall &node)
{
	print(str.stringify(node));
	tab_level++;

	auto signature = get_type(*node.name);

	if(!signature.is_function)
	{
		type = DataType::Invalid;

		error = true;
		Util::Error::At(
		    node.name->token,
		    "call to non-function type '", node.name->token.value, "'"
		).print(file, source);
	}
	else
	{
		if(node.arguments.size() != signature.parameter_types.size())
		{
			error = true;
			Util::Error::At(
			    node.name->token,
			    "mismatched number of arguments to '", node.name->token.value,
			    "': expected ", signature.parameter_types.size(),
			    ", found ", node.arguments.size()
			).print(file, source);
		}

		for(size_t i = 0; i < (node.arguments.size() < signature.parameter_types.size() ? node.arguments.size() : signature.parameter_types.size()); i++)
		{
			auto arg = get_type(*node.arguments[i]);
			auto param = signature.parameter_types[i];

			if(arg != DataType::Invalid && param != DataType::Invalid && arg != param)
			{
				error = true;
				Util::Error::At(
				    node.name->token,
				    "mismatched argument types for '", node.name->token.value,
				    "': expected '", param,
				    "', found '", arg, "'"
				).print(file, source);
			}
		}

		type = DataType::Variable(signature.value);
	}

	tab_level--;
	print(": ", type);
}

void TypeChecker::visit(InfixOperator &node)
{
	print(str.stringify(node));
	tab_level++;

	auto left_type = get_type(*node.left);
	auto right_type = get_type(*node.right);

	type = check_infix(node.token, node.left.get(), left_type, node.right.get(), right_type);

	tab_level--;
	print(": ", type);
}
void TypeChecker::visit(PrefixOperator &node)
{
	print(str.stringify(node));
	tab_level++;

	auto operand_type = get_type(*node.operand);
	type = check_prefix(node.token, node.operand.get(), operand_type);

	tab_level--;
	print(": ", type);

}
void TypeChecker::visit(PostfixOperator &node)
{
	print(str.stringify(node));
	tab_level++;

	auto operand_type = get_type(*node.operand);
	type = check_postfix(node.token, node.operand.get(), operand_type);

	tab_level--;
	print(": ", type);
}
void TypeChecker::visit(ReturnExpr &node)
{
	print(str.stringify(node));
	tab_level++;

	type = node.value
	       ? get_type(*node.value)
	       : DataType::Unit;

	tab_level--;
	print(": ", type);
}
void TypeChecker::visit(IfExpr &node)
{
	print(str.stringify(node));
	tab_level++;

	auto condition_type = get_type(*node.condition);
	if(condition_type != DataType::Boolean)
	{
		error = true;
		Util::Error(
		{1, 1},
		"mismatched type '", condition_type, "' for condition"
		).print(file, source);
	}

	auto if_type = get_type(*node.if_branch);
	auto _type = if_type;

	if(node.else_branch)
	{
		auto else_type = get_type(*node.else_branch);
		if(if_type != else_type)
		{
			_type = DataType::Invalid;
			error = true;
			Util::Error(
			{1, 1},
			"mismatched types '", if_type,
			"' and '", else_type,
			"' for if/else branches"
			).print(file, source);
		}
	}

	type = _type;

	tab_level--;
	print(": ", type);
}
void TypeChecker::visit(WhileExpr &node)
{
	print(str.stringify(node));
	tab_level++;

	auto condition = get_type(*node.condition);
	if(condition != DataType::Boolean)
	{
		error = true;
		Util::Error(
		{1, 1},
		"mismatched type '", condition,
		"' for condition"
		).print(file, source);
	}

	node.body->accept(*this);

	type = DataType::Unit;

	tab_level--;
	print(": ", type);
}

// general

void TypeChecker::visit(Invalid &node)
{
	type = DataType::Invalid;

	print(str.stringify(node), " : ", type);
}
void TypeChecker::visit(Block &node)
{
	auto return_type = DataType::Unit;

	if(node.statements.empty())
	{
		type = return_type;
		print(str.stringify(node), " : ", type);
		return;
	}

	print(str.stringify(node));
	tab_level++;

	bool found = false;
	for(const auto &stmt : node.statements)
	{
		// find return expression
		if(auto expr_stmt = dynamic_cast<ExprStatement *>(stmt.get()))
		{
			if(auto ret_expr = dynamic_cast<ReturnExpr *>(expr_stmt->expr.get()))
			{
				if(!found)
					return_type = get_type(*ret_expr);
				else stmt->accept(*this);
				found = true;
			}
			else stmt->accept(*this);
		}
		else stmt->accept(*this);
	}

	type = return_type;

	tab_level--;
	print(": ", type);
}
void TypeChecker::visit(Parameter &node)
{
	print(str.stringify(node));
	tab_level++;

	type = get_type(*node.type);

	tab_level--;
	print(": ", type);
}
void TypeChecker::visit(Type &node)
{
	if(node.token.value == "()") type = DataType::Unit;
	else if(node.token.value == "Int") type = DataType::Integer;
	else if(node.token.value == "String") type = DataType::String;
	else if(node.token.value == "Bool") type = DataType::Boolean;
	else
	{
		type = DataType::Invalid;

		error = true;
		Util::Error::At(
		    node.token,
		    "invalid type '", node.token.value, "'"
		).print(file, source);
	}

	print(str.stringify(node), " : ", type);
}
