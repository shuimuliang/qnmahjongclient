#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>
#include "cocos2d.h"
#include <sstream>
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_WP8)
#include <dirent.h>
#include <sys/stat.h>
#endif
#include <stdlib.h>

USING_NS_CC;
using namespace std;

namespace Engine
{

	namespace utils
	{
		/**
		* 创建文件夹
		*/
		bool mkdirs(const char *pDir);
		/**
		* 解压
		*/
		bool uncompress(string outFileName);

		/**
		* 删除
		*/
		bool deleteFile(string path);


		bool cmkdir(const char* dir);

		void cremoveDir(const char *path);

		bool crename(const char* source,const char* newname);

		/**
		* 将string 转成颜色值 类似"FF00FF"
		*/
		ccColor3B transFromStringToColor( const string& cStr );

		int getHex( char c );
		std::vector<std::string> split(std::string str,std::string pattern);
		//字符串替换
		string replaceString(const string& srcStr , const string& findStr,const string& replaceStr);
		//java string format 不能超过500字符
		string jStrFormat(string contentStr,string replaceStr);
		//获取文件大小 
		long computeFileSize(const char* fullPath);
		//获取A点相对B点移动x像素后的C点坐标
		CCPoint getMoveToPoint(const CCPoint &pCur,const CCPoint &pTarget,int dis);
		/*
		实现字符串到整数或者浮点数的转化
		*/
		int convertToInt(const char* str);
		double convertToDouble(const char* str);
        
		int mystrncasecmp(const char *s1, const char *s2, int n);

		template<typename T> void deletePtrVector(vector<T*>& ptrVec)
		{
			typename vector<T*>::iterator it = ptrVec.begin();
			while(it != ptrVec.end())
			{
				CC_SAFE_DELETE(*it);
				++it;
			}
			ptrVec.clear();
		}
		template<typename T, typename U> void deletePtrMap(map<T,U*>& ptrMap)
		{
			typename map<T,U*>::iterator it = ptrMap.begin();
			while(it != ptrMap.end())
			{
				CC_SAFE_DELETE(it->second);
				++it;
			}
			ptrMap.clear();
		}

		int randomBetweenIntDialog(int max, int min);

		void writeBinary(const char* fileName , const char *buf,unsigned int size);

		//remove \xF0(4) and \xE2(3) 
		//http://apps.timwhitlock.info/emoji/tables/unicode
		string removeEmoj(const string &src);

	}
}
#endif /* __UTILS_H__ */
