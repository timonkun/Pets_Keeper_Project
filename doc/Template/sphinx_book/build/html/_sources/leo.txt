Leo编辑器
============

我们使用Leo编辑器管理和编辑所有的扩展程序和rst文件，\ :ref:`fig-leo`\ 是用Leo 4.9打开“master.leo”时的画面。

.. _fig-leo:

.. figure:: images/leo.png
    :width: 12.0cm

    Leo的界面截图

按钮工具栏
+++++++++++++++

.. _sec-leo-buttons:

打开“master.leo”之后，可以在窗口的上方看到如下图所示的按钮工具栏。

.. table:: Leo的按钮工具栏说明

    ============= ===================================
    按钮名        功能
    ============= ===================================
    script-button Leo自带的按钮，用它可以创建新的按钮 
    show-tree     显示并调整提纲窗口的宽度
    hide-tree     隐藏提纲窗口
    make-html     将当前的书籍项目编译为HTML
    make-pdf      将当前的书籍项目编译为PDF
    clean         清除当前的书籍项目的编译结果
    ============= ===================================

其中，make-html、make-pdf以及clean等三个按钮，需要提纲栏中的当前节点为某个书籍项目的子节点。

快速输入宏
+++++++++++++++

在“master.leo”中定义了可快速输入各种命令的宏，其节点路径为：

.. code-block:: none

    @chapters-->Scripts-->@command rst-macro
    
输入宏之后按CTRL+1即可执行，将其扩展为对应的文本。下表列出了一些常用的宏：    
    
.. table:: 快速输入文本的宏

    ========   ========================
    输入       输出
    ========   ========================
    table      table命令
    inc>       literalinclude命令
    math       math命令
    fig>       figure命令
    _s         章节标签
    _f         图表标签
    sec        章节参照
    fig        图表参照
    m          行内math命令 
    tl         tlink命令
    tt         ttip命令
    tw         twaring命令
    tc         tcode命令
    ta         tanim命令
    cb         code-block命令
    t          topic命令
    l          超链接 
    数字       对应的符号，如❶
    ->         →
    ========   ========================

其中带“>”的宏可以输入参数，例如“fig>leo.png”、“inc>example.py>1”等。

