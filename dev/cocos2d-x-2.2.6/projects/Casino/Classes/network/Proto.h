#ifndef __PROTO_H__
#define __PROTO_H__

#include <vector>
#include "pbc/pbc_include.h"
#include "cocos2d.h"
USING_NS_CC;

#define DEF_PACK_PUB "pub"
#define DEF_PACK_PF "pf"

#include <string>
#include <stdio.h>


class CProtoBase
{
public:
	enum eParseType{ eEncode = 0, eDecode };
	enum eMsgDef
	{
		NONE = -1,
		ABSMessage = 0,
		VerUpdate = 100,
	};

	struct lstring
	{
		lstring() : str(NULL), len(0){}
		const char* str;
		unsigned int len;
	};


	CProtoBase(const char* pack, const char* proto, eMsgDef msgSeed = NONE) : _wmsg(NULL), _rmsg(NULL)
	{
		if(!pack || !proto){printf("[CProtoBase] invalid proto"); return;}
		_msgSeed = msgSeed;
		_packName = pack;
		_protoName = proto;
	}

	eMsgDef getMsgSeed(){return _msgSeed;}

	virtual void destroy()
	{
		if(_wmsg){pbc_wmessage_delete(_wmsg); _wmsg = NULL;}
		if(_rmsg){pbc_rmessage_delete(_rmsg); _rmsg = NULL;}
	}

	virtual void encode(struct pbc_env * env, char** data, unsigned int* size)
	{
		// check
		if(!env){printf("[encode] env is nil"); return;}
		if(_wmsg){printf("_wmsg has existed"); return;}

		// create message (find from pb file)
		char buff[128] = {0};
		sprintf(&buff[0],"%s.%s",_packName.c_str(), _protoName.c_str());
		_wmsg = pbc_wmessage_new(env, &buff[0]);
		if(!_wmsg){printf("[encode] create %s wmsg failed",&buff[0]); return;}

		// parse
		parse(eEncode);

		// encode
		struct pbc_slice slice;
		pbc_wmessage_buffer(_wmsg, &slice);

		// save
		if(data) *data = (char*)slice.buffer;
		if(size) *size = slice.len;
	}

	virtual void encode(struct pbc_wmessage* wmsg, const char* name)
	{
		// check
		if(!wmsg || !name){printf("[encode] invalid arguments"); return;}
		if(_wmsg){printf("_wmsg has existed"); return;}

		// create message
		_wmsg = pbc_wmessage_message(wmsg, name);
		if(!_wmsg){printf("[encode] create %s wmsg failed",name); return;}

		// parse
		parse(eEncode);
	}

	virtual void decode(struct pbc_env * env, const char* data, unsigned int size)
	{
		// check
		if(!env || !data){printf("[decode] invalid arguments"); return;}
		if(_rmsg){printf("_rmsg has existed"); return;}

		// create message
		struct pbc_slice slice = {(void*)data, size};
		char buff[128] = {0};
		sprintf(&buff[0],"%s.%s",_packName.c_str(), _protoName.c_str());
		_rmsg = pbc_rmessage_new(env, &buff[0], &slice);
		if(!_rmsg){printf("[decode] create %s _rmsg failed",&buff[0]); return;}

		// parse
		parse(eDecode);

		// free
		pbc_rmessage_delete(_rmsg);
		_rmsg = NULL;
	}

	virtual void decode(struct pbc_rmessage* rmsg, const char* name, unsigned int index)
	{
		// check
		if(!rmsg || !name){printf("invalid arguments"); return;}

		// create message
		_rmsg = pbc_rmessage_message(rmsg, name, index);
		if(!_rmsg){printf("[decode] create %s _rmsg failed",name); return;}

		// parse
		parse(eDecode);
	}

	virtual void parse(eParseType type) = 0;

#define parse_def virtual void parse(eParseType type)

#define rp_lstring(k,v,i)\
	if(type == eEncode){pbc_wmessage_string(_wmsg, k, v.str, v.len);} \
	else{v.str = pbc_rmessage_string(_rmsg, k , i , (int*)&v.len);}

#define rp_string(k,v,i) \
	if(type == eEncode){pbc_wmessage_string(_wmsg, k, v.c_str(), -1);} \
	else{v = pbc_rmessage_string(_rmsg, k , i , NULL);}

#define rp_int(k,v,i) \
	if(type == eEncode){pbc_wmessage_integer(_wmsg, k, v, -1);} \
	else{v = pbc_rmessage_integer(_rmsg, k , i , NULL);}

#define rp_class(k,v,i) \
	if(type == eEncode){v.encode(_wmsg, k);} \
	else{v.decode(_rmsg, k, i);}

#define parse_string(parm) rp_string(#parm,parm,0)
#define parse_lstring(parm) rp_lstring(#parm,parm,0)
#define parse_int(parm) rp_int(#parm,parm,0)
#define parse_class(parm) rp_class(#parm,parm,0)

#define parse_vector(t, vec, f) \
	if(type == eEncode){ \
		for(unsigned int i=0; i<vec.size(); ++i){ f(#vec, vec[i], i); } \
	} \
	else{ \
		unsigned int n = pbc_rmessage_size(_rmsg, #vec); \
		for(unsigned int i=0; i<n; ++i){ \
			t v; \
			f(#vec, v, i); \
			vec.push_back(v); \
		} \
	}

protected:
	struct pbc_wmessage* _wmsg;
	struct pbc_rmessage* _rmsg;

private:
	eMsgDef _msgSeed;
	std::string _packName;
	std::string _protoName;
};


class CPB_MessagePair : public CProtoBase
{
public:
	CPB_MessagePair() : CProtoBase(DEF_PACK_PUB, "MessagePair") {}
	unsigned int action_type;
	lstring message_bytes;
	parse_def
	{
		parse_int(action_type);
		parse_lstring(message_bytes);
	}
};

class CPB_ABSMessage : public CProtoBase
{
public:
	CPB_ABSMessage() : CProtoBase(DEF_PACK_PUB, "ABSMessage", ABSMessage) {}
	std::vector<CPB_MessagePair> msgList;
	std::string token;
	unsigned int subversion;
	unsigned int message_id;
	unsigned int timestamp;

	parse_def
	{
		parse_vector(CPB_MessagePair, msgList, rp_class);
		parse_string(token);
		parse_int(subversion);
		parse_int(message_id);
		parse_int(timestamp);
	}
};

class CPB_DownPackInfo : public CProtoBase
{
public:
	CPB_DownPackInfo() : CProtoBase(DEF_PACK_PUB, "DownPackInfo") {}
	std::string url;		// 下载地址
	int size;				//  大小 xxKb
	std::string desc;		//  desc
	virtual void parse(eParseType type)
	{
		parse_string(url);
		parse_int(size);
		parse_string(desc);
	}
};

class CPB_VerUpdateSend : public CProtoBase
{
public:
	CPB_VerUpdateSend() : CProtoBase(DEF_PACK_PF, "VerUpdateSend", VerUpdate) {}
	int channelID;		
	int OS;				
	parse_def
	{
		parse_int(channelID);
		parse_int(OS);
	}
};

class CPB_VerUpdateRecv : public CProtoBase
{
public:
	CPB_VerUpdateRecv() : CProtoBase(DEF_PACK_PF, "VerUpdateRecv", VerUpdate) {}
	std::string url;
	std::string desc;
	std::string showVersion;
	int numVersion;
	std::vector<CPB_DownPackInfo> downPackInfo;
	parse_def
	{
		parse_string(url);
		parse_string(desc);
		parse_string(showVersion);
		parse_int(numVersion);
		parse_vector(CPB_DownPackInfo, downPackInfo, rp_class);
	}
	void print(){
		CCLOG("CPB_VerUpdateRecv = {");
		CCLOG("url =%s" , url.c_str());
		CCLOG("desc =%s" , desc.c_str());
		CCLOG("showVersion =%s" , showVersion.c_str());
		CCLOG("numVersion = %d" , numVersion);
		CCLOG("downPackInfo.size = %d" , downPackInfo.size());
		for(unsigned int i =0 ;i<downPackInfo.size();++i)
		{
			CPB_DownPackInfo info = downPackInfo.at(i);
			CCLOG("i=%d",i);
			CCLOG("url = %s",info.url.c_str());
			CCLOG("info = %d , %s, %s",info.size , info.url.c_str() , info.desc.c_str());
		}
		CCLOG("}");
		
	}
};
#endif
