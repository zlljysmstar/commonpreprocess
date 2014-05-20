项目简介

由于直接从网页HTML中获取得到的结构化三元组实体信息存在噪音等不可预防的bug, 数据质量很难从源头得以保证。

为了确保数据质量，同时减少归一化过程中的工作量，特开发本系统。

该系统以完全基于模板/规则化的方式统一处理多种类别数据信息，使得数据归一化过程中的针对不同类别的数据只需要稍加删改规则即可。

同时，本系统完美支持多种预设置的规则，在系统自动识别并判断噪音的效果未达到理想情况时，可以人工指定特殊的过滤与修改规则，最终，以多种有效方式确保了数据质量，使数据质量尽可能达到可以使用的标准。
版本信息

发布日期： 2013-07-29

开发语言: C++

第三方依赖：BOOST 1.50.0 / ccode 3.5.2 

配置文件相关说明（具体请参考源代码中的配置文件）

功能说明

1. 执行流程 [说明：各阶段是否需要进行，通过上节第一个配置文件指定]

[1] 采用统一的预先定义的规范来处理数据，数据预处理阶段
[2] 自主学习全文信息，判断子段平均长度等，保存记录，为后续的枚举类型属性值拆分做准备
[3] 枚举类型属性值预拆分
[4] 通过人工规则对拆分做进一步修正
[5] 日期处理，统一格式为：YYYY-MM-DD
[6] 噪音处理，数据清洗，采用统一规则进行
[7] 噪音处理，数据清洗，采用人工指定规则进行
[8] 输出结果

2. 枚举类型属性值拆分相关说明

[1] 拆分最后阶段，通过对不同种语言类型的组合进行不同的处理，进行后续修正，减少错误率。
    目前已经支持的语言组合：纯中文，纯英文，中文-英文，中文-韩文，中文-日文，中文-阿拉伯文，中文-西班牙语，中文-越南语，以及上述不同小语种的组合。
下一版本预计新增功能

[1] 将属性名归一化加入与本系统合并，构建统一的属性名归一化词表。<br/>
[2] ......<br/>
测试用例

[1] 输入文件 [原始数据]

1	baidu	1992-5-1 1993-5-1
2	baidu	1992/5/1
3	baidu	5/1/1988
4	baidu	abc12/24/98 abc1/12/99
5	baidu	abc99-9-1
6	baidu	1922年5月22日
7	baidu	1922年5月
8	baidu	5月4日
9	baidu	http://www.baidu.com
10	baidu	吉林 大学
11	baidu	a 吉林
12	baidu	吉林、a
13	baidu	&11111;&amp;abc
14	官方网站	吉林、a
15	官网	吉林、a
16	描述	吉林 大学
17	baidu	@@@吉林 大学
18	baidu	计算机 科学与技术
19	baidu	亚历山大 / John Smith / 大仲马
20:	baidu	亚历山大 / John-Smith / 大仲马
21(2100)	baidu	亚历山大 小说-都市言情 大仲马
22(2200)	作者	亚历山大: 小说-都市言情: 大仲马:
23	作者	亚历山大 (小说-都市言情) 大仲马
24	author	亚历山大 (小说-都市言情) 大仲马
25	25author	亚历山大 (小说-都市言情) 大仲马
26	Test1	亚历山大_小说-都市言情_大仲马
27	Test2	亚历山大_小说-都市言情_大仲马

[2] 输出结果 [无枚举类型属性值拆分]

1	baidu	1992-05-01 1993-05-01
2	baidu	1992-05-01
3	baidu	1988-05-01
4	baidu	abc1998-12-24 abc1999-01-12
5	baidu	abc1999-09-01
6	baidu	1922-05-22
7	baidu	1922-05
8	baidu	05-04
9	baidu	http://www.baidu.com
10	baidu	吉林 大学
11	baidu	a 吉林
12	baidu	吉林、a
13	baidu	&11111;&amp;abc
14	官方网站	吉林、a
15	官网	吉林、a
16	描述	吉林 大学
17	baidu	@@@吉林 大学
18	baidu	计算机 科学与技术
19	baidu	亚历山大 / John Smith / 大仲马
20	baidu	亚历山大 / John-Smith / 大仲马
21	baidu	亚历山大 小说-都市言情 大仲马
22	作者	亚历山大 小说-都市言情 大仲马
23	作者	亚历山大  大仲马
24	author	亚历山大 (小说-都市言情) 大仲马
25	author	亚历山大 (小说-都市言情) 大仲马
26	Test1	小说-都市言情
27	Test2	亚历山大123456大仲马

[3] 输出结果 [启用枚举类型属性值拆分模块]

1	baidu	1992-05-01 1993-05-01
2	baidu	1992-05-01
3	baidu	1988-05-01
4	baidu	abc1998-12-24 abc1999-01-12
5	baidu	abc1999-09-01
6	baidu	1922-05-22
7	baidu	1922-05
8	baidu	05-04
9	baidu	http://www.baidu.com
10	baidu	吉林
10	baidu	大学
11	baidu	a 吉林
12	baidu	吉林
12	baidu	a
13	baidu	&11111;&amp;abc
14	官方网站	吉林、a
15	官网	吉林、a
16	描述	吉林 大学
17	baidu	@@@吉林 大学
18	baidu	计算机
18	baidu	科学与技术
19	baidu	亚历山大
19	baidu	John Smith
19	baidu	大仲马
20	baidu	亚历山大
20	baidu	John-Smith
20	baidu	大仲马
21	baidu	亚历山大
21	baidu	小说-都市言情
21	baidu	大仲马
22	作者	亚历山大
22	作者	小说-都市言情
22	作者	大仲马
23	作者	亚历山大
23	作者	
23	作者	大仲马
24	author	亚历山大 (小说-都市言情) 大仲马
25	author	亚历山大 (小说-都市言情) 大仲马
26	Test1	小说-都市言情
27	Test2	亚历山大123456大仲马
升级日志

Feature        BugFix        Warning        Change

2014-05-05

[1] 采用Boost Library重构了部分代码，更改了配置文件读取格式。

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

[1] 新增枚举类型属性值拆分模块人工配置属性名白名单与属性值白名单功能.
[2] 新增配置文件scw.conf, scw_rules.conf, 用于对数据清洗与归一化做人工规则指定.

BugFix:

[1] 修正了前一版本中存在对包含有特殊符号时, 纯中文枚举类型属性值拆分错误或无法拆分问题.

