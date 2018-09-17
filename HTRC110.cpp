#include "Arduino.h"
#include "HTRC110.h"

// Команды HTRC110
#define SET_CONFIG_PAGE   0x40
#define GET_CONFIG_PAGE   0x04
#define WRITE_TAG         0xC0
#define WRITE_TAG_N       0x10
#define READ_PHASE        0x08
#define READ_TAG          0xE0
#define SET_SAMPLING_TIME 0x80
#define TX_DIS			  0x01
#define PD 				  0x04
#define PD_MODE			  0x08


HTRC110 :: HTRC110 (int SCKL, int DATA)
{
  _sckl = SCKL;
  _data = DATA;
  pinMode(_sckl, OUTPUT);
}

int HTRC110 :: Init(int quarts)
{
  int _a;
  pinMode(_sckl, OUTPUT);
  Cmd(SET_CONFIG_PAGE + 0x30 + quarts);
  Cmd(SET_CONFIG_PAGE + 0x20 + 0x00);
  Cmd(SET_CONFIG_PAGE + 0x10 + 0x00);
  Cmd(SET_CONFIG_PAGE + 0x00 + 0x02);
  _a = Cmd(GET_CONFIG_PAGE + 0x00);
  if ((_a & 0x0F) != 0x02) return (1);
  _a = CheckAnt();
  if (_a == 1) return (2);
  Ast_Adjust(0x3F);
  return (0);
}

int HTRC110 :: CheckAnt ()
{
  int _a;

  _a = Cmd(GET_CONFIG_PAGE + 0x03);
  if (_a & 0x10) return (1);
  return (0);
}

void HTRC110 :: ReadMode(bool i)
{
	if (i) {
			Cmd(SET_CONFIG_PAGE + 0x20 + 0x0A);
			Cmd(SET_CONFIG_PAGE + 0x20 + 0x0B);
			Cmd(SET_CONFIG_PAGE + 0x20 + 0x00);
			Cmd(READ_TAG);
			pinMode(_data, INPUT);
			}
	else	{
			digitalWrite(_sckl, HIGH);
			pinMode(_data, OUTPUT);
			}
}

int HTRC110 :: ReadConfig(int Page)
{
	int _a;
	_a = Cmd(GET_CONFIG_PAGE + Page);
	return (_a & 255);
}

void HTRC110 :: IdleMode(bool i)
{
	int _a;
	_a = Cmd(GET_CONFIG_PAGE + 0x01);
	if (i) _a =_a & ~PD; else _a = _a | PD;
	_a = _a & ~PD_MODE;
	_a = _a & 0x0F;
	Cmd(SET_CONFIG_PAGE + 0x10 + _a);
}

void HTRC110 :: PowerMode(bool i)
{
	int _a;
	_a = Cmd(GET_CONFIG_PAGE + 0x01);
	if (i) {
		_a = _a & ~PD; 
		   }
	else {
		_a = _a | PD;
		 };
	_a = _a & 0x0F;
	Cmd(SET_CONFIG_PAGE + 0x10 + _a);
}

void HTRC110 :: DriverMode(bool i)
{
	int _a;
	_a = Cmd(GET_CONFIG_PAGE + 0x01);
	if (i) _a = _a & ~TX_DIS; else _a = _a | TX_DIS;
	_a = _a & ~PD;
	_a = _a & 0x0F;
	Cmd(SET_CONFIG_PAGE + 0x10 + _a);
}

void HTRC110 :: Write_Tag_N (int a, bool i)
{
	_a=a & 0x0F;
	if (i) Cmd(WRITE_TAG_N+_a);
		else digitalWrite(_sckl, HIGH);
}

void HTRC110 :: Write_Tag (bool i)
{
	if (i) Cmd(WRITE_TAG);
		else digitalWrite(_sckl, HIGH);
}

void HTRC110 :: Ast_Adjust(int offset)
{
  int _a;

  _a = Cmd(READ_PHASE);
  _a = (_a << 1);
  _a = (_a + offset);
  _a = (_a & 0x3F);
  Cmd(SET_SAMPLING_TIME | _a);

}

int HTRC110 :: Cmd(int _cmd)
{
  int _i, _cnt, _mem;


  if ((_cmd & 0xC0) == 0xC0) _cnt = 3;
  else _cnt = 8;
  _mem = _cmd & 0xF0;
  pinMode(_data, OUTPUT);
  digitalWrite(_data, LOW);
  digitalWrite(_sckl, LOW);
  delayMicroseconds(10);
  digitalWrite(_sckl, HIGH);
  delayMicroseconds(10);
  digitalWrite(_data, HIGH);
  delayMicroseconds(10);
  digitalWrite(_sckl, LOW);
  digitalWrite(_data, LOW);

  for (_i = 0 ; _i < _cnt; _i++)
  {
    if (_cmd & 0x80)    digitalWrite(_data, HIGH);
    else              digitalWrite(_data, LOW);
    delayMicroseconds(10);
    digitalWrite(_sckl, HIGH);
    delayMicroseconds(10);
    digitalWrite(_sckl, LOW);
    _cmd = _cmd << 1;
  }

  digitalWrite(_data, LOW);
  if (_cnt == 3) return (0);
  if (_mem == 0)
  {
    pinMode(_data, INPUT);
    for (_i = 0; _i < 8; _i++)
    {
      digitalWrite(_sckl, HIGH);
      delayMicroseconds(10);
      _cmd = _cmd << 1;
      if (digitalRead(_data))  _cmd |= 1;
      digitalWrite(_sckl, LOW);
      delayMicroseconds(10);
    }
    pinMode(_data, OUTPUT);
    return (_cmd & 255);
  }
}

void HTRC110 :: Set (int SCKL, int DATA) 
{
  _sckl = SCKL;
  _data = DATA;
  pinMode(_sckl, OUTPUT);
}


