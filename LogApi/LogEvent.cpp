#include "precompiled.h"

CLogEvent gLogEvent;

void CLogEvent::ServerActivate(edict_t* pEdictList, int edictCount, int clientMax)
{
	if (gLogApi.EventEnabled(__func__))
	{ 
		nlohmann::ordered_json Event;

		Event["Event"] = __func__;

		Event["Server"] = gLogApi.GetServerInfo();

		Event["EdictCount"] = edictCount;

		Event["ClientMax"] = clientMax;

		gLogApi.SendEvent(LogApi::Events::ServerActivate, Event);
	}
}

void CLogEvent::ServerDeactivate()
{
	if (gLogApi.EventEnabled(__func__))
	{
		nlohmann::ordered_json Event;

		Event["Event"] = __func__;

		Event["Server"] = gLogApi.GetServerInfo();

		gLogApi.SendEvent(LogApi::Events::ServerDeactivate, Event);
	}
}

void CLogEvent::ServerAlertMessage(ALERT_TYPE aType, const char* szBuffer)
{
	if (gLogApi.EventEnabled(__func__))
	{
		if (szBuffer)
		{
			if (szBuffer[0u] != '\0')
			{
				nlohmann::ordered_json Event;

				Event["Event"] = __func__;

				Event["Server"] = gLogApi.GetServerInfo();

				Event["Type"] = aType;

				Event["Message"] = szBuffer;

				gLogApi.SendEvent(LogApi::Events::ServerAlertMessage, Event);
			}
		}
	}
}

void CLogEvent::ServerInfo()
{
	if (gLogApi.EventEnabled(__func__))
	{
		nlohmann::ordered_json Event;

		Event["Event"] = __func__;

		Event["Server"] = gLogApi.GetServerInfo();

		gLogApi.SendEvent(LogApi::Events::ServerInfo, Event);
	}
}

void CLogEvent::ClientConnect(edict_t* pEdict, const char* pszName, const char* pszAddress, char szRejectReason[128])
{
	if (gLogApi.EventEnabled(__func__))
	{
		nlohmann::ordered_json Event;

		if (!FNullEnt(pEdict))
		{
			Event["Event"] = __func__;

			Event["Server"] = gLogApi.GetServerInfo();

			Event["Player"] = gLogPlayer.GetPlayerJson(pEdict);
		}

		gLogApi.SendEvent(LogApi::Events::ClientConnect, Event);
	}
}

void CLogEvent::ClientPutInServer(edict_t* pEdict)
{
	if (gLogApi.EventEnabled(__func__))
	{
		nlohmann::ordered_json Event;

		if (!FNullEnt(pEdict))
		{
			Event["Event"] = __func__;

			Event["Server"] = gLogApi.GetServerInfo();

			Event["Player"] = gLogPlayer.GetPlayerJson(pEdict);
		}
		
		gLogApi.SendEvent(LogApi::Events::ClientPutInServer, Event);
	}
}

void CLogEvent::ClientDisconnect(edict_t* pEdict, bool Crash, const char* Reason)
{
	if (gLogApi.EventEnabled(__func__))
	{
		nlohmann::ordered_json Event;

		if (!FNullEnt(pEdict))
		{
			Event["Event"] = __func__;

			Event["Server"] = gLogApi.GetServerInfo();

			Event["Player"] = gLogPlayer.GetPlayerJson(pEdict);

			Event["Crash"] = Crash;

			Event["Reason"] = Reason ? Reason : "";
		}

		gLogApi.SendEvent(LogApi::Events::ClientDisconnect, Event);
	}
}

void CLogEvent::ClientKill(edict_t* pEdict)
{
	if (gLogApi.EventEnabled(__func__))
	{
		nlohmann::ordered_json Event;

		if (!FNullEnt(pEdict))
		{
			Event["Event"] = __func__;

			Event["Server"] = gLogApi.GetServerInfo();

			Event["Player"] = gLogPlayer.GetPlayerJson(pEdict);
		}

		gLogApi.SendEvent(LogApi::Events::ClientKill, Event);
	}
}

void CLogEvent::ClientUserInfoChanged(edict_t* pEdict, char* InfoBuffer)
{
	if (gLogApi.EventEnabled(__func__))
	{
		nlohmann::ordered_json Event;

		if (!FNullEnt(pEdict))
		{
			Event["Event"] = __func__;

			Event["Server"] = gLogApi.GetServerInfo();

			Event["Player"] = gLogPlayer.GetPlayerJson(pEdict);

			Event["InfoBuffer"] = InfoBuffer ? InfoBuffer : "";
		}

		gLogApi.SendEvent(LogApi::Events::ClientUserInfoChanged, Event);
	}
}

void CLogEvent::ClientCommand(edict_t* pEdict)
{
	if (gLogApi.EventEnabled(__func__))
	{
		nlohmann::ordered_json Event;

		if (!FNullEnt(pEdict))
		{
			auto Command = g_engfuncs.pfnCmd_Argv(0);

			if (Command)
			{
				if (Command[0u] != '\0')
				{
					Event["Event"] = __func__;

					Event["Server"] = gLogApi.GetServerInfo();

					Event["Player"] = gLogPlayer.GetPlayerJson(pEdict);

					Event["Command"] = Command;

					Event["Args"] = "";

					auto Args = g_engfuncs.pfnCmd_Args();

					if (Args)
					{
						if (Args[0u] != '\0')
						{
							Event["Args"] = Args;
						}
					}
				}
			}
		}

		gLogApi.SendEvent(LogApi::Events::ClientCommand, Event);
	}
}

void CLogEvent::ClientSay(edict_t* pEdict)
{
	if (gLogApi.EventEnabled(__func__))
	{
		nlohmann::ordered_json Event;

		if (!FNullEnt(pEdict))
		{
			std::string Type = g_engfuncs.pfnCmd_Argv(0) ? g_engfuncs.pfnCmd_Argv(0) : "";

			if (!Type.empty())
			{
				if (Type.length() > 0)
				{
					if (Type.compare("say") == 0 || Type.compare("say_team") == 0)
					{
						std::string Message = g_engfuncs.pfnCmd_Args() ? g_engfuncs.pfnCmd_Args() : "";

						if (!Message.empty())
						{
							if (Message.length() > 0)
							{
								Message.erase(std::remove(Message.begin(), Message.end(), '\"'),Message.end());

								Event["Event"] = __func__;

								Event["Server"] = gLogApi.GetServerInfo();

								Event["Player"] = gLogPlayer.GetPlayerJson(pEdict);

								Event["Type"] = Type;

								Event["Message"] = Message;
							}
						}
					}
				}
			}
		}

		gLogApi.SendEvent(LogApi::Events::ClientSay, Event);
	}
}

void CLogEvent::ClientMenuHandle(edict_t* pEdict, std::string Callback, P_MENU_ITEM Item)
{
	if (gLogApi.EventEnabled(__func__))
	{
		if (!Callback.empty())
		{
			nlohmann::ordered_json Event;

			if (!FNullEnt(pEdict))
			{
				Event["Event"] = Callback;

				Event["Server"] = gLogApi.GetServerInfo();

				Event["Player"] = gLogPlayer.GetPlayerJson(pEdict);

				Event["Item"]["Info"] = Item.Info;

				Event["Item"]["Text"] = Item.Text;

				Event["Item"]["Disabled"] = Item.Disabled;

				Event["Item"]["Extra"] = Item.Extra;
			}

			gLogApi.SendEvent(LogApi::Events::ClientMenuHandle, Event);
		}
	}
}
