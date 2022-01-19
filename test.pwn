#include <a_samp>
#include <sscanf2>
#include <Pawn.CMD>
#include <td-message>

#define TDN_MODE_DEFAULT
#include <td-notification> /* https://github.com/sanpezlo/td-notification */

main(){}

CMD:notification(playerid, params[])
{
	new string[256], text[256];
	if(sscanf(params, "s[256]", text)) return SendClientMessage(playerid, -1, "/notification [message]");
	format(string, sizeof(string), "%s", text);
	SendNotification(playerid, string);
	return 1;
}

CMD:message(playerid, params[])
{
	new string[256], text[256];
	if(sscanf(params, "s[256]", text)) return SendClientMessage(playerid, -1, "/message [message]");
	format(string, sizeof(string), "%s", text);
	SendMessage(playerid, string);
	return 1;
}

CMD:textall(playerid, params[])
{
	new string[256], text[256];
	if(sscanf(params, "s[256]", text)) return SendClientMessage(playerid, -1, "/textall [message]");
	format(string, sizeof(string), "%s", text);
	SendNotification(playerid, string);
	SendMessage(playerid, string);
	return 1;
}
