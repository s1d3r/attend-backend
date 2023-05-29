/*
 * DataBase.cpp
 *
 *  Created on: 13 мая 2023 г.
 *      Author: mr_s
 */

#include "DataBase.h"
#include <openssl/sha.h>
#include <Utils.h>

namespace NFB::Atd {

//---------------------------- Телефоны ----------------------------
uint64_t packPhoneNumber(const std::string &str)
{
	if(str.size() <= 10)
		ERROR_RN << "Номер телефона '" << str << "' не действителен";
	else {
		std::string mainNumber(str.end()-10, str.end());
		std::string country(str.begin(), str.end()-10);

		if(country[0] == '+')
			country = std::to_string(std::stoi(std::string(str.begin()+1, str.end()-10))+1);
		return std::stoull(country+mainNumber);
	}
}

std::string unpackPhoneNumber(uint64_t num)
{
	std::string numStr = std::to_string(num);
	std::string country = std::string(numStr.begin(), numStr.end()-10);
	return "+"+std::to_string(std::stoull(country)-1)+std::string(numStr.end()-10, numStr.end());
}

std::string normalizePhoneNumber(const std::string &str)
{
	return unpackPhoneNumber(packPhoneNumber(str));
}

//--------------------------- База данных --------------------------

std::tm InfoPair::getDateTime() const
{
	std::tm time;
	time.tm_year = Year;
	time.tm_mon = Month;
	time.tm_mday = Day;

	std::mktime(&time);

	return time;
}

crow::json::wvalue InfoPair::toJson() const
{
	crow::json::wvalue json;

	json["A"] = AuditoriumId;
	json["B"] = BuildingId;
	json["D"] = DisciplineId;
	json["K"] = KindOfWorkId;
	json["L"] = LecturerId;
	json["S"] = StreamId;
	json["G"] = GroupId;
	json["GS"] = GroupSubId;

	json["Yr"] = Year;
	json["Mth"] = Month;
	json["Day"] = Day;

	json["iB"] = IsBan;
	json["TS"] = TimeStart;
	json["TE"] = TimeEnd;

	json["Upt"] = Update;
	json["iC"] = IsCustom;
	json["iR"] = IsReleased;

	return json;
}

void InfoPair::fromJson(const crow::json::rvalue &json)
{
	AuditoriumId = json["A"].u();
	BuildingId = json["B"].u();
	DisciplineId = json["D"].u();
	KindOfWorkId = json["K"].u();
	LecturerId = json["L"].u();
	StreamId = json["S"].u();
	GroupId = json["G"].u();
	GroupSubId = json["GS"].u();

	Year = json["Yr"].u();
	Month = json["Mth"].u();
	Day = json["Day"].u();

	IsBan = json["iB"].b();
	TimeStart = json["TS"].s();
	TimeEnd = json["TE"].s();
}

std::string InfoPair::getHash()
{
	unsigned char hash[SHA_DIGEST_LENGTH];
	char symbolic[SHA_DIGEST_LENGTH*2+1];
	symbolic[SHA_DIGEST_LENGTH*2] = '\0';
	char data[offsetof(InfoPair, IsBan)+10];
	std::copy((const char*) this, (const char*) this + offsetof(InfoPair, IsBan), data);
	std::copy(TimeStart.c_str(), TimeStart.c_str()+5, data+offsetof(InfoPair, IsBan));
	std::copy(TimeEnd.c_str(), TimeEnd.c_str()+5, data+offsetof(InfoPair, IsBan)+5);

	SHA1((const unsigned char*) data, offsetof(InfoPair, IsBan)+10, hash);
	for(int i = 0; i < SHA_DIGEST_LENGTH; i++)
	{
		symbolic[i*2] = '0'+(hash[i] & 0xf);
		if(symbolic[i*2] > '9')
			symbolic[i*2] -= '9'-'a'+1;

		symbolic[i*2+1] = '0'+((hash[i] >> 4) & 0xf);
		if(symbolic[i*2+1] > '9')
			symbolic[i*2+1] -= '9'-'a'+1;

	}

	return (const char*) symbolic;
}

InfoAccount::~InfoAccount() = default;
crow::json::wvalue InfoAccount::toJson() const { return crow::json::wvalue(); }
void InfoAccount::fromJson(const crow::json::rvalue &json) {}

InfoSession::~InfoSession() = default;
crow::json::wvalue InfoSession::toJson() const { return crow::json::wvalue(); }
void InfoSession::fromJson(const crow::json::rvalue &json) {}

InfoAccountStudent::~InfoAccountStudent() = default;
crow::json::wvalue InfoAccountStudent::toJson() const { return crow::json::wvalue(); }
void InfoAccountStudent::fromJson(const crow::json::rvalue &json) {}

InfoSessionStudent::~InfoSessionStudent() = default;
crow::json::wvalue InfoSessionStudent::toJson() const { return crow::json::wvalue(); }
void InfoSessionStudent::fromJson(const crow::json::rvalue &json) {}

InfoAccountTeacher::~InfoAccountTeacher() = default;
crow::json::wvalue InfoAccountTeacher::toJson() const { return crow::json::wvalue(); }
void InfoAccountTeacher::fromJson(const crow::json::rvalue &json) {}

InfoSessionTeacher::~InfoSessionTeacher() = default;
crow::json::wvalue InfoSessionTeacher::toJson() const { return crow::json::wvalue(); }
void InfoSessionTeacher::fromJson(const crow::json::rvalue &json) {}

InfoAccountOmni::~InfoAccountOmni() = default;
crow::json::wvalue InfoAccountOmni::toJson() const { return crow::json::wvalue(); }
void InfoAccountOmni::fromJson(const crow::json::rvalue &json) {}

InfoSessionOmni::~InfoSessionOmni() = default;
crow::json::wvalue InfoSessionOmni::toJson() const { return crow::json::wvalue(); }
void InfoSessionOmni::fromJson(const crow::json::rvalue &json) {}



crow::json::wvalue InfoWifi::toJson() const
{
	crow::json::wvalue json;

	json["S"] = SSID;
	json["B"] = BSSID;
	json["R"] = RSSI;
	json["F"] = Frequency;

	return json;
}

void InfoWifi::fromJson(const crow::json::rvalue &json)
{
	SSID = json["S"].s();
	BSSID = json["B"].s();
	RSSI = json["R"].i();
	Frequency = json["F"].i();
}

crow::json::wvalue InfoBluetooth::toJson() const
{
	crow::json::wvalue json;

	json["N"] = Name;
	json["M"] = MAC;

	return json;
}

void InfoBluetooth::fromJson(const crow::json::rvalue &json)
{
	Name = json["N"].s();
	MAC = json["M"].s();
}

void InfoLecturer::fromJson(const crow::json::rvalue &json)
{
	Lecturer = json["L"].s();
	LecturerRank = json["R"].s();
	LecturerTitle = json["T"].s();
}

crow::json::wvalue InfoLecturer::toJson() const
{
	crow::json::wvalue json;

	json["L"] = Lecturer;
	json["R"] = LecturerRank;
	json["T"] = LecturerTitle;

	return json;
}

void InfoScheduleLocale::fromJson(const crow::json::rvalue &json)
{
}

crow::json::wvalue InfoScheduleLocale::toJson() const
{
	crow::json::wvalue json;
	crow::json::wvalue jsonMap;

	for(auto iter : Auditoriums)
		jsonMap[std::to_string(iter.first)] = iter.second;

	json["A"] = std::move(jsonMap);

	for(auto iter : Buildings)
		jsonMap[std::to_string(iter.first)] = iter.second;

	json["B"] = std::move(jsonMap);

	for(auto iter : Discipline)
		jsonMap[std::to_string(iter.first)] = iter.second;

	json["D"] = std::move(jsonMap);

	for(auto iter : KindOfWork)
		jsonMap[std::to_string(iter.first)] = iter.second;

	json["K"] = std::move(jsonMap);

	for(auto iter : Lecturers)
		jsonMap[std::to_string(iter.first)] = iter.second.toJson();

	json["L"] = std::move(jsonMap);

	for(auto iter : Groups)
		jsonMap[std::to_string(iter.first)] = iter.second;

	json["G"] = std::move(jsonMap);

	for(auto iter : GroupsSub)
		jsonMap[std::to_string(iter.first)] = iter.second;

	json["GS"] = std::move(jsonMap);

	for(auto iter : StreamsId)
	{
		crow::json::wvalue stream;
		crow::json::wvalue::list ids;
		for(auto key : iter.second.first)
			ids.push_back(key);

		stream["ids"] = std::move(ids);
		stream["title"] = iter.second.second;
		jsonMap[std::to_string(iter.first)] = std::move(stream);
	}

	json["SI"] = std::move(jsonMap);

	return json;
}

void DataBase::fromJson(const crow::json::rvalue &json)
{

}

crow::json::wvalue DataBase::toJson() const
{
	return crow::json::wvalue{};
}

std::string enumAccountTypeToString(EnumAccountType type)
{
	if(type == EnumAccountType::Student)
		return "s";
	else if(type == EnumAccountType::Teacher)
		return "t";
	else if(type == EnumAccountType::Omni)
		return "o";
	else
		ERROR_RN << "Не действительный тип аккаунта Enum";
}

EnumAccountType enumAccountTypeFromString(const std::string &type)
{
	if(type == "s")
		return EnumAccountType::Student;
	else if(type == "t")
		return EnumAccountType::Teacher;
	else if(type == "o")
		return EnumAccountType::Omni;
	else
		ERROR_RN << "Не действительный тип аккаунта Enum";
}

void DataBase::run(const bool &needShutdown)
{
}

void DataBase::checkSchedule()
{
	static bool OneLoad = false;
	if(OneLoad)
		ERROR_RN << "А загружать нужно только один раз!";

	rapidjson::Document document;
	document.Parse(makeRequestGet("https://rasp.omgtu.ru/api/schedule/group/447?start=2023.04.01&finish=2023.07.1&lng=1").c_str());
	auto lock = Schedule.lockWrite();
	auto locales = lock->Locale.lockWrite();
	auto &locale = locales++[1];

	for(auto beg = document.Begin(), end = document.End(); beg != end; beg++)
	{
		if(!beg->IsObject())
			continue;

		InfoPair pair;
		auto &begin = *beg;

		try {
			pair.AuditoriumId = begin["auditoriumOid"].GetInt();
			if(locale.Auditoriums.find(pair.AuditoriumId) == locale.Auditoriums.end())
				locale.Auditoriums[pair.AuditoriumId] = begin["auditorium"].GetString();

			pair.BuildingId = begin["buildingOid"].GetInt();
			if(locale.Buildings.find(pair.BuildingId) == locale.Buildings.end())
				locale.Buildings[pair.BuildingId] = begin["building"].GetString();

			pair.DisciplineId = begin["disciplineOid"].GetInt();
			if(locale.Discipline.find(pair.DisciplineId) == locale.Discipline.end())
				locale.Discipline[pair.DisciplineId] = begin["discipline"].GetString();

			pair.KindOfWorkId = begin["kindOfWorkOid"].GetInt();
			if(locale.KindOfWork.find(pair.KindOfWorkId) == locale.KindOfWork.end())
				locale.KindOfWork[pair.KindOfWorkId] = begin["kindOfWork"].GetString();

			pair.LecturerId = begin["lecturerOid"].GetInt();
			if(locale.Lecturers.find(pair.LecturerId) == locale.Lecturers.end())
			{
				InfoLecturer lecturer;
				lecturer.Lecturer = begin["lecturer"].GetString();
				lecturer.LecturerRank = begin["lecturer_rank"].GetString();
				lecturer.LecturerTitle = begin["lecturer_title"].GetString();
				locale.Lecturers[pair.LecturerId] = lecturer;
			}

			if(begin["streamOid"].GetInt())
			{
				pair.StreamId = begin["streamOid"].GetInt();
				if(locale.StreamsId.find(pair.StreamId) == locale.StreamsId.end())
					locale.StreamsId[pair.StreamId] = {{}, begin["stream"].GetString()};
			} else if(begin["groupOid"].GetInt()) {
				pair.GroupId = begin["groupOid"].GetInt();
				if(locale.Groups.find(pair.GroupId) == locale.Groups.end())
					locale.Groups[pair.GroupId] = begin["group"].GetString();
			} else if(begin["subGroupOid"].GetInt())
			{
				pair.GroupSubId = begin["subGroupOid"].GetInt();
				if(locale.GroupsSub.find(pair.GroupSubId) == locale.GroupsSub.end())
					locale.GroupsSub[pair.GroupSubId] = begin["subGroup"].GetString();
			}

			int year, month, day;
			std::sscanf(begin["date"].GetString(), "%d.%d.%d", &year, &month, &day);
			pair.Year = year;
			pair.Month = month;
			pair.Day = day;
			pair.IsBan = begin["isBan"].GetBool();

			pair.TimeStart = begin["beginLesson"].GetString();
			pair.TimeEnd = begin["endLesson"].GetString();

			pair.Update = stringTimeToSeconds(begin["modifieddate"].GetString());
			if(pair.Update >= EventLast)
				EventLast = pair.Update;

			lock->Pairs[pair.Year][pair.Month][pair.Day].push_back(pair);

		} catch(const std::exception &exc)
		{
			std::cout << "Ошибка обработки пары\n" << exc.what() << std::endl;
		}
	}

	InfoPair customPair;
	{
		customPair.AuditoriumId = 130;
		customPair.BuildingId = 16;
		customPair.DisciplineId = 1;
		customPair.KindOfWorkId = 4;
		customPair.LecturerId = 678;
		customPair.StreamId = 0;
		customPair.GroupId = 447;
		customPair.GroupSubId = 0;
		customPair.Year = 2023;
		customPair.Month = 05;
		customPair.Day = 15;
		customPair.IsBan = false;
		customPair.TimeStart = "00:01";
		customPair.TimeEnd = "23:59";
		customPair.Update = 16;
		customPair.IsCustom = false;
		customPair.IsReleased = false;
	};

	lock->Pairs[customPair.Year][customPair.Month][customPair.Day].push_back(customPair);
	customPair.Day++;
	lock->Pairs[customPair.Year][customPair.Month][customPair.Day].push_back(customPair);
}

void DataBase::checkLocale()
{

}

SharedMutex<std::map<uint64_t, std::unique_ptr<InfoAccount>>>::LockRead DataBase::sessionLock(const std::string &session, EnumAccountType type, InfoAccount **info, uint64_t *id)
{
	uint64_t idAccount;
	{
		auto lock = TableSessions[type].lockRead();
		auto iter = lock->find(session);
		if(iter == lock->end() || !iter->second || iter->second->TimeExpared <= uint64_t(time(nullptr)))
			ERROR_RN << "Сессия не действительна";

	//	if(iter->second.TimeExpared <= uint64_t(time(nullptr)))
	//	{
	//		auto lockW = TableSessions.lockWrite();
	//#ifdef ATTEND_DEBUG
	//		std::cout << "Удалена сессия " << iter->first << " ; Студент " << iter->second.Student << std::endl;
	//#endif
	//		lockW->erase(iter);
	//		ERROR_RN();
	//	}

		const_cast<uint64_t&>(iter->second->TimeExpared) = uint64_t(time(nullptr)) + iter->second->TimeStep;
		idAccount = iter->second->Account;
	}

	{
		auto lock = TableAccount[type].lockRead();
		auto iter = lock->find(idAccount);
		if(iter == lock->end() || !iter->second)
			ERROR_RN << "Аккаунт не существует";

		if(id)
			*id = iter->first;

		*info = dynamic_cast<InfoAccount*>(iter->second.get());
		return lock;
	}
}


SharedMutex<std::map<uint64_t, std::unique_ptr<InfoAccount>>>::LockRead DataBase::sessionLockStudent(const std::string &session, InfoAccountStudent **info, uint64_t *id)
{
	InfoAccount *acc = nullptr;
	auto lock = sessionLock(session, EnumAccountType::Student, &acc, id);
	*info = dynamic_cast<InfoAccountStudent*>(acc);

#ifdef ATTEND_DEBUG
	assert(*info, "Не действительный тип указателя аккаунта");
#endif

	return lock;
}

SharedMutex<std::map<uint64_t, std::unique_ptr<InfoAccount>>>::LockRead DataBase::sessionLockTeacher(const std::string &session, InfoAccountTeacher **info, uint64_t *id)
{
	InfoAccount *acc = nullptr;
	auto lock = sessionLock(session, EnumAccountType::Teacher, &acc, id);
	*info = dynamic_cast<InfoAccountTeacher*>(acc);

#ifdef ATTEND_DEBUG
	assert(*info, "Не действительный тип указателя аккаунта");
#endif

	return lock;
}

SharedMutex<std::map<uint64_t, std::unique_ptr<InfoAccount>>>::LockRead DataBase::sessionLockOmni(const std::string &session, InfoAccountOmni **info, uint64_t *id)
{
	InfoAccount *acc = nullptr;
	auto lock = sessionLock(session, EnumAccountType::Omni, &acc, id);
	*info = dynamic_cast<InfoAccountOmni*>(acc);

#ifdef ATTEND_DEBUG
	assert(*info, "Не действительный тип указателя аккаунта");
#endif

	return lock;
}


} /* namespace NFB::Atd */
