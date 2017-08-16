#ifndef _I_COMMAND_H_
#define _I_COMMAND_H_

#include "../interface/INotification.h"
#include "../interface/ISend.h"

namespace Engine
{

	class ICommand:public ISend
	{
	public:
		virtual ~ICommand()
		{
		}
		virtual void execute(INotification & notification) =0; 
	};

}
#endif


