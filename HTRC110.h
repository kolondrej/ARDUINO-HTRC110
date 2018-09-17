/*
Библиотека ARDUINO для работы с HTRC110 RFID
HTRC110 (int SCKL, int DATA) DOUT соединен вместе с DATA
Set (int SCKL, int DATA) первоначальная настройка выводов
Init(quarts) инициализация HTRC110.quarts 0 -4Мгц, 1 -12Мгц, 2 -8Мгц, 3 -16Мгц
Также производит проверку антенны и подстройку фазы. 
Возвращает 1, если не прошла инициализация.
		   2, если не прошла проверка антенны.
		   0, если все успешно.
ReadMode(i) перевод HTRC110 в режим чтения. На DATA идет поток данных с транспондера. 1 в режим чтения, 0 выход из режима.
IdleMode(i) управление режимом ожидания. Необходимо, если есть устройства, которые тактируются от HTRC.
PowerMode(i) управление режимом питания. Перевод в спящий режим, но работает интерфейс передачи данных.
DriverMode(i) управление режимом работы драйвера антенны. Включает или выключает антенну.
Write_Tag_N(a, i) Запись в транспондер. В DATA поток данных. a - время импульса*8мкс, если a=0, то длина импульса равна длине импульса на DATA. i=0 выход из режима записи.
WriteTag(i) Запись в транспондер. В DATA поток данных. Время импульса равно выставленному ранее Write_Tag_N(a, i). i=0 выход из режима записи.
*/

#ifndef HTRC110_h
#define HTRC110_h


#include "Arduino.h"


class HTRC110
{
  public:
    HTRC110 (int SCKL, int DATA);
    void Set (int SCKL, int DATA);
    int Init(int quarts);
    void ReadMode(bool i);
	void IdleMode(bool i);
	void PowerMode(bool i);
	void DriverMode(bool i);
	void Write_Tag_N(int a, bool i);
	void Write_Tag(bool i);
	

  private:
    void Ast_Adjust(int offset);
	int CheckAnt();
    int Cmd(int cmd);
	int ReadConfig(int Page);
    int _a, _sckl, _data;
    int _cmd;
};

#endif
