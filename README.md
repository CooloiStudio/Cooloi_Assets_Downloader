# Cooloi_Assets_Downloader
Cooloi_Assets_Downloader base on Cocos2d-x 3.8.1

##配置说明
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
| --- | :-- |
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

P.S. 关于蛋疼的 VER 值

使用 cocos2d 内置的 AssetsManager 需要提供两个值，zip包的下载地址(必须为下载得到zip的url)，以及这个zip包的`版本信息`(即本项目中的VER)。AssetsManager 会对版本信息进行对比，当`版本信息`字符串不一致的时候(没有版本号先后或大小，因为只是单纯的`版本信息`)，AssetsManager 才会从 url 去下载zip。

为了让 AssetsManager 可以下载项目期望的每个包，无论它是否重复，这里推荐使用一个获取网络时间的url。

好吧我承认只是因为我用不来 cocos2d 内置的 Downloader。

##一个简化的工作流程流程示例

step1：

从 http://host:port/src_list.zip 获取最新的资源列表，本例中将获得src_list.zip 。

step2:

程序解压 src_list.zip，获得 src_list.json。程序会将之与 loc_list.json 做对比，找出需要更新的或者添加的资源。

step3:

更具资源列表提供的文件名称，从 SER 说明的服务地址下载资源，如本例中的 http://host:port/ ，假设最新的资源包名为：new201512312359.zip，程序将使用这个url下载资源  http://host:port/new201512312359.zip


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
