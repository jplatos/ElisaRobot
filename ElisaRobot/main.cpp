#pragma comment(lib, "libusb-1.0.lib")

#include <stdio.h>
#include <Windows.h>
#include "libusb.h"
#include "cUSBDevice.h"
#include <chrono>

#include "cElisaRobot.h"
#include "cElisaRobotEngine.h"
using namespace std;

void RobotStatusChanged(cElisaRobot& robot)
{
	if (robot.Proximity0() > 600)
		robot.SetSpeed(-30);
}

int main()
{
	//cUSBDevice device;
	int robotAddress = 3540;
	/*int ret = device.Open();
	if (ret < 0)
	{
		printf("Error %d", ret);
		return 1;
	}*/
	cElisaRobotEngine engine;
	if (!engine.Init())
	{
		printf("Failed to initilize Engine\n");
		return 1;
	}
	cElisaRobot& robot = *(engine.AddRobot(robotAddress));
	robot.StatusChanged = RobotStatusChanged;
	
	chrono::time_point<chrono::system_clock> last, current;
	last = chrono::system_clock::now();

	engine.Start();

	int index = 0;
	while (true)
	{
		Sleep(4);
		
		char baseSpeed = 0x15;		
		
		if (GetKeyState(0x52)>=0 && GetKeyState(0x47)>=0 && GetKeyState(0x42)>=0)
		{
			robot.SetLedColor(eElisaRobotLedColor::Black);
		}
		else
		{
			robot.SetLedColor(eElisaRobotLedColor::Black);

			if (GetKeyState(0x52) < 0)
				robot.SetLedRed(0x64);
			if (GetKeyState(0x47) < 0)
				robot.SetLedGreen(0x64);
			if (GetKeyState(0x42) < 0)
				robot.SetLedBlue(0x64);
		}
		
		if (GetKeyState(0x53) < 0)
			robot.SetGreenLeds(0xff);
		else
			robot.SetGreenLeds(0x00);
		
		if (GetKeyState(VK_UP) >= 0 && GetKeyState(VK_DOWN) >= 0 && GetKeyState(VK_LEFT) >= 0 && GetKeyState(VK_RIGHT) >= 0)
		{
			robot.SetSpeed(0);
		}
		else
		{
			if (GetKeyState(VK_UP) < 0)
			{
				robot.SetSpeed(baseSpeed);
				if (GetKeyState(VK_LEFT) < 0)
				{
					robot.SetSpeedRight(2*baseSpeed);
				}
				if (GetKeyState(VK_RIGHT) < 0)
				{
					robot.SetSpeedLeft(2*baseSpeed);
				}
			}
			else if (GetKeyState(VK_DOWN) < 0)
			{
				robot.SetSpeed(-baseSpeed);
				if (GetKeyState(VK_LEFT) < 0)
				{
					robot.SetSpeedRight(-2 * baseSpeed);
				}
				if (GetKeyState(VK_RIGHT) < 0)
				{
					robot.SetSpeedLeft(-2 * baseSpeed);
				}
			}
			else if (GetKeyState(VK_LEFT) < 0)
			{
				robot.SetSpeedRight(baseSpeed);
				robot.SetSpeedLeft(0);
			}
			else if (GetKeyState(VK_RIGHT) < 0)
			{
				robot.SetSpeedLeft(baseSpeed);
				robot.SetSpeedRight(0);
			}
		}

		current = chrono::system_clock::now();
		std::chrono::duration<double,std::milli> elapsed = current - last;
		if (elapsed.count() > 1000)
		{
			last = current;
			time_t ttp = std::chrono::system_clock::to_time_t(current);
			struct tm * timeinfo;

			timeinfo = localtime(&ttp);
			printf("FPS at %02d:%02d:%02d: %d TPS\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, engine.GetTicksPerSecond());
		}
		//
		//current = chrono::system_clock::now();
		//std::chrono::duration<double,std::milli> elapsed = current - last;
		//if (elapsed.count() > 100)
		//{
		//	last = current;
		//	greenLeds <<= 1;
		//	if (greenLeds == 0)
		//		greenLeds = 1;
		//	buffer[7] = greenLeds;
		//}
		
		/*current = chrono::system_clock::now();
		std::chrono::duration<double,std::milli> elapsed = current - last;
		if (elapsed.count() > 200)
		{
			last = current;
			robot.SetLedColor(ElisaRobotLedColors[index++]);
			index = index % 16;
		}*/

		/*robot.WriteCommand(buffer);
		ret = device.Send(buffer, 16);
		
		unsigned char receiver[64] = { 0 };
		ret = device.Receive(receiver,64);
		robot.ReadStatus(receiver);*/

		if (GetKeyState(VK_ESCAPE)<0)
			break;
	}
	engine.Stop();
	//device.Close();
	/*robot.Reset();
	robot.WriteCommand(buffer);
	ret = device.Send(buffer, 16);*/
	
	
	
	return 0;
}