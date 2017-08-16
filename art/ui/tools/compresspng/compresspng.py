# -*- coding: utf-8 -*-

"""将PNG图片用pngquant进行批量压缩
安装: http://pngquant.org
"""

import os
import sys
import os.path
import subprocess

def compress_root_path(d):
    for item in os.listdir(d):
        absfilepath = os.path.join(d, item)
        if os.path.isfile(absfilepath):
            prefix, ext = os.path.splitext(item)
            if ext == ".png":
                destfile = os.path.join(d, prefix+".out")
                cmd = "pngquant --force %s --ext .png" % (absfilepath,)
                os.system(cmd)
        elif os.path.isdir(absfilepath):
            compress_root_path(absfilepath)

helpdoc = """
压缩目录
python compresspng.py -d <dir>
"""

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print helpdoc

    option = sys.argv[1]
    RootPath = os.path.realpath(sys.argv[2])
    if option == '-d':
        compress_root_path(RootPath)
