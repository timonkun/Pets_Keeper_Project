安装编辑环境
==================



Python和Sphinx
+++++++++++++++

首先到Python的官方网站下载并安装2.6或2.7系列的Python运行环境。

.. tlink::
    
    http://python.org/getit/
    
    Python的下载地址

Sphinx是一套使用reStructuredText作为标记语言的文档生成工具。本环境所提供的所有插件均可在Sphinx v1.1.2中正常使用，如果需要单独升级Sphinx库，可以在控制台中输入如下命令：

.. code-block:: none

    easy_install -U sphinx

如果高版本的Sphinx无法正确运行插件，请使用如下命令安装1.1.2版本的Sphinx：

.. code-block:: none

    easy_install sphinx==1.1.2

.. topic:: reStructuredText

    restructuredText是一种简单易用的所见即所得的纯文本标记语法。可以通过转换工具将其转换为HTML、latex、PDF等多种格式。通常restructuredText的扩展名为"rst"。

Leo
++++

Leo是一个用Python编写的提纲式程序编辑器，我们用它组织和编辑构成书籍内容的reStructuredText文档，并管理Sphinx的插件程序、HTML模板以及配置文档等。

.. tlink::

    http://sourceforge.net/projects/leo/files/Leo/
    
    Leo的下载地址，可以下载源程序版或打包版，由于系统中已经安装了Python环境，因此推荐安装源程序版

MiKTeX
++++++

.. tlink::

    http://miktex.org/
    
    MiKTex是一个Windows下的Tex编译环境，我们用它将Sphinx自动生成的LaTex源文件编译成PDF文件
    
安装完成之后，执行：

.. code-block:: none

    xelatex sample.tex
    
就可以将“sample.tex”编译成“sample.pdf”。

书籍目录
++++++++++++

.. _sec-book-folder:


.. tlink::

    http://hyry.dip.jp/files/books.zip
    
    下载本书的编辑环境

编写书籍项目的目录结构如下：

.. code-block:: none

    [books]
        master.leo     -- 管理所有内容的leo文件    
        [exts]         -- 插件和模板
        [sphinxbook]   -- 本书的文件夹
        [xxxbook]      -- 其它书籍的文件夹

其中exts文件夹中包含了所有Sphinx插件程序以及LaTex和HTML的模板。而其它文件夹均为Sphinx书籍的文件夹。每本书籍的目录结构如下：

.. code-block:: none

    [sphinxbook]
        make.bat      -- 编译书籍的批处理脚本
        [source]      -- 书籍的源文件
            conf.py   -- 书籍配置
            *.rst     -- 各个章节的reStructuredText文件
            [images]  -- 保存所有插图的文件夹
            [codes]   -- 保存所有代码的文件夹
        [build]
            [latex]   -- PDF的编译输出文件夹
            [html]    -- HTML的编译输出文件夹
            
在书籍文件夹下运行“make.bat html”命令将书籍编译成HTML格式，而运行“make.bat latex”则编译成LaTex格式。这些命令可以通过Leo的按钮工具栏(\ :ref:`sec-leo-buttons`\ )运行。 

.. twarning::

    为了保证程序能正常运行，请保证所有路径中没有空格或中文。

字体
++++++

使用合适的字体可以使编辑环境用起来更舒适，使书籍更容易阅读。在“master.leo”中使用“YaHei Mono”字体，它中文字体采用微软雅黑，英文字体采用等宽的Consolas字体。可以通过如下节点中的QT样式表修改Leo编辑器所使用的字体：

.. code-block:: none

    @chapters-->@settings-->qtGui plugin-->@data qt-gui-plugin-style-sheet
    
.. tlink::

    yahei_mono.7z
    
    YaHei Mono字体：中文字体采用微软雅黑，英文字体采用等宽的Consolas字体
    
书籍的HTML版本的字体可以通过书籍模板的样式表进行修改，而PDF版本的字体通过书籍配置文件“conf.py”中的latex_preamble配置进行修改，例如本书中采用如下字体配置：

.. code-block:: none

    \setCJKsansfont[BoldFont={STXihei},ItalicFont={STXihei}]{STXihei}
    \setCJKromanfont[BoldFont={STXihei},ItalicFont={STXihei}]{STXihei}
    \setCJKmainfont[BoldFont={STXihei},ItalicFont={STXihei}]{STXihei}
    \setCJKmonofont[BoldFont={STXihei},ItalicFont={STXihei}]{STXihei}

STXihei字体的中文名为华文细黑，在网络上可以搜索到它的下载地址。

