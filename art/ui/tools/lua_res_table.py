# -*- coding: utf-8 -*-

"""遍历图片目录，生成资源映射表"""

import os
import sys
import os.path
import subprocess
import shutil

cur_path = sys.path[0]
des_path = cur_path + "/../../../dev/cocos2d-x-2.2.6/projects/Casino/Resources/lua/model"

file_name = []
texture_name = {}

def traverse_sub_path(d, index):
    for item in os.listdir(d):
        absfilepath = os.path.join(d, item)
        if os.path.isfile(absfilepath):
            prefix, ext = os.path.splitext(item)
            if ext == ".png" or ext.lower() == ".jpg" or ext.lower() == ".jpeg":
                texture_name[item] = index
        elif os.path.isdir(absfilepath):
            traverse_sub_path(absfilepath, index)

def traverse_root_path(d):
    count = 1
    for item in os.listdir(d):
        absfilepath = os.path.join(d, item)
        if os.path.isfile(absfilepath):
            prefix, ext = os.path.splitext(item)
            if ext == ".png" or ext.lower() == ".jpg" or ext.lower() == ".jpeg":
                file_name.append(item)
                count = count + 1
        elif os.path.isdir(absfilepath) and item != ".svn":
            file_name.append(item + ".plist")
            file_name.append(item + ".pvr.ccz")
            traverse_sub_path(absfilepath, count)
            count = count + 2
            print item + ".plist"
            print item + ".pvr.ccz"

def output(file_dir, texture_dir):
    str = "local RES_FILE_NAME = {\n"
    for f_n in file_name:
        str = str + "\t\"" + f_n + "\",\n"
    str = str + "}\nreturn RES_FILE_NAME"

    f = open(file_dir, "w+")
    f.write(str)
    f.close()


    str = "local TEXTURE_NAME= {\n"
    for t_n in texture_name:
        str = str + "\t[\"" + t_n + "\"] = %d,\n" %(texture_name[t_n])
    str = str + "}\nreturn TEXTURE_NAME"

    f = open(texture_dir, "w+")
    f.write(str)
    f.close()

    if os.path.exists(des_path + "/" + file_dir):
        os.remove(des_path + "/" + file_dir)
    shutil.move(cur_path + "/" + file_dir, des_path)

    if os.path.exists(des_path + "/" + texture_dir):
        os.remove(des_path + "/" + texture_dir)
    shutil.move(cur_path + "/" + texture_dir, des_path)
    print "mission complete"

helpdoc = """
搜索目录
python lua_res_table.py -d <dir>
"""

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print helpdoc

    option = sys.argv[1]
    RootPath = os.path.realpath(sys.argv[2])
    if option == '-d':
        traverse_root_path(RootPath)
        output("ResFile.lua", "ResTexture.lua")
