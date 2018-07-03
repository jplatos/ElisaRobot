#ifndef __CELISAROBOT_H__
#define __CELISAROBOT_H__
#pragma once


enum class eElisaRobotLedColor {
	White = 0x646464,
	Silver = 0x4b4b4b,
	Gray = 0x323232,
	Black = 0x000000,
	Red = 0x640000,
	Marron = 0x320000,
	Yellow = 0x646400,
	Olive = 0x323200,
	Lime = 0x006400,
	Green = 0x003200,
	Aqua = 0x006464,
	Teal = 0x003232,
	Blue = 0x000064,
	Navy = 0x000032,
	Fuchsia = 0x640064,
	Purple = 0x320032
};

eElisaRobotLedColor ElisaRobotLedColors[] = { eElisaRobotLedColor::White, eElisaRobotLedColor::Silver ,eElisaRobotLedColor::Gray, eElisaRobotLedColor::Black ,
eElisaRobotLedColor::Red, eElisaRobotLedColor::Marron , eElisaRobotLedColor::Yellow , eElisaRobotLedColor::Olive , eElisaRobotLedColor::Lime , eElisaRobotLedColor::Green,
eElisaRobotLedColor::Aqua, eElisaRobotLedColor::Teal , eElisaRobotLedColor::Blue , eElisaRobotLedColor::Navy , eElisaRobotLedColor::Fuchsia , eElisaRobotLedColor::Purple };

class cElisaRobot
{


protected:
	unsigned short m_RobotAddress;
#pragma pack(push, 1)
	unsigned char m_LedRed;
	unsigned char m_LedGreen;
	unsigned char m_LedBlue;
	unsigned char m_Flags;	// first two bits are dedicated to the IRs:0x00 => all IRs off, 0x01 = > back IR on, 0x02 = > front IRs on, 0x03 = > all IRs on
							// third bit is reserved for enabling / disabling IR remote control(0 = >disabled, 1 = >enabled)
							// fourth bit is used for sleep(1 = > go to sleep for 1 minute)
							// fifth bit is used to calibrate all sensors(proximity, ground, accelerometer) and reset odometry
							// sixth bit is reserved(used by radio station)
							// seventh bit is used for enabling / disabling onboard obstacle avoidance
							// eight bit is used for enabling / disabling onboard cliff avoidance
	unsigned char m_SpeedRight; //speed expressed in 1 / 5 of mm / s(i.e.a value of 10 means 50 mm / s); MSBit indicate direction : 1 = forward, 0 = backward; values from 0 to 127
	unsigned char m_SpeedLeft; //speed expressed in 1 / 5 of mm / s(i.e.a value of 10 means 50 mm / s); MSBit indicate direction : 1 = forward, 0 = backward; values from 0 to 127
	unsigned char m_SmallLed;
	unsigned char m_Flags2; //bit0 is used for odometry calibration, remaining bits unused
#pragma pack(pop)

	unsigned short m_Proximity0;
	unsigned short m_Proximity1;
	unsigned short m_Proximity2;
	unsigned short m_Proximity3;
	unsigned short m_Proximity4;
	unsigned short m_Proximity5;
	unsigned short m_Proximity6;
	unsigned short m_Proximity7;
	unsigned short m_Ground0;
	unsigned short m_Ground1;
	unsigned short m_Ground2;
	unsigned short m_Ground3;
	unsigned short m_AccelerometerX;
	unsigned short m_AccelerometerY;
	unsigned short m_AccelerometerZ;
	unsigned char  m_LastIRCommand;
	unsigned short m_ProximityAmbient0;
	unsigned short m_ProximityAmbient1;
	unsigned short m_ProximityAmbient2;
	unsigned short m_ProximityAmbient3;
	unsigned short m_ProximityAmbient4;
	unsigned short m_ProximityAmbient5;
	unsigned short m_ProximityAmbient6;
	unsigned short m_ProximityAmbient7;
	unsigned short m_GroundAmbient0;
	unsigned short m_GroundAmbient1;
	unsigned short m_GroundAmbient2;
	unsigned short m_GroundAmbient3;
	unsigned short m_BateryStatus;
	unsigned int   m_LeftSteps;
	unsigned int   m_RightSteps;
	unsigned short m_Theta;
	unsigned short m_PositionX;
	unsigned short m_PositionY;
	unsigned char  m_Selector;
	bool		   m_InCharge;
	bool		   m_ButtonPressed;
	bool		   m_ChargedCompletelly;

#pragma region Flags constant
	const unsigned char cFlags_IR_AllOff = 0x00;
	const unsigned char cFlags_IR_BackOn = 0x01;
	const unsigned char cFlags_IR_FrontOn = 0x02;
	const unsigned char cFlags_IR_AllOn = 0x03;
	const unsigned char cFlags_RemoteControlOn = 0x04;
	const unsigned char cFlags_RemoteControlOff = ~0x04;
	const unsigned char cFlags_SleepOn = 0x08;
	const unsigned char cFlags_SleepOff = ~0x08;
	const unsigned char cFlags_CalibrateOn = 0x10;
	const unsigned char cFlags_CalibrateOff = ~0x10;
	const unsigned char cFlags_ObstacleAvoidanceOn = 0x40;
	const unsigned char cFlags_ObstacleAvoidanceOff = ~0x40;
	const unsigned char cFlags_CliffAvoidanceOn = 0x80;
	const unsigned char cFlags_CliffAvoidanceOff = ~0x80;
	const unsigned char cFlags2_OdometryCalibrationOff = 0x00;
	const unsigned char cFlags2_OdometryCalibrationOn = 0x01;
#pragma endregion

public:
#pragma region Callback function called when Inner registry are changed
	void(*StatusChanged)(cElisaRobot&) = nullptr;
#pragma endregion


	cElisaRobot(unsigned short robotAddress) : m_RobotAddress(robotAddress) { Reset(); }
	inline void Reset();
	inline void WriteCommand(unsigned char* buffer);
	inline void ReadStatus(unsigned char* buffer);

	inline unsigned short RobotAddress() { return m_RobotAddress; }
#pragma region Methods for manipulation LED color
	inline void SetLedColor(eElisaRobotLedColor color);
	inline void SetLedRed(unsigned char value);
	inline void SetLedGreen(unsigned char value);
	inline void SetLedBlue(unsigned char value);
	inline void SetLedColor(unsigned char red, unsigned char green, unsigned char blue);
#pragma endregion

#pragma region Methods for manipulation with speed of the robot
	inline void SetSpeedRawLeft(unsigned char value);
	inline void SetSpeedRawRight(unsigned char value);
	inline void SetSpeedLeft(char value);
	inline void SetSpeedRight(char value);
	inline void SetSpeed(char left, char right);
	inline void SetSpeed(char value);
#pragma endregion

#pragma region Mathods for manipulating small green leds
	inline void SetGreenLeds(unsigned char value);
#pragma endregion
#pragma region Status getters
	unsigned short Proximity0() { return m_Proximity0; }
	unsigned short Proximity1() { return m_Proximity1; }
	unsigned short Proximity2() { return m_Proximity2; }
	unsigned short Proximity3() { return m_Proximity3; }
	unsigned short Proximity4() { return m_Proximity4; }
	unsigned short Proximity5() { return m_Proximity5; }
	unsigned short Proximity6() { return m_Proximity6; }
	unsigned short Proximity7() { return m_Proximity7; }
	unsigned short Ground0() { return m_Ground0; }
	unsigned short Ground1() { return m_Ground1; }
	unsigned short Ground2() { return m_Ground2; }
	unsigned short Ground3() { return m_Ground3; }
	unsigned short AccelerometerX() { return m_AccelerometerX; }
	unsigned short AccelerometerY() { return m_AccelerometerY; }
	unsigned short AccelerometerZ() { return m_AccelerometerZ; }
	//unsigned char  LastIRCommand() { return ;}
	unsigned short ProximityAmbient0() { return m_ProximityAmbient0; }
	unsigned short ProximityAmbient1() { return m_ProximityAmbient1; }
	unsigned short ProximityAmbient2() { return m_ProximityAmbient2; }
	unsigned short ProximityAmbient3() { return m_ProximityAmbient3; }
	unsigned short ProximityAmbient4() { return m_ProximityAmbient4; }
	unsigned short ProximityAmbient5() { return m_ProximityAmbient5; }
	unsigned short ProximityAmbient6() { return m_ProximityAmbient6; }
	unsigned short ProximityAmbient7() { return m_ProximityAmbient7; }
	unsigned short GroundAmbient0() { return m_GroundAmbient0; }
	unsigned short GroundAmbient1() { return m_GroundAmbient1; }
	unsigned short GroundAmbient2() { return m_GroundAmbient2; }
	unsigned short GroundAmbient3() { return m_GroundAmbient3; }
	unsigned short BateryStatus() { return m_BateryStatus; }
	unsigned int   LeftSteps() { return m_LeftSteps; }
	unsigned int   RightSteps() { return m_RightSteps; }
	unsigned short Theta() { return m_Theta; }
	unsigned short PositionX() { return m_PositionX; }
	unsigned short PositionY() { return m_PositionY; }
	unsigned char  Selector() { return m_Selector; }
	bool		   InCharge() { return m_InCharge; }
	bool		   ButtonPressed() { return m_ButtonPressed; }
	bool		   ChargedCompletelly() { return m_ChargedCompletelly; }
#pragma endregion
};

void cElisaRobot::Reset()
{
	m_LedRed = 0;
	m_LedGreen = 0;
	m_LedBlue = 0;
	m_Flags = 0;
	m_SpeedRight = 0;
	m_SpeedLeft = 0;
	m_SmallLed = 0;
	m_Flags2 = 0;
}

void cElisaRobot::WriteCommand(unsigned char* buffer)
{
	buffer[0] = 0x27;				// command that robot changes its state
	buffer[1] = m_LedRed;           // R
	buffer[2] = m_LedBlue;			// B
	buffer[3] = m_LedGreen;			// G
	buffer[4] = m_Flags;			// activate IR remote control and other flags
	buffer[5] = m_SpeedRight;       // speed right (in percentage)
	buffer[6] = m_SpeedLeft;        // speed left (in percentage)
	buffer[7] = m_SmallLed;         // small green leds
	buffer[8] = m_Flags2;
	buffer[14] = (m_RobotAddress >> 8) & 0xFF;     // address of the robot
	buffer[15] = m_RobotAddress & 0xFF;
}

void cElisaRobot::ReadStatus(unsigned char* buffer)
{
	switch (buffer[0])
	{
	case 3:
		//ID = 3 (1)	Prox0(2)	Prox1(2)	Prox2(2)	Prox3(2)	Prox5(2)	Prox6(2)	Prox7(2)	Flags(1)
		m_Proximity0 = buffer[2] << 8 | buffer[1];
		m_Proximity1 = buffer[4] << 8 | buffer[3];
		m_Proximity2 = buffer[6] << 8 | buffer[5];
		m_Proximity3 = buffer[8] << 8 | buffer[7];
		m_Proximity5 = buffer[10] << 8 | buffer[9];
		m_Proximity6 = buffer[12] << 8 | buffer[11];
		m_Proximity7 = buffer[14] << 8 | buffer[13];
		m_InCharge = (buffer[15] & 0x01) == 1;//bit0 : 0 = robot not in charge; 1 = robot in charge
		m_ButtonPressed = (buffer[15] & 0x02) == 0;//bit1 : 0 = button pressed; 1 = button not pressed
		m_ChargedCompletelly = (buffer[15] & 0x02) == 1;//bit2 : 0 = robot not charged completely; 1 = robot charged completely
		break;
	case 4:
		//ID=4 (1)	Prox4 (2)	Ground0 (2)	Ground1 (2)	Ground2 (2)	Ground3 (2)	AccX (2)	AccY (2)	TV remote (1)
		m_Proximity4 = buffer[2] << 8 | buffer[1];
		m_Ground0 = buffer[4] << 8 | buffer[3];
		m_Ground1 = buffer[6] << 8 | buffer[5];
		m_Ground2 = buffer[8] << 8 | buffer[7];
		m_Ground3 = buffer[10] << 8 | buffer[9];
		m_AccelerometerX = buffer[12] << 8 | buffer[11];
		m_AccelerometerY = buffer[14] << 8 | buffer[13];
		m_LastIRCommand = buffer[15];
		break;
	case 5:
		//ID=5 (1)	ProxAmbient0 (2)	ProxAmbient1 (2)	ProxAmbient2 (2)	ProxAmbient3 (2)	ProxAmbient5 (2)	ProxAmbient6 (2)	ProxAmbient7 (2)	Selector (1)
		m_ProximityAmbient0 = buffer[2] << 8 | buffer[1];
		m_ProximityAmbient1 = buffer[4] << 8 | buffer[3];
		m_ProximityAmbient2 = buffer[6] << 8 | buffer[5];
		m_ProximityAmbient3 = buffer[8] << 8 | buffer[7];
		m_ProximityAmbient5 = buffer[10] << 8 | buffer[9];
		m_ProximityAmbient6 = buffer[12] << 8 | buffer[11];
		m_ProximityAmbient7 = buffer[14] << 8 | buffer[13];
		m_Selector = buffer[15];
		break;
	case 6:
		//ID = 6 (1)	ProxAmbient4(2)	GroundAmbient0(2)	GroundAmbient1(2)	GroundAmbient2(2)	GroundAmbient3(2)	AccZ(2)	Battery(2)	Free(1)
		m_ProximityAmbient4 = buffer[2] << 8 | buffer[1];
		m_GroundAmbient0 = buffer[4] << 8 | buffer[3];
		m_GroundAmbient1 = buffer[6] << 8 | buffer[5];
		m_GroundAmbient2 = buffer[8] << 8 | buffer[7];
		m_GroundAmbient3 = buffer[10] << 8 | buffer[9];
		m_AccelerometerZ = buffer[12] << 8 | buffer[11];
		m_BateryStatus = buffer[14] << 8 | buffer[13];
		break;
	case 7:
		//ID=7 (1)	LeftSteps (4)	RightSteps (4)	theta (2)	xpos (2)	ypos (2)	Free (1)	
		m_LeftSteps = buffer[4] << 24 | buffer[3] << 16 | buffer[2] << 8 | buffer[1];
		m_RightSteps = buffer[8] << 24 | buffer[7] << 16 | buffer[6] << 8 | buffer[5];
		m_Theta = buffer[10] << 8 | buffer[9];
		m_PositionX = buffer[12] << 8 | buffer[11];
		m_PositionY = buffer[14] << 8 | buffer[13];
		break;
	}

	if (StatusChanged != nullptr)
	{
		StatusChanged(*this);
	}
}

#pragma region Methods for manipulation LED color
void cElisaRobot::SetLedColor(eElisaRobotLedColor color)
{
	m_LedRed = ((int)color) >> 16;
	m_LedGreen = (((int)color) >> 8) & 0xff;
	m_LedBlue = ((int)color) & 0xff;
}

void cElisaRobot::SetLedRed(unsigned char value)
{
	if (value > 100)
		value = 100;
	m_LedRed = value;
}

void cElisaRobot::SetLedGreen(unsigned char value)
{
	if (value > 100)
		value = 100;
	m_LedGreen = value;
}

void cElisaRobot::SetLedBlue(unsigned char value)
{
	if (value > 100)
		value = 100;
	m_LedBlue = value;
}

void cElisaRobot::SetLedColor(unsigned char red, unsigned char green, unsigned char blue)
{
	SetLedRed(red);
	SetLedGreen(green);
	SetLedBlue(blue);
}
#pragma endregion

#pragma region Methods for manipulation with speed of the robot
void cElisaRobot::SetSpeedRawLeft(unsigned char value)
{
	m_SpeedLeft = value;
}

void cElisaRobot::SetSpeedRawRight(unsigned char value)
{
	m_SpeedRight = value;
}

void cElisaRobot::SetSpeedLeft(char value)
{
	char direction = (char)0x80;
	if (value < 0)
	{
		direction = 0;
		value = -value;
	}
	SetSpeedRawLeft(value | direction);
}

void cElisaRobot::SetSpeedRight(char value)
{
	char direction = (char)0x80;
	if (value < 0)
	{
		direction = 0;
		value = -value;
	}
	SetSpeedRawRight(value | direction);
}

void cElisaRobot::SetSpeed(char left, char right)
{
	SetSpeedLeft(left);
	SetSpeedRight(right);
}

void cElisaRobot::SetSpeed(char value)
{
	SetSpeedLeft(value);
	SetSpeedRight(value);
}

#pragma endregion

#pragma region Methods for manipulation with small green leds around robot

void cElisaRobot::SetGreenLeds(unsigned char value)
{
	m_SmallLed = value;
}

#pragma endregion

#endif
