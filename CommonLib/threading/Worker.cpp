#include "Worker.h"
#include "Work.h"
#include <thread>
#include "../Utilities/Utilities.h"
#include "../Defines.h"

Worker::Worker(const std::string& debug_name)
{
	m_WorkThread = new std::thread([&] { Run(); });
	cl::SetThreadName(m_WorkThread->get_id(), debug_name.c_str());
}

Worker::~Worker()
{
	m_IsDone = true;
	m_IsRunning = false;

	m_WorkThread->join();
	SAFE_DELETE(m_WorkThread);
}

bool Worker::IsDone() const
{
	return m_IsDone;
}

void Worker::AddWork(const Work& aWorkOrder)
{
	m_Work = aWorkOrder;
	m_IsDone = false;
}

void Worker::Stop()
{
	m_IsRunning = false;
	m_IsDone = true;
}

void Worker::Run()
{
	while (m_IsRunning)
	{
		while (m_IsDone)
		{
			std::this_thread::sleep_for(std::chrono::microseconds(1)); 
			if (!m_IsRunning)
				break;
		}

		if (!m_IsDone)
		{
			m_Work.DoWork();
			m_IsDone = true;
		}
	}
}
