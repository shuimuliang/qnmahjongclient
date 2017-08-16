#include "Utils.h"
#include <fstream>
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32&&CC_TARGET_PLATFORM != CC_PLATFORM_WP8)
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32||CC_TARGET_PLATFORM == CC_PLATFORM_WP8)  
#include <direct.h>  
#include <io.h>  
#else  
#include <stdarg.h>  
#include <sys/stat.h>  
#endif  


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32||CC_TARGET_PLATFORM == CC_PLATFORM_WP8)    
#define ACCESS _access  
#define MKDIR(a) _mkdir((a))  
#define STRDUP   _strdup
#else
#define ACCESS access  
#define MKDIR(a) mkdir((a), S_IRWXU | S_IRWXG | S_IRWXO)  
#define STRDUP   strdup
//#include <sys/statfs.h>  
#endif
#include <stdio.h>
#include "support/zip_support/unzip.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>  
#define _MAXDIR_   256
char dirname[_MAXDIR_], rootdir[_MAXDIR_];

namespace Engine
{
	namespace utils
	{
#define MAX_FILENAME   512
#define BUFFER_SIZE    8192

		int mystrncasecmp(const char *s1, const char *s2, int n)  
		{  
			int c1, c2;  
			do {  
				c1 = tolower(*s1++);  
				c2 = tolower(*s2++);  
			} while((--n > 0) && c1 == c2 && c1 != 0);  
			return c1 - c2;  
		} 

		bool deleteFile( string path )
		{
			//函数说明：remove()会删除参数pathname 指定的文件. 如果参数pathname 为一文件, 则调用unlink()处理,若参数pathname 为一目录, 则调用rmdir()来处理. 请参考unlink()与rmdir().

			//返回值：成功则返回0, 失败则返回-1, 错误原因存于errno.

			//http://blog.csdn.net/leaf1984zh/article/details/6232004
			if(remove(path.c_str())) 
			{
				CCLog("Could not delete the file %s \n",path.c_str()); 
				return false;
			}
			return true;
		}

		bool cmkdir(const char* dir)
		{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32&&CC_TARGET_PLATFORM != CC_PLATFORM_WP8)
			mode_t processMask = umask(0);
			int ret = mkdir(dir, S_IRWXU | S_IRWXG | S_IRWXO);
			umask(processMask);
			if (ret != 0 && (errno != EEXIST))
			{
				CCLog("create folder (%s) fail error:%d",dir,ret);
				return false;
			}

			return true;
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
			wstringstream wss;
			//wss<<dir;1
			const wchar_t* wdir;
			wdir = wss.str().c_str();
			BOOL ret = CreateDirectoryW(wdir, NULL);
			if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
			{
				return false;
			}
			return true;
#else
			BOOL ret = CreateDirectoryA(dir, NULL);
			if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
			{
				return false;
			}
			return true;
#endif
		}

		bool crename( const char* source,const char* newname )
		{
			if(!ACCESS(source,0))//如果文件存在:
			{
				if(rename(source,newname))
				{
					CCLog("file :%s rename to %s fail",source,newname);
					return false;
				}
			}else
			{
				CCLog("file :%s not exist",source);
				return false;
			}
			return true;
		}

		void cremoveDir(const char *path)
		{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32&&CC_TARGET_PLATFORM != CC_PLATFORM_WP8)  
			DIR* dp = NULL;
			DIR* dpin = NULL;
			char *pathname = (char*)malloc(_MAXDIR_);
			struct dirent* dirp;
			dp = opendir(path);
			if(dp == NULL)
			{
				CCLog("%s\n", "your input directory is not exist!");
				return;
			}
			while((dirp = readdir(dp)) != NULL)
			{
				if(strcmp(dirp->d_name, "..") == 0 || strcmp(dirp->d_name, ".") == 0)
					continue;
				strcpy(pathname, path);
				strcat(pathname, "/");
				strcat(pathname, dirp->d_name);
				dpin = opendir(pathname);
				if(dpin != NULL)
				{
					cremoveDir(pathname);

				}
				else
				{
					deleteFile(pathname);
					//remove(pathname);
				}
				strcpy(pathname, "");
				if(dpin)
				{
					closedir(dpin);
				}
				
				dpin = NULL;
			}
			rmdir(path);
			closedir(dp);
			free(pathname);
			pathname = NULL;
			dirp = NULL;
#endif
		}

		bool mkdirs(const char *pDir)  
		{  
			int i = 0;  
			int iRet;  
			int iLen;  
			char* pszDir;  

			if(NULL == pDir)  
			{  
				return false;  
			}  

			pszDir = STRDUP(pDir);  
			iLen = strlen(pszDir);  
			//CCLOG("create folder:%s",pszDir);

			iRet = cmkdir(pszDir);  
			if (iRet)  
			{  
				//试着创建一下
				free(pszDir);
				return true;  
			} 

			// 创建中间目录  从1开始是为了不是空串创建
			for (i = 1;i < iLen;i ++)  
			{  
				if (pszDir[i] == '\\' || pszDir[i] == '/')  
				{   
					pszDir[i] = '\0';  

					//如果不存在,创建  
					iRet = ACCESS(pszDir,0);  
					if (iRet != 0)  
					{  
						iRet = cmkdir(pszDir);  
						if (!iRet)  
						{  
							CCLOG("create folder fail,%s",pszDir);
							free(pszDir); 
							return false;  
						}   
					}  
					//支持linux,将所有\换成/  
					pszDir[i] = '/';  
				}   
			}  

			iRet = cmkdir(pszDir);  
			free(pszDir);  
			if (!iRet)  
			{  
				CCLOG("create folder fail,%s",pszDir);
				return false;  
			}   
			return true;  
		}  


		bool uncompress(string outFileName)
		{

			// Open the zip file
			unzFile zipfile = unzOpen(outFileName.c_str());
			if (! zipfile)
			{
				CCLOG("can not open downloaded zip file %s", outFileName.c_str());
				return false;
			}

			// Get info about the zip file
			unz_global_info global_info;
			if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
			{
				CCLOG("can not read file global info of %s", outFileName.c_str());
				unzClose(zipfile);
			}

			// Buffer to hold data read from the zip file
			char readBuffer[BUFFER_SIZE];

			// Loop to extract all files.
			uLong i;
			for (i = 0; i < global_info.number_entry; ++i)
			{
				// Get info about current file.
				unz_file_info fileInfo;
				char fileName[MAX_FILENAME];
				if (unzGetCurrentFileInfo(zipfile,
					&fileInfo,
					fileName,
					MAX_FILENAME,
					NULL,
					0,
					NULL,
					0) != UNZ_OK)
				{
					CCLOG("can not read file info");
					unzClose(zipfile);
					return false;
				}

				size_t pos = outFileName.find_last_of("/");
				string dir;
				if (pos != std::string::npos)
				{
					dir = outFileName.substr(0,pos+1);
				}
				string fullPath = dir + fileName;
				// Check if this entry is a directory or a file.
				const size_t filenameLength = strlen(fileName);
				if (fileName[filenameLength-1] == '/')
				{
					// Entry is a direcotry, so create it.
					// If the directory exists, it will failed scilently.
					int iRet = ACCESS(fullPath.c_str(),0);  
					if (iRet != 0)  
					{  
						if (!mkdirs(fullPath.c_str()))
						{
							CCLOG("can not create directory %s", fullPath.c_str());
							unzClose(zipfile);
							return false;
						}
					}
				}
				else
				{
					// Entry is a file, so extract it.

					// Open current file.
					if (unzOpenCurrentFile(zipfile) != UNZ_OK)
					{
						CCLOG("can not open file %s", fileName);
						unzClose(zipfile);
						return false;
					}

					size_t pos = fullPath.find_last_of("/");
					string path;
					if (pos != std::string::npos)
					{
						path = fullPath.substr(0,pos+1);
					}
					int iRet = ACCESS(path.c_str(),0);  
					if (iRet != 0)  
					{ 
						if (!mkdirs(path.c_str()))
						{
							CCLOG("can not create directory %s", fullPath.c_str());
							unzClose(zipfile);
							return false;
						}
					}

					// Create a file to store current file.
					FILE *out = fopen(fullPath.c_str(), "wb");
					if (! out)
					{
						CCLOG("can not open destination file %s", fullPath.c_str());
						unzCloseCurrentFile(zipfile);
						unzClose(zipfile);
						return false;
					}

					// Write current file content to destinate file.
					int error = UNZ_OK;
					do
					{
						error = unzReadCurrentFile(zipfile, readBuffer, BUFFER_SIZE);
						if (error < 0)
						{
							CCLOG("can not read zip file %s, error code is %d", fileName, error);
							unzCloseCurrentFile(zipfile);
							unzClose(zipfile);
							return false;
						}

						if (error > 0)
						{
							fwrite(readBuffer, error, 1, out);
						}
					} while(error > 0);

					fclose(out);
				}

				unzCloseCurrentFile(zipfile);

				// Goto next entry listed in the zip file.
				if ((i+1) < global_info.number_entry)
				{
					if (unzGoToNextFile(zipfile) != UNZ_OK)
					{
						CCLOG("can not read next file");
						unzClose(zipfile);
						return false;
					}
				}
			}


			unzClose(zipfile);
			return true;
		}

		cocos2d::ccColor3B transFromStringToColor( const string& cStr )
		{
			const int NUMBER_OF_COLOR = 3;
			if(cStr.size()<NUMBER_OF_COLOR*2) return ccBLUE;	
			int result[NUMBER_OF_COLOR];
			for(int i =0 ;i<NUMBER_OF_COLOR;++i)
			{
				char tempC = cStr.at(i);
				result[i]=getHex(cStr.at(2*i))*16+getHex(cStr.at(2*i+1));
			}
			return ccc3(result[0],result[1],result[2]);
		}

		int getHex( char c )
		{
			int result = 0;
			//if <10
			int temp = c - '0';
			if(temp<10&&temp>-1){
				result = temp;
			}else{
				temp = c - 'A';
				if(temp<6&&temp>-1){
					result = temp+10;
				}
			}
			return result;
		}

		
		//字符串分割函数
		std::vector<std::string> split(std::string str,std::string pattern)
		{
			std::string::size_type pos;
			std::vector<std::string> result;
			str+=pattern;//扩展字符串以方便操作
			unsigned int size=str.size();

			for(unsigned int i=0; i<size; i++)
			{
				pos = str.find(pattern,i);
				if(pos < size)
				{
					std::string s=str.substr(i,pos-i);
					result.push_back(s);
					i=pos+pattern.size()-1;
				}
			}
			return result;
		}

		string replaceString( const string& srcStr , const string& findStr,const string& replaceStr )
		{
			string result = srcStr;
			string::size_type pos = 0;

			while((pos = result.find(findStr, pos)) != string::npos) {
				result = result.replace(pos, findStr.size(), replaceStr);
				pos++;
			}
			return result;
		}

		string jStrFormat( string contentStr,const string replaceStr )
		{
			char s[1000];
			sprintf(s,contentStr.c_str(),replaceStr.c_str());
			return s;
		}


		long computeFileSize(const char* fullPath)  
		{   
			FILE *fp = fopen(fullPath, "r");  
			fseek(fp, 0, SEEK_END);  
			long length = ftell(fp);  
			fclose(fp);  
			return length;  
		}  

		cocos2d::CCPoint getMoveToPoint( const CCPoint &pCur,const CCPoint &pTarget,int dis)
		{
			float disRToC = ccpDistance(pCur,pTarget);
			if(disRToC>1){
				float rat = dis/disRToC;
				CCPoint target;
				target.x = (pTarget.x-pCur.x)*rat+pCur.x;
				target.y = (pTarget.y-pCur.y)*rat+pCur.y;
				return target;
			}
			return pCur;
		}

		int convertToInt( const char* str )
		{
			int result = 0;
			sscanf(str,"%d",&result);
			return result;	
		}

		double convertToDouble( const char* str )
		{
			double result = 0.0;
			sscanf(str,"%f",&result);
			return result;
		}

		int randomBetweenIntDialog( int max, int min )
		{
			return std::rand() % (max - min) + min;   
		}

		void writeBinary( const char* fileName , const char *buf,unsigned int size )
		{
			string path = CCFileUtils::sharedFileUtils()->getWritablePath() + fileName;
			ofstream file;
			file.open(path.c_str(),ios::out|ios::binary);
			file.write(buf,size);
			file.close();
		}

		string removeEmoj( const string &src )
		{
			string result;
			int f0StartId =0;
			
			int e2StartId = 0;
			string e2Temp;

			for(size_t i = 0 ;i < src.size() ; ++i)
			{
                unsigned char curChar = src.at(i);
				if(f0StartId >0){
					--f0StartId;
					continue;
				}
				
				if(e2StartId>0){
					--e2StartId;
					e2Temp += src.at(i);
					continue;
				}

				if(e2Temp.size()==3){
					unsigned char midChar = e2Temp.at(1);
					bool isEmoj = midChar>=0x9C&&midChar<=0x9E;
					if(!isEmoj){ result += e2Temp;}
					e2Temp.clear();
				}

				if(curChar==0xF0){
					f0StartId = 3;//0xF0 means 0xff ff ff ff 
					continue;
				}

				if(curChar==0xE2){//\xE2\x9E\xB0
					e2StartId = 2;
					e2Temp += (char)0xE2;
					continue;
				}

				result += src.at(i);
			}
			return result;
		}
	}

}