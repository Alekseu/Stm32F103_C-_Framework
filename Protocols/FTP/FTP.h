/*
 * SMB.h
 *
 *  Created on: 23 авг. 2017 г.
 *      Author: hudienko_a
 */

#ifndef FTP_FTP_H_
#define FTP_FTP_H_

//+d=/, nnnnnnnnnnnn, SSSSSSSSSSSS, D, MM, YYYY, s, mm, hh

///todo добавить юзер пароль для ftp в настройки
extern "C"
{
#include "../../Device/w5500/Ethernet/W5500/w5500.h"
#include "../../Device/w5500/Ethernet/socket.h"
#include "string.h"
#include "stdio.h"
#include "../SDCard/inc/ff.h"
#include "../Extention/delay.h"
}

static const char* msg110 ="110 MARK %s = %s.";
static const char* msg120= "120 Service ready in nnn minutes.";
static const char* msg125= "125 Data connection already open; transfer starting.";
static const char* msg150= "150 File status okay; about to open data connection.";
static const char* msg150recv= "150 Opening BINARY mode data connection for %s (%i bytes).";
static const char* msg150stor= "150 Opening BINARY mode data connection for %s.";
static const char* msg200 = "200 Command okay.";
static const char* msg202= "202 Command not implemented, superfluous at this site.";
static const char* msg211= "211 System status, or system help reply.";
static const char* msg212= "212 Directory status.";
static const char* msg213= "213 File status.";
static const char* msg214= "214 %s.";
static const char* msg214SYST= "214 %s system type.";
static const char* msg215= "215 WIN";
static const char* msg220 = "220 Welcome to RiverPark Stand FTP";
static const char* msg221= "221 Goodbye.";
static const char* msg225= "225 Data connection open; no transfer in progress.";
static const char* msg226= "226 Closing data connection.";
static const char* msg227= "227 Entering Passive Mode (%i,%i,%i,%i,%i,%i).";
static const char* msg230= "230 User logged in, proceed.";
static const char* msg250= "250 Requested file action okay, completed.";
static const char* msg257PWD= "257 \"%s\" is current directory.";
static const char* msg257= "257 \"%s\" created.";
static const char* msg331= "331 User name okay, need password.";
static const char* msg332= "332 Need account for login.";
static const char* msg350= "350 Requested file action pending further information.";
static const char* msg421= "421 Service not available, closing control connection.";
static const char* msg425= "425 Can't open data connection.";
static const char* msg426= "426 Connection closed; transfer aborted.";
static const char* msg450= "450 Requested file action not taken.";
static const char* msg451= "451 Requested action aborted: local error in processing.";
static const char* msg452= "452 Requested action not taken.";
static const char* msg500= "500 Syntax error, command unrecognized.";
static const char* msg501= "501 Syntax error in parameters or arguments.";
static const char* msg502= "502 Command not implemented.";
static const char* msg503= "503 Bad sequence of commands.";
static const char* msg504= "504 Command not implemented for that parameter.";
static const char* msg530= "530 Not logged in.";
static const char* msg532= "532 Need account for storing files.";
static const char* msg550= "550 Requested action not taken.";
static const char* msg551= "551 Requested action aborted: page type unknown.";
static const char* msg552= "552 Requested file action aborted.";
static const char* msg553= "553 Requested action not taken.";
static const char* pasv= "227 Entering Passive Mode (192,168,0,100,4,31).";


class FTP
{

public:
	enum Command
	{
		ABOR, //Прервать передачу файла
		CDUP, //Сменить директорию на вышестоящую.
		CWD, // Сменить директорию.
		DELE, //Удалить файл (DELE filename).
		EPSV, //Войти в расширенный пассивный режим. Применяется вместо PASV.
		HELP, //Выводит список команд, принимаемых сервером.
		LIST, //Возвращает список файлов директории. Список передаётся через соединение данных.
		MDTM, //Возвращает время модификации файла.
		MKD, //Создать директорию.
		NLST,//Возвращает список файлов директории в более кратком формате, чем LIST. Список передаётся через соединение данных.
		NOOP, //Пустая операция.
		PASS, //Пароль.
		PASV, //Войти в пассивный режим. Сервер вернёт адрес и порт, к которому нужно подключиться, чтобы забрать данные. Передача начнётся при введении следующих команд: RETR, LIST и т. д.
		PORT, //Войти в активный режим. Например PORT 12,34,45,56,78,89. В отличие от пассивного режима для передачи данных сервер сам подключается к клиенту.
		PWD, //Возвращает текущую директорию.
		QUIT, // Отключиться.
		REIN, //Реинициализировать подключение.
		RETR, // Скачать файл. Перед RETR должна быть команда PASV или PORT.
		RMD, //Удалить директорию.
		SIZE, //Возвращает размер файла.
		STOR, //Закачать файл. Перед STOR должна быть команда PASV или PORT.
		SYST, // Возвращает тип системы (UNIX, WIN, …).
		TYPE, //Установить тип передачи файла (бинарный, текстовый).
		USER, //Имя пользователя для входа на сервер.
		OPTS,
		FEAT,
		RNFR,
		RNTO,
		RECEIVING_FILE,
		SENDING_FILE,
		SENDING_LIST

	};

	void init()
	{
		_port =1055;
		_socket  = socket(6,Sn_MR_TCP,21,0);
		_socket2 = socket(7,Sn_MR_TCP,_port,0);
		_socket3 = socket(5,Sn_MR_TCP,_port,0);
		listen(_socket);
		listen(_socket2);
		cwdName[0]='/';
		cwdName[1]=1;
	}

	void periodic()
	{
		int len=0;

		if(_counter++>=3)
		{
			_counter=0;
			soc_status1 = getSn_SR(_socket);

			if(((com==RECEIVING_FILE) || (com ==SENDING_FILE) || (com ==SENDING_LIST) )&& soc_status1==SOCK_ESTABLISHED )
			{
				CommandWoker(_socket,0,0);
			}
			else
			{
				switch(soc_status1)
					{
					case SOCK_CLOSED:
						_socket  = socket(_socket,Sn_MR_TCP,21,0);
						listen(_socket);
						welcome = false;
						break;
					case SOCK_INIT:

						break;
					case SOCK_LISTEN:

						break;

					case SOCK_ESTABLISHED:
						if(!welcome)
						{
							welcome = true;
							SendData(_socket,msg220);
						}

						len= getSn_RX_RSR(_socket);
						if(len >0)
						{
							CommandParcer(_socket ,len);
						}

						break;
					case SOCK_CLOSE_WAIT:
						close(_socket);
						break;
					}
			}



			soc_status2 = getSn_SR(_socket2);

			switch(soc_status2)
			{
			case SOCK_CLOSED:
				_socket2 = socket(_socket2,Sn_MR_TCP,_port,0);
				listen(_socket2);
				break;
			case SOCK_INIT:

				break;
			case SOCK_LISTEN:

				break;

			case SOCK_ESTABLISHED:

				len= getSn_RX_RSR(_socket2);
				if(len >0)
				{
					int pos=0;
					len = recv(_socket2,gDATABUF,len);
					send(_socket2,(uint8_t*)"200 Atata.\r\n",12);
				}

				break;
			case SOCK_CLOSE_WAIT:
				close(_socket2);
				break;
			}


			soc_status3 = getSn_SR(_socket3);

			switch(soc_status2)
			{
			case SOCK_CLOSED:
				_socket3 = socket(5,Sn_MR_TCP,_port,0);
				break;
			case SOCK_INIT:

				break;
			case SOCK_LISTEN:

				break;

			case SOCK_ESTABLISHED:

				break;
			case SOCK_CLOSE_WAIT:
				close(_socket3);
				break;
			}
		}

	}

	void CommandParcer( int socket , int len)
	{
		com = NOOP;
		memset(gDATABUF,0,len+10);
		int _len = recv(socket,gDATABUF,len);

		uint8_t* command=0;
		uint8_t command_length=0;

		uint8_t* data=0;
		uint8_t data_length=0;
		bool start=false;
		bool command_found =false;



		for(int i=0;i<len;i++)
		{
			if(!start)
			{
				command = &gDATABUF[i];
				start=true;
			}
			if((gDATABUF[i]==' ' || gDATABUF[i]=='\r')&& !command_found)
			{
				command_found =true;
				command_length = i-1;
				data = &gDATABUF[i+1];

				switch(command[0])
				{
				case 'A':
					com = ABOR;
					break;
				case 'C':
					if(command[1]=='D'){com=CDUP;}
					if(command[1]=='W'){com=CWD;}
					break;
				case 'D':
					com=DELE;
					break;
				case 'M':
					if(command[1]=='D'){com=MDTM;}
					if(command[1]=='K'){com=MKD;}
					break;
				case 'L':
					com=LIST;
					break;
				case 'E':
					break;
				case 'P':
					if(command[1]=='A' && command[2]=='S'&& command[3]=='S'){com=PASS;}
					if(command[1]=='A' && command[2]=='S'&& command[3]=='V'){com=PASV;}
					if(command[1]=='O'){com=PORT;}
					if(command[1]=='W'){com=PWD;}
					break;
				case 'N':
					com=NOOP;
					break;
				case 'F':
					com = FEAT;
					break;
				case 'H':
					com = HELP ;
					break;
				case 'R':
					if(command[1]=='E'){com=RETR;}
					if(command[1]=='M'){com=RMD;}
					if(command[1]=='N'&& command[2]=='F'){com=RNFR;}
					if(command[1]=='N'&& command[2]=='T'){com=RNTO;}
					break;
				case 'S':
					if(command[1]=='I'){com=SIZE;}
					if(command[1]=='T'){com=STOR;}
					if(command[1]=='Y'){com=SYST;}
					break;
				case 'T':
					com = TYPE;
					break;
				case 'U':
					com = USER ;
					break;
				case 'O':
					com = OPTS;
					break;
				case 'Q':
					com=QUIT;
					break;
				}

			}
			if(gDATABUF[i]=='\r' && gDATABUF[i+1]=='\n')
			{
				data_length = i-command_length-3;
				CommandWoker(socket,data, data_length);
				start  =false;
				command_found =false;
			}
		}

	}

	void CommandWoker(int socket, uint8_t* data, int size )
	{

		bool ok = false;
		int len=1;

		switch(com)
		{
		case USER:
			SendData(socket,msg230);

			break;

		case QUIT:
			SendData(socket,msg221);

			break;

		case PORT:
			nr = sscanf((const char*)data, "%u,%u,%u,%u,%u,%u", &(ip[0]), &(ip[1]), &(ip[2]), &(ip[3]), &pHi, &pLo);
			if (nr != 6) {
				SendData(socket,msg501);
			} else {
				SendData(socket, msg200);
			}

			break;
		case PASV:
			SendData(socket,pasv);

			break;

		case LIST:
			if(  !fs_opendir( & dir, cwdName ))
			{
				SendData(socket,msg550);
			}
			else if( dataConnect())
			{
				SendData(socket,"150 Accepted data connection" );

				com = SENDING_LIST;
			}
			else
			{
				SendData(socket, "500 Error " );
				com=NOOP;
			}

			break;

		case SYST:
			SendData(socket,msg215);

			break;

		case PWD:
			SendData(socket, (char*)"257 \"",5 );
			SendData(socket, (char*)cwdName ,strlen(cwdName));
			SendData(socket, "\" is your current directory" );

			break;

		case CWD:
			if( fs_exists( (char*)data ))
			{
				memset(cwdName,0,sizeof(cwdName));
				strcpy(cwdName, (char*)data);
				cwdName[size+2]=0;
				//cwdName[size+1]=0;
				memset ((char*)&dir, 0, sizeof(DIR));
				fs_opendir(&dir, cwdName);
				SendData(socket, "250 Directory successfully changed." );
			}

			else
			{
				SendData(socket ,"550 Failed to change directory." );
			}

			break;

		case CDUP:
			if( strlen( cwdName ) > 1 )  // do nothing if cwdName is root
			{
				// if cwdName ends with '/', remove it (must not append)
				if( cwdName[ strlen( cwdName ) - 1 ] == '/' )
					cwdName[ strlen( cwdName ) - 1 ] = 0;
				// search last '/'
				char * pSep = strrchr( cwdName, '/' );
				ok = pSep > cwdName;
				// if found, ends the string on its position
				if( ok )
				{
					* pSep = 0;
					ok = fs_exists( cwdName );
				}
			}
			// if an error appends, move to root
			if( ! ok )
				strcpy( cwdName, "/" );
			SendData(socket, "200 Ok. Current directory is " );
			SendData(socket, cwdName );

			break;

		case MKD:
			memcpy( path, cwdName ,(strlen(cwdName)-1));
			memcpy( path+(strlen(cwdName)-1), "/",1 );
			memcpy( path+(strlen(cwdName)-1)+1, (char*)data ,size+2);
			if(f_mkdir(path)==FR_OK)
			{
				SendData(socket, "250 Makedir OK " );
			}
			else
			{
				SendData(socket, "500 Error make dir" );
			}

			com=NOOP;
			break;

		case RMD:
			memcpy( path, cwdName ,(strlen(cwdName)-1));
			memcpy( path+(strlen(cwdName)-1), "/",1 );
			memcpy( path+(strlen(cwdName)-1)+1, (char*)data ,size+2);
			if(f_unlink(path)==FR_OK)
			{
				SendData(socket, "250 Removed dir OK " );
			}
			else
			{
				SendData(socket, "500 Error remove dir" );
			}
			com=NOOP;
			break;

		case RNFR:
			memcpy( path, cwdName ,(strlen(cwdName)-1));
			memcpy( path+(strlen(cwdName)-1), "/",1 );
			memcpy( path+(strlen(cwdName)-1)+1, (char*)data ,size+2);

			SendData(socket, "250 Rename from OK " );

			break;

		case RNTO:

			memcpy( path, cwdName ,(strlen(cwdName)-1));
						memcpy( path+(strlen(cwdName)-1), "/",1 );
						memcpy( path+(strlen(cwdName)-1)+1, (char*)data ,size+2);

						SendData(socket, "250 Rename to OK " );

			break;

		case TYPE:
			if( data[0]== 'A' )
				SendData(socket, "200 TYPE is now ASII" );
			else if(data[0]== 'I' )
				SendData(socket,"200 TYPE is now 8-bit binary" );
			else
				SendData(socket, "504 Unknow TYPE" );

			com=NOOP;
			break;

		case STOR:
			memcpy( path, cwdName ,(strlen(cwdName)-1));
			memcpy( path+(strlen(cwdName)-1), "/",1 );
			memcpy( path+(strlen(cwdName)-1)+1, (char*)data ,size+2);
			if( f_open( & file, (char*)path, FA_CREATE_ALWAYS | FA_WRITE ) != FR_OK )
			{
				SendData(socket, "451 Can't open/create " );
				SendData(socket, (char*)data );
			}
			else if(! dataConnect())
			{
				f_close( & file );
				SendData(socket, "500 Error " );
				com=NOOP;
			}
			else
			{
				SendData(socket, "150 Connected to port " );
				com = RECEIVING_FILE;
				nw = 1;

			}

			break;

		case RETR:
			memcpy( path, cwdName ,(strlen(cwdName)-1));
			memcpy( path+(strlen(cwdName)-1), "/",1 );
			memcpy( path+(strlen(cwdName)-1)+1, (char*)data ,size+2);
			if( f_open( & file, path, FA_READ ) != FR_OK )
			{
				SendData(socket, "451 Can't open/create " );
				SendData(socket, (char*)data );
			}
			else if(! dataConnect())
			{
				f_close( & file );
				SendData(socket, "500 Error " );
				com=NOOP;
			}
			else
			{
				nb = 1;
				SendData(socket, "150 Connected to port " );
				com = SENDING_FILE;
			}

			break;

		case DELE:
			memcpy( path, cwdName ,(strlen(cwdName)-1));
			memcpy( path+(strlen(cwdName)-1), "/",1 );
			memcpy( path+(strlen(cwdName)-1)+1, (char*)data ,size+2);
			if( f_unlink(path)==FR_OK)
			{
				SendData(socket, "250 Delete file OK." );
			}
			else
			{
				SendData(socket, "500 Error " );
			}
			com=NOOP;
			break;

		case SIZE:
			memcpy( path, cwdName ,(strlen(cwdName)-1));
			memcpy( path+(strlen(cwdName)-1), "/",1 );
			memcpy( path+(strlen(cwdName)-1)+1, (char*)data ,size+2);
			if( f_open( & file, path, FA_READ ) != FR_OK )
			{
				SendData(socket, "451 Can't open/create " );
				SendData(socket, (char*)data );
			}
			else
			{
				SendData(socket, (char*)"250 ",4 );
				SendData(socket, i2str(f_size(& file)));

			}
			f_close( & file);

			break;

		case NOOP:
			break;

		case RECEIVING_FILE:

				len =dataRead();

				f_write(&file, &buff, len, (UINT *) & nw);
				if(_syncCounter++>=5)
				{
					_syncCounter=0;
					f_sync(&file);
					int a=0;
				}

				if (len<=0 && nw<=0)
				{
					f_close(&file);
					SendData(socket,"226 File receive OK." );
					dataClose();
					com=NOOP;
				}
				else
				{
					com = RECEIVING_FILE;
				}

			break;

		case SENDING_FILE:

			nb=0;
			result = f_read( & file, buff, sizeof(buff), (UINT *) & nb );
			if(result == FR_OK)
			{

				dataWrite(buff, nb );
				com = SENDING_FILE;
				if(nb<sizeof(buff))
				{
					nb=0;
				}
			}
			else
			{
				SendData(socket, "500 Error " );
				nb=0;
			}

			if( nb <= 0 )
			{
				f_close(&file);
				SendData(socket,"226 File send OK." );
				dataClose();
				com=NOOP;
			}
			break;

		case SENDING_LIST:


			result = f_readdir(&dir, &finfo);
			if ((result == FR_OK) && finfo.fname[0]!=0)
			{

				if( finfo.fname[0] == '.' )
					break;

				if( finfo.fattrib & AM_DIR )
				{
					strcpy( buff, "+/,\t" );
					strcat( buff, lfn[0] == 0 ? finfo.fname : lfn );
					if(strlen(finfo.fname)<12)
					{
						for(int i=0;i<12-strlen(finfo.fname);i++)
						{
							strcat( buff, " " );
						}
					}
				}
				else
				{
					strcpy( buff, "+r,\t" );
					strcat( buff, lfn[0] == 0 ? finfo.fname : lfn );
					if(strlen(finfo.fname)<12)
					{
						for(int i=0;i<12-strlen(finfo.fname);i++)
						{
							strcat( buff, " " );
						}
					}
				}
				strcat( buff, ",\t" );
				strcat( buff, i2str( finfo.fsize ));
				if(strlen(i2str( finfo.fsize ))<12)
				{
					for(int i=0;i<12-strlen(i2str( finfo.fsize ));i++)
					{
						strcat( buff, " " );
					}
				}
				strcat( buff, ",\t" );
				strcat( buff, i2str( finfo.fdate&0x0F ));
				strcat( buff, ",\t" );
				strcat( buff, i2str( finfo.fdate>>4&0x0F ));
				strcat( buff, ",\t" );
				strcat( buff, i2str(1980+((finfo.fdate>>8)&0xFF) ));

				strcat( buff, ",\t" );
				strcat( buff, i2str( finfo.ftime&0x0F ));
				strcat( buff, ",\t" );
				strcat( buff, i2str( finfo.fdate>>4&0x3F ));
				strcat( buff, ",\t" );
				strcat( buff, i2str((finfo.fdate>>10)&0x1F));

				strcat( buff, "\r\n" );
				dataWrite( buff );
				com=SENDING_LIST;
			}
			else
			{
			dataClose();
			SendData(socket,"226 Directory send OK." );
			memset((char*)&finfo,0,sizeof(FILINFO));
			com = NOOP;
			}

			break;

		default:
			SendData(socket,msg200);
			com=NOOP;
			break;
		}


	}

	bool dataConnect()
	{
		if(ip[0]!=0&&ip[1]!=0 )
		{
			connect(_socket3,ip,((uint16_t) pHi << 8) | (uint16_t) pLo);
			unsigned char soc_status =0;
			int timeout = 100;
			while(soc_status!=SOCK_ESTABLISHED && --timeout>0)
			{
				soc_status = getSn_SR(_socket);
				_delay_ms(250);
			}
			if(timeout>0) return true;
			else
				return false;
		}
		else if(soc_status2 == SOCK_ESTABLISHED)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void dataWrite(char* data )
	{
		send(_socket3,(uint8_t*)data,strlen(data));
	}

	void dataWrite(char* data, int length )
	{
		send(_socket3,(uint8_t*)data,length);
	}

	int dataRead()
	{
		int len =0;
		len=getSn_RX_RSR(_socket3);
		int timeout = 100;
		while(len==0 && --timeout>0 )
		{
			len=getSn_RX_RSR(_socket3);
			_delay_ms(1);
		}

		if(len>0)
		{
			if(len>sizeof(buff)) len =sizeof(buff)-1;
			len = recv(_socket3,(uint8_t*)buff,len);
		}
		return len;
	}

	void dataClose()
	{
		disconnect(_socket3);
		_socket3 = socket(5,Sn_MR_TCP,_port,0);
	}

	void SendData(int socketNum, const char* data)
	{
		send(socketNum,(uint8_t*)data,strlen(data));
		send(socketNum,(uint8_t*)"\r\n",2);
	}

	void SendData(int socketNum, char* data , int size)
	{
		send(socketNum,(uint8_t*)data,size);
	}

	char * i2strZ( char * s, uint32_t i, uint8_t z )
	{
		char * psi = s + ( z > 0 ? z : sizeof( s ));

		* -- psi = 0;
		if( i == 0 )
			* -- psi = '0';
		for( ; i; i /= 10 )
			* -- psi = '0' + i% 10;
		if( z > 0 )
			while( psi > s )
				* -- psi = '0';
		return psi;
	}

	char * i2str( uint32_t i )
	{
		return i2strZ( str, i, 0 );
	}

	bool fs_exists( char * path )
	{
		if(  path[0]=='/'  )
			return true;

		char *  path0 = path;

		return f_stat( path0, & finfo ) == FR_OK;
	}

	bool fs_opendir( DIR * pdir, char * dirName )
	{
		char * dirName0 = dirName;
		uint8_t ffs_result;

		ffs_result = f_opendir( pdir, dirName0 );
		return ffs_result == FR_OK;
	}

	bool makePath( char * fullName )
	{
		strcpy( cwdName, fullName );
		return true;
	}

private:

	short _port;
	int _socket;
	int _socket2;
	int _socket3;
	bool welcome;
	uint8_t gDATABUF[512];
	char buff[1024];
	char cwdName[128];

	uint8_t connectaddr[32];
	char lfn[64];
	char path[64];

	int nr;
	unsigned pHi, pLo;
	unsigned char ip[4];
	char      str[ 15 ];

	uint16_t nm = 0;
	DIR dir;
	FILINFO   finfo;
	FIL file;
	FRESULT result;

	unsigned char soc_status1;
	unsigned char soc_status2;
	unsigned char soc_status3;

	uint16_t nw;
	uint16_t nb;
	Command com;
	int _counter;
	int _syncCounter;
};


#endif /* FTP_FTP_H_ */
