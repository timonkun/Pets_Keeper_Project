扩展程序
============

为了让最终的作品格式更美观和规范，我们提供了一些Sphinx插件程序和模板，本章对这些插件和模板进行介绍。为了让Sphinx能找到插件和模板，需要编辑书籍项目的配置文件“conf.py”中的路径设置。在此文件开头添加：

::

    _exts = "../../exts"
    sys.path.append(os.path.abspath(_exts))

并修改HTML模板相关的配置：

::
    
    html_theme = 'book'
    html_theme_path = [_exts +"./theme"]

LaTeX的编号
++++++++++++++

.. tcode::

    number_ref.py
    
    为LaTeX文件添加带编号的章节和插图参照，适合制作印刷版的PDF文档
    
需要进行编号的插图使用以“fig”开头的标签，例如：

.. code-block:: none
    
    \ :ref:`fig-leo`\ 是Leo 4.9的界面截图。
    
    .. _fig-leo:
    
    .. figure:: images/leo.png
        :width: 12.0cm
    
        Leo的界面截图

.. ttip::

    在Leo编辑器中，可以输入“fig>leo”并按CTRL+1，快速生成上面的代码。
    
需要进行引用的章节可以用以“sec”开头的标签，例如：

.. code-block:: none

    章节名
    ======
    
    .. _sec-test:
    
    这是一个章节。

    这是一个引用：\ :ref:`sec-test`\ 。
    
例如：关于书籍目录的相关说明请阅读\ :ref:`sec-book-folder`\ 。
    
.. ttip::

    为了让章节标签包含在章节内部，本插件对以“sec”开头的标签进行特殊处理，因此可以在章节名之下定义标签。

代码说明标签
++++++++++++++++++

.. tcode::

    number_label.py
    
    为代码添加如“❶❷”的说明标签
    
为了对代码中的重要语句进行说明，本插件对代码中的“#❶”等进行处理。例如：

::

    import os
    print os.getcwd() #❶
    print os.environ  #❷

❶输出当前路径，❷输出环境变量。   

.. ttip::

    在Leo编辑器中，可以通过输入数字并按CTRL+1，快速输入“❶❷”等符号。如果通过“literalinclude”命令从外部文件载入代码段，则可以在代码中使用“#{1}”、“#{2}”等标签，它们会自动被转换为对应的数字符号。
    
.. twarning::

    目前此功能只支持Python语言。

带图标的块
+++++++++++++++

.. tcode::

    block.py
    
    可以在文章中间插入带图的块
    
本扩展程序提供了5种图片块，例如：

.. code-block:: none

    .. ttip:: 
    
        这个一个小提示。
        
生成：

.. ttip:: 

    这个一个小提示。
    
.. code-block:: none
    
    .. tcode:: 
    
        example.py
        
        这是一个例子程序
  
生成：

.. tcode:: 

    example.py

    这是一个例子程序  

.. code-block:: none

            
    .. twarning:: 
    
        警告，如果你看到这个警告，那么请无视它。
        
生成：

.. twarning:: 

    警告，如果你看到这个警告，那么请无视它。
    
.. code-block:: none
        
    .. tlink:: 
    
        http://hyry.dip.jp
        
        欢迎访问我们的主页
        
生成：

.. tlink:: 

    http://hyry.dip.jp
    
    欢迎访问我们的主页
    
.. code-block:: none

    .. tanim::

        demo.avi
        
        这是一个动画演示文件
        
生成：

.. tanim::

    demo.avi
    
    这是一个动画演示文件  
    
为了添加新的图标块命令“tnews”，需要准备两个图标文件：“news.png”和“news.pdf”，将它们分别放到下面两个目录中：   

.. code-block:: none

    exts\latexstyle\news.pdf 
    exts\theme\book\static\news.png
    
并编辑“block.py”文件，在其中的setup()中添加：

::

    app.add_directive('tnews', MakeFileDirective("tnews"))

HTML的中文分词
+++++++++++++++++++

.. tcode::

    chinese_search.py
    
    增加HTML的中文搜索功能
    
本扩展程序使用\ `SmallSeg <http://code.google.com/p/smallseg/>`_\ 对中文进行分词。

插入代码片段
++++++++++++++++++

.. tcode::

    literal_include.py
    
    修改literalinclude命令，为其添加section选项，可从源程序中载入文件中的部分源代码
    
    
例如程序“example.py”的内容如下：

.. literalinclude:: codes/example.py

使用下面的命令可以载入其中以“###1###”包围的部分。

.. code-block:: none

    .. literalinclude:: codes/example.py
        :section: 1
        
结果为：

.. literalinclude:: codes/example.py
    :section: 1

自动选择图片
++++++++++++++++++

.. tcode::

    image.py
    
    根据输出格式自动选择图像

当figure命令的图像名参数以“.*”结尾时，Sphinx将根据输出格式选择合适的图像。例如输出HTML时优先选择PNG图像，而输出PDF时优先选择PDF图像。本扩展程序在此基础上，添加了通过文件名选择图像的功能。具体的使用方法请参考\ :ref:`sec-tips-figure`\ 。

