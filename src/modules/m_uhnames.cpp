/*       +------------------------------------+
 *       | Inspire Internet Relay Chat Daemon |
 *       +------------------------------------+
 *
 *  InspIRCd: (C) 2002-2007 InspIRCd Development Team
 * See: http://www.inspircd.org/wiki/index.php/Credits
 *
 * This program is free but copyrighted software; see
 *            the file COPYING for details.
 *
 * ---------------------------------------------------
 */

#include "users.h"
#include "channels.h"
#include "modules.h"
#include "inspircd.h"

static const char* dummy = "ON";

/* $ModDesc: Provides aliases of commands. */

class ModuleUHNames : public Module
{
	CUList nl;
 public:
	
	ModuleUHNames(InspIRCd* Me)
		: Module::Module(Me)
	{
	}

	void Implements(char* List)
	{
		List[I_OnPreCommand] = List[I_OnUserList] = List[I_On005Numeric] = 1;
	}

	virtual ~ModuleUHNames()
	{
	}

	virtual Version GetVersion()
	{
		return Version(1,1,0,1,VF_VENDOR,API_VERSION);
	}

        virtual void On005Numeric(std::string &output)
	{
		output.append(" UHNAMES");
	}

	Priority Prioritize()
	{
		return (Priority)ServerInstance->PriorityBefore("m_namesx.so");
	}

        virtual int OnPreCommand(const std::string &command, const char** parameters, int pcnt, userrec *user, bool validated, const std::string &original_line)
	{
		irc::string c = command.c_str();
		/* We don't actually create a proper command handler class for PROTOCTL,
		 * because other modules might want to have PROTOCTL hooks too.
		 * Therefore, we just hook its as an unvalidated command therefore we
		 * can capture it even if it doesnt exist! :-)
		 */
		if (c == "PROTOCTL")
		{
			if ((pcnt) && (!strcasecmp(parameters[0],"UHNAMES")))
			{
				user->Extend("UHNAMES",dummy);
				return 1;
			}
		}
		return 0;
	}

	/* IMPORTANT: This must be prioritized above NAMESX! */
	virtual int OnUserList(userrec* user, chanrec* Ptr, CUList* &ulist)
	{
		if (user->GetExt("UHNAMES"))
		{
			nl.clear();
			for (CUList::iterator i = ulist->begin(); i != ulist->end(); i++)
				nl[i->second->GetFullHost()] = i->second;
			nameslist = &nl;
		}
		return 0;		
 	}
};


class ModuleUHNamesFactory : public ModuleFactory
{
 public:
	ModuleUHNamesFactory()
	{
	}

	~ModuleUHNamesFactory()
	{
	}

		virtual Module * CreateModule(InspIRCd* Me)
	{
		return new ModuleUHNames(Me);
	}
};


extern "C" void * init_module( void )
{
	return new ModuleUHNamesFactory;
}

