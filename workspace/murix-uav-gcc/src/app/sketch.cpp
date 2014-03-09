/*
 * sketch.cpp
 *
 *  Created on: 23/02/2014
 *      Author: mpontes
 */

#include <Energia.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include <bitlash.h>
#include <murixdrone.h>

#include <EtherCard.h>


void test_all_serials(void *pvParameters){

	Serial.begin(115200);
	Serial1.begin(115200);
	Serial2.begin(115200);
	Serial3.begin(115200);
	Serial4.begin(115200);
	Serial5.begin(115200);
	Serial6.begin(115200);
	Serial7.begin(115200);
	for(;;){
		Serial.println("0");
		Serial1.println("1");
		Serial2.println("2");
		Serial3.println("3");
		Serial4.println("4");
		Serial5.println("5");
		Serial6.println("6");
		Serial7.println("7");
		vTaskDelay(1000);
	}
}

void task_bitlash(void *pvParameters){

	initBitlash(115200);
	for(;;){
		runBitlash();
	}
}



void serial_events(void *pvParameters){
	for(;;){
		serialEventRun();
	}
}

void led_Task(void *pvParameters)
{
	pinMode(GREEN_LED,OUTPUT);
	int i=0;
	for(;;){


		if(i%2){
			analogWrite(GREEN_LED,10);
		} else {
			analogWrite(GREEN_LED,0);
		}
		i++;

		vTaskDelay(1000);
	}
}


EtherCard ether;

//callback that prints received packets to the serial port
void udpSerialPrint(uint16_t port, byte ip[4], const char *data, uint16_t len) {
	ether.printIp(ip);
	Serial.print(":");
	Serial.println(port);
	Serial.println(data);
	Serial.println(len);
}

void task_network_test(void *parm){

#define STATIC 0  // set to 1 to disable DHCP (adjust myip/gwip values below)
#if STATIC
// ethernet interface ip address
static byte myip[] = { 192,168,0,9 };
// gateway ip address
static byte gwip[] = { 192,168,0,254 };
#endif

	// ethernet mac address - must be unique on your network
	static byte mymac[] = { 0x00,0x4f,0x49,0x00,0x00,0x04 };


	Serial.begin(115200);
	Serial.println("\n[backSoon]");

	//eg. ether.begin(sizeof Ethernet::buffer, mymac, PD_1, 3) //PD_1 - ENC CS Pin, SPImodule 3
	if (ether.begin(sizeof(ether.bufferpkt), mymac, PD_1,3) == 0) //DEFAULT: PB_5 - ENC CS Pin, SPImodule 2
		Serial.println( "Failed to access Ethernet controller");

#if STATIC
	ether.staticSetup(myip, gwip);
#else
	if (!ether.dhcpSetup()) Serial.println("DHCP failed");
#endif

	ether.printIp("IP:  ", ether.myip);
	ether.printIp("GW:  ", ether.gwip);
	ether.printIp("DNS: ", ether.dnsip);

	//register udpSerialPrint() to port 1337
	ether.udpServerListenOnPort(&udpSerialPrint, 1337);
	//register udpSerialPrint() to port 42.
	ether.udpServerListenOnPort(&udpSerialPrint, 42);
	//
	for(;;){
		//this must be called for ethercard functions to work.
		ether.packetLoop(ether.packetReceive());
	}

}


void setup()
{
	// put your setup code here, to run once:
	Serial.begin(115200);

	//xTaskCreate(led_Task, "led", 128, NULL, 0, NULL);
	//xTaskCreate(serial_events, "serialrx", 128, NULL, 0, NULL);
	//xTaskCreate(test_all_serials, "serialtx", 128, NULL, 0, NULL);
	//xTaskCreate(task_bitlash, "bitlash", 128, NULL, 0, NULL);

	xTaskCreate(task_network_test, "network", 512, NULL, 0, NULL);

	create_all_drone_tasks();


	vTaskStartScheduler();
}


void loop()
{
	// put your main code here, to run repeatedly:
	Serial.println("freertos sched die!!");
}

