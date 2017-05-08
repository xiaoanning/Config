#pragma mark -  Xcode Target

target 定义了生成的唯一 product, 它将构建该 product 所需的文件和处理这些文件所需的指令集整合进 build system 中。Projects 会包含一个或者多个 targets,每一个 target 将会产出一个 product.

这些指令以 build setting 和 build phases 的形式存在，你可在 Xcode 的项目编辑器(TARGETS->Build Setting, TARGETS->Build Phases)中进行查看和编辑。target 中的 build setting 参数继承自 project 的 build settings, 但是你可以在 target 中修改任意 settings 来重写 project settings，这样，最终生效的 settings 参数以在 target 中设置的为准. Project 可包含多个 target, 但是在同一时刻，只会有一个 target 生效，可用 Xcode 的 scheme 来指定是哪一个 target 生效.


target 和其生成的 product 可与另一个 target 有关，如果一个 target 的 build 依赖于另一个 target 的输出，那么我们就说前一个 target 依赖于后一个 target .如果这些 target 在同一个 workspace 中，那么 Xcode 能够发现这种依赖关系，从而使其以我们期望的顺序生成 products.这种关系被称为隐式依赖关系。同时，你可以显示指定 targets 之间的依赖关系，并且这种依赖关系会覆盖 Xcode 推测出的隐式依赖关系。

指定 targets 之间的依赖关系的地方在 Project Editor->TRAGETS->Build Phases->Target Dependencies 处设置


#pragma mark -  Xcode Project

Xcode project 是一个仓库，该仓库包含了所有的文件，资源和用于生成一个或者多个 software products 的信息。它包含一个或者多个 targets，其中的每一个 target 指明了如何生成 products。project 为其拥有的所有 targets 定义了默认的 build settings，当然，每一个 target 能够制定其自己的 build settings，且 target 的 build settings 会重写 project 的 build settings。

Xcode project 文件包含以下信息:

源文件的引用:
源码，包括头文件和实现文件
内部和外部的库或者框架
资源文件
图片文件
Interface Builder(nib)文件
文件结构导航中用来组织源文件的组
Project-level build configurations.你可以为 project 指定多个 build configuration，例如，project 中默认包含 debug 和 release 两种 build settings.
Targets, 每一个 target 指定了：
project 生成的 product
生成 product 所需的源文件
生成 product 所需的配置文件，包括对其他 targets 的依赖以及一些其他设置；当 targets 的 build configurations 没有重写 project-level 的 build settings 时，会直接使用 project-level 的 build setting.
可执行环境，该环境用于调试或者测试程序，每个可执行环境会指定：
运行或者调试程序时加载的可执行程序
传递给可执行程序的命令行参数
运行程序时需设置的环境变量
project 可独立存在，也可被包含在 workspace 中。

#pragma mark -  Build Setting 的继承关系


build setting 中包含了 product 生成过程中所需的参数信息。你可以在 project-level 和 target-level 层指定 build settings。project-level 的 build settings 适用于 project 中的所有targets，但是当 target-level 的 build settings 重写了 project-level 的 build settings，以 target-level 中的 build settings 中的值为准。


一个 build configaration 指定了一套 build settings 用于生成某一 target 的 product，例如，在 Xcode 创建项目时默认就有两套独立的 build configarations, 分别用于生成 debug 和 release 模式下的 product


除了创建工程时生成的默认 build settings，你也可以自定义 project-level 或者 target-level 的 build settings.

关于继承关系，The Unofficial Guide to xcconfig files 这里也有详细的说明，强烈建议阅读
https://pewpewthespells.com/blog/xcconfig_guide.html#BuildSettingInheritance

#pragma mark -  使用 xcconfig 文件来配置不同开发阶段的环境

可新建一种 build configuration, 由 Xcode 自动生成的 Release 复制而来

项目 ===> project ===> info ===> configurations

并命名为 PreRelease

新建了 Debug.xcconfig/PreRelease.xcconfig/Release.xcconfig 配置对应于开发阶段、预发版阶段、上传 AppStore 三种情况下的打包。

根据项目情况，每个配置文件中都包含同样的 key 值，内容大致如下:


PRODUCT_NAME = testing
PRODUCT_NAME_ORIGINAL = $(PRODUCT_NAME)
FOO_MyApp = MyAppsName
FOO_testing = MyAppsNewName
BAR = $(FOO_$(PRODUCT_NAME))

//网络请求baseurl
WEBSERVICE_URL = @"http:\/\/127.0.0.1"

xturl = @"sdf"$(WEBSERVICE_URL)

//友盟配置
UMENG_APPKEY = @"xxxvvv555999=="

//极光推送配置
JPUSH_DEVELOPMENT_APPKEY = @"nnncccvvvwww"
IS_PRODUCATION = NO

#include "Generator.xcconfig"


可在配置文件中包含其他配置文件，其中 Generator.xcconfig 文件的内容是:

GCC_PREPROCESSOR_DEFINITIONS = $(inherited) WEBSERVICE_URL='$(WEBSERVICE_URL)' MESSAGE_SYSTEM_URL='$(MESSAGE_SYSTEM_URL)' UMENG_APPKEY='$(UMENG_APPKEY)'  IS_PRODUCATION='$(IS_PRODUCATION)'

其作用是将配置文件中定义的常量定义成预编译宏，以便于在代码中获取

GCC_PREPROCESSOR_DEFINITIONS 是 GCC 预编译头参数，通常我们可以在 Project 文件下的 Build Settings 对预编译宏定义进行默认赋值。在 Xcode7 下的路径为 Build Settings->Apple LLVM 7.x Preprocessing->Preprocessor Macros

想必大家看这个宏的名字已经知道它的作用了, 使用上和在 pch 头文件中添加宏定义没有太大的区别, 但有以下好处:

Xcode 的 Project 的 Build Settings 是由一个 plist 文件进行描述的, plist 本质上是一个 XML 配置文件, 通过外部的脚本比较容易去修改。
Preprocessor Macros 可以按照 Configuration 选项进行默认配置, 也就是说可以根据不同的环境预先制定不同定义的宏，或者为不同环境下的相同变量定义不同的值
xcconfig 支持可以根据不同的 Configuration 选项配置不同的文件。不同的 xcconfig 可以指定不同的 Build Settings 里的属性值, 这样子我们就可以通过项目 xcconfig 去修改 GCC_PREPROCESSOR_DEFINITIONS 的值了(最终目的就达到了)。

配置文件中变量定义好之后，怎么让 Xcode 自动加载呢？如下图设置所示，是将 project-level 的 build settings 基于配置文件，三种情况的 (project ==> info==>)configurations 分别选择与之对应的配置文件。


当我们想把 project-level 或者 target-level 中的 Build Settings 的设置挪动到 xcconfig 配置文件来设置时，是否需要一个个手动输入呢？当然不是，直接在 Build Settings 中选中你想要在 xcconfig 中配置的键值对所在行（当然也可以选多行），command + c复制，然后到对应的 xcconfig 中去粘贴就好了，记得在 Build Settings 中改为你想要的值后再复制，如果为默认值的话则只可复制其键。如果需要改回去的话，还是选中这行，command + delete 就恢复默认值了。

现在我们将设置挪动到了配置文件中，所有的配置文件都是键值对类型的文本文件，但是当同一个键同时存在于 target-level、project-level 和配置文件中时，到底是哪一个键值对起作用了呢？

注意: Xcode以从左至右的顺序设置解析的优先级，从左至右优先级降低，最左边的具有最高优先级，即 target-level > project-level > 自定义配置文件 > iOS 默认配置；且最左列 Resolved 列显示的是最终使用的值。那么如何使 Xcode 使用配置文件中的配置项呢？这需要选中要使用配置文件的行，点击 Delete 按键，你会发现项目的默认设置已经被删除，且 xcconfig 的配置文件列被标记为绿色。标记为绿色代表该列的值生效，其值应该与 Resolved 列的值相同。

最后，你可以像如下示例使用 xcconfig 中定义的宏：

NSLog(@"webservice url: %@, umeng appkey: %@", WEBSERVICE_URL, UMENG_APPKEY);


通过以上步骤，就达到了使用 xcconfig 文件来配置开发不同阶段时的环境变量的目的了。


#pragma mark -  配置脚本

在配置文件里写 setup_script = setup.sh







