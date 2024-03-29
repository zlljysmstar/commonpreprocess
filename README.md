<h2>项目简介</h2>

由于直接从网页HTML中获取得到的结构化三元组实体信息存在噪音等不可预防的bug, 数据质量很难从源头得以保证。

为了确保数据质量，同时减少归一化过程中的工作量，特开发本系统。

该系统以完全基于模板/规则化的方式统一处理多种类别数据信息，使得数据归一化过程中的针对不同类别的数据只需要稍加删改规则即可。

同时，本系统完美支持多种预设置的规则，在系统自动识别并判断噪音的效果未达到理想情况时，可以人工指定特殊的过滤与修改规则，最终，以多种有效方式确保了数据质量，使数据质量尽可能达到可以使用的标准。

发布日期： 2013-07-29

开发语言: C++

第三方依赖：BOOST 1.50.0 / ccode 3.5.2 

<h2>功能说明</h2>

(一) 执行流程 [说明：各阶段是否需要进行，通过上节第一个配置文件指定]

[1] 采用统一的预先定义的规范来处理数据，数据预处理阶段<br/>
[2] 自主学习全文信息，判断子段平均长度等，保存记录，为后续的枚举类型属性值拆分做准备<br/>
[3] 枚举类型属性值预拆分<br/>
[4] 通过人工规则对拆分做进一步修正<br/>
[5] 日期处理，统一格式为：YYYY-MM-DD<br/>
[6] 噪音处理，数据清洗，采用统一规则进行<br/>
[7] 噪音处理，数据清洗，采用人工指定规则进行<br/>
[8] 输出结果

(二) 枚举类型属性值拆分相关说明

[1] 拆分最后阶段，通过对不同种语言类型的组合进行不同的处理，进行后续修正，减少错误率。目前已经支持的语言组合：纯中文，纯英文，中文-英文，中文-韩文，中文-日文，中文-阿拉伯文，中文-西班牙语，中文-越南语，以及上述不同小语种的组合。

<h2>下一版本预计新增功能</h2>

[1] 将属性名归一化加入与本系统合并，构建统一的属性名归一化词表。<br/>
[2] ......

<h2>升级日志</h2>

Feature&nbsp;&nbsp;BugFix&nbsp;&nbsp;Warning&nbsp;&nbsp;Change

2014-05-05

[1] 采用Boost Library重构了部分代码，更改了配置文件的读取格式。

2013-08-06

New Feature:

[1] 配置文件新增关键字，支持人为指定枚举类型属性值拆分时是否保留原有条目信息

2013-07-29

New Feature:

[1] 新增加规则文件配置节指定关键字, 可以在目标模板中指定源模板正则组别号.

BugFix:

[1] 修正了部分情况下替换目标模板失效的问题.

2013-07-26

New Feature:

[1] 新增枚举类型属性值拆分模块人工配置属性名白名单与属性值白名单功能.<br/>
[2] 新增配置文件scw.conf, scw_rules.conf, 用于对数据清洗与归一化做人工规则指定.

BugFix:

[1] 修正了前一版本中存在对包含有特殊符号时, 纯中文枚举类型属性值拆分错误或无法拆分问题.

