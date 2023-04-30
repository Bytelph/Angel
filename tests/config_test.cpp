#include "../angel/log.h"
#include "../angel/config.h"
#include <yaml-cpp/yaml.h>
#include <iostream>


/**************************************************************************************************************************
 * file: config_test.cpp
 * 测试内容：
 * print_yaml:
 *
 *
 * *************************************************************************************************************************/
angel::ConfigVar<int>::ptr g_int_value_config = angel::Config::Lookup("system.port", (int)8080, "system port");
//angel::ConfigVar<float>::ptr g_int_float_value_config = angel::Config::Lookup("system.port", (float)8080, "system port");
//angel::ConfigVar<float>::ptr g_int_float2_value_config = angel::Config::Lookup("system.port", (float)80.20, "system port");
angel::ConfigVar<std::vector<int> >::ptr g_int_vector_value_config = angel::Config::Lookup("system.int_vec", std::vector<int>{1,2}, "system int vac");
angel::ConfigVar<std::list<int> >::ptr g_int_list_value_config = angel::Config::Lookup("system.int_list", std::list<int>{1,3,2}, "system int_list");
angel::ConfigVar<std::set<int> >::ptr g_int_set_value_config = angel::Config::Lookup("system.int_set", std::set<int>{1,3,2}, "system int_set");
angel::ConfigVar<std::unordered_set<int> >::ptr g_int_unordered_set_value_config = angel::Config::Lookup("system.int_unordered_set", std::unordered_set<int>{1,3,2}, "system int_set");

angel::ConfigVar<std::map<std::string, int> >::ptr g_str_int_map_value_config = angel::Config::Lookup("system.str_int_map", std::map<std::string, int>{{"k", 2}}, "system int_map");
angel::ConfigVar<std::unordered_map<std::string, int> >::ptr g_str_int_unordered_map_value_config = angel::Config::Lookup("system.str_int_unordered_map", std::unordered_map<std::string, int>{{"k", 2}}, "system int_umap");

void print_yaml(const YAML::Node& node, int level)
{
    if(node.IsScalar())
    {
        ANGEL_LOG_INFO(ANGEL_LOG_ROOT()) << std::string(level*4, ' ') << node.Scalar() << "-" << node.Type() << "-" << level;
    }else if(node.IsNull()){
        ANGEL_LOG_INFO(ANGEL_LOG_ROOT()) << std::string(level*4, ' ') << "NULL-" << node.Type() << "-" << level;
    }else if(node.IsMap()){
        for(auto it = node.begin(); it !=node.end(); ++it)
        {
            ANGEL_LOG_INFO(ANGEL_LOG_ROOT()) << std::string(level*4, ' ') << it->first << "-" << it->second.Type() << "-" << level;
            print_yaml(it->second, level+1);
        }
    }else if(node.IsSequence()){
        for(size_t i =0; i < node.size(); ++i)
        {
            ANGEL_LOG_INFO(ANGEL_LOG_ROOT()) << std::string(level*4, ' ') << i << "-" << node[i].Type() << "-" << level;
            print_yaml(node[i], level+1);
        }
    }
}

void test_yaml()
{
    YAML::Node root = YAML::LoadFile("/home/bytecat/log.yaml");
    ANGEL_LOG_INFO(ANGEL_LOG_ROOT()) << root["test"].IsDefined();
    ANGEL_LOG_INFO(ANGEL_LOG_ROOT()) << root["logs"].IsDefined();
    ANGEL_LOG_INFO(ANGEL_LOG_ROOT()) << root;

    print_yaml(root, 0);
}
//自定义类型
class Person
{
public:
    std::string toString() const
    {
        std::stringstream ss;
        ss << "[Person name: " << m_name
           << "age: " << m_age
           << "sex: " << m_sex << "]";
        return ss.str();
    }
    bool operator==(const Person& other) const
    {
        return m_name == other.m_name && m_age == other.m_age && m_sex == other.m_sex;
    }

public:
    std::string m_name = "";
    int m_age = 0;
    bool m_sex = true;
};

namespace angel
{
//@brief 1.YAML string转为：Person
template<>
class LexicalCast<std::string, Person>
{
public:
    Person operator()(const std::string& v)
    {
        YAML::Node node = YAML::Load(v);
        Person p;
        p.m_name = node["name"].as<std::string>();
        p.m_age = node["age"].as<int>();
        p.m_sex= node["sex"].as<bool>();
        return p;
    }
};

//@brief 2. list格式转化为YAML string格式
template<>
class LexicalCast<Person, std::string>
{
public:
    std::string operator()(const Person& p)
    {
        YAML::Node node;
        node["name"] = p.m_name;
        node["age"] = p.m_age;
        node["sex"] = p.m_sex;
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

}

angel::ConfigVar<Person>::ptr g_person = angel::Config::Lookup("class.person", Person(), "system person");

void test_class()
{
    ANGEL_LOG_INFO(ANGEL_LOG_ROOT()) << "before: " << g_person->getValue().toString() << "-" << g_person->toString();

    YAML::Node root = YAML::LoadFile("/home/bytecat/log.yaml");
    angel::Config::LoadFromYaml(root);
    
    ANGEL_LOG_INFO(ANGEL_LOG_ROOT()) << "after: " << g_person->getValue().toString() << "-" << g_person->toString();
}

void test_config()
{
    ANGEL_LOG_INFO(ANGEL_LOG_ROOT()) << "before:" << g_int_value_config->getValue();
    g_person->addListener([](const Person& old_value, const Person& new_value){
        ANGEL_LOG_INFO(ANGEL_LOG_ROOT()) << "the old value: " << old_value.toString() << " the new value: " << new_value.toString(); 
    }); 

#define XX(g_var, name, prefix) \
    { \
        auto& v = g_var->getValue(); \
        for(auto& i : v) { \
            ANGEL_LOG_INFO(ANGEL_LOG_ROOT()) << #prefix " " #name ": " << i; \
        } \
        ANGEL_LOG_INFO(ANGEL_LOG_ROOT()) << #prefix " " #name " yaml: " << g_var->toString(); \
    }
#define XX_M(g_var, name, prefix) \
    { \
        auto& v = g_var->getValue(); \
        for(auto& i : v) { \
            ANGEL_LOG_INFO(ANGEL_LOG_ROOT()) << #prefix " " #name ": {" \
                    << i.first << " - " << i.second << "}"; \
        } \
        ANGEL_LOG_INFO(ANGEL_LOG_ROOT()) << #prefix " " #name " yaml: " << g_var->toString(); \
    }
    XX(g_int_vector_value_config, int_vec, before);
    XX(g_int_list_value_config, int_list, before);
    XX(g_int_set_value_config, int_set, before);
    XX(g_int_unordered_set_value_config, int_unordered_set, before);
    XX_M(g_str_int_map_value_config, str_int_map, before);
    XX_M(g_str_int_unordered_map_value_config, str_int_unordered_map, before);
    

    YAML::Node root = YAML::LoadFile("/home/bytecat/log.yaml");
    angel::Config::LoadFromYaml(root);

    ANGEL_LOG_INFO(ANGEL_LOG_ROOT()) << "after:" << g_int_value_config->getValue();
  //  ANGEL_LOG_INFO(ANGEL_LOG_ROOT()) << "before:" << g_int_float_value_config->toString();
    g_person->addListener([](const Person& old_value, const Person& new_value){
        ANGEL_LOG_INFO(ANGEL_LOG_ROOT()) << "the old value: " << old_value.toString() << " the new value: " << new_value.toString(); 
    });
    XX(g_int_vector_value_config, int_vec, after);
    XX(g_int_list_value_config, int_list, after);
    XX(g_int_set_value_config, int_set, after);
    XX(g_int_unordered_set_value_config, int_unordered_set, after);
    XX_M(g_str_int_map_value_config, str_int_map, after);
    XX_M(g_str_int_unordered_map_value_config, str_int_unordered_map, after);
    
}
void test_log()
{
    static angel::Logger::ptr rootlogger = ANGEL_LOG_NAME("root");
    ANGEL_LOG_INFO(rootlogger) << "hello system" << std::endl;
    std::cout << angel::LoggerMgr::GetInstance()->toYamlString() << std::endl;
    YAML::Node root = YAML::LoadFile("/home/bytecat/log.yaml");
    angel::Config::LoadFromYaml(root);
    std::cout << angel::LoggerMgr::GetInstance()->toYamlString() << std::endl;
    ANGEL_LOG_INFO(rootlogger) << "after hello system" << std::endl;
    ANGEL_LOG_DEBUG(rootlogger) << "after hello system" << std::endl;
    ANGEL_LOG_ERROR(rootlogger) << "after hello system" << std::endl;
    ANGEL_LOG_WARN(rootlogger) << "after hello system" << std::endl;
    ANGEL_LOG_FATAL(rootlogger) << "after hello system" << std::endl;
}

int main(int argc, char** argv)
{
    //ANGEL_LOG_INFO(ANGEL_LOG_ROOT() ) << g_int_value_config-> getValue();
    //test_yaml();
   // test_config();
    //test_class();
    test_log();
    return 0;
}


