# Cooloi_Assets_Downloader
Cooloi_Assets_Downloader base on Cocos2d-x 3.8.1

需要给项目配置json文件
样例如下

Cooloi_ASDL.json
```
{
  "URL" : "http://host:port/src_list.zip",
  "VER" : "http://turanga.deskxd.com/thanks/pubdate/",
  "DIR" : "download",
  "TRY" : "10",
  "NAME" : "src_list.json",
  "LOCAL_NAME" : "loc_list.json",
  "SER" : "http://host:port/"
}
```
|key|explain|
|URL|包含资源列表json的文件下载地址|
|VER|cocos2d引擎要求的版本号，在这个项目中一切都是必要下载，所以推荐使用时间戳|
|DIR|本地存放文件夹名称|
|TRY|下载失败后重试的次数|
|NAME|资源列表json的文件名|
|LOCAL_NAME|已经下载到本地的资源的列表|
|SER|资源服务下载地址或前缀|

还需要配置debug模式使用的json

Cooloi_ASDL_DEBUG.json

内容与 Cooloi_ASDL.json 相同

该文件将在debug模式中生效(Xcode中需要设置COCOS_DEBUG=1; eclipse暂不生效)

<!--Cooloi_ASDL.conf-->
<!---->
<!--```-->
<!--# url for Release-->
<!--URL = http://example.com:port/SRC_LIST.zip-->
<!---->
<!--# ver for check-->
<!--VER = http://turanga.deskxd.com/thanks/pubdate/-->
<!---->
<!--# directory's name-->
<!--DIR = download-->
<!---->
<!--# max retry-->
<!--TRY = 10-->
<!---->
<!--# file name to check-->
<!--NAME = SRC_LIST-->
<!---->
<!--# local list file name-->
<!--LOCAL_NAME = LOC_LIST-->
<!---->
<!--# server host with port-->
<!--SER = http://example.com:port/-->
<!--```-->
<!---->
<!--Cooloi_ASDL_DEBUG.conf-->
<!---->
<!--```-->
<!--# url for debug-->
<!--URL = http://example.com:port/SRC_LIST.zip-->
<!---->
<!--# ver for check-->
<!--VER = http://turanga.deskxd.com/thanks/pubdate/-->
<!---->
<!--# directory's name-->
<!--DIR = download-->
<!---->
<!--# max retry-->
<!--TRY = 10-->
<!---->
<!--# file name to check-->
<!--NAME = SRC_LIST-->
<!---->
<!--# local list file name-->
<!--LOCAL_NAME = LOC_LIST-->
<!---->
<!--# server host with port-->
<!--SER = http://example.com:port/-->
<!--```-->
