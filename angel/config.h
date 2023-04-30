#ifndef __ANGEL_CONFIG_H__
#define __ANGEL_CONFIG_H__

#include <iostream>
#include <memory>
#include <string>
#include <algorithm>
#include <functional>
#include <boost/lexical_cast.hpp>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include <yaml-cpp/yaml.h>

#include "log.h"
#include "util.h"

/* 备忘录：
 *YAML的安装位置在：usr/local/include/yaml-cpp
 *
 */

namespace angel
{
/*----------------类型转换模板------------------*/

//通用类型
template<typename F, typename T>
class LexicalCast
{
public:
    T operator()(const F& v)
    {
        return boost::lexical_cast<T>(v);
    }
};
/*
 *偏特化
 */
//@brief 1.YAML string转为：std::vector<T>
template<typename T>
class LexicalCast<std::string, std::vector<T> >
{
public:
    std::vector<T> operator()(const std::string& v)
    {
        YAML::Node node = YAML::Load(v);
        typename std::vector<T> vec;
        std::stringstream ss;
        for(size_t i = 0; i < node.size(); i++)
        {
            ss.str("");
            ss << node[i];
            vec.push_back(LexicalCast<std::string, T>()(ss.str()) );
        }
        return vec;
    }
};

//@brief 2.vector格式转化为YAML string格式
template<typename T>
class LexicalCast<std::vector<T>, std::string>
{
public:
    std::string operator()(const std::vector<T>& v)
    {
        YAML::Node node(YAML::NodeType::Sequence);
        for(auto& i : v)
        {
            node.push_back(YAML::Load(LexicalCast<T,std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

//@brief 1.YAML string转为：list
template<typename T>
class LexicalCast<std::string, std::list<T> >
{
public:
    std::list<T> operator()(const std::string& v)
    {
        YAML::Node node = YAML::Load(v);
        typename std::list<T> vec;
        std::stringstream ss;
        for(size_t i = 0; i < node.size(); i++)
        {
            ss.str("");
            ss << node[i];
            vec.push_back(LexicalCast<std::string, T>()(ss.str()) );
        }
        return vec;
    }
};

//@brief 2. list格式转化为YAML string格式
template<typename T>
class LexicalCast<std::list<T>, std::string>
{
public:
    std::string operator()(const std::list<T>& v)
    {
        YAML::Node node(YAML::NodeType::Sequence);
        for(auto& i : v)
        {
            node.push_back(YAML::Load(LexicalCast<T,std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

//brief 类型转换YAML转为set
template<typename T>
class LexicalCast<std::string, std::set<T> >
{
public:
    std::set<T> operator()(const std::string& v)
    {
        YAML::Node node = YAML::Load(v);
        typename std::set<T> st;
        std::stringstream ss;
        for(size_t i = 0; i < node.size(); ++i)
        {
            ss.str("");
            ss << node[i];
            st.insert(LexicalCast<std::string, T>()(ss.str()));
        }
        return st;
    }
};

//brief 类型转换set转为YANL
template<typename T>
class LexicalCast<std::set<T>, std::string>
{
public:
    std::string operator()(const std::set<T> v)
    {
        YAML::Node node(YAML::NodeType::Sequence);
        for(auto& i : v)
        {
            node.push_back(YAML::Load(LexicalCast<T,std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

//brief 类型转换YAML转为unordered_set
template<typename T>
class LexicalCast<std::string, std::unordered_set<T> >
{
public:
    std::unordered_set<T> operator()(const std::string& v)
    {
        YAML::Node node = YAML::Load(v);
        typename std::unordered_set<T> ust;
        std::stringstream ss;
        for(size_t i = 0; i < node.size(); ++i)
        {
            ss.str();
            ss << node[i];
            ust.insert(LexicalCast<std::string, T>()(ss.str()));
        }
        return ust;
    }
};

//brief 类型转换unordered_set转为YAML
template<typename T>
class LexicalCast<std::unordered_set<T>, std::string>
{
public:
    std::string operator()(const std::unordered_set<T> v)
    {
        YAML::Node node(YAML::NodeType::Sequence);
        for(auto& i : v)
        {
            node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

//brief 类型转换YAML转为map
template<typename T>
class LexicalCast<std::string, std::map<std::string, T> >
{
public:
    std::map<std::string, T> operator()(const std::string& v)
    {
        YAML::Node node = YAML::Load(v);
        typename std::map<std::string, T> mp;
        std::stringstream ss;
        for(auto it = node.begin(); it != node.end(); ++it)
        {
            ss.str("");
            ss << it->second;
            mp.insert(std::make_pair(it->first.Scalar(), LexicalCast<std::string, T>()(ss.str())));
        }
        return mp;
    }
};

//brief 类型转换map转为YAML格式
template<typename T>
class LexicalCast<std::map<std::string, T>, std::string>
{
public:
    std::string operator()(const std::map<std::string, T> v)
    {
        YAML::Node node(YAML::NodeType::Map);
        for(auto& i : v)
        {
            node[i.first] = YAML::Load(LexicalCast<T,std::string>()(i.second));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

/**
 * @brief 类型转换模板类片特化(YAML String 转换成 std::unordered_map<std::string, T>)
 */
template<typename T>
class LexicalCast<std::string, std::unordered_map<std::string, T> > {
public:
    std::unordered_map<std::string, T> operator()(const std::string& v) {
        YAML::Node node = YAML::Load(v);
        typename std::unordered_map<std::string, T> vec;
        std::stringstream ss;
        for(auto it = node.begin();
                it != node.end(); ++it) {
            ss.str("");
            ss << it->second;
            vec.insert(std::make_pair(it->first.Scalar(),
                        LexicalCast<std::string, T>()(ss.str())));
        }
        return vec;
    }
};

/**
 * @brief 类型转换模板类片特化(std::unordered_map<std::string, T> 转换成 YAML String)
 */
template<typename T>
class LexicalCast<std::unordered_map<std::string, T>, std::string> {
public:
    std::string operator()(const std::unordered_map<std::string, T>& v) {
        YAML::Node node(YAML::NodeType::Map);
        for(auto& i : v) {
            node[i.first] = YAML::Load(LexicalCast<T, std::string>()(i.second));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};



/*--------------------配置类--------------------*/    
class ConfigVarBase
{
public:
    typedef std::shared_ptr<ConfigVarBase> ptr;
    ConfigVarBase(const std::string& name, const std::string& description = "")
     :m_name(name)
     ,m_description(description)
     {
        std::transform(m_name.begin(), m_name.end(), m_name.begin(), ::tolower);
     }
    virtual ~ConfigVarBase(){};

    const std::string& getName() const {return m_name;}
    const std::string& getDescription() const {return m_description;}

    virtual std::string toString() = 0;
    virtual bool fromString(const std::string& val) = 0;
    virtual std::string getTypeName() const =0;

private:
    std::string m_name;
    std::string m_description;
};

/*
 *采用仿函数： 
 *FormStr T operator()(const std::string&)
 *ToStr std::string operator()(const T&) 
 */
template<class T, class FromStr = LexicalCast<std::string, T>
    ,class ToStr = LexicalCast<T, std::string> >
class ConfigVar : public ConfigVarBase
{
public:
    typedef std::shared_ptr<ConfigVar> ptr;
    typedef std::function<void (const T& old_value, const T& new_value)> on_change_cb;

    ConfigVar(const std::string& name
            ,const T& default_value
            ,const std::string& description = "")
        :ConfigVarBase(name, description)
         ,m_val(default_value){}

    std::string toString() override
    {
        try{
            return ToStr()(m_val);
        } catch(std::exception& e){
            ANGEL_LOG_ERROR(ANGEL_LOG_ROOT()) << "Configvar ::toString exception" << e.what() << "convert:" << TypeToName<T>() << "to string" << "name =" << "m_name";
        }
        return "";
    }

    bool fromString(const std::string& val) override
    {
        try{
            setValue(FromStr()(val));
        } catch(std::exception& e){
            ANGEL_LOG_ERROR(ANGEL_LOG_ROOT()) << "Configvar ::fromString exception" << e.what() << "converT: string to " << TypeToName<T>() << "to string" << "name = " << "m_name "<< "  -  " << val;
        }
        return false;
    }

    const T getValue() const {return m_val;}
    void setValue(const T& v)
    {
        if(v == m_val) {return;}
        for(auto& i : m_cbs) {i.second(m_val, v);}
        m_val = v;
    }

    std::string getTypeName() const override {return TypeToName<T>();}
    
    uint64_t addListener(on_change_cb cb)
    {
        static uint64_t s_fun_id = 0;
        ++s_fun_id;
        m_cbs[s_fun_id] = cb;
        return s_fun_id;
    }

    void delListener(uint64_t key)
    {
        m_cbs.erase(key);
    }

    on_change_cb getListener(uint64_t key)
    {
        auto it = m_cbs.find(key);
        return it = m_cbs.end() ? nullptr : it->second;
    }

    void clearListener()
    {
        m_cbs.clear();
    }

private:
    T m_val;
    std::map<uint64_t, on_change_cb> m_cbs;
};

class Config
{
public:
    typedef std::unordered_map<std::string, ConfigVarBase::ptr> ConfigVarMap;
    template<class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name, const T& default_value, const std::string& description = "")
    {
        auto it =GetDatas().find(name);
        if(it != GetDatas().end())
        {
            auto tmp = std::dynamic_pointer_cast<ConfigVar<T> >(it->second);
            if(tmp)
            {
                ANGEL_LOG_INFO(ANGEL_LOG_ROOT() ) << "LOOKUP NAME" << name << "exists";
                return tmp;
            }else{
                ANGEL_LOG_ERROR(ANGEL_LOG_ROOT()) << "Lookup name" << name << "exists but type not " << TypeToName<T>() << "real_type" << it->second->getTypeName() << "" << it->second->toString();
                return nullptr;
            }

        }
        if(name.find_first_not_of("abcdefghijklmnopqrstuvwxyz._012345678") != std::string::npos)
        {
            ANGEL_LOG_ERROR(ANGEL_LOG_ROOT()) << "lookup name invalid " << name;
            throw std::invalid_argument(name);
        }
        typename ConfigVar<T>::ptr v(new ConfigVar<T>(name, default_value, description));
        GetDatas()[name] = v;
        return v;
    }

    template <class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name)
    {
        auto it = GetDatas().find(name);
        if(it == GetDatas().end()){return nullptr;}
        return std::dynamic_pointer_cast<ConfigVar<T> >(it->second);
    }

    static void LoadFromYaml(const YAML::Node& root);
    static void LoadFromConDir(const std::string& path, bool force = false);
    static ConfigVarBase::ptr LookupBase(const std::string& name);
    static void Visit(std::function<void(ConfigVarBase::ptr)> cd);
private:
    static ConfigVarMap& GetDatas()
    {
        static ConfigVarMap s_datas;
        return s_datas;
    }

};

}


#endif
