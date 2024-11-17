#include "Variables.h"

#include <map>
#include <string>

extern "C"
{
#include "execute/t_env/t_env_internals.h"
#include "execute/t_env/t_env.h"
#include "word/expansions/expand.h"
};

Variables::Variables(const std::map<std::string, std::string> &env) : last_status(0)
{
	this->variables = nullptr;
	for (auto& [key, value] : env)
	{
		t_env_entry entry = {strdup(key.c_str()), strdup(value.c_str())};
		env_push_front(&this->variables, entry); // should use env_insert?
	}
}

Variables::~Variables()
{
	//env_destroy(&this->variables); // bad: this leaks rn
}

t_expansion_variables Variables::as_c_struct() const
{
	return (t_expansion_variables) {this->variables, this->last_status};
}
