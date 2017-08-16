# -*- coding: utf-8 -*-

import os
import sys
import os.path
import subprocess
RootPath = ""

def build_root_path(d):
    for item in os.listdir(d):
        absfilepath = os.path.join(d, item)
        if os.path.isfile(absfilepath):
            prefix, ext = os.path.splitext(item)
            if ext == ".lua":
                destfile = os.path.join(d, prefix+".lua")
                cmd = "../../cocos2d-x-2.2.5/scripting/lua/luajit/src/src/luajit -b %s %s" % (absfilepath,destfile)
                os.system(cmd)
                # 防止误删目录
                # if destfile.startswith(d):
                 #   os.remove(absfilepath)
        elif os.path.isdir(absfilepath):
            build_root_path(absfilepath)

def remove_dotout_file(d):
    for item in os.listdir(d):
        absfilepath = os.path.join(d, item)
        if os.path.isfile(absfilepath):
            prefix, ext = os.path.splitext(item)
            if ext == ".out":
                # 防止误删目录
                if absfilepath.startswith(d):
                    os.remove(absfilepath)
        else:
            remove_dotout_file(absfilepath)

helpdoc =  """
build lua脚本
python buildlua.py -build <dir>
清理lua脚本生成的out文件
python buildlua.py -clean <dir>
"""

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print helpdoc

    option = sys.argv[1]
    RootPath = os.path.realpath(sys.argv[2])
    if option == '-build':
        build_root_path(RootPath)
    elif option == '-clean':
        remove_dotout_file(RootPath)
