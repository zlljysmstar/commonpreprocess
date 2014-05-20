// author: Zhu, Liang. email: zhuliang2011@foxmail.com build-date: 2014-04-18

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;
#include "ul_ccode_string.h"
#include "common_core.h"
#include "formater.h"

CProcessUnify::CProcessUnify()
{
    m_Entity.clear();
    m_attr_filter_list.clear();
    m_value_filter_list.clear();
    m_scw_force_process_date_format = false;
    m_scw_force_process_value_split = false;
}

CProcessUnify::~CProcessUnify()
{
    m_Entity.clear();
    m_attr_filter_list.clear();
    m_value_filter_list.clear();
}

void CProcessUnify::PrintInformation(std::string attribute,std::string value)
{
    // Process Value - trim like (12) data
    boost::xpressive::sregex expression = boost::xpressive::sregex::compile("\\(\\d+\\)$");
    value = boost::xpressive::regex_replace(value,expression,"");
    
    m_Entity[1] = attribute;
    m_Entity[2] = value;
    
    for(unsigned int i=0;i<m_Entity.size()-1;++i)
        if(m_Entity.size() <= 9 || (m_Entity.size() > 9&&(i<3||(i>=3&&i<m_Entity.size()-6&&m_Entity[i]!="")||i>=m_Entity.size()-6)))
            cout<<m_Entity[i]<<"\t";
    cout<<m_Entity[m_Entity.size()-1]<<endl;
}

std::string CProcessUnify::part_replace(const std::string& value,boost::xpressive::sregex& expression,unsigned int idx_year,unsigned int idx_month,unsigned int idx_day,std::string join,bool flag)
{
    std::string rstValue = value;
    boost::xpressive::sregex_iterator pos(value.begin(),value.end(),expression);
    while(pos != boost::xpressive::sregex_iterator())
    {
        std::string ori_text = (*pos)[2] + join + (*pos)[3] + join + (*pos)[4];
        int year = atoi((*pos)[idx_year].str().c_str());
        if(flag)
        {
            if(year > 30)
                year += 1900;
            else
                year += 2000;
        }
        expression = boost::xpressive::sregex::compile(ori_text.c_str());
        char s[15];
        sprintf(s,"%04d-%02d-%02d",year,atoi((*pos)[idx_month].str().c_str()),atoi((*pos)[idx_day].str().c_str()));
        rstValue = boost::xpressive::regex_replace(rstValue,expression,s);
        ++pos;
    }

    return rstValue;
}

std::pair<int,std::vector<int> > CProcessUnify::GetCountPosInfo(std::string value,std::string sep)
{
    int nCount = 0;
    std::vector<int> posVector;
    posVector.clear();
    
    std::string::size_type pos = 0;

    while(pos < value.length() && (pos = value.find(sep,pos)) != std::string::npos)
    {
        ++nCount;
        posVector.push_back(pos);
        ++pos;
    }

    return make_pair(nCount,posVector);
}

bool CProcessUnify::split_process(std::string attribute,std::string value,std::string sep,int MinValue,bool flag)
{
    bool result = false;
    if(value.find(sep) != std::string::npos && GetCountPosInfo(value,sep).first >= MinValue)
    {
        // 判断单TERM平均长度
        if(sep != ";" && sep != "；" && sep != "。")
        {
            double avg_length = cal_avg_length(value,sep);
            if(avg_length > SINGLE_TERM_MAX_LENGTH)
                return result;
        }
    
        // 对需要进行枚举类型属性值拆分的条目，判断是否需要保留原始的记录信息
        if(m_scw_reserve_original_item_info)
        {
            PrintInformation(m_Entity[1],m_Entity[2]);    
        }

        boost::xpressive::sregex split_reg = boost::xpressive::sregex::compile(sep);
        boost::xpressive::sregex_token_iterator pos(value.begin(),value.end(),split_reg,-1);
        
        bool isCat = false; // 记录是否需要进行拆分段拼接
        std::string remain_value = "";
        
        while(pos != boost::xpressive::sregex_token_iterator())
        {
            std::string strTemp = *pos;
            boost::xpressive::sregex regex1 = boost::xpressive::sregex::compile("^[[:space:]]*");
            boost::xpressive::sregex regex2 = boost::xpressive::sregex::compile("[[:space:]]*$");
            strTemp = boost::xpressive::regex_replace(strTemp,regex1,"");
            strTemp = boost::xpressive::regex_replace(strTemp,regex2,"");
            if(flag)
            {
                regex1 = boost::xpressive::sregex::compile("\\(.*?\\)|（.*?）");
                if(boost::xpressive::regex_search(strTemp,regex1))
                    PrintInformation(attribute,strTemp);
                else
                {
                    regex1 = boost::xpressive::sregex::compile("\\([^(]*$|（[^）]*$");
                    regex2 = boost::xpressive::sregex::compile("^[^(]*\\)|^[^（]*）");
                    if(boost::xpressive::regex_search(strTemp,regex1))
                    {
                        isCat = true; 
                        remain_value.clear();
                        remain_value = strTemp;
                    }
                    else if(boost::xpressive::regex_search(strTemp,regex2))
                    {
                        isCat = false;
                        remain_value = remain_value + sep + strTemp;
                        PrintInformation(attribute,remain_value);
                        remain_value.clear();
                    }
                    else
                    {
                        if(isCat)
                        {
                            remain_value = remain_value + sep + strTemp;
                        }
                        else
                            PrintInformation(attribute,strTemp);
                    }
                }
            }
            else
                PrintInformation(attribute,strTemp);
            ++pos;
        }
        if(!remain_value.empty())
            PrintInformation(attribute,remain_value);
        
        result = true;
    }

    return result;
}

// 计算以sep作为分隔符时，被拆分的子串的平均长度
double CProcessUnify::cal_avg_length(std::string value,std::string sep)
{
    unsigned int nLength = 0;
    unsigned int nCount = 0;
    boost::xpressive::sregex split_reg = boost::xpressive::sregex::compile(sep);
    boost::xpressive::sregex_token_iterator pos(value.begin(),value.end(),split_reg,-1);
    while(pos != boost::xpressive::sregex_token_iterator())
    {
        std::string tmpString = *pos;
        nLength += tmpString.length();
        ++nCount;
        ++pos;
    }

    return static_cast<double>(nLength)/nCount;
}

bool CProcessUnify::LoadData()
{
    std::string strLine = "";
    
    bool isSuccess = false;
    while(getline(cin,strLine))
    {
        isSuccess = true;
        m_Entity.clear();
        boost::split(m_Entity,strLine,boost::is_any_of("\t"),boost::token_compress_on);

        if(m_Entity.size() < 3)
        {
            isSuccess = false;
            break;
        }

        if(m_Entity.size() > 9)
        {
            for(unsigned int i=3;i<m_Entity.size()-6;++i)
            {
                m_Entity[2] += m_Entity[i];
                m_Entity[i] = "";
            }
        }

        // 预先处理判断
        if(m_isLoadRules)
        {
            process_unify_by_rules();
        }
       
        // 日期处理判断
        if(m_scw_force_process_date_format)
        {
            m_Entity[2] = process_date(m_Entity[1],m_Entity[2]);
        }

        // 属性值拆分判断
        if(m_scw_force_process_value_split)
        {
            process_unify(m_Entity[1],m_Entity[2]);
        }
        else
        {
            PrintInformation(m_Entity[1],m_Entity[2]);
        }
    }

    if(!isSuccess)
        return false;

    return true;
}

bool CProcessUnify::LoadUserDict(std::string dict_path)
{
    namespace fs = boost::filesystem;
    namespace io = boost::iostreams;
    if(!fs::exists(dict_path + "/attr.fl.list")||!fs::exists(dict_path + "/value.fl.list")){
        std::cerr<<"[ERROR.02].ATTR.FL.LIST OR VALUE.FL.LIST FILE NOT FOUND! PLEASE CHECK!"<<std::endl;
        return false;
    }
    io::stream_buffer<io::file_source> buf_attr(dict_path + "/attr.fl.list");
    io::stream_buffer<io::file_source> buf_value(dict_path + "/value.fl.list");

    std::istream in_attr(&buf_attr);
    std::istream in_value(&buf_value);
    
    if(!in_attr||!in_value)
        return false;

    std::string strLine = "";
    while(getline(in_attr,strLine))
    {
        if(strLine != "")
            m_attr_filter_list.push_back(strLine);
    }

    strLine = "";
    while(getline(in_value,strLine))
    {
        if(strLine != "")
            m_value_filter_list.push_back(strLine);
    }

    return true;
}

bool CProcessUnify::LoadScwRules(std::string dict_path)
{
    std::string scw_conf_path = dict_path + "/scw.conf";
    std::string scw_rules_conf_path = dict_path + "/scw_rules.conf";

    namespace fs = boost::filesystem;
    if(!fs::exists(scw_conf_path)||!fs::exists(scw_rules_conf_path)){
        std::cerr<<"[ERROR.05].SCW OR SCW RULES CONF FILE NOT FOUND! PLEASE CHECK!"<<std::endl;
        return false;
    }
    
    // Load Rules Using boost::property_tree Library
    read_ini(scw_conf_path,pt_ini);
    
    // 加载全局配置参数
    std::string select_cs = pt_ini.get<string>("GLB.SELECT_CONFIG_SECTION");

    // 根据需求，判断是否需要进行预先加载规则文件信息
    m_isLoadRules = pt_ini.get<bool>(select_cs+".SCW_FORCE_FILTER_TEMPLATE",false);
    m_scw_force_process_date_format = pt_ini.get<bool>(select_cs+".SCW_FORCE_PROCESS_DATE_FORMAT",false);
    m_scw_force_process_value_split = pt_ini.get<bool>(select_cs+".SCW_FORCE_PROCESS_VALUE_SPLIT",false);
    MaxSegLength = pt_ini.get<int>(select_cs+".SCW_VARS_MAX_SEGMENT_LENGTH",500);
    SINGLE_TERM_MAX_LENGTH = pt_ini.get<double>(select_cs+".SCW_VARS_SINGLE_TERM_MAX_LENGTH",20);
    m_scw_reserve_original_item_info = pt_ini.get<bool>(select_cs+".SCW_RESERVE_ORIGINAL_ITEM_INFO",false);
   
    if(m_isLoadRules)
        read_info(scw_rules_conf_path,pt);

    return true;
}

std::string CProcessUnify::process_date(std::string attribute,std::string value)
{
    if(attribute.find("网站") != std::string::npos || attribute.find("三围") != std::string::npos)
        return value;

    // 1992-5-1
    boost::xpressive::sregex expression = boost::xpressive::sregex::compile("(\\d{4})-(\\d{1,2})-(\\d{1,2})");
    value = boost::xpressive::regex_replace(value,expression,formater(1,2,3));

    // 1992/5/1
    expression = boost::xpressive::sregex::compile("(\\d{4})/(\\d{1,2})/(\\d{1,2})");
    value = boost::xpressive::regex_replace(value,expression,formater(1,2,3));

    // 5/1/1988
    expression = boost::xpressive::sregex::compile("(\\d{1,2})/(\\d{1,2})/(\\d{4})");
    value = boost::xpressive::regex_replace(value,expression,formater(3,1,2));

    // 12/24/98
    expression = boost::xpressive::sregex::compile("(^|\\D)(\\d{1,2})/(\\d{1,2})/(\\d{2})(\\D|$)");
    value = part_replace(value,expression,4,2,3,"/",true);

    // 99-9-1
    expression = boost::xpressive::sregex::compile("(^|\\D)(\\d{2})-(\\d{1,2})-(\\d{1,2})(\\D|$)");
    value = part_replace(value,expression,2,3,4,"-",true);

    // 1922年5月22日
    expression = boost::xpressive::sregex::compile("(\\d{4})年(\\d{1,2})月(\\d{1,2})日");
    value = boost::xpressive::regex_replace(value,expression,formater(1,2,3));

    // 1922年5月
    expression = boost::xpressive::sregex::compile("(\\d{4})年(\\d{1,2})月");
    value = boost::xpressive::regex_replace(value,expression,formater(1,2,INVALID_INDEX,false,YEAR_MONTH));

    // 5月4日
    expression = boost::xpressive::sregex::compile("(\\d{1,2})月(\\d{1,2})日");
    value = boost::xpressive::regex_replace(value,expression,formater(INVALID_INDEX,1,2,false,MONTH_DAY));

    return value;
}

void CProcessUnify::process_unify(std::string attribute,std::string value)
{       
    // 属性名包含"详细"或者属性值字节总长超过阈值，则该记录为特长文本，不予处理
    if(value.length() > MaxSegLength)
    {
        PrintInformation(attribute,value);
        return;
    }

    std::vector<std::string>::const_iterator pos = m_attr_filter_list.begin();
    bool flag = true;
    for(;pos != m_attr_filter_list.end();++pos)
    {
        if(attribute.find(*pos) != std::string::npos)
        {
            flag = false;
            break;
        }
    }
    if(!flag)
    {
        PrintInformation(attribute,value);
        return;
    }
    else
    {
        pos = m_value_filter_list.begin();
        flag = true;
        for(;pos != m_value_filter_list.end();++pos)
        {
            if(value.find(*pos) != std::string::npos)
            {
                flag = false;
                break;
            }
        }
        if(!flag)
        {
            PrintInformation(attribute,value);
            return;
        }
    }
    
    // 判断属性值是否为URL，如果是URL，则不做处理
    boost::xpressive::sregex url_reg = boost::xpressive::sregex::compile("http://.*?$");
    if(boost::xpressive::regex_search(value,url_reg))
    {
        PrintInformation(attribute,value);
        return;
    }

    // 语义判断
    boost::xpressive::sregex semantic_reg = boost::xpressive::sregex::compile("以.*?(\\d/\\d,)+");
    if(boost::xpressive::regex_search(value,semantic_reg))
    {
        PrintInformation(attribute,value);
        return;
    }

    // 判断是否为复杂描述
    int count1 = GetCountPosInfo(value,"。").first;
    int count2 = GetCountPosInfo(value,";").first;
    int count3 = GetCountPosInfo(value,"；").first;
    int count4 = GetCountPosInfo(value,",").first;
    int count5 = GetCountPosInfo(value,"，").first;
    if(count1>1&&(count2>1||count3>1)&&(count4>1||count5>1))
    {
        PrintInformation(attribute,value);
        return;
    }

    // 句号处理 - 优先级别：1（最高）
    if(split_process(attribute,value,"。",3,false))
        return;
    // 分号处理 - 优先级别：2
    if(GetCountPosInfo(value,",").first > 0 || GetCountPosInfo(value,"，").first > 0 || GetCountPosInfo(value,":").first > 0 || GetCountPosInfo(value,"：").first > 0)
    {
        boost::xpressive::sregex special_reg = boost::xpressive::sregex::compile("&.{3,6};");
        if(!boost::xpressive::regex_search(value,special_reg))
            if(split_process(attribute,value,";",1,false))
                return;
        if(split_process(attribute,value,"；",1,false))
            return;
    }
    // 逗号处理 - 优先级别：3
    if(split_process(attribute,value,",",1,true))
        return;
    if(split_process(attribute,value,"，",1,true))
        return;
    // 顿号处理 - 优先级别：4
    if(split_process(attribute,value,"、",1,true))
        return;
    // "/"处理 - 优先级别：5
    if(split_process(attribute,value,"/",1,true))
        return;
    // 空格处理 - 只在全中文环境下有效
    std::string tmpValue = value;
    boost::xpressive::sregex blank_reg = boost::xpressive::sregex::compile("[[:space:]]*");
    tmpValue = boost::xpressive::regex_replace(tmpValue,blank_reg,"");
    boost::xpressive::sregex sl_reg = boost::xpressive::sregex::compile("-");
    tmpValue = boost::xpressive::regex_replace(tmpValue,sl_reg,"");
    if(ul_ccode_pure_lang(tmpValue.c_str(),tmpValue.length(),1,3))
    {
        if(split_process(attribute,value," ",1,false))
            return;
    }

    PrintInformation(attribute,value);
}

void CProcessUnify::process_unify_by_rules()
{
    std::string select_rs = pt_ini.get<string>("GLB.SELECT_RULES_SECTION","R1");
    BOOST_AUTO(child, pt.get_child(select_rs));

    for(BOOST_AUTO(pos,child.begin());pos != child.end(); ++pos)
    {
        std::string item = pos->second.get_child("_1").data();
        std::string src = pos->second.get_child("_3").data();
        std::string obj = pos->second.get_child("_4").data();
        if(item != "GLB" && item != m_Entity[1])
            continue;
        int nIndex = boost::lexical_cast<int>(pos->second.get_child("_2").data());
        if(nIndex >= m_Entity.size())
            continue;
        // 判断源模板数据来源
        std::string pat_src = "";
        boost::xpressive::sregex pat_judge = boost::xpressive::sregex::compile("###\\d+?###");
        if(boost::xpressive::regex_match(src,pat_judge))
        {
            int nIdx = atoi(src.substr(3,src.length()-6).c_str());
            if(nIdx >= m_Entity.size())
                continue;
            pat_src = m_Entity[nIdx];
        }
        else
        {
            pat_src = src;
        }
        // 判断目标模板数据来源
        std::string pat_desc = "";
        boost::xpressive::sregex reg_src = boost::xpressive::sregex::compile(pat_src.c_str());
        std::string::const_iterator start = m_Entity[nIndex].begin();
        std::string::const_iterator end = m_Entity[nIndex].end();
        if(boost::xpressive::regex_match(obj,pat_judge))
        {
            int nIdx = atoi(obj.substr(3,obj.length()-6).c_str());
            boost::xpressive::smatch match;
            boost::xpressive::regex_search(m_Entity[nIndex],match,reg_src);
            // 如何指定组索引超出范围，则忽略该条规则，继续处理下一条
            if(nIdx >= match.size())
                continue;
            pat_desc = match[nIdx];
        }
        else
        {
            pat_desc = obj;
        }
        // 进行全局替换
        m_Entity[nIndex] = boost::xpressive::regex_replace(m_Entity[nIndex],reg_src,pat_desc);
    }
}

bool CProcessUnify::run(std::string dict_path)
{
    if(!LoadScwRules(dict_path)){
        std::cerr<<"[ERROR.03].LOAD SCW RULES FAILED! PLEASE CHECK!"<<std::endl;
        return false;
    }

    if(!LoadUserDict(dict_path))
        return false;

    if(!LoadData()){
        std::cerr<<"[ERROR.04].INPUT DATA FORMAT ERROR! PLEASE CHECK!"<<std::endl;
        return false;
    }

    return true;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 number: */

