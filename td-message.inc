/* original code: https://github.com/sanpezlo/td-notification */

#include <a_samp>
#include <sscanf2> /* https://github.com/maddinat0r/sscanf/releases */
#include <Pawn.CMD> /* https://github.com/katursis/Pawn.CMD/releases */
#include <td-string-width> /* https://github.com/kristoisberg/samp-td-string-width */
#define FILTERSCRIPT

//- Macros
#define funcion%0(%1)                   forward%0(%1); public%0(%1)

//- Datos TextDraw notification
#define MAX_NT 							4
#define NT_POS_X 						9.000000	//7.000000
#define NT_POS_Y 						135.000000 	//135.000000
#define NT_FONT 						1
#define NT_LETTER_SIZE_X 				0.174999 	//0.175000
#define NT_LETTER_SIZE_Y 				1.100000 	//1.100000
#define NT_SIZE 						125.000000 	//112.000000
#define NT_COLOR 						0xFFFFFFFF
#define NT_COLOR_BOX 					0x00000077
#define NT_PROPORTIONAL 				1
#define NT_DISTANCE  					5
#define NT_TIME  				 		8000
#define MAX_NT_TEXT 					800
#define NT_MODE_DOWN

enum InfoNotification
{
    Use,
    Line,
    Text[MAX_NT_TEXT],
    PlayerText:TextDraw,
    Float:MinPosY,
    Float:MaxPosY,
    Hide
}
new tiNotification[MAX_PLAYERS][MAX_NT][InfoNotification];

//- Funciones
funcion SendMessage(playerid, const reason[]) //SendNotification
{
    for(new cycle; cycle < MAX_NT; cycle++)
    {
        if(!tiNotification[playerid][cycle][Use])
        {
            tiNotification[playerid][cycle][Text][0] = EOS;
            new text[MAX_NT_TEXT];
            for(new len = strlen(reason), pos; pos < len; pos ++)
            {
                switch(reason[pos])
                {
                    case 'à': text[pos] = 151;
                    case 'á': text[pos] = 152;
                    case 'â': text[pos] = 153;
                    case 'ä': text[pos] = 154;
                    case 'À': text[pos] = 128;
                    case 'Á': text[pos] = 129;
                    case 'Â': text[pos] = 130;
                    case 'Ä': text[pos] = 131;
                    case 'è': text[pos] = 157;
                    case 'é': text[pos] = 158;
                    case 'ê': text[pos] = 159;
                    case 'ë': text[pos] = 160;
                    case 'È': text[pos] = 134;
                    case 'É': text[pos] = 135;
                    case 'Ê': text[pos] = 136;
                    case 'Ë': text[pos] = 137;
                    case 'ì': text[pos] = 161;
                    case 'í': text[pos] = 162;
                    case 'î': text[pos] = 163;
                    case 'ï': text[pos] = 164;
                    case 'Ì': text[pos] = 138;
                    case 'Í': text[pos] = 139;
                    case 'Î': text[pos] = 140;
                    case 'Ï': text[pos] = 141;
                    case 'ò': text[pos] = 165;
                    case 'ó': text[pos] = 166;
                    case 'ô': text[pos] = 167;
                    case 'ö': text[pos] = 168;
                    case 'Ò': text[pos] = 142;
                    case 'Ó': text[pos] = 143;
                    case 'Ô': text[pos] = 144;
                    case 'Ö': text[pos] = 145;
                    case 'ù': text[pos] = 169;
                    case 'ú': text[pos] = 170;
                    case 'û': text[pos] = 171;
                    case 'ü': text[pos] = 172;
                    case 'Ù': text[pos] = 146;
                    case 'Ú': text[pos] = 147;
                    case 'Û': text[pos] = 148;
                    case 'Ü': text[pos] = 149;
                    case 'ñ': text[pos] = 174;
                    case 'Ñ': text[pos] = 173;
                    case '¡': text[pos] = 64;
                    case '¿': text[pos] = 175;
                    case '`': text[pos] = 177;
                    case '&': text[pos] = 38;
                    default:  text[pos] = reason[pos];
                }
            }

            strcat(tiNotification[playerid][cycle][Text], text, MAX_NT_TEXT);

            tiNotification[playerid][cycle][Use] = 1;

            LinesNotification(playerid, cycle);

            #if defined NT_MODE_DOWN

            MinPosYTDNotification(playerid, cycle);
            MaxPosYTDNotification(playerid, cycle);

            #endif

            #if defined NT_MODE_UP

            MaxPosYTDNotification(playerid, cycle);
            MinPosYTDNotification(playerid, cycle);

            #endif

            tiNotification[playerid][cycle][Hide] = -1;

            CreateNotification(playerid, cycle);

            SetTimerEx("TimerHideNotification", NT_TIME, false, "i", playerid);

            return 1;
        }
    }
    return -1;
}

funcion TimerHideNotification(playerid)
{
    for(new cycle; cycle < MAX_NT; cycle++)
    {
        if(tiNotification[playerid][cycle][Hide] == -1)
        {
            tiNotification[playerid][cycle][Use] = 0;
            if(tiNotification[playerid][cycle][TextDraw] != PlayerText:-1)
            {
                PlayerTextDrawDestroy(playerid, tiNotification[playerid][cycle][TextDraw]);
                tiNotification[playerid][cycle][Line] = 0;
                tiNotification[playerid][cycle][Text][0] = EOS;
                tiNotification[playerid][cycle][MinPosY] = 0;
                tiNotification[playerid][cycle][MaxPosY] = 0;
                tiNotification[playerid][cycle][TextDraw] = PlayerText:-1;
            }
            tiNotification[playerid][cycle][Hide] = -1;
            UpdateNotification(playerid);

            return 1;
        }
    }
    return 0;
}

//- stocks
stock CreateNotification(playerid, NT)
{
    if(tiNotification[playerid][NT][Use] == 1)
    {
        if(tiNotification[playerid][NT][TextDraw] != PlayerText:-1)
        {
            PlayerTextDrawDestroy(playerid, tiNotification[playerid][NT][TextDraw]);
        }
        tiNotification[playerid][NT][TextDraw] = CreatePlayerTextDraw(playerid, NT_POS_X, tiNotification[playerid][NT][MinPosY], tiNotification[playerid][NT][Text]);
        PlayerTextDrawFont(playerid, tiNotification[playerid][NT][TextDraw], NT_FONT);
        PlayerTextDrawLetterSize(playerid, tiNotification[playerid][NT][TextDraw], NT_LETTER_SIZE_X, NT_LETTER_SIZE_Y);
        PlayerTextDrawTextSize(playerid, tiNotification[playerid][NT][TextDraw], floatadd(NT_POS_X, NT_SIZE), 1.000000);
        PlayerTextDrawSetOutline(playerid, tiNotification[playerid][NT][TextDraw], 1);
        PlayerTextDrawSetShadow(playerid, tiNotification[playerid][NT][TextDraw], 0);
        PlayerTextDrawAlignment(playerid, tiNotification[playerid][NT][TextDraw], 1);
        PlayerTextDrawColor(playerid, tiNotification[playerid][NT][TextDraw], NT_COLOR);
        PlayerTextDrawBackgroundColor(playerid, tiNotification[playerid][NT][TextDraw], NT_COLOR_BOX);
        PlayerTextDrawBoxColor(playerid, tiNotification[playerid][NT][TextDraw], NT_COLOR_BOX);
        PlayerTextDrawUseBox(playerid, tiNotification[playerid][NT][TextDraw], 1);
        PlayerTextDrawSetProportional(playerid, tiNotification[playerid][NT][TextDraw], NT_PROPORTIONAL);
        PlayerTextDrawSetSelectable(playerid, tiNotification[playerid][NT][TextDraw], 0);
        PlayerTextDrawShow(playerid, tiNotification[playerid][NT][TextDraw]);
    }
    return 1;
}

stock UpdateNotification(playerid)
{
    for(new cycle = 0; cycle < MAX_NT; cycle ++)
    {
        if(!tiNotification[playerid][cycle][Use])
        {
            if(cycle != MAX_NT - 1)
            {
                if(tiNotification[playerid][cycle + 1][Use])
                {
                    tiNotification[playerid][cycle][Use] = tiNotification[playerid][cycle + 1][Use];
                    tiNotification[playerid][cycle][Line] = tiNotification[playerid][cycle + 1][Line];
                    strcat(tiNotification[playerid][cycle][Text], tiNotification[playerid][cycle + 1][Text], MAX_NT_TEXT);
                    tiNotification[playerid][cycle][TextDraw] = tiNotification[playerid][cycle + 1][TextDraw];
                    tiNotification[playerid][cycle][Hide] = tiNotification[playerid][cycle + 1][Hide];

                    tiNotification[playerid][cycle + 1][Use] = 0;
                    tiNotification[playerid][cycle + 1][Line] = 0;
                    tiNotification[playerid][cycle + 1][Text][0] = EOS;
                    tiNotification[playerid][cycle + 1][TextDraw] = PlayerText:-1;
                    tiNotification[playerid][cycle + 1][MinPosY] = 0;
                    tiNotification[playerid][cycle + 1][MaxPosY] = 0;
                    tiNotification[playerid][cycle + 1][Hide] = -1;

                    #if defined NT_MODE_DOWN

                    MinPosYTDNotification(playerid, cycle);
                    MaxPosYTDNotification(playerid, cycle);

                    #endif

                    #if defined NT_MODE_UP

                    MaxPosYTDNotification(playerid, cycle);
                    MinPosYTDNotification(playerid, cycle);

                    #endif
                }
            }
        }
        else if(tiNotification[playerid][cycle][Use])
        {
            if(cycle != 0)
            {
                if(!tiNotification[playerid][cycle - 1][Use])
                {
                    tiNotification[playerid][cycle - 1][Use] = tiNotification[playerid][cycle][Use];
                    tiNotification[playerid][cycle - 1][Line] = tiNotification[playerid][cycle][Line];
                    strcat(tiNotification[playerid][cycle - 1][Text], tiNotification[playerid][cycle][Text], MAX_NT_TEXT);
                    tiNotification[playerid][cycle - 1][TextDraw] = tiNotification[playerid][cycle][TextDraw];
                    tiNotification[playerid][cycle - 1][Hide] = tiNotification[playerid][cycle][Hide];

                    tiNotification[playerid][cycle][Use] = 0;
                    tiNotification[playerid][cycle][Line] = 0;
                    tiNotification[playerid][cycle][Text][0] = EOS;
                    tiNotification[playerid][cycle][TextDraw] = PlayerText:-1;
                    tiNotification[playerid][cycle][MinPosY] = 0;
                    tiNotification[playerid][cycle][MaxPosY] = 0;
                    tiNotification[playerid][cycle][Hide] = -1;

                    #if defined NT_MODE_DOWN

                    MinPosYTDNotification(playerid, cycle - 1);
                    MaxPosYTDNotification(playerid, cycle - 1);

                    #endif

                    #if defined NT_MODE_UP

                    MaxPosYTDNotification(playerid, cycle - 1);
                    MinPosYTDNotification(playerid, cycle - 1);

                    #endif
                }
            }
        }
        CreateNotification(playerid, cycle);
    }
    return 1;
}
stock MinPosYTDNotification(playerid, NT)
{
    #if defined NT_MODE_DOWN

    if(NT == 0)
    {
        tiNotification[playerid][NT][MinPosY] = NT_POS_Y;
    }
    else
    {
        tiNotification[playerid][NT][MinPosY] = tiNotification[playerid][NT - 1][MaxPosY] + NT_DISTANCE;
    }
    return 1;

    #endif

    #if defined NT_MODE_UP

    tiNotification[playerid][NT][MinPosY] = tiNotification[playerid][NT][MaxPosY] - ((NT_LETTER_SIZE_Y * 2) + 2) - ((NT_LETTER_SIZE_Y * 5.75) * tiNotification[playerid][NT][Line]) - ((tiNotification[playerid][NT][Line] - 1) * ((NT_LETTER_SIZE_Y * 2) + 2 + NT_LETTER_SIZE_Y)) - (NT_LETTER_SIZE_Y + 3);
    return 1;

    #endif
}

stock MaxPosYTDNotification(playerid, NT)
{
    #if defined NT_MODE_DOWN

    tiNotification[playerid][NT][MaxPosY] = tiNotification[playerid][NT][MinPosY] + (NT_LETTER_SIZE_Y * 2) + 2 + (NT_LETTER_SIZE_Y * 5.75 * tiNotification[playerid][NT][Line]) + ((tiNotification[playerid][NT][Line] - 1) * ((NT_LETTER_SIZE_Y * 2) + 2 + NT_LETTER_SIZE_Y)) + NT_LETTER_SIZE_Y + 3;
    return 1;

    #endif

    #if defined NT_MODE_UP

    if(NT == 0)
    {
        tiNotification[playerid][NT][MaxPosY] = NT_POS_Y;
    }
    else
    {
        tiNotification[playerid][NT][MaxPosY] = tiNotification[playerid][NT - 1][MinPosY] - NT_DISTANCE;
    }
    return 1;

    #endif
}

stock LinesNotification(playerid, NT)
{
    new lines = 1, Float:width, lastspace = -1, supr, len = strlen(tiNotification[playerid][NT][Text]);
    for(new i = 0; i < len; i ++)
    {
        if(tiNotification[playerid][NT][Text][i] == '~')
        {
            if(supr == 0)
            {
                supr = 1;
                if(tiNotification[playerid][NT][Text][i+2] != '~') SendClientMessage(playerid, -1, "Error: '~' used incorrectly'");
            }
            else if(supr == 1) supr = 0;
        }
        else
        {
            if(supr == 1)
            {
                if(tiNotification[playerid][NT][Text][i] == 'n')
                {
                    lines ++;
                    lastspace = -1;
                    width = 0;
                }
            }
            else
            {
                if(tiNotification[playerid][NT][Text][i] == ' ') lastspace = i;
                width += NT_LETTER_SIZE_X * GetTextDrawCharacterWidth(tiNotification[playerid][NT][Text][i], NT_FONT, bool:NT_PROPORTIONAL);
                if(width > NT_SIZE - 3)
                {
                    if(lastspace != i && lastspace != -1)
                    {
                        lines ++;
                        i = lastspace;
                        lastspace = -1;
                        width = 0;
                    }
                    else
                    {
                        lines ++;
                        lastspace = -1;
                        width = 0;
                    }
                }
            }
        }
    }
    tiNotification[playerid][NT][Line] = lines;
    return 1;
}

//- OnGameModeInit
public OnGameModeInit()
{
    for(new playerid = 0; playerid < MAX_PLAYERS; playerid++)
    {
        for(new NT = 0; NT < MAX_NT; NT++)
        {
            tiNotification[playerid][NT][TextDraw] = PlayerText:-1;
            tiNotification[playerid][NT][Hide] = -1;
        }
    }
    #if defined NT_OnGameModeInit
		return NT_OnGameModeInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeInit //
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit //
#endif

#define OnGameModeInit NT_OnGameModeInit
#if defined NT_OnGameModeInit
	forward NT_OnGameModeInit();
#endif

//- OnFilterScriptInit
public OnFilterScriptInit()
{
    for(new playerid = 0; playerid < MAX_PLAYERS; playerid++)
    {
        for(new NT = 0; NT < MAX_NT; NT++)
        {
            tiNotification[playerid][NT][TextDraw] = PlayerText:-1;
            tiNotification[playerid][NT][Hide] = -1;
        }
    }
    #if defined NT_OnFilterScriptInit
		return NT_OnFilterScriptInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnFilterScriptInit
	#undef OnFilterScriptInit
#else
	#define _ALS_OnFilterScriptInit
#endif

#define OnFilterScriptInit NT_OnFilterScriptInit
#if defined NT_OnFilterScriptInit
	forward NT_OnFilterScriptInit();
#endif

//- OnPlayerDisconnect
public OnPlayerDisconnect(playerid, reason)
{
    for(new cycle; cycle < MAX_NT; cycle++)
    {
        tiNotification[playerid][cycle][Use] = 0;
        tiNotification[playerid][cycle][Line] = 0;
        tiNotification[playerid][cycle][Text][0] = EOS;
        tiNotification[playerid][cycle][MinPosY] = 0;
        tiNotification[playerid][cycle][MaxPosY] = 0;
        tiNotification[playerid][cycle][Hide] = -1;
        tiNotification[playerid][cycle][TextDraw] = PlayerText:-1;
    }
    #if defined NT_OnPlayerDisconnect
		return NT_OnPlayerDisconnect(playerid, reason);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect NT_OnPlayerDisconnect
#if defined NT_OnPlayerDisconnect
	forward NT_OnPlayerDisconnect(playerid, reason);
#endif
