///*
// * Http.h
// *
// *  Created on: 17 окт. 2016 г.
// *      Author: hudienko_a
// */
//
//#ifndef NET_HTTP_H_
//#define NET_HTTP_H_
//
//#include "Stack.h"
//#include "../Utils/delegate.h"
//#include "../Utils/Utils.h"
//
//extern "C"
//{
//	#include <string.h>
//	#include "../Drivers/SDCard/inc/ff.h"
//}
//
//
//const char http_200[] = "HTTP/1.0 200 OK\r\n";
//const char http_404[] = "HTTP/1.0 404 Not Found\r\n";
//const char http_server[] = "\r\nServer: Stand\r\n";
//const char http_content_type[] = "Content-Type: ";
//const char http_content_length[] = "Content-Length: ";
//const char http_connection_close[] = "Connection: close\r\n";
//const char http_linebreak[] = "\r\n";
//const char http_header_end[] = "\r\n\r\n";
//const char http_not_found[] = "<h1>404 - Not Found</h1>";
//
//const char http_text_plain[] = "text/plain";
//const char http_text_html[] = "text/html";
//const char http_text_js[] = "text/javascript";
//const char http_text_css[] = "text/css";
//const char http_image_gif[] = "image/gif";
//const char http_image_png[] = "image/png";
//const char http_image_jpeg[] = "image/jpeg";
//
//const char http_ext_txt[] = "txt";
//const char http_ext_htm[] = "htm";
//const char http_ext_html[] = "html";
//const char http_ext_js[] = "js";
//const char http_ext_css[] = "css";
//const char http_ext_gif[] = "gif";
//const char http_ext_png[] = "png";
//const char http_ext_jpg[] = "jpg";
//const char http_ext_jpeg[] = "jpeg";
//
//const char*  mime_type_table[][2] =
//{
//	{http_ext_txt, http_text_plain},
//	{http_ext_htm, http_text_html},
//	{http_ext_html, http_text_html},
//	{http_ext_js, http_text_js},
//	{http_ext_css, http_text_css},
//	{http_ext_gif, http_image_gif},
//	{http_ext_png, http_image_png},
//	{http_ext_jpg, http_image_jpeg},
//	{http_ext_jpeg, http_image_jpeg}
//};
//
//const char* Error = "Error\r\n";
//const char* postHeader = "\r\n\r\n";
//const char* contentstart ="<html><head><title>Process Monitor</title></head><style>td{border:1px solid #000}</style><body>";
//const char* table_head = "<div style=' position:absolute; left:30% '><table style=' border:2px solid #000'><thead><tr><td>Task_id</td><td>TaskName</td><td>StackSize</td><td>FreeStack</td><td>State</td><td>Priority</td></tr></thead><tbody>";
//const char* table_end = "</tbody></table></div>";
//const char* contentend=		"</body></html>";
//
//enum HttpState
//{
//	Closed = 0xFF,
//	WrireHeader =1,
//	WriteData = 2,
//	NotFound =3,
//
//};
//
//struct HTTPStatus
//{
//	HttpState state;
//	uint16_t bytesRead;
//	uint16_t numBytes;
//	char* mime;
//	FIL fp;
//	UINT br; //счетчик прочитанных байт
//	FRESULT res;
//
//	char url[32];
//
//	bool controller;
//	uint8_t ControllerId;
//};
//
//class Http
//{
//public:
//	Http(uint16_t port)
//	{
//		Stack::pStack->OnConnect = NewDelegate(this,&Http::OnConnect);
//		Stack::pStack->OnDisconnect = NewDelegate(this,&Http::OnDisconnect);
//		_OnReadWrite = NewDelegate(this,&Http::OnReadWrite);
//		//_OnWrite = NewDelegate(this,&Http::OnWrite);
//		 f_mount(0, &fs);
//		 for(int i=0;i<10;i++)
//		 {
//			 memset(_states[i].url,0,32);
//			 _states[i].controller = false;
//		 }
//	}
//
//	void OnConnect(uint8_t id)
//	{
//		Stack::pStack->SetCallbackTCP( id,_OnReadWrite);
//		_states[id].state =WrireHeader;
//	}
//
//	void OnDisconnect(uint8_t id)
//	{
//		Stack::pStack->DeleteCallbackTCP(id);
//		_states[id].state=Closed;
//	}
//
//	void OnReadWrite(uint8_t id,uint8_t* data ,uint16_t length, uint16_t* lengthWrite)
//	{
//		char baseUrl[] = "index.html ";
//		char *req = (char*)data;
//		char *p, *params,*url,*name, *value;
//		uint16_t length_header;
//		uint8_t* pbuf = data;
//		if(length && _states[id].state!=WriteData){
//
//			if( (memcmp(req, "GET ", 4) == 0) && ((p = strchr(req + 4, ' ')) != 0) )
//			{
//				url = req + 4;
//				*p = 0;
//
//				if((params = strchr(url, '?')))
//					*(params++) = 0;
//
//				// расковыриваем параметры
//				while(params)
//				{
//					if((p = strchr(params, '&')))
//						*(p++) = 0;
//
//					name = params;
//					if((value = strchr(name, '=')))
//						*(value++) = 0;
//
//					if( (strcmp(name, "pwm") == 0 ) && value )
//					{
//
//					}
//
//					params = p;
//				}
//
//				if(strcmp(url, "/") == 0)
//				{
//					url= baseUrl;
//				}
//
//				//
//				if(!_controllerProcessor(id,url , data ,&length_header ))
//				{
//
//					//_states[id].state =NotFound;
//					memcpy(data,(uint8_t*)http_404,strlen(http_404));
//					data+=strlen(http_404);
//					//content type
//					memcpy(data,(uint8_t*)http_content_type,strlen(http_content_type));
//					data+=strlen(http_content_type);
//					memcpy(data,(uint8_t*)http_text_html,strlen(http_text_html));
//					data+=strlen(http_text_html);
//					//server
//					memcpy(data,(uint8_t*)http_server,strlen(http_server));
//					data+=strlen(http_server);
//					//content length
//					memcpy(data,(uint8_t*)http_content_length,strlen(http_content_length));
//					data+=strlen(http_content_length);
//					itoa(strlen(http_not_found), str, 10);
//					memcpy(data,(uint8_t*)str,strlen(str));
//					data+=strlen(str);
//					//connection
//					memcpy(data,(uint8_t*)http_connection_close,strlen(http_connection_close));
//					data+=strlen(http_connection_close);
//					memcpy(data,(uint8_t*)http_linebreak,strlen(http_linebreak));
//					data+=strlen(http_linebreak);
//					memcpy(data,(uint8_t*)http_not_found,strlen(http_not_found));
//					data+=strlen(http_not_found);
//					memcpy(data,(uint8_t*)http_header_end,strlen(http_header_end));
//					data+=strlen(http_header_end);
//					Stack::pStack->CloseTCP(id,true);
//					*lengthWrite = data-pbuf;
//					return;
//				}
//
//				*lengthWrite = length_header;
//			}
//			else
//				if( (memcmp(req, "POST ", 4) == 0) && ((p = strchr(req + 4, ' ')) != 0) )
//				{
//					url = req + 4;
//					*p = 0;
//
//					if((params = strchr(url, '?')))
//						*(params++) = 0;
//
//					// расковыриваем параметры
//					while(params)
//					{
//						if((p = strchr(params, '&')))
//							*(p++) = 0;
//
//						name = params;
//						if((value = strchr(name, '=')))
//							*(value++) = 0;
//
//						if( (strcmp(name, "pwm") == 0 ) && value )
//						{
//
//						}
//
//						params = p;
//					}
//
//					//обработка пост
//
//				}
//		}
//		else
//		{
//			char str[16];
//			if(_states[id].controller)
//			{
//				if(_states[id].bytesRead >= _states[id].numBytes)
//				{
//					memcpy(data,(uint8_t*)http_header_end,strlen(http_header_end));
//					data+=strlen(http_header_end);
//					f_close(&_states[id].fp);
//					Stack::pStack->CloseTCP(id,true);
//				}
//				else
//				{
//					_controllerData(0,(char*)buff,(uint16_t*)&_states[id].br);
//					_states[id].bytesRead+=_states[id].br;
//					memcpy(data,buff,_states[id].br);
//					data+=_states[id].br;
//				}
//			}
//			else
//			{
//				if(_states[id].bytesRead >= _states[id].numBytes)
//				{
//					memcpy(data,(uint8_t*)http_header_end,strlen(http_header_end));
//					data+=strlen(http_header_end);
//					f_close(&_states[id].fp);
//					Stack::pStack->CloseTCP(id,true);
//				}
//				else
//				{
//					uint8_t length=0;
//					if(_states[id].numBytes-_states[id].bytesRead >128)
//					{
//						length = 64;
//					}
//					else
//					{
//						length = _states[id].numBytes-_states[id].bytesRead;
//					}
//					_states[id].res = f_read(&_states[id].fp,buff,length,&_states[id].br);
//				}
//				if(_states[id].res==FR_OK)
//				{
//					_states[id].bytesRead+=_states[id].br;
//
//					_templateProcessor();
//
//					memcpy(data,buff,_states[id].br);
//					data+=_states[id].br;
//				}
//				else
//				{
//					memcpy(data,(uint8_t*)Error,strlen(Error));
//					data+=strlen(Error);
//					memcpy(data,(uint8_t*)http_header_end,strlen(http_header_end));
//					data+=strlen(http_header_end);
//					Stack::pStack->CloseTCP(id,true);
//				}
//			}
//
//			*lengthWrite = data-pbuf;
//		}
//
//
//	}
//
//
//private:
//
//	 char str[16];
//	HTTPStatus _states[10];
//	CDelegate4<void,uint8_t,uint8_t*,uint16_t,uint16_t*>  _OnReadWrite;
//
//	FATFS	fs;
//	BYTE buff[128]; //буфер для чтения / записи файла
//
//
//	bool _fileProcessor(uint8_t id, char* url)
//	{
//
//		uint8_t length = 0;
//		bool ext =false;
//		for(int i=0;i<32;i++)
//		{
//			if(url[i]=='.')
//			{
//				ext = true;
//			}
//			if(url[i]=='\0'||url[i]==' ')
//			{
//				length=i;
//				break;
//			}
//
//		}
//
//		memset(_states[id].url,0,32);
//		memcpy(_states[id].url,url,length);
//		if(!ext)
//		{
//			memcpy(_states[id].url+length,".html",5);
//		}
//		_states[id].res =f_open(&_states[id].fp,(const char*)_states[id].url,FA_READ|FA_WRITE);
//		if(_states[id].res==FR_OK)
//		{
//			_states[id].bytesRead = 0;
//			_states[id].numBytes = _states[id].fp.fsize;
//
//			return true;
//		}
//		return false;
//	}
//
//	bool _controllerProcessor(uint8_t id, char* url , uint8_t* data, uint16_t* length)
//	{
//		uint8_t* pbuf = data;
//		bool header=false;
//		if(strcmp(url, "/wifiscan") == 0)
//		{
//			_states[id].bytesRead = 0;
//			_states[id].numBytes =5;
//			_states[id].controller = true;
//			header =true;
//		}
//
//		if( _fileProcessor(id,url))
//		{
//			_states[id].controller = false;
//			 header = true;
//		}
//
//		 if(header)
//		 {
//
//
//			 memcpy(data,(uint8_t*)http_200,strlen(http_200));
//			 data+=strlen(http_200);
//			 //content type
//			 memcpy(data,(uint8_t*)http_content_type,strlen(http_content_type));
//			 data+=strlen(http_content_type);
//
//			 _states[id].mime = (char*)httpd_get_mime_type(_states[id].url);
//			 memcpy(data,(uint8_t*)_states[id].mime,strlen(_states[id].mime));
//			 data+=strlen(_states[id].mime);
//
//			 //server
//			 memcpy(data,(uint8_t*)http_server,strlen(http_server));
//			 data+=strlen(http_server);
//
//			 //content length
//			 memcpy(data,(uint8_t*)http_content_length,strlen(http_content_length));
//			 data+=strlen(http_content_length);
//			 itoa(_states[id].fp.fsize, str, 10);
//			 memcpy(data,(uint8_t*)str,strlen(str));
//			 data+=strlen(str);
//
//			 //connection
//			 memcpy(data,(uint8_t*)http_connection_close,strlen(http_connection_close));
//			 data+=strlen(http_connection_close);
//
//			 memcpy(data,(uint8_t*)http_linebreak,strlen(http_linebreak));
//			 data+=strlen(http_linebreak);
//
//			 _states[id].state = WriteData;
//			 *length = data-pbuf;
//			 return true;
//		 }
//		 return false;
//	}
//
//	bool _controllerData(uint8_t controllerId,char* buff, uint16_t* length)
//	{
//		memcpy(buff,"Hello",5);
//		*length = 5;
//		return true;
//	}
//
//	void _templateProcessor()
//	{
//
//	}
//
//	const char *httpd_get_mime_type(char *url)
//	{
//		const char *t_ext, *t_type;
//		char *ext;
//		uint8_t i;
//
//		if((ext = strrchr(url, '.')))
//		{
//			ext++;
//			//перевод регистра
//			//strlwr(ext);
//
//			for(i = 0; i < sizeof(mime_type_table); ++i)
//			{
//				t_ext = (const char*)(mime_type_table[i][0]);
//
//				if(!strcmp(ext, t_ext))
//				{
//					t_type = (const char*)(mime_type_table[i][1]);
//					return t_type;
//				}
//			}
//		}
//		return 0;
//	}
//
//};
//
//
//
//#endif /* NET_HTTP_H_ */
