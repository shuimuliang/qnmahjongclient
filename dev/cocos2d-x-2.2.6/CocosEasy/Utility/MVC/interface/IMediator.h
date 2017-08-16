#ifndef __MEDIATOR_H__
#define __MEDIATOR_H__

#include<vector>
#include<string>
#include "../interface/ICommand.h"

using std::vector;
using std::string;

namespace Engine
{

	typedef std::vector<std::string> Interests; 

	class IMediator : public ICommand
	{
	public:

		IMediator(const string &name);

		IMediator();

		virtual ~IMediator(void);

		std::string getName(){return m_mediatorName;} 

		virtual Interests listNotificationInterests() = 0; 

	protected:
		string m_mediatorName; 

	};

}
#endif //__MEDIATOR_H__
