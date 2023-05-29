/*
 * main.cpp
 *
 *  Created on: 24 апр. 2023 г.
 *      Author: mr_s
 */

//#define __USE_TIME_BITS64
//#define ATTEND_DEBUG

#include <Utils.h>
#include <SharedMutex.hpp>

#include "crow.h"
#include "crow/json.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <openssl/sha.h>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <ctime>
#include <random>
#include <DataBase.h>

#define VAR_CHECK(arg) std::string arg = req.url_params.get(#arg)
#define VAR_CHECK_N(arg, check) std::string arg = req.url_params.get(#arg) ? req.url_params.get(#arg) : (check ? nullptr : "")

/*
 * {
 * 	"res": "OK",
 *
 * }
 *
 * {
 * 	"res": "ERR",
 * 	"mask": 0
 * }
 */



namespace NFB::Atd {

SharedMutex<std::mt19937_64> RAND64(0);
#define GENERATE RAND64.lockWrite()++()
template<typename T>
uint64_t createMapId(const std::map<uint64_t, T> &map)
{
	uint64_t id;
	int tryes = 0;
	while(true)
	{
		id = GENERATE;
		tryes++;
		if(map.find(id) == map.end())
			return id;

		if(tryes % 1000 == 0)
			std::cout << "WARN: совершено " << tryes << " попыток создания уникального идентификатора" << std::endl;
	}
}

template<typename T>
std::string createMapId(const std::map<std::string, T> &map)
{
	std::string id;
	int tryes = 0;
	while(true)
	{
		id = generateUuidV4();
		tryes++;
		if(map.find(id) == map.end())
			return id;

		if(tryes % 1000 == 0)
			std::cout << "WARN: совершено " << tryes << " попыток создания UUID" << std::endl;
	}
}

//------------------------- Декларации api -------------------------

crow::response apiAccountRegister(const crow::request& req);
crow::response apiAccountLogin(const crow::request& req);
crow::response apiAccountSessionCheck(const crow::request &req);
crow::response apiStudentHandlePair(const crow::request &req);
crow::response apiTeacherHandlePair(const crow::request &req);
crow::response apiScheduleGetFor(const crow::request &req);
crow::response apiScheduleSearchFor(const crow::request &req);
crow::response apiScheduleLocale(const crow::request &req);

DataBase Base;

int main()
{
	Initializer lock;
	Base.checkSchedule();
	Base.checkLocale();

	crow::SimpleApp app;

	CROW_ROUTE(app, "/") ([]()
	{
		crow::response res;
		res.set_header("Content-type", "text/plain; charset=utf-8");
		res.body = "NFBTeam Api Service\n"
				"  /attend: Attend Server Api";
		return res;
	});

	CROW_ROUTE(app, "/attend/") ([]()
	{
		crow::response res;
		res.set_header("Content-type", "text/plain; charset=utf-8");
		res.body = "Let's Api!\n"
				"  /v1: v0.5 alpha\n";
		return res;
	});

	CROW_ROUTE(app, "/attend/v1/") ([]()
	{
		crow::response res;
		res.set_header("Content-type", "text/plain; charset=utf-8");
		res.body = "Welcome to Attend Server Api v0.5\n"
				"/account:\n"
				"  /register: type=s|t, number=Номер телефона, email=Почта, password=Пароль\n"
				"    type=s: stud_code=Номер студенческого билета\n"
				"\n"
				"  /login: login=Номер телефона или почта, password=Пароль, type=s|t\n"
				"\n"
				"  /check_session: session, type=s|t\n"
				"\n"
				"/student:\n"
				"  /give_pair_operational_data: session=uuid, wifi=[{}], gps, bluetooth=[{}]\n"
				"\n"
				"/teacher:\n"
				"  /handle_pair: session=uuid, type\n"
				"    type=start, gps_yaw, gps_pitch, wifi=[{}], bluetooth={}, pair={}\n"
				"    type=check\n"
				"    type=stop\n"
				"    type=update, gps_yaw, gps_pitch, wifi=[{}], bluetooth={}\n"
				"\n"
				"/schedule:\n"
				"  /get: day_from, day_to=2023.05.01 (включительно), type=group|student|teacher|auditorium, id=19, ?update=389478923, ?session\n"
				"\n"
				"  /search: term=КЗИ, type=group|student|teacher|auditorium\n"
				"\n"
				"  /locale: type=auditorium|buildings|discipline|kind_of_work|lecturer|group|group_sub|stream, lang=1";
		return res;
	});

	CROW_ROUTE(app, "/attend/v1/account/register") (&apiAccountRegister);
	CROW_ROUTE(app, "/attend/v1/account/login") (&apiAccountLogin);
	CROW_ROUTE(app, "/attend/v1/account/check_session") (&apiAccountSessionCheck);
	CROW_ROUTE(app, "/attend/v1/student/handle_pair") (&apiStudentHandlePair);
	CROW_ROUTE(app, "/attend/v1/teacher/handle_pair") (&apiTeacherHandlePair);
	CROW_ROUTE(app, "/attend/v1/schedule/get") (&apiScheduleGetFor);
	CROW_ROUTE(app, "/attend/v1/schedule/search") (&apiScheduleSearchFor);
	CROW_ROUTE(app, "/attend/v1/schedule/locale") (&apiScheduleLocale);
	CROW_ROUTE(app, "/attend/v1/schedule/last_update") ([](const crow::request& req)
	{
		crow::json::wvalue json;
		json["update"] = Base.EventLast;
		return json;
	});

	app.port(31337).multithreaded().run();

	return 0;
}

crow::response apiAccountRegister(const crow::request& req)
{
	try {
		VAR_CHECK(number);
		VAR_CHECK(email);
		VAR_CHECK(password);
		VAR_CHECK(type);

		VAR_CHECK_N(stud_code, type == "s");

        /*
         * 0 - номер телефона уже использован
         * 1 - email уже зарегистрирован
         * 2 - номер билета уже зарегистрирован
         *
         * 8 - номер не действителен
         */

        uint32_t error_mask = 0;

        uint64_t phoneNumber = 0;
		try {
			phoneNumber = packPhoneNumber(number);
		} catch(...)
		{
			error_mask |= 1 << 8;
		}

#ifdef ATTEND_DEBUG
        if(error_mask & (1 << 8))
            std::cout << "Номер не действителен" << std::endl;
#endif

        EnumAccountType acType = enumAccountTypeFromString(type);
		auto table = Base.TableAccount[acType].lockRead();
		for(auto &iter : *table)
		{
			if(iter.second->Phone == phoneNumber)
				error_mask |= 1 << 0;
			if(iter.second->Email == email)
				error_mask |= 1 << 1;

			if(acType == EnumAccountType::Student)
			{
				InfoAccountStudent *info = dynamic_cast<InfoAccountStudent*>(iter.second.get());
				assert(info);
				if(info->StudCode == stud_code)
					error_mask |= 1 << 2;
			}
		}

        crow::json::wvalue json;
        if(error_mask == 0)
        {
        	std::unique_ptr<InfoAccount> account;

        	if(acType == EnumAccountType::Student)
        	{
				std::unique_ptr<InfoAccountStudent> acc = std::make_unique<InfoAccountStudent>();
				acc->StudCode = stud_code;
				account = std::move(acc);
        	} else if(acType == EnumAccountType::Teacher) {
				account = std::make_unique<InfoAccountTeacher>();
        	} else if(acType == EnumAccountType::Omni) {
				account = std::make_unique<InfoAccountOmni>();
        	}

        	account->Phone = phoneNumber;
        	account->Email = email;
        	account->Password = password;

			auto lock = Base.TableAccount[acType].lockWrite();
			lock++[createMapId(*lock)] = std::move(account);

        	json["res"] = "OK";
#ifdef ATTEND_DEBUG
            std::cout << "Зарегистрирован" << std::endl;
#endif
        } else {
        	json["res"] = "ERR";
        	json["mask"] = error_mask;
        }

        return crow::response{std::move(json)};
	} catch(const std::exception &exc)
	{
		CROW_ERROR_RESULT
	}
}

crow::response apiAccountLogin(const crow::request& req)
{
	try {
		VAR_CHECK(login);
		VAR_CHECK(password);
		VAR_CHECK(type);

		uint64_t phoneNumber = 0;
		try {
			phoneNumber = packPhoneNumber(login);
		} catch(...) {}

        /*
         * 0 - данные не действительны
         * 1 - предел по созданным сессиям (>= 10)
         */

        EnumAccountType acType = enumAccountTypeFromString(type);
		uint64_t id;
        uint32_t error_mask = 1;
		for(auto &iter : *Base.TableAccount[acType].lockRead())
		{
			assert(iter.second);
			if((iter.second->Phone == phoneNumber || iter.second->Email == login) && iter.second->Password == password)
			{
				error_mask &= ~(1 << 0);
				id = iter.first;
				break;
			}
		}

		//Проверяем существующие сессии
		int session_count = 0;
		if(!error_mask)
		{
			for(auto &iter : *Base.TableSessions[acType].lockRead())
				if(iter.second->Account == id)
					session_count++;

			if(session_count >= 10)
				error_mask |= 1 << 1;
		}

        crow::json::wvalue json;
        if(!error_mask)
		{
			auto lock = Base.TableSessions[acType].lockWrite();
			std::string session_id = createMapId(*lock);
			std::unique_ptr<InfoSession> ses;

			if(acType == EnumAccountType::Student)
				ses = std::make_unique<InfoSessionStudent>();
			else if(acType == EnumAccountType::Teacher)
				ses = std::make_unique<InfoSessionTeacher>();
			else if(acType == EnumAccountType::Omni)
				ses = std::make_unique<InfoSessionOmni>();


			ses->Account = id;
			ses->TimeCreated = uint64_t(time(nullptr));
			ses->TimeStep = 300*30;
			ses->TimeExpared = uint64_t(time(nullptr)) + ses->TimeStep;

			json["res"] = "OK";
			json["ses"] = session_id;
			json["exp"] = ses->TimeExpared;

			lock++[session_id] = std::move(ses);
#ifdef ATTEND_DEBUG
			std::cout << "Сессия создана, количество " << session_count+1 << std::endl;
#endif
		} else {
			json["res"] = "ERR";
			json["mask"] = error_mask;
		}

		return crow::response{std::move(json)};
	} catch(const std::exception &exc)
	{
		CROW_ERROR_RESULT
	}
}

crow::response apiAccountSessionCheck(const crow::request &req)
{
	try {
		VAR_CHECK(session);
		VAR_CHECK(type);

		crow::json::wvalue json;
		EnumAccountType acType = enumAccountTypeFromString(type);
		{
			auto lock = Base.TableSessions[acType].lockRead();
			auto iter = lock->find(session);
			if(iter != lock->end())
			{
				assert(iter->second);
				json["res"] = "OK";
				json["type"] = "alive";
				json["exp"] = iter->second->TimeExpared;
				return crow::response{json};
			}
		}

#ifndef NFB_ATD_NOT_AVAILABLE
		{
			auto lock = TableSessionsStudentsRaw.lockRead();
			auto iter = lock->find(session);
			if(iter != lock->end())
			{
				json["res"] = "OK";
				json["type"] = "raw";
				return crow::response{json};
			}
		}
#endif

		json["res"] = "ERR";
		json["mask"] = 1;
		return crow::response{json};

	} catch(const std::exception &exc)
	{
		return crow::response{400, exc.what()};
	}
}

crow::response apiStudentHandlePair(const crow::request &req)
{
	try {
		VAR_CHECK(session);
		VAR_CHECK(type);
		crow::json::wvalue json;

		uint64_t id;
		InfoAccountStudent stud;
		{
			InfoAccountStudent *student;
			auto lockAccountRead = Base.sessionLockStudent(session, &student, &id);
			stud = *student;
		}

		if(type == "available")
		{
			//Найти текущие активные пары
			auto lock = Base.TableStudentsMarks.lockRead();
			std::vector<InfoPair> pairs;
			for(auto iter : *lock)
			{
				if(iter.second.Pair.GroupId == stud.GroupId || iter.second.Pair.GroupSubId == stud.GroupSubId)
					pairs.push_back(iter.second.Pair);
			}

		    crow::json::wvalue::list array;
		    for(auto &iter : pairs)
		    {
		    	array.push_back(iter.toJson());
		    }

			json["res"] = "OK";
			json["data"] = std::move(array);

		} else if(type == "start")
		{
			VAR_CHECK(pair);

			InfoPair infoPair;
			infoPair.fromJson(crow::json::load(pair));

			{
				auto lock = Base.TableStudentsMarks.lockRead();
				std::string hash = infoPair.getHash();
				auto iter = lock->find(infoPair.getHash());
				if(iter == lock->end())
				{
					json["res"] = "ERR";
					json["mask"] = 1 << 0;

					return crow::response{std::move(json)};
				}
			}

			auto lock = Base.TableAccount[EnumAccountType::Student].lockWrite();
			InfoAccountStudent *student;
			Base.sessionLockStudent(session, &student, &id);

			student->IsReleasing = true;
			student->ActivePair = infoPair;
			student->NextInterval = time(0);

			json["res"] = "OK";
			json["time_from"] = "1500";
			json["time_to"] = "";
			json["mask"] = 7;

			//Вернуть необходимые данные mask gps wifi blue

		} else if(type == "check")
		{
			if(stud.IsReleasing)
			{
				json["res"] = "OK";
				json["pair"] = stud.ActivePair.toJson();

				auto lock = Base.TableStudentsMarks.lockWrite();
				auto iter = lock->find(stud.ActivePair.getHash());
				if(iter == lock->end())
				{
					json["type"] = "e";

					//Удалили запись об активной паре (пара завершилась)
					lock = nullptr;
					try {
						InfoAccountStudent *student;
						auto lock = Base.sessionLockStudent(session, &student, &id);
						student->IsReleasing = false;
					} catch(...) {}

				} else {
					json["type"] = "iR";

					float score = 0;
					for(auto &p : iter->second.Students[id])
						score += p.second;

					json["score"] = score;
				}
			} else {
				json["res"] = "ERR";
				json["mask"] = 1 << 0;
			}
		} else if(type == "oper_data")
		{
			VAR_CHECK(gps_latitude);
			VAR_CHECK(gps_longitude);
			VAR_CHECK(wifi);
			VAR_CHECK(bluetooth);

			{
				auto lock = Base.TableAccount[EnumAccountType::Student].lockWrite();
				InfoAccountStudent *student;
				Base.sessionLockStudent(session, &student, &id);

				if(uint64_t(time(0)) < student->NextInterval)
				{
					json["res"] = "ERR";
					json["time_from"] = student->NextInterval;
					json["mask"] = 1 << 0;

					return crow::response{std::move(json)};
				}

				while(uint64_t(time(0)) > student->NextInterval)
					student->NextInterval += 1500;
			}

			auto lock = Base.TableStudentsMarks.lockWrite();
			auto iter = lock->find(stud.ActivePair.getHash());
			if(iter == lock->end())
			{
				json["res"] = "OK";
				json["type"] = "e";

				return crow::response{std::move(json)};
			}

			iter->second.Students[id].push_back({0, 1});

			float score = 0;
			for(auto &p : iter->second.Students[id])
				score += p.second;

			json["res"] = "OK";
			json["type"] = "iR";
			json["accept_mask"] = 7;
			json["score"] = score;


		}

		return crow::response{std::move(json)};
	} catch(const std::exception &exc)
	{
		CROW_ERROR_RESULT
	}
}

void subApiStopPair(const std::string &hash)
{
	std::vector<uint64_t> passed;
	InfoStudentMarks marks;

	{
		auto lock = Base.TableStudentsMarks.lockWrite();
		auto iter = lock->find(hash);
		if(iter == lock->end())
			ERROR_RN << "Нечего останавливать";

		for(auto student : iter->second.Students)
		{
			float score = 0;
			for(std::pair<size_t, float> &val : student.second)
				score += val.second;

			if(score >= 1.5)
				passed.push_back(student.first);
		}

		marks = iter->second;
		lock->erase(iter);
	}

	auto lockSchedule = Base.Schedule.lockWrite();
	auto &day = lockSchedule->Pairs[marks.Pair.Year][marks.Pair.Month][marks.Pair.Day];

	for(auto &pair : day)
		if(pair.getHash() == marks.Pair.getHash())
		{
			pair.Students = passed;
			pair.IsReleased = true;
			return;
		}

	marks.Pair.Students = passed;
	marks.Pair.IsReleased = true;
	day.push_back(marks.Pair);
}

crow::response apiTeacherHandlePair(const crow::request &req)
{
	try {
		VAR_CHECK(session);
		VAR_CHECK(type);
		crow::json::wvalue json;

		/*
		 * Начало пары, если пара уже была реализована, перезаписывает её.
		 * Требует
		 * 	scan_mask - uint -> битовая маска:
		 * 		0 - gps
		 * 		1 - wifi
		 * 		2 - bluetooth
		 *
		 *	gps_latitude - double
		 *	gps_longitude - double
		 *	wifi - base64 -> [{wifi}]
		 *	bluetooth - base64 -> [{wifi}]
		 *	pair - base64 -> {pair}
		 *
		 */
		if(type == "start")
		{
			VAR_CHECK(scan_mask);
			VAR_CHECK(gps_latitude);
			VAR_CHECK(gps_longitude);
			VAR_CHECK(wifi);
			VAR_CHECK(bluetooth);
			VAR_CHECK(pair);

			std::string decodeWifi = base64Decode(wifi),
					decodePair = base64Decode(pair);

			crow::json::rvalue array = crow::json::load(decodeWifi);

			uint64_t id;
			InfoAccountTeacher *teacher;
			auto lockAccountRead = Base.sessionLockTeacher(session, &teacher, &id);

			InfoPair activePair;
			activePair.fromJson(crow::json::load(decodePair));
			std::string hash = activePair.getHash();

			{
				auto lockAccountWrite = Base.TableAccount[EnumAccountType::Teacher].lockWrite();

				if(teacher->ActivePair.size())
				{
					json["res"] = "ERR";
					json["mask"] = 1 << 0;
					return crow::response{std::move(json)};
				}
			}

			{
				auto lockMarks = Base.TableStudentsMarks.lockWrite();
				InfoStudentMarks data;
				data.Pair = activePair;
				data.TimeStart = uint64_t(time(0));
				data.TimeEnd = uint64_t(time(0)+90*60);
				data.Teacher.push_back({});

				InfoWifi infoW;
				for(auto &objW : array)
				{
					infoW.fromJson(objW);
					data.Teacher[0].Wifis.push_back(infoW);
				}

				InfoBluetooth infoB;
				infoB.Name = "ATT_"+generateUuidV4();
				infoB.MAC = bluetooth;
				json["blue"] = infoB.Name;
				data.Teacher[0].Blues.push_back(infoB);
				data.Teacher[0].GpsLatitude = std::stod(gps_latitude);
				data.Teacher[0].GpsLongitude = std::stod(gps_longitude);
				data.Teacher[0].Time = time(0);

				data.AverageData = data.Teacher[0];

				lockMarks++[hash] = data;
			}
			{
				auto lockAccountWrite = Base.TableAccount[EnumAccountType::Teacher].lockWrite();
				teacher->ActivePair = hash;
			}

			json["res"] = "OK";

		} else if(type == "check") {
			std::string hash;

			{
				uint64_t id;
				InfoAccountTeacher *teacher;
				auto lockAccountRead = Base.sessionLockTeacher(session, &teacher, &id);

				hash = teacher->ActivePair;
			}

			if(hash.size())
			{
				auto lock = Base.TableStudentsMarks.lockRead();
				auto iter = lock->find(hash);

				if(iter == lock->end())
					json["ans"] = false;
				else {
					json["ans"] = true;
					json["pair"] = iter->second.Pair.toJson();
				}
			} else
				json["ans"] = false;

			json["res"] = "OK";

		} else if(type == "stop") {
			std::string hash;

			{
				uint64_t id;
				InfoAccountTeacher *teacher;
				auto lockAccountRead = Base.sessionLockTeacher(session, &teacher, &id);
				auto lockAccountWrite = Base.TableAccount[EnumAccountType::Teacher].lockWrite();

				hash = teacher->ActivePair;
				teacher->ActivePair = "";
			}

			if(!hash.size())
				ERROR_RN << "Мммм";

			{
				auto lock = Base.TableStudentsMarks.lockRead();
				auto iter = lock->find(hash);

				if(iter == lock->end())
				{
					json["res"] = "ERR";
					json["mask"] = 0;
				} else {
					lock = nullptr;
					subApiStopPair(hash);
					json["res"] = "OK";
				}
			}

		} else if(type == "update") {
			VAR_CHECK(gps_latitude);
			VAR_CHECK(gps_longitude);
			VAR_CHECK(wifi);

			std::string decodeWifi = base64Decode(wifi);

			std::string hash;

			{
				uint64_t id;
				InfoAccountTeacher *teacher;
				auto lockAccountRead = Base.sessionLockTeacher(session, &teacher, &id);

				hash = teacher->ActivePair;
			}

			if(!hash.size())
				ERROR_RN << "Ммммм 2";

			auto lock = Base.TableStudentsMarks.lockWrite();
			auto iter = lock->find(hash);
			if(iter == lock->end())
			{
				json["res"] = "ERR";
				json["mask"] = 1 << 0;
			} else {
				std::vector<crow::json::rvalue> array = crow::json::load(decodeWifi).lo();
				InfoLocation data;

				InfoWifi infoW;
				for(auto &wifi : array)
				{
					infoW.fromJson(wifi);
					data.Wifis.push_back(infoW);
				}

				data.Blues = iter->second.Teacher[0].Blues;
				data.GpsLatitude = std::stod(gps_latitude);
				data.GpsLongitude = std::stod(gps_longitude);
				data.Time = time(0);

				iter->second.Teacher.push_back(std::move(data));

				//Рассчитываем среднее значение
				data = iter->second.Teacher[0];

				for(size_t ret = 1; ret<iter->second.Teacher.size(); ret++)
				{
					data.GpsLatitude += iter->second.Teacher[ret].GpsLatitude;
					data.GpsLongitude += iter->second.Teacher[ret].GpsLongitude;
					data.Time = iter->second.Teacher[ret].Time;
					for(auto iter2 : iter->second.Teacher[ret].Wifis)
					{
						bool added = false;
						for(auto iter3 : data.Wifis)
						{
							if(iter3.BSSID == iter2.BSSID)
							{
								added = true;
								iter3.RSSI += iter2.RSSI;
								iter3.RSSI /= 2;
								break;
							}
						}

						if(added)
							continue;

						data.Wifis.push_back(iter2);
					}

					data.GpsLatitude /= 2;
					data.GpsLongitude /= 2;
				}

				iter->second.AverageData = data;
			}

		} else
			ERROR_RN << "Что хотим? Не понятно";

		return crow::response{std::move(json)};
	} catch(const std::exception &exc)
	{
		CROW_ERROR_RESULT
	}
}

crow::response apiScheduleGetFor(const crow::request &req)
{
	try {
		VAR_CHECK(day_from);
		VAR_CHECK(day_to);
		VAR_CHECK(type);
		VAR_CHECK(id);
		VAR_CHECK_N(update, false);
		VAR_CHECK_N(session, false);

		uint32_t ID = std::stoul(id), lang = 1;
		size_t timeUpdate = update.size() ? std::stoull(update) : 0;

		if(type != "group" && type != "student" && type != "teacher" && type != "auditorium")
			ERROR_RN << "Не понятно что от нас хотят";

		uint32_t year1, month1, day1, year2, month2, day2;
		{
			int _year1, _month1, _day1, _year2, _month2, _day2;
			std::sscanf(day_from.c_str(), "%d.%d.%d", &_year1, &_month1, &_day1); //TODO: проверить на количество обработанных чисел
			std::sscanf(day_to.c_str(), "%d.%d.%d", &_year2, &_month2, &_day2);

			year1 = _year1;
			month1 = _month1;
			day1 = _day1;
			year2 = _year2;
			month2 = _month2;
			day2 = _day2;
		}

		bool isStudent = 0;
		uint64_t accountId = 0;
		EnumAccountType acType;
		if(session.size() && !Base.sessionExist(session, acType, accountId))
			ERROR_RN << "Сессия не действительна";

		if(accountId != 0 && acType == EnumAccountType::Student)
			isStudent = true;

		crow::json::wvalue json;
		json["res"] = "OK";
	    crow::json::wvalue::list array;

		auto lockSchedule = Base.Schedule.lockRead();
		auto lockLocale = lockSchedule->Locale.lockRead();
		auto &locale = lockLocale->find(lang)->second;

		for(auto &iterYear : lockSchedule->Pairs)
		{
			if(!(iterYear.first >= year1 && iterYear.first <= year2))
				continue;

			uint32_t localMonth1, localMonth2;
			if(year1 == year2)
			{
				localMonth1 = month1;
				localMonth2 = month2;
			} else if(iterYear.first == year1) {
				localMonth1 = month1;
				localMonth2 = 12;
			} else if(iterYear.first == year2) {
				localMonth1 = 1;
				localMonth2 = month1;
			}

			for(auto &iterMonth : iterYear.second)
			{
				if(!(iterMonth.first >= localMonth1 && iterMonth.first <= localMonth2))
					continue;

				uint32_t localDay1, localDay2;
				if(year1 == year2 && month1 == month2)
				{
					localDay1 = day1;
					localDay2 = day2;
				} else if(iterYear.first == year1 && iterMonth.first == month1) {
					localDay1 = day1;
					localDay2 = 31;
				} else if(iterYear.first == year2 && iterMonth.first == month2) {
					localDay1 = 1;
					localDay2 = day2;
				}

				for(auto &iterDay : iterMonth.second)
				{
					if(!(iterDay.first >= localDay1 && iterDay.first <= localDay2))
						continue;

					if(type == "group")
					{
						std::vector<uint32_t> keys;
						{
							auto iter = locale.Groups.find(ID);
							keys.push_back(ID);
							if(iter != locale.Groups.end())
								for(auto subGroup : locale.GroupsSub)
									if(subGroup.second.find(iter->second) != std::string::npos)
										keys.push_back(subGroup.first);

							iter = locale.GroupsSub.find(ID);
							if(iter != locale.GroupsSub.end())
								for(auto group : locale.Groups)
									if(iter->second.find(group.second) != std::string::npos)
									{
										keys.push_back(group.first);
										break;
									}

							std::vector<uint32_t> streams;
							for(auto stream : locale.StreamsId)
							{
								std::vector<int> intersection;
								std::set_intersection(stream.second.first.begin(), stream.second.first.end(), keys.begin(), keys.end(), std::back_inserter(intersection));
								if(intersection.size())
									streams.push_back(stream.first);
							}

							keys.insert(keys.end(), streams.begin(), streams.end());
						}

						for(auto pair : iterDay.second)
						{
							if(pair.Update <= timeUpdate)
								continue;

							for(auto key : keys)
								if(pair.GroupId == key || pair.GroupSubId == key || pair.StreamId == key)
								{
									crow::json::wvalue object = pair.toJson();
									if(session.size())
									{
										if(isStudent)
										{
											for(auto s : pair.Students)
												if(s == accountId)
												{
													object["V"] = true;
													break;
												}
										} else {
											if(pair.IsReleased)
												object["V"] = true;
											object["VC"] = pair.Students.size();
										}
									}

									array.push_back(std::move(object));
									break;
								}
						}

					} else if(type == "teacher") {
						for(auto pair : iterDay.second)
						{
							if(pair.Update <= timeUpdate)
								continue;

							if(pair.LecturerId == ID)
							{
								crow::json::wvalue object = pair.toJson();
								if(session.size())
								{
									if(isStudent)
									{
										for(auto s : pair.Students)
											if(s == accountId)
											{
												object["V"] = true;
												break;
											}
									} else {
										if(pair.IsReleased)
											object["V"] = true;
										object["VC"] = pair.Students.size();
									}
								}

								array.push_back(std::move(object));
								break;
							}
						}

					} else
						for(auto pair : iterDay.second)
						{
							//TODO: аудитории и студенты
						}
				}
			}
		}

		json["pairs"] = std::move(array);

		return crow::response{std::move(json)};
	} catch(const std::exception &exc)
	{
		CROW_ERROR_RESULT
	}
}

crow::response apiScheduleSearchFor(const crow::request &req)
{
	try {
		VAR_CHECK(term);
		VAR_CHECK(type);

		if(type != "group" && type != "student" && type != "teacher" && type != "auditorium")
			ERROR_RN << "Не понятно что от нас хотят";

		if(type == "teacher")
			type = "person";

		crow::json::wvalue json;

		try {
			std::string answer = makeRequestGet("https://rasp.omgtu.ru/api/search?term="+term+"&type="+type);
			rapidjson::Document ans;
			ans.Parse(answer.c_str());

			for(auto beg = ans.Begin(), end = ans.End(); beg != end; beg++)
			{
				auto &begin = *beg;
				begin.RemoveMember("description");
				begin.RemoveMember("type");
			}

			json["res"] = "OK";
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			ans.Accept(writer);
			json["ans"] = crow::json::load(buffer.GetString());
		} catch(...) {
			json["res"] = "ERR";
			json["mask"] = 1 << 0;
		}

		return crow::response{std::move(json)};
	} catch(const std::exception &exc)
	{
		CROW_ERROR_RESULT
	}
}

crow::response apiScheduleLocale(const crow::request &req)
{
	try {
		VAR_CHECK_N(lang, false);

		if(lang.size() && lang != "1")
			ERROR_RN << "А ничего кроме русского и не поддерживается, lang = 1";

		uint32_t langNumber = std::stoul(lang);

		crow::json::wvalue json;
		json["res"] = "OK";
		json["data"] = Base.Schedule.lockRead()->Locale.lockRead()->find(langNumber)->second.toJson();

		return crow::response{std::move(json)};
	} catch(const std::exception &exc)
	{
		CROW_ERROR_RESULT
	}
}
}
/*
 * https://rasp.omgtu.ru/api/schedule/group/447?start=2023.04.24&finish=2023.04.30&lng=1
 * Запрос групп c
 * type=student type=person type=auditorium
 *
 * 08:00 - 09:30
 * 09:40 - 11:10
 * 11:35 - 13:05
 * 13:15 - 14:45
 * 15:10 - 16:40
 * 16:50 - 18:20
 * 18:30 - 20:00
 *
 */

int main()
{
	return NFB::Atd::main();
}
