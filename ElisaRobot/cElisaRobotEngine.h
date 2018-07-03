#ifndef __CELISAROBOTENGINE_H__
#define __CELISAROBOTENGINE_H__
#pragma once

#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <stdio.h>
#include "cUSBDevice.h"
#include "cElisaRobot.h"
using namespace std;

class cElisaRobotEngine 
{
protected:
	static const unsigned int BufferSize = 64;
	thread* m_MainThread;
	mutex m_TickMutex;
	mutex m_MainLoopLock;
	bool m_ShouldExit = false;
	chrono::steady_clock::time_point m_LastTime;
	chrono::steady_clock::time_point m_StartTime;
	unsigned long long m_TickCount;

	cUSBDevice m_Device;
	cElisaRobot* m_Robots[4];
	unsigned int m_RobotsCount = 0;
	unsigned char m_SendBuffer[BufferSize] = { 0 };
	unsigned char m_ReceiveBuffer[BufferSize] = { 0 };
	
	inline void MainLoop();	
public:
	cElisaRobotEngine() : m_MainThread(nullptr) {}
	~cElisaRobotEngine();
	inline bool Init();
	inline void Start();
	inline void Stop();
	inline unsigned int GetTicksPerSecond();

	inline cElisaRobot* AddRobot(unsigned short robotAddress);
	inline bool PrepareCommand();
	inline void ProcessStatus();
};

cElisaRobotEngine::~cElisaRobotEngine()
{
	m_Device.Close();
	Stop();
	for (unsigned int i = 0; i < m_RobotsCount; i++)
		delete m_Robots[i];
}

bool cElisaRobotEngine::Init()
{
	int ret = m_Device.Open();
	if (ret < 0)
	{
		return false;
	}
	return true;
}

void cElisaRobotEngine::Start()
{
	m_StartTime = chrono::steady_clock::now();
	m_TickCount = 0;
	m_ShouldExit = false;
	m_LastTime = chrono::steady_clock::now();
	m_MainThread = new thread(&cElisaRobotEngine::MainLoop, this);
	m_MainThread->detach();
}

void cElisaRobotEngine::Stop()
{	
	m_ShouldExit = true;
	
	m_MainLoopLock.lock();
	m_MainLoopLock.unlock();
	if (m_MainThread != nullptr)
	{
		delete m_MainThread;
		m_MainThread = nullptr;
	}
	
}

void cElisaRobotEngine::MainLoop()
{
	chrono::steady_clock::duration minWait = chrono::duration_cast<chrono::nanoseconds>(chrono::microseconds(1000));
	m_MainLoopLock.lock();
	while (!m_ShouldExit)
	{
		chrono::steady_clock::time_point actualTime = chrono::steady_clock::now();
		chrono::steady_clock::duration difference = actualTime - m_LastTime;
		if (difference < minWait)
		{
			this_thread::sleep_for(chrono::microseconds(100));
			continue;
		}
		
		m_TickMutex.lock();
		m_LastTime = actualTime;
		m_TickCount++;
		m_TickMutex.unlock();
				
		if (PrepareCommand())
		{
			m_Device.Send(m_SendBuffer, cElisaRobotEngine::BufferSize);
			if (m_Device.Receive(m_ReceiveBuffer, cElisaRobotEngine::BufferSize) == 0);
				ProcessStatus();
		}
	}
	// reset
	for (unsigned int i = 0; i < m_RobotsCount; i++)
	{
		m_Robots[i]->Reset();
	}
	PrepareCommand();
	m_Device.Send(m_SendBuffer, cElisaRobotEngine::BufferSize);

	m_MainLoopLock.unlock();
}

inline unsigned int  cElisaRobotEngine::GetTicksPerSecond()
{
	m_TickMutex.lock();
	chrono::steady_clock::time_point actualTime = chrono::steady_clock::now();
	chrono::steady_clock::duration difference = actualTime - m_StartTime;
	unsigned long long ticks = m_TickCount;
	m_TickMutex.unlock();
	auto seconds = chrono::duration_cast<chrono::seconds>(difference);
	if (seconds.count() < 1)
		return 0;
	else 
		return ticks / seconds.count();
}

inline cElisaRobot* cElisaRobotEngine::AddRobot(unsigned short robotAddress)
{
	m_Robots[m_RobotsCount] = new cElisaRobot(robotAddress);
	return m_Robots[m_RobotsCount++];
}

bool cElisaRobotEngine::PrepareCommand()
{
	if (m_RobotsCount == 0)
		return false;
	for (unsigned int i = 0; i < m_RobotsCount; i++)
	{
		m_Robots[i]->WriteCommand(&m_SendBuffer[16*i]);
	}
	return true;
}

void cElisaRobotEngine::ProcessStatus()
{
	if (m_RobotsCount == 0)
		return;
	for (unsigned int i = 0; i < m_RobotsCount; i++)
	{
		m_Robots[i]->ReadStatus(&m_ReceiveBuffer[16 * i]);
	}
}


#endif
