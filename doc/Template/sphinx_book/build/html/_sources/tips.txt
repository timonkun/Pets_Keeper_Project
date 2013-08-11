解决方案
============

本章列出一些在写书过程中经常会遇到的问题。

表格
++++++

.. csv-table:: Python中的常用类型
   :header: 类型,描述,例子
   :widths: 15, 30, 30
   :quote: $ 
   :delim: |

    str|一个由字符组成的不可更改的有串行。在Python 3.x里，字符串由Unicode字符组成。|'Wikipedia', "Wikipedia"
    bytes|一个由字节组成的不可更改的有串行。|b'Some ASCII', b"Some ASCII"
    list|可以包含多种类型的可改变的有串行|[4.0, 'string', True]
    tuple|可以包含多种类型的不可改变的有串行|(4.0, 'string', True)
    set, frozenset|与数学中集合的概念类似。无序的、每个元素唯一。|{4.0, 'string', True}, frozenset([4.0, 'string', True])
    dict|一个可改变的由键值对组成的无串行。|{'key1': 1.0, 3: False}
    int|精度不限的整数|42
    float|浮点数。精度与系统相关。|3.1415927
    complex|复数|3+2.7j
    bool|逻辑值。|只有两个值：True和False

插图
++++++

.. _sec-tips-figure:


当图像文件名以".*"结尾时，将根据输出格式自动选择图像文件。例如，\ :ref:`fig-fftexamplerectangle`\ 采用的文件名为“.*”，它对应两个文件：“fft_example_rectangle.png”和“fft_example_rectangle.pdf”。输出HTML时将选用PNG文件，而输出PDF时将选用PDF文件。

.. code-block:: none

    .. figure:: images/fft_example_rectangle.*

.. _fig-fftexamplerectangle:

.. figure:: images/fft_example_rectangle.*
    :width: 14.0cm

    用正弦波合成矩形波
    
\ :ref:`fig-numpyaccess2d`\ 采用的文件名为“numpy_access2d.*”，对应两个文件：“numpy_access2d.html.png”和“numpy_access2d.latex.png”。

.. code-block:: none

    .. figure:: images/numpy_access2d.*

.. _fig-numpyaccess2d:

.. figure:: images/numpy_access2d.*
    :width: 12.0cm

    二维NumPy数组的下标存取

插入代码
++++++++++++

.. literalinclude:: codes/example.py    
    :linenos:
    :language: python
    :lines: 1, 3-5

.. code-block:: matlab

    %DOCLINK - Provide windows dir HREF text(clipboard)
    
    string_temp_ahead = fullfile('jar:file:///', matlabroot, 'help');
    
    clipboard('copy', href_for_codelib);

