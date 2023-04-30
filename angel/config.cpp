#include "config.h"
#include <string>

namespace angel
{
    
static angel::Logger::ptr g_logger = ANGEL_LOG_NAME("system");

ConfigVarBase::ptr Config::LookupBase(const std::string& name)
{
    auto it = GetDatas().find(name);
    return it == GetDatas().end() ? nullptr : it->second;
}

static void ListAllMenber(const std::string& prefix, const YAML::Node& node, std::list<std::pair<std::string, const YAML::Node> >& output)
{
    if(prefix.find_first_not_of("abcdefghijklmnopqrstuvwxyz._0123456789") != std::string::npos)
    {
        ANGEL_LOG_ERROR(ANGEL_LOG_ROOT()) << "config invalid name: " << prefix << " " << node;
        return;
    }
    output.push_back(std::make_pair(prefix,node));
    if(node.IsMap())
    {
        for(auto it = node.begin();it != node.end(); ++it)
        {
            ListAllMenber(prefix.empty() ? it->first.Scalar() : prefix + "." + it->first.Scalar(), it->second, output);
        }
    }
}


void Config::LoadFromYaml(const YAML::Node& root)
{
    std::list<std::pair<std::string, const YAML::Node> > all_nodes;
    ListAllMenber("", root, all_nodes);
    for(auto& i : all_nodes)
    {
        std::string key = i.first;
        if(key.empty()) {continue;}
        std::transform(key.begin(), key.end(), key.begin(), ::tolower);
        ConfigVarBase::ptr var = LookupBase(key);
        if(var)
        {
            if(i.second.IsScalar())
            {  
                var->fromString(i.second.Scalar());
            }else{
                std::stringstream ss;
                ss << i.second;
                var->fromString(ss.str());
            }
        }
    }
}

void Config::LoadFromConDir(const std::string& path, bool force)
{
    std::cout << path << force;
    return;
}

void Config::Visit(std::function<void(ConfigVarBase::ptr)> cb)
{
    ConfigVarMap& m = GetDatas();
    for(auto it = m.begin(); it != m.end(); ++it)
    {
        cb(it->second);
    }
}

}
