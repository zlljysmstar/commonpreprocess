// author: Zhu, Liang. email: zhuliang2011@foxmail.com build-date: 2014-05-05

#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/xpressive/xpressive_dynamic.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/filesystem.hpp>

class CProcessUnify
{
private:
    bool m_isLoadRules; // 标志：用于记录是否需要加载人工规则信息
    bool m_scw_force_process_date_format;   // 标志：用于记录是否需要处理日期格式归一化，默认值为FALSE
    bool m_scw_force_process_value_split;   // 标志：用于记录是否需要对枚举类型属性值进行拆分处理，默认值为FALSE
    bool m_scw_reserve_original_item_info;  // 标志：用于判断是否需要保留枚举类型属性值拆分前的原始条目信息
    unsigned int MaxSegLength;
    double SINGLE_TERM_MAX_LENGTH;
private:
    boost::property_tree::ptree pt_ini;
    boost::property_tree::ptree pt;
    // 记录实体单条记录
    std::vector<std::string> m_Entity;
    // 保存属性名处理白名单
    std::vector<std::string> m_attr_filter_list;
    // 保存属性值处理白名单
    std::vector<std::string> m_value_filter_list;
private:
    // 格式化输出/打印相关信息
    void PrintInformation(std::string attribute,std::string value);
    // 部分替换
    std::string part_replace(const std::string& value,boost::xpressive::sregex& expression,unsigned int idx_year,unsigned int idx_month,unsigned int idx_day,std::string join,bool flag = false);
    // 取得分隔符数目/位置信息，用于枚举类型属性值拆分时作为参考标准
    std::pair<int,std::vector<int> > GetCountPosInfo(std::string value,std::string sep);
    // 主拆分处理过程（拆分处理主执行流）
    bool split_process(std::string attribute,std::string value,std::string sep,int MinValue,bool flag);
    // 计算平均子段长度
    double cal_avg_length(std::string value,std::string sep);
    // 加载输入数据，用于后续处理
    bool LoadData();
    // 加载用户配置文件，用于过滤属性名与属性值无须处理情况
    bool LoadUserDict(std::string dict_path);
    // 加载SCW配置文件，用于预处理数据
    bool LoadScwRules(std::string dict_path);
    // 依据人工指定规则，进行数据清洗
    void process_unify_by_rules(); // 函数未包含任何参数，因并未明确指定一定要处理属性名或属性值信息
    // 日期与枚举类型属性值的拆分
    std::string process_date(std::string attribute,std::string value);    // 处理日期格式
    void process_unify(std::string attribute,std::string value);   // 处理属性值的拆分
public:
    CProcessUnify();
    ~CProcessUnify();
    bool run(std::string dict_path);
};

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 number: */
