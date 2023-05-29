/*
 * DataBase.h
 *
 *  Created on: 13 мая 2023 г.
 *      Author: mr_s
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#define NFB_ATD_NOT_AVAILABLE

#include <string>
#include <ctime>
#include <vector>
#include <map>
#include "crow/json.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <SharedMutex.hpp>

namespace NFB::Atd {

#define BASE_COPY_MOVE(name) \
	name() = default; \
	name(const name&) = default; \
	name(name&&) = default; \
	name& operator=(const name&) = default; \
	name& operator=(name&&) = default

#define BASE_COPY_MOVE_JSON(name) BASE_COPY_MOVE(name); \
	crow::json::wvalue toJson() const; \
	void fromJson(const crow::json::rvalue &json); \
	name(const crow::json::rvalue &obj) { fromJson(obj); } \
	name& operator=(const crow::json::rvalue &obj) { fromJson(obj); return *this; } \
	operator crow::json::wvalue() const { return toJson(); }

#define BASE_COPY_MOVE_JSON_VIRTUAL(name) BASE_COPY_MOVE(name); \
	virtual crow::json::wvalue toJson() const; \
	virtual void fromJson(const crow::json::rvalue &json); \
	name(const crow::json::rvalue &obj) { fromJson(obj); } \
	name& operator=(const crow::json::rvalue &obj) { fromJson(obj); return *this; } \
	operator crow::json::wvalue() const { return toJson(); }

class InfoPair {
public:
	uint32_t AuditoriumId, BuildingId, DisciplineId, KindOfWorkId,
		LecturerId, StreamId = 0, GroupId = 0, GroupSubId = 0;
	uint16_t Year;
	uint8_t Month, Day;
	bool IsBan = false;
	std::string TimeStart, TimeEnd;

	size_t Update = 0;
	bool IsCustom = false, IsReleased = false;
	std::vector<uint64_t> Students;

public:
	BASE_COPY_MOVE_JSON(InfoPair);

	std::tm getDateTime() const;
	std::string getHash();
};


//---------------------------- Аккаунты ----------------------------
class InfoAccount {
public:
	uint64_t Phone = 0;
	std::string Email, Password;

public:
	BASE_COPY_MOVE_JSON_VIRTUAL(InfoAccount);
	virtual ~InfoAccount();
};

class InfoSession {
public:
	uint64_t Account; // Идентификатор аккаунта
	uint64_t TimeCreated, TimeExpared, TimeStep /* Дельта в секундах между началом и временем просрочки сессии */;

public:
	BASE_COPY_MOVE_JSON_VIRTUAL(InfoSession);
	virtual ~InfoSession();
};

#ifndef NFB_ATD_NOT_AVAILABLE
class InfoAccountRaw {
public:
	uint64_t Phone;
	std::string Email, Password;

	int CodePhone, CodeEmail;
	bool CheckPhone, CheckEmail;

public:
	BASE_COPY_MOVE_JSON_VIRTUAL(InfoAccountRaw);
	virtual ~InfoAccountRaw();
};

class InfoSessionRaw {
public:
	BASE_COPY_MOVE_JSON_VIRTUAL(InfoSessionRaw);
	virtual ~InfoSessionRaw();
};
#endif


//---------------------------- Студенты ----------------------------
//	Аккаунт студента
class InfoAccountStudent : public InfoAccount {
public:
	std::string StudCode;

	uint32_t GroupId = 447, GroupSubId = 1342;
	InfoPair ActivePair; //Текущая реализуемая пара
	bool IsReleasing = false;
	size_t NextInterval = 0;

public:
	BASE_COPY_MOVE_JSON_VIRTUAL(InfoAccountStudent);
	virtual ~InfoAccountStudent();
};

//	Сессия студента
class InfoSessionStudent : public InfoSession {
public:
	BASE_COPY_MOVE_JSON_VIRTUAL(InfoSessionStudent);
	virtual ~InfoSessionStudent();
};

#ifndef NFB_ATD_NOT_AVAILABLE
//	Не подтверждённый аккаунт студента
class InfoAccountRawStudent : public InfoAccountRaw {
	std::string StudCode;

public:
	BASE_COPY_MOVE_JSON_VIRTUAL(InfoAccountRawStudent);
	virtual ~InfoAccountRawStudent();
};

//	Не подтверждённая сессия студента
class InfoSessionRawStudent : public InfoSessionRaw {
public:
	BASE_COPY_MOVE_JSON_VIRTUAL(InfoSessionRawStudent);
	virtual ~InfoSessionRawStudent();
};
#endif


//-------------------------- Преподаватели -------------------------
//	Аккаунт преподавателя
class InfoAccountTeacher : public InfoAccount {
public:
	std::string ActivePair; //Текущая реализуемая пара

public:
	BASE_COPY_MOVE_JSON_VIRTUAL(InfoAccountTeacher);
	virtual ~InfoAccountTeacher();
};

//	Сессия преподавателя
class InfoSessionTeacher : public InfoSession {
public:
	BASE_COPY_MOVE_JSON_VIRTUAL(InfoSessionTeacher);
	virtual ~InfoSessionTeacher();
};

#ifndef NFB_ATD_NOT_AVAILABLE
//	Не подтверждённый аккаунт преподаваателя
class InfoAccountRawTeacher : public InfoAccountRaw {
public:

public:
	BASE_COPY_MOVE_JSON_VIRTUAL(InfoAccountRawTeacher);
	virtual ~InfoAccountRawTeacher();
};

//	Не подтверждённая сессия преподавателя
class InfoSessionRawTeacher : public InfoSessionRaw {
public:
	BASE_COPY_MOVE_JSON_VIRTUAL(InfoSessionRawTeacher);
	virtual ~InfoSessionRawTeacher();
};
#endif


//------------------------------ Omni ------------------------------
//	Аккаунт администратора
class InfoAccountOmni : public InfoAccount {
public:

public:
	BASE_COPY_MOVE_JSON_VIRTUAL(InfoAccountOmni);
	virtual ~InfoAccountOmni();
};

//	Сессия Администратора
class InfoSessionOmni : public InfoSession {
public:
	BASE_COPY_MOVE_JSON_VIRTUAL(InfoSessionOmni);
	virtual ~InfoSessionOmni();
};


//--------------------------- Расписание ---------------------------
//  Информация о лекторе
class InfoLecturer {
public:
	std::string Lecturer, LecturerRank, LecturerTitle;

public:
	BASE_COPY_MOVE_JSON(InfoLecturer);
};

//  Локали
class InfoScheduleLocale {
public:
	std::map<uint32_t, std::string> Auditoriums;
	std::map<uint32_t, std::string> Buildings;
	std::map<uint32_t, std::string> Discipline;
	std::map<uint32_t, std::string> KindOfWork;
	std::map<uint32_t, InfoLecturer> Lecturers;
	std::map<uint32_t, std::string> Groups;
	std::map<uint32_t, std::string> GroupsSub;
	std::map<uint32_t, std::pair<std::vector<uint32_t>, std::string>> StreamsId;

public:
	BASE_COPY_MOVE_JSON(InfoScheduleLocale);
};

//	Расписание
class InfoSchedule {
public:
	//1 - русский
	SharedMutex<std::map<uint32_t, InfoScheduleLocale>> Locale;
	std::map<uint32_t, std::map<uint32_t, std::map<uint32_t, std::vector<InfoPair>>>> Pairs;
	std::map<uint32_t, std::vector<crow::json::wvalue>> Events;
};

//------------------------- Местоположение -------------------------
// Wifi
class InfoWifi {
public:
	std::string SSID, BSSID;
	int RSSI, Frequency;

public:
	BASE_COPY_MOVE_JSON(InfoWifi);
};

// Bluetooth
struct InfoBluetooth {
public:
	std::string Name, MAC;

public:
	BASE_COPY_MOVE_JSON(InfoBluetooth);
};

// Полученные данные местоположения
class InfoLocation {
public:
	std::vector<InfoWifi> Wifis;
	std::vector<InfoBluetooth> Blues;
	double GpsLatitude, GpsLongitude;

	size_t Time;

public:
	BASE_COPY_MOVE(InfoLocation);
};

// Сессия отметок студентов
struct InfoStudentMarks {
	InfoPair Pair;
	size_t TimeStart, TimeEnd;
	std::string Description;

	InfoLocation AverageData;
	std::vector<InfoLocation> Teacher;

	//3 интервала отметки
	std::map<uint64_t, std::vector<std::pair<size_t, float>>> Students;
};


//--------------------------- База данных --------------------------
enum struct EnumAccountType {
	Student, Teacher, Omni
};

std::string enumAccountTypeToString(EnumAccountType type);
EnumAccountType enumAccountTypeFromString(const std::string &type);

class DataBase {
public:
	// Расписание
	SharedMutex<InfoSchedule> Schedule;

	// Таблицы
	std::map<EnumAccountType, SharedMutex<std::map<uint64_t, std::unique_ptr<InfoAccount>>>> TableAccount;
	std::map<EnumAccountType, SharedMutex<std::map<std::string, std::unique_ptr<InfoSession>>>> TableSessions;
#ifndef NFB_ATD_NOT_AVAILABLE
	std::map<EnumAccountType, SharedMutex<std::map<uint64_t, std::unique_ptr<InfoAccountRaw>>>> TableAccountRaw;
	std::map<EnumAccountType, SharedMutex<std::map<std::string, std::unique_ptr<InfoSessionRaw>>>> TableSessionsRaw;
#endif

	// Активные пары
	SharedMutex<std::map<std::string, InfoStudentMarks>> TableStudentsMarks;

	// Время последнего события и пропускное время событий
	size_t EventLast = 0, EventStart = 0;

public:
	BASE_COPY_MOVE_JSON(DataBase);

	/* Цикл обработки базы
	 * Цикл сам проверяет изменения в расписании
	 */
	void run(const bool &needShutdown);
	void checkSchedule();
	void checkLocale();

	SharedMutex<std::map<uint64_t, std::unique_ptr<InfoAccount>>>::LockRead sessionLock(const std::string &session, EnumAccountType type, InfoAccount **info, uint64_t *id = nullptr);
	bool sessionCheck(const std::string &session, EnumAccountType type)
	{
		try {
			InfoAccount *acc;
			sessionLock(session, type, &acc);
		} catch(...) {
			return false;
		}

		return true;
	}

	bool sessionExist(const std::string &session)
	{
		for(int i = 0; i<3; i++)
			if(sessionCheck(session, (EnumAccountType) i))
				return true;

		return false;
	}

	bool sessionExist(const std::string &session, EnumAccountType &acType, uint64_t &id)
	{
		InfoAccount *acc;
		for(int i = 0; i<3; i++)
		{
			try {
				sessionLock(session, (EnumAccountType) i, &acc, &id);
				acType = (EnumAccountType) i;
				return true;
			} catch(...) {
			}
		}

		return false;
	}

	SharedMutex<std::map<uint64_t, std::unique_ptr<InfoAccount>>>::LockRead sessionLockStudent(const std::string &session, InfoAccountStudent **info, uint64_t *id = nullptr);
	SharedMutex<std::map<uint64_t, std::unique_ptr<InfoAccount>>>::LockRead sessionLockTeacher(const std::string &session, InfoAccountTeacher **info, uint64_t *id = nullptr);
	SharedMutex<std::map<uint64_t, std::unique_ptr<InfoAccount>>>::LockRead sessionLockOmni(const std::string &session, InfoAccountOmni **info, uint64_t *id = nullptr);
};


//---------------------------- Телефоны ----------------------------
uint64_t packPhoneNumber(const std::string &str);
std::string unpackPhoneNumber(uint64_t num);
std::string normalizePhoneNumber(const std::string &str);

#undef BASE_COPY_MOVE
#undef BASE_COPY_MOVE_JSON
} /* namespace NFB::Atd */

#endif /* DATABASE_H_ */
