/*
 * http_server.c
 *
 *  Created on: 19/01/2014
 *      Author: mpontes
 */


#include <boost/asio.hpp>
#include <boost/atomic.hpp>
#include <boost/shared_ptr.hpp>


#include "murix_http_server.h"


boost::atomic<int> atomic_http_requests(0);

#include "murix_camera.h"




/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

#include "libbmp24.hpp"

double YUV2RGB_CONVERT_MATRIX[3][3] = { { 1, 0, 1.4022 }, { 1, -0.3456, -0.7145 }, { 1, 1.771, 0 } };

uint8_t clamp(float input)
{
	if (input < 0) input = 0;
	if (input > 255) input = 255;
	return (uint8_t) abs(input);
}

//
libbmp24::Bitmap ConvertYUV2RGB(uint8_t* yuvFrame, int width, int height){


	libbmp24::Bitmap bm;

	bm.createBitmap(width, height);

	int uIndex = width * height;
	int vIndex = uIndex + ((width * height) >> 2);
	int gIndex = width * height;
	int bIndex = gIndex * 2;

	int temp = 0;

	//
	uint8_t* rgbFrame = new uint8_t[width * height * 3];


	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// R
			temp = (int)(yuvFrame[y * width + x] + (yuvFrame[vIndex + (y / 2) * (width / 2) + x / 2] - 128) * YUV2RGB_CONVERT_MATRIX[0][2]);
			rgbFrame[y * width + x] = (uint8_t)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
			// G
			temp = (int)(yuvFrame[y * width + x] + (yuvFrame[uIndex + (y / 2) * (width / 2) + x / 2] - 128) * YUV2RGB_CONVERT_MATRIX[1][1] + (yuvFrame[vIndex + (y / 2) * (width / 2) + x / 2] - 128) * YUV2RGB_CONVERT_MATRIX[1][2]);
			rgbFrame[gIndex + y * width + x] = (uint8_t)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
			// B
			temp = (int)(yuvFrame[y * width + x] + (yuvFrame[uIndex + (y / 2) * (width / 2) + x / 2] - 128) * YUV2RGB_CONVERT_MATRIX[2][1]);
			rgbFrame[bIndex + y * width + x] = (uint8_t)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));


			bm.plot(x, y, rgbFrame[y * width + x], rgbFrame[gIndex + y * width + x], rgbFrame[bIndex + y * width + x]);
		}
	}

	//
	delete rgbFrame;

	return bm;

}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////




void http_thread_server_session(boost::shared_ptr<boost::asio::ip::tcp::socket> sock)
{
	const static char http_html_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
	const static char http_html_hdr_bmp[] = "HTTP/1.1 200 OK\r\nContent-type: image/bmp\r\n\r\n";
	const static char http_html_hdr_404[] = "HTTP/1.1 404 Not Found\r\nContent-type: text/html\r\n\r\n";
	const static char http_html_not_found[]="<html><head><title>Not Found</title></head><body>Sorry, the object you requested was not found.</body><html>";

	try
	{

		for (;;)
		{
			char data[1024];
			boost::system::error_code error;
			size_t length = sock->read_some(boost::asio::buffer(data), error);

			if (error == boost::asio::error::eof){
				// Connection closed cleanly by peer.
				printf("connection drop eof\r\n");
				break;
			}
			else if (error){
				// Some other error.
				throw boost::system::system_error(error);
			}

			///////////////////////////////////////////////////////////

			data[length]=0;
			printf("http_thread_server_session[[[%s]]]\r\n",data);

			atomic_http_requests++;

			////////////////////////////////////////////////////////////////

			char uri[256];

			////////////////////////////////////////////////////////////////

			const char delimiters[] = " ";
			char *running;
			char *token;
			int token_count=0;

			running = data;
			while(true){
				token = strsep (&running, delimiters);
				if(token==NULL) break;

				if(token_count==1){
					snprintf(uri,sizeof(uri),"%s",token);
				}

				printf("token[%d]=%s len=%d\r\n",token_count,token,strlen(token));
				token_count++;

			}

			////////////////////////////////////////////////////////////////

			printf("uri=%s\r\n",uri);

			//////////////////////////////////////////////////////////////

			if(strcmp(uri,"/")==0){
				////////////////////////////////////////////////////////////////
				int requests = atomic_http_requests;
				snprintf(data,sizeof(data),""
						"<html>"
						"<head>"
						" <title>MURIX DRONE</title>"
						"</head>"
						"<body>"
						"<hr>"
						"<table border=1>"
						"<tr>"
						" <td></td>"
						" <td><button>PITCH FORWARD</<button></td>"
						" <td></td>"
						" <td>-</td>"
						" <td> </td>"
						" <td><button>HEIGHT UP</<button></td>"
						" <td></td>"
						"</tr>"
						"<tr>"
						" <td><button>ROLL LEFT</<button></td>"
						" <td></td>"
						" <td><button>ROLL RIGHT</<button></td>"
						" <td>-</td>"
						" <td><button>YAW CW</<button></td>"
						" <td></td>"
						" <td><button>YAW CCW</<button></td>"
						"</tr>"
						"<tr>"
						" <td></td>"
						" <td><button>PITCH BACKWARD</<button></td>"
						" <td></td>"
						" <td>-</td>"
						" <td></td>"
						" <td><button>HEIGHT DOWN</<button></td>"
						" <td></td>"
						"</tr>"
						"</table>"
						"<hr>"
						"<img src=horizontal.bmp width=640 height=480 alt=horizontal>"
						"<hr>"
						"<img src=vertical.bmp width=176 height=144 alt=vertical>"
						"<hr>"
						"</body>"
						"</html>");
				/////////////////////////////////////////////////////////////////
				boost::asio::write(*sock, boost::asio::buffer(http_html_hdr,sizeof(http_html_hdr)-1));
				boost::asio::write(*sock, boost::asio::buffer(data, strlen(data)));
			}

			//////////////////////////////////////////////////////////////////////

			else if(strcmp(uri,"/vertical.bmp")==0){

				camera_context_t ctx = camera_v();
				libbmp24::Bitmap bmp = ConvertYUV2RGB(ctx.buf,ctx.width,ctx.height);


				std::stringstream os;
				bmp.serialize(os);

				boost::asio::write(*sock, boost::asio::buffer(http_html_hdr_bmp,sizeof(http_html_hdr_bmp)-1));
				boost::asio::write(*sock, boost::asio::buffer(os.str().c_str(), os.str().size()));

			}

			/////////////////////////////////////////////////////////////////////////////////////////

			else if(strcmp(uri,"/horizontal.bmp")==0){

				camera_context_t ctx = camera_h();
				libbmp24::Bitmap bmp = ConvertYUV2RGB(ctx.buf,ctx.width,ctx.height);


				std::stringstream os;
				bmp.serialize(os);

				boost::asio::write(*sock, boost::asio::buffer(http_html_hdr_bmp,sizeof(http_html_hdr_bmp)-1));
				boost::asio::write(*sock, boost::asio::buffer(os.str().c_str(), os.str().size()));

			}

			//////////////////////////////////////////////////////////////////////

			else {
				boost::asio::write(*sock, boost::asio::buffer(http_html_hdr_404,sizeof(http_html_hdr_404)-1));
				boost::asio::write(*sock, boost::asio::buffer(http_html_not_found,sizeof(http_html_not_found)));
			}

			(*sock).close();
			break;

		}

	}
	catch (std::exception& e)
	{
		std::cerr << "Exception in thread: " << e.what() << "\n";
	}
}

void http_thread_server(void){

	boost::asio::io_service io_service;
	boost::asio::ip::tcp::acceptor a(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 80));
	while(true)
	{
		boost::shared_ptr<boost::asio::ip::tcp::socket> sock(new boost::asio::ip::tcp::socket(io_service));
		a.accept(*sock);
		http_thread_server_session(sock);
		//boost::thread t(boost::bind(http_thread_server_session, sock));
	}
}
