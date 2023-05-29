/*
 * SharedMutex.hpp
 *
 *  Created on: 13 мая 2023 г.
 *      Author: mr_s
 */

#ifndef SHAREDMUTEX_HPP_
#define SHAREDMUTEX_HPP_

#include <Utils.h>
#include <boost/thread/shared_mutex.hpp>

template<typename T>
class SharedMutex {
	T Obj;
	boost::shared_mutex Mutex;

public:
	template<typename ...Args>
	SharedMutex(Args&& ... args)
		: Obj(args...)
	{}
	~SharedMutex() = default;

	friend class LockRead;
	friend class LockWrite;

	class LockRead {
		SharedMutex *Handler;
		boost::shared_lock<boost::shared_mutex> Lock;
		void check() const { if(!Lock) throw std::exception(); }

	public:
		LockRead(SharedMutex *handler)
			: Handler(handler), Lock(handler->Mutex)
		{}
		LockRead(const LockRead&) = delete;
		LockRead(LockRead&&) = default;
		LockRead& operator=(const LockRead&) = delete;
		LockRead& operator=(LockRead&&) = default;

		~LockRead() = default;

		const T& operator*() 		const { check(); return Handler->Obj; }
		const T* operator->() 		const { check(); return &Handler->Obj; }
		const T& operator++(int) 	const { check(); return Handler->Obj; }

		LockRead& operator=(std::nullptr_t)
		{
			if(Lock)
				Lock.unlock();
			return *this;
		}

		operator bool() const { return (bool) Lock; }
	};

	class LockWrite {
			SharedMutex *Handler;
			boost::upgrade_lock<boost::shared_mutex> Lock;
			void check() const { if(!Lock) throw std::exception(); }

		public:
			LockWrite(SharedMutex *handler)
				: Handler(handler), Lock(handler->Mutex)
			{}
			LockWrite(const LockWrite&) = delete;
			LockWrite(LockWrite&&) = default;
			LockWrite& operator=(const LockWrite&) = delete;
			LockWrite& operator=(LockWrite&&) = default;

			~LockWrite() = default;

			T& operator*() 		const { check(); return Handler->Obj; }
			T* operator->() 	const { check(); return &Handler->Obj; }
			T& operator++(int) 	const { check(); return Handler->Obj; }

			LockWrite& operator=(std::nullptr_t)
			{
				if(Lock)
					Lock.unlock();
				return *this;
			}

			operator bool() const { return (bool) Lock; }
		};

	LockRead lockRead() const { return const_cast<SharedMutex*>(this); }
	LockWrite lockWrite() { return this; }
};


#endif /* SHAREDMUTEX_HPP_ */
