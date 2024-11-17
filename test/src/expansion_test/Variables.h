#ifndef VARIABLES_H
#define VARIABLES_H

#include <map>
#include <string>

extern "C"
{
#include "execute/t_env/t_env_internals.h"
#include "execute/t_env/t_env.h"
#include "word/expansions/expand.h"
};

struct Variables
{
    t_env *variables;
    int last_status;
    //std::map<std::string, std::string> variables;

    Variables(const std::map<std::string, std::string> &env);
    Variables(const std::map<std::string, std::string> &env, int last_status);
    ~Variables();

    t_expansion_variables as_c_struct() const;
};

#endif // VARIABLES_H
